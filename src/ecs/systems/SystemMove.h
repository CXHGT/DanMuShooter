#ifndef __SYSTEM_MOVE_H__
#define __SYSTEM_MOVE_H__

#include "System.h"

namespace danMuGame::system
{
    class SystemMove : public System
    {
    public:
        explicit SystemMove(entt::registry &);
        virtual void update(float dt) override;
    };
}

#endif