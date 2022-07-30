#include "SystemCamera.h"
#include "ECS/Components/ComponentCamera.hpp"
#include "ECS/Components/ComponentBase.hpp"
#include "Managers/PlayerManager.h"
#include "Managers/GameManager.h"
#include "CxhUtils.h"
#include <random>

using namespace danMuGame::component;

#define CAMERA_MOVE_SPEED 30
#define CAMERA_VIEW_FRAME 600

namespace danMuGame::system
{
    SystemCamera::SystemCamera(entt::registry &registry)
        : System(registry)
    {
        auto entity = m_registry.create();
        m_registry.emplace_or_replace<ComponentCamera>(entity, 1.f, Point2(1, 1), (entt::entity)UINT32_MAX, 0);
    }

    void SystemCamera::update(float dt)
    {
        auto gameStatue = GameManager::GetInstance()->getGameStatue();
        
        if (gameStatue == GameStatue::Running)
        {
            auto cameraEntity = m_registry.view<ComponentCamera>();
            auto &camera = cameraEntity.get<ComponentCamera>(*(cameraEntity.begin()));
            if (camera.viewFrame > 0)
            {
                if (camera.viewEntity != (entt::entity)UINT32_MAX)
                {
                    auto &player = m_registry.get<ComponentPlayer>(camera.viewEntity);
                    if (player.isUse == true)
                    {
                        auto &move = m_registry.get<ComponentMove>(camera.viewEntity);
                        auto distance = CxhUtils::getEuclideanDistance(Vector2(camera.viewPoint), move.position);
                        if (distance < CAMERA_MOVE_SPEED)
                        {
                            camera.viewPoint = move.position.toPoint2();
                        }
                        else
                        {
                            auto angle = CxhUtils::getAngle(Vector2(camera.viewPoint), move.position);
                            camera.viewPoint += Point2((int32_t)(cos(angle * DEC) * CAMERA_MOVE_SPEED), (int32_t)(sin(angle * DEC) * CAMERA_MOVE_SPEED));
                        }
                    }
                }
                --(camera.viewFrame);
            }
            else if (camera.viewEntity == (entt::entity)UINT32_MAX || camera.viewFrame == 0)
            {
                static std::random_device rd;
                if (PlayerManager::GetInstance()->getRemainingUserPlayerList().size() > 0)
                {
                    auto index = rd() % PlayerManager::GetInstance()->getRemainingUserPlayerList().size();
                    auto it = PlayerManager::GetInstance()->getRemainingUserPlayerList().begin();
                    while (index > 0)
                    {
                        --index;
                        ++it;
                    }
                    auto entity = *it;
                    auto &player = m_registry.get<ComponentPlayer>(entity);
                    if (player.isUse == true)
                    {
                        camera.viewFrame = CAMERA_VIEW_FRAME;
                        camera.viewEntity = entity;
                    }
                }
                else if (PlayerManager::GetInstance()->getRemainingPlayerList().size() > 0)
                {
                    auto index = rd() % PlayerManager::GetInstance()->getRemainingPlayerList().size();
                    auto it = PlayerManager::GetInstance()->getRemainingPlayerList().begin();
                    while (index > 0)
                    {
                        --index;
                        ++it;
                    }
                    auto entity = *it;
                    auto &player = m_registry.get<ComponentPlayer>(entity);
                    if (player.isUse == true)
                    {
                        camera.viewFrame = CAMERA_VIEW_FRAME;
                        camera.viewEntity = entity;
                    }
                }
            }
            else
            {
                camera.viewEntity = (entt::entity)UINT32_MAX;
            }
            // auto playerEntity = PlayerManager::GetInstance()->getPlayerVector()[0];
            // auto &move = m_registry.get<ComponentMove>(playerEntity);

            // camera.viewPoint = move.position.toPoint2(); // - Point2(500,500);
        }
    }
}
