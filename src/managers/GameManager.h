#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include <vector>
#include <stack>
#include "entt/entt.hpp"
#include "GameStatue.h"
#include "scenes/GameScene.h"
#include <memory>
#include "res/GameMap.h"
#include "ScoreData.hpp"
#include <mutex>


namespace danMuGame
{
    class SystemManager;
    class PlayerManager;
    class GameSettings;
    class BulletManager;

    class GameManager
    {
    private:
        static GameManager *m_instance;
        static std::once_flag m_flag;
        GameStatue m_gameStatue;
        entt::registry m_registry;
        GameScene *m_gameScene = nullptr;
        
        // SystemManager *m_systemManager = nullptr;
        // PlayerManager *m_playerManager = nullptr;
        // BulletManager *m_bulletManager = nullptr;
        // GameSettings *m_setting = nullptr;

        GameMap *m_nowGameMap = nullptr;
        int32_t frameCount = 0;
        int32_t nowTimeValue = 0;

        int32_t timeCount;

        int32_t playerReturnTime;

        std::vector<ScoreData> scoreVecCatch;
        std::vector<InvestData> investVecCatch;

    private:
        GameManager();
        ~GameManager();
        GameManager(const GameManager &) = delete;
        GameManager(GameManager &&) = delete;
        GameManager &operator=(const GameManager &) = delete;
        GameManager &operator=(GameManager &&) = delete;

        class GC
        {
        public:
            ~GC()
            {
                if (GameManager::m_instance)
                {
                    delete GameManager::m_instance;
                    GameManager::m_instance = nullptr;
                }
            }
        };

    public:
        static GameManager *GetInstance();

        void initialization();

    public:

        int32_t gameTestCount = 0;

        void gameRestart() noexcept;

        void update(float dt) noexcept;

        void update_1_second() noexcept;

        void setGameStatue(GameStatue gameStatue) noexcept;

        inline GameStatue getGameStatue()
        {
            return this->m_gameStatue;
        }

        inline void setScene(GameScene *gameScene)
        {
            this->m_gameScene = gameScene;
            gameScene->setRegistry(&m_registry);
        }

        inline entt::registry &getRegistry()
        {
            return this->m_registry;
        }

        inline GameMap *getNowGameMap()
        {
            return this->m_nowGameMap;
        }

        inline int32_t getNowTime()
        {
            return this->nowTimeValue;
        }
    };
}

#endif
