#ifndef __SYSTEM_FIREARMS_WEAPON_H__
#define __SYSTEM_FIREARMS_WEAPON_H__

#include "System.h"

namespace danMuGame::system
{
    class SystemFirearmsWeapon : public System
    {
    public:
        explicit SystemFirearmsWeapon(entt::registry &);
        virtual void update(float dt) override;
    };
}

#endif