#include "SystemGamePrepare.h"
#include "ECS/Components/ComponentCamera.hpp"
#include "ECS/Components/ComponentBase.hpp"
#include "ECS/Components/ComponentAnimation.hpp"
#include "Managers/PlayerManager.h"
#include "Managers/GameManager.h"
#include <random>

using namespace danMuGame::component;

#define DELEGATE()

namespace danMuGame::system
{
    SystemGamePrepare::SystemGamePrepare(entt::registry &registry)
        : System(registry)
    {
    }

    void SystemGamePrepare::update(float dt)
    {
        static Vector2 vecotr2(200, 240);
        static std::random_device rd;

        switch (GameManager::GetInstance()->getGameStatue())
        {
        case GameStatue::Prepare:
            DELEGATE()
            {
                auto cameraEntity = m_registry.view<component::ComponentCamera>();
                auto &camera = cameraEntity.get<component::ComponentCamera>(*(cameraEntity.begin()));
                camera.viewPoint.setX(900);
                camera.viewPoint.setY(700);
                camera.viewEntity = (entt::entity)UINT32_MAX;
                camera.viewFrame = 0;

                auto view = m_registry.view<ComponentPlayer, ComponentAnimation, ComponentMove>();
                for (auto entity : view)
                {
                    auto &player = view.get<ComponentPlayer>(entity);
                    auto &animation = view.get<ComponentAnimation>(entity);
                    auto &move = view.get<ComponentMove>(entity);
                    if (player.isUse == true)
                    {
                        int x = player.index / 10;
                        int y = player.index % 10;
                        move.position.setX(x * 90);
                        move.position.setY(y * 90);
                        move.position += vecotr2;
                    }
                }
            }
            break;
        case GameStatue::Distribute:
            DELEGATE()
            {
                auto view = m_registry.view<ComponentPlayer, ComponentAnimation, ComponentMove>();
                for (auto entity : view)
                {
                    auto &move = view.get<ComponentMove>(entity);
                    auto *gameMap = GameManager::GetInstance()->getNowGameMap();
                    while (true)
                    {
                        int x = rd() % gameMap->getMapSize().width();
                        int y = rd() % gameMap->getMapSize().height();
                        if (gameMap->isCanMove(Point2(x, y), PLAYER_BOX_SIZE) == true)
                        {
                            move.position.setX(x * gameMap->getPixmapSize().width());
                            move.position.setY(y * gameMap->getPixmapSize().height());
                            break;
                        }
                    }
                }
                GameManager::GetInstance()->setGameStatue(GameStatue::Running);
            }
            break;
        }
    }

}
