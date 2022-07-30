#ifndef __GAME_SETTINGS_H__
#define __GAME_SETTINGS_H__

#include <string>
#include <fstream>
#include <ostream>
#include <mutex>
#include "nlohmann/json.hpp"

namespace danMuGame
{
    class GameSettings
    {
    private:
        const char *fileName = "settings.json";

        static GameSettings *m_instance;
        static std::once_flag m_flag;

        int m_playerMaxCount;   // 玩家数量
        int m_robotMaxCount;    // 人机数量
        bool m_gameCodeIsSave;  // 主播身份码是否保存
        std::string m_gameCode; // 主播身份码

    private:
        GameSettings();
        ~GameSettings();
        GameSettings(const GameSettings &) = delete;
        GameSettings(GameSettings &&) = delete;
        GameSettings &operator=(const GameSettings &) = delete;
        GameSettings &operator=(GameSettings &&) = delete;

        class GC
        {
        public:
            ~GC()
            {
                if (GameSettings::m_instance)
                {
                    delete GameSettings::m_instance;
                    GameSettings::m_instance = nullptr;
                }
            }
        };

    public:
        static GameSettings *GetInstance();

        void initialization();
        
        void save();

    public:
        inline void setRobotCount(int count)
        {
            this->m_robotMaxCount = count;
        }
        inline void setPlayerMaxCount(int count)
        {
            this->m_playerMaxCount = count;
        }
        inline void setGameCodeIsSave(bool isSave)
        {
            this->m_gameCodeIsSave = isSave;
        }
        inline void setGameCode(const std::string &code)
        {
            this->m_gameCode = code;
        }

        inline int getRobotCount() const
        {
            return m_robotMaxCount;
        }
        inline int getPlayerMaxCount() const
        {
            return m_playerMaxCount;
        }
        inline bool getGameCodeIsSave() const
        {
            return m_gameCodeIsSave;
        }
        inline const std::string &getGameCode() const
        {
            return this->m_gameCode;
        }
    };
}
#endif // SETTINGS_H
