#include "SystemMove.h"
#include "ECS/Components/ComponentCamera.hpp"
#include "ECS/Components/ComponentBase.hpp"
#include "Managers/PlayerManager.h"
#include "ECS/Components/ComponentAngle.h"
#include "CxhUtils.h"
#include <iostream>
#include "fmt/format.h"

using namespace danMuGame::component;

namespace danMuGame::system
{
    SystemMove::SystemMove(entt::registry &registry)
        : System(registry)
    {
    }

    void SystemMove::update(float dt)
    {
        auto view = m_registry.view<ComponentMove, ComponentMoveStack>();
        for (auto entity : view)
        {
            auto &move = view.get<ComponentMove>(entity);
            float moveSpeed = move.speed;
            if(moveSpeed <= 0)
            {
                continue;
            }

            auto &moveStack = view.get<ComponentMoveStack>(entity);
            if (moveStack.moveRoundStack.empty() == false)
            {
                if (moveStack.walkCount <= 0)
                {
                    // 获取要移动的目的地
                    auto &nowWalkingPoint = moveStack.moveRoundStack.top();
                    // 当前位置到目的地的角度
                    float angle = CxhUtils::getAngle(move.position, nowWalkingPoint);

                    if (m_registry.any_of<ComponentAngle>(entity))
                    {
                        // 记录人物要转向的角度，普通的移动权重为0
                        auto &componentAngle = m_registry.get<ComponentAngle>(entity);
                        componentAngle.setGotoAngle(angle, 0);
                    }

                    // 记录移动角度
                    moveStack.targetAngle = angle;
                    // 到目的地要移动几步
                    moveStack.walkCount = (int)(CxhUtils::getEuclideanDistance(move.position, nowWalkingPoint) / moveSpeed) + 1;
                }

                --(moveStack.walkCount);

                if (moveStack.walkCount <= 0)
                {
                    float distance = CxhUtils::getEuclideanDistance(move.position, moveStack.moveRoundStack.top());
                    // std::cout << fmt::format("distance : {} , speed : {}", distance, move.speed) << std::endl;
                    if (distance > moveSpeed)
                    {
                        // 获取要移动的目的地
                        auto &nowWalkingPoint = moveStack.moveRoundStack.top();
                        // 当前位置到目的地的角度
                        float angle = CxhUtils::getAngle(move.position, nowWalkingPoint);

                        if (m_registry.any_of<ComponentAngle>(entity))
                        {
                            // 记录人物要转向的角度，普通的移动权重为0
                            auto &componentAngle = m_registry.get<ComponentAngle>(entity);
                            componentAngle.setGotoAngle(angle, 0);
                        }

                        // 记录移动角度
                        moveStack.targetAngle = angle;
                        // 到目的地要移动几步
                        moveStack.walkCount = (int)(CxhUtils::getEuclideanDistance(move.position, nowWalkingPoint) / moveSpeed) + 1;
                    }
                    else
                    {
                        move.position.setX(moveStack.moveRoundStack.top().x());
                        move.position.setY(moveStack.moveRoundStack.top().y());
                        moveStack.moveRoundStack.pop();
                    }
                }
                else
                {
                    move.position += Vector2(cos(moveStack.targetAngle * DEC) * moveSpeed, sin(moveStack.targetAngle * DEC) * moveSpeed);
                }
            }
        }
    }
}