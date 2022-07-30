#include "SystemBullet.h"
#include "ECS/Components/ComponentBullet.h"
#include "ECS/Components/ComponentBase.hpp"
#include "ECS/Components/ComponentHealth.h"
#include "Managers/PlayerManager.h"
#include "Managers/BulletManager.h"
#include "Scenes/GameGui.h"
#include "CxhUtils.h"
#include <random>

using namespace danMuGame::component;

#define SUCK_BLOOD_VALUE 0.2f
#define HARM_PLUS_VALUE 1.25f
#define TRIUMPH_VALUE 240
#define HARVEST_VALUE 0.08f

namespace danMuGame::system
{
    static std::random_device rd;

    SystemBullet::SystemBullet(entt::registry &registry)
        : System(registry)
    {
    }

    void SystemBullet::update(float dt)
    {
        auto view = m_registry.view<ComponentBullet>();
        for (auto entity : view)
        {
            auto &bullet = view.get<ComponentBullet>(entity);
            if (bullet.isUse == true)
            {
                if (bullet.endIndex < bullet.moveRoute.size() - 1)
                {
                    ++(bullet.endIndex);
                }
                else if (bullet.startIndex > 0 && bullet.startIndex < bullet.moveRoute.size() - 2)
                {
                    ++(bullet.startIndex);
                }
                else if (bullet.startIndex >= bullet.moveRoute.size() - 2)
                {
                    BulletManager::GetInstance()->addIdleBulletEntity(entity);
                    auto &targetHealth = m_registry.get<ComponentHealth>(bullet.targetEntity);
                    auto &targetCrisis = m_registry.get<ComponentCrisis>(bullet.targetEntity);
                    auto &bulletData = m_registry.get<ComponentPlayerData>(bullet.attacker);
                    auto &attackerPlayer = m_registry.get<ComponentPlayer>(bullet.attacker);
                    auto &targetPlayer = m_registry.get<ComponentPlayer>(bullet.targetEntity);
                    // injure target entity
                    // ...
                    if (targetHealth.health > 0)
                    {
                        if (targetPlayer.skill == PlayerSkill::DODGE && rd() % 100 <= 15)
                        {
                            return;
                        }

                        auto &playerMove = m_registry.get<ComponentMove>(bullet.targetEntity);
                        auto distance = CxhUtils::getEuclideanDistance(playerMove.position, bullet.moveRoute[bullet.endIndex]);
                        int injure = 0;

                        if (distance < 2) // 爆头，伤害x2
                        {
                            injure = bullet.injure * 2;
                        }
                        else if (distance < 8) // 普通，伤害x1
                        {
                        }
                        else if (distance < 15) // 小微偏离，伤害x0.75
                        {
                            injure = bullet.injure * 0.75f;
                        }
                        else if (distance < 20) // 偏离, 伤害x0.5
                        {
                            injure = bullet.injure * 0.5f;
                        }
                        else if (distance < 30) // 重度偏离，伤害x0.25
                        {
                            injure = bullet.injure * 0.25f;
                        }
                        else if (distance < 60) // 离谱偏离，伤害x0.1
                        {
                            injure = bullet.injure * 0.1f;
                        }
                        if (injure > 0)
                        {
                            targetCrisis.beAttackedAtEnemy = bullet.attacker;
                            targetCrisis.situation = PlayerSituation::BE_ATTACKED;
                            if (attackerPlayer.skill == PlayerSkill::SUCK_BLOOD) // 吸血
                            {
                                auto &attackerHealth = m_registry.get<ComponentHealth>(bullet.attacker);
                                if (attackerHealth.health > 0)
                                {
                                    auto blood = injure * SUCK_BLOOD_VALUE;
                                    if (blood == 0)
                                    {
                                        blood = 1;
                                    }
                                    attackerHealth.Health(injure * SUCK_BLOOD_VALUE);
                                }
                            }
                            else if (attackerPlayer.skill == PlayerSkill::HARM_PLUS) // 中伤
                            {
                                injure = injure * HARM_PLUS_VALUE;
                            }
                            else if (attackerPlayer.skill == PlayerSkill::HARVEST && targetHealth.health <= targetHealth.maxHealth * HARVEST_VALUE) // 收割
                            {
                                injure = targetHealth.health;
                            }

                            targetHealth.Injure(injure);
                            bulletData.damageValue += injure;

                            if (targetHealth.health <= 0)
                            {
                                auto &attackPlayer = m_registry.get<ComponentPlayer>(bullet.attacker);

                                auto &targetPlayer = m_registry.get<ComponentPlayer>(bullet.targetEntity);
                                auto &attackPlayerFirearmsWeapon = m_registry.get<ComponentFirearmsWeapon>(bullet.attacker);
                                if (attackPlayerFirearmsWeapon.mainWeapon.weaponBase != nullptr)
                                {
                                    GameGui::GetInstance()->addMainMessage(fmt::format("{} 使用 {} 淘汰了 {}",
                                                                                       attackPlayer.name,
                                                                                       attackPlayerFirearmsWeapon.mainWeapon.weaponBase->name,
                                                                                       targetPlayer.name));
                                }
                                else
                                {
                                    GameGui::GetInstance()->addMainMessage(fmt::format("{} 淘汰了 {}", attackPlayer.name, targetPlayer.name));
                                }

                                if (attackerPlayer.skill == PlayerSkill::TRIUMPH) // 凯旋
                                {
                                    auto &attackerHealth = m_registry.get<ComponentHealth>(bullet.attacker);
                                    attackerHealth.Health(TRIUMPH_VALUE);
                                }

                                bulletData.eliminations++;
                                //...
                            }
                        }
                    }
                }
                else if (bullet.endIndex == (bullet.moveRoute.size() - 1) && bullet.startIndex == 0)
                {
                    ++(bullet.startIndex);
                }
            }
        }
    }
}
