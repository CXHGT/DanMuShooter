#ifndef __SYSTEM_CAMERA_H__
#define __SYSTEM_CAMERA_H__

#include "System.h"

namespace danMuGame::system
{
    class SystemCamera : public System
    {
    public:
        explicit SystemCamera(entt::registry &);
        virtual void update(float dt) override;
    };
}

#endif