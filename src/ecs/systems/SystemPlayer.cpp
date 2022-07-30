#include "SystemPlayer.h"
#include "ECS/Components/ComponentBase.hpp"
#include "ECS/Components/ComponentHealth.h"
#include "res/GameSource.h"
#include "managers/PlayerManager.h"
#include "managers/GameManager.h"
#include "managers/BulletManager.h"
#include <iostream>
#include "CxhUtils.h"
#include <random>
#include "fmt/format.h"

using namespace danMuGame::component;

namespace danMuGame::system
{
    SystemPlayer::SystemPlayer(entt::registry &registry)
        : System(registry)
    {
    }

    void SystemPlayer::update(float dt)
    {
        static int32_t delayFrame = 0;
        if (delayFrame > 10)
        {
            auto &playerList = PlayerManager::GetInstance()->getRemainingPlayerList();
            for (auto it = playerList.begin(); it != playerList.end(); ++it)
            {
                auto &crisis = m_registry.get<ComponentCrisis>(*it);
                crisis.nearestEnemy = (entt::entity)UINT32_MAX;
                crisis.nearbyEnemies.clear();
                crisis.nearestDistance = 999999.f;
            }
            for (auto it = playerList.begin(); it != playerList.end();)
            {
                entt::entity playerEntity = *it;
                auto &crisis = m_registry.get<ComponentCrisis>(*it);
                auto &move = m_registry.get<ComponentMove>(*it);
                auto &player = m_registry.get<ComponentPlayer>(*it);
                ++it;
                for (auto subIt = it; subIt != playerList.end(); ++subIt)
                {
                    auto subEntity = *subIt;
                    auto &subPlayer = m_registry.get<ComponentPlayer>(*subIt);
                    if (player.team != subPlayer.team || player.team == 0)
                    {
                        auto &subCrisis = m_registry.get<ComponentCrisis>(*subIt);
                        auto &subMove = m_registry.get<ComponentMove>(*subIt);
                        auto distance = CxhUtils::getEuclideanDistance(move.position, subMove.position);
                        if (crisis.nearestDistance > distance)
                        {
                            crisis.nearestDistance = distance;
                            crisis.nearestEnemy = subEntity;
                        }
                        if (subCrisis.nearestDistance > distance)
                        {
                            subCrisis.nearestDistance = distance;
                            subCrisis.nearestEnemy = playerEntity;
                        }
                        if (distance < NEARBY_ENEMY_DISTANCE)
                        {
                            crisis.nearbyEnemies.push_back(subEntity);
                            subCrisis.nearbyEnemies.push_back(playerEntity);
                        }
                    }
                }
            }
            delayFrame = 0;
        }
        else
        {
            ++delayFrame;
        }

        auto view = m_registry.view<ComponentPlayer, ComponentHealth>();
        for (auto entity : view)
        {
            auto &player = view.get<ComponentPlayer>(entity);
            auto &componentHealth = view.get<ComponentHealth>(entity);
            // auto &componentAngle = view.get<ComponentAngle>(entity);
            // auto &componentFirearmsWeapon = view.get<ComponentFirearmsWeapon>(entity);

            if (player.isUse == false)
            {
                continue;
            }

            // 状态机运行
            player.stateMachine->update();


        }
    }
}
