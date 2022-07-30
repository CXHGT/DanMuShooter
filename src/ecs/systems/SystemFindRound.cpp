#include "SystemFindRound.h"
#include "ECS/Components/ComponentCamera.hpp"
#include "ECS/Components/ComponentBase.hpp"
#include "Managers/PlayerManager.h"
#include "Managers/GameManager.h"
#include "CxhUtils.h"

using namespace danMuGame::component;

#define PATH_SMOOTHING_SIZE 5

namespace danMuGame::system
{
    SystemFindRound::SystemFindRound(entt::registry &registry)
        : System(registry)
    {
    }

    // 路径平滑处理
    void SystemFindRound::PathSmoothing(AStarNode *aStarItem, int32_t boxSize)
    {
        if (aStarItem != nullptr)
        {
            GameMap *gameMap = GameManager::GetInstance()->getNowGameMap();
            AStarNode *startNode = aStarItem;
            AStarNode *endNode = aStarItem;
            static std::vector<Point2> catchPoint;
            catchPoint.clear();
            while (aStarItem->m_parent != nullptr)
            {
                bool isOk = true;
                for (int i = 0; i < PATH_SMOOTHING_SIZE; ++i)
                {
                    if (startNode->m_parent == nullptr)
                    {
                        break;
                    }
                    startNode = startNode->m_parent;
                }
                // 2个点的位置不能一样，不然会导致死循环
                if (startNode == endNode)
                {
                    break;
                }
                CxhUtils::getLinePoint2(catchPoint, startNode->point, endNode->point);
                // 判断直接节点是否有不可用移动的地方，若没有，2点直连处理
                for (const auto &point : catchPoint)
                {
                    if (gameMap->isCanMove(point, boxSize) == false)
                    {
                        isOk = false;
                        break;
                    }
                }
                if (isOk == true)
                {
                    endNode->m_parent = startNode;
                }

                endNode = startNode;
            }
        }
    }

    void SystemFindRound::update(float dt)
    {
        auto view = m_registry.view<ComponentMoveStack, ComponentFindRound>();
        for (auto entity : view)
        {
            GameMap *gameMap = GameManager::GetInstance()->getNowGameMap();
            auto &moveStack = view.get<ComponentMoveStack>(entity);
            auto &findRound = view.get<ComponentFindRound>(entity);
            if (findRound.needFindRound == true)
            {
                AStarNode *aStarItem = gameMap->getRelativePositionRoute(findRound.startRelativePosition, findRound.endRelativePosition, findRound.boxSize);

                SystemFindRound::PathSmoothing(aStarItem, findRound.boxSize);
                while (aStarItem != nullptr)
                {
                    moveStack.moveRoundStack.push(Point2(aStarItem->point.x() * gameMap->getPixmapSize().width(), aStarItem->point.y() * gameMap->getPixmapSize().height()));
                    aStarItem = aStarItem->m_parent;
                }

                if (moveStack.moveRoundStack.empty() == false)
                {
                    moveStack.moveRoundStack.pop();
                }
                findRound.needFindRound = false;
            }
        }
    }
}
