#ifndef __SYSTEM_ANGLE_H__
#define __SYSTEM_ANGLE_H__

#include "System.h"

namespace danMuGame::system
{
    class SystemAngle : public System
    {
    public:
        explicit SystemAngle(entt::registry &);
        virtual void update(float dt) override;
    };
}

#endif