#ifndef __SYSTEM_GAME_DAN_MU_H__
#define __SYSTEM_GAME_DAN_MU_H__

#include "System.h"

namespace danMuGame::system
{
    class SystemGameDanMu : public System
    {
    public:
        explicit SystemGameDanMu(entt::registry &);
        virtual void update(float dt) override;

    private:
        std::string getNormalWeapon();
        std::string getHighLevelWeapon();
    };
}

#endif