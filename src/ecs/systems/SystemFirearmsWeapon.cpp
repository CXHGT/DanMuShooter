#include "SystemFirearmsWeapon.h"
#include "ECS/Components/ComponentBase.hpp"
#include "ECS/Components/ComponentFirearmsWeapon.h"
#include "Managers/PlayerManager.h"
#include <iostream>

using namespace danMuGame::component;

namespace danMuGame::system
{
    SystemFirearmsWeapon::SystemFirearmsWeapon(entt::registry &registry)
        : System(registry)
    {
    }

    void SystemFirearmsWeapon::update(float dt)
    {
        auto view = m_registry.view<ComponentFirearmsWeapon>();
        for (auto entity : view)
        {
            auto &componentFirearmsWeapon = view.get<ComponentFirearmsWeapon>(entity);
            auto &weapon = componentFirearmsWeapon.mainWeapon;
            auto *weaponBase = weapon.weaponBase;
            if (weapon.isCanShoot == false && weaponBase != nullptr)
            {
                if (weapon.currentRecoveryFrame > 0 && weapon.currentBullets <= 0)
                {
                    ++(weapon.currentRecoveryFrame);
                    if(weapon.currentRecoveryFrame >= weaponBase->recoveryFrame)
                    {
                        weapon.currentRecoveryFrame = 0;
                        weapon.currentBullets = weaponBase->numberOfBullets;
                        weapon.isCanShoot = true;
                    }
                }
                else if (weapon.currentBullets <= 0)
                {
                    weapon.currentRecoveryFrame = 1;
                }
                else if (weapon.currentIntervalFrame < weaponBase->attackIntervalFrame)
                {
                    ++(weapon.currentIntervalFrame);
                    if(weapon.currentIntervalFrame >= weaponBase->attackIntervalFrame)
                    {
                        weapon.isCanShoot = true;
                    }
                }
            }
        }
    }
}