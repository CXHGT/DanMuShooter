#ifndef __SYSTEM_PLAYER_H__
#define __SYSTEM_PLAYER_H__

#include "System.h"

namespace danMuGame::component
{
    class ComponentAnimation;
    class ComponentMove;
    class ComponentMoveStack;
    class ComponentAngle;
}

namespace danMuGame::system
{
    class SystemPlayer : public System
    {
    public:
        explicit SystemPlayer(entt::registry &);
        virtual void update(float dt) override;

    private:
    };
}

#endif