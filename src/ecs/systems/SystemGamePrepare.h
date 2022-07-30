#ifndef __SYSTEM_GAME_PREPARE_H__
#define __SYSTEM_GAME_PREPARE_H__

#include "System.h"

namespace danMuGame::system
{
    class SystemGamePrepare : public System
    {
    public:
        explicit SystemGamePrepare(entt::registry &);
        virtual void update(float dt) override;
    };
}

#endif