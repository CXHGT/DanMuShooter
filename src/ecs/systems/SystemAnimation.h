#ifndef __SYSTEM_ANIMATION_H__
#define __SYSTEM_ANIMATION_H__

#include "System.h"

namespace danMuGame::system
{
    class SystemAnimation : public System
    {
    public:
        explicit SystemAnimation(entt::registry &);
        virtual void update(float dt) override;
    };
}

#endif