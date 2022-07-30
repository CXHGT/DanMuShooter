#ifndef __SYSTEM_BULLET_H__
#define __SYSTEM_BULLET_H__

#include "System.h"

namespace danMuGame::system
{
    class SystemBullet : public System
    {
    public:
        explicit SystemBullet(entt::registry &);
        virtual void update(float dt) override;
    };
}

#endif