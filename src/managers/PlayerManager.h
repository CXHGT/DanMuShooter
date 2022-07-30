#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include <vector>
#include <stack>
#include "entt/entt.hpp"
#include "GameStatue.h"
#include "scenes/GameScene.h"
#include <memory>
#include <vector>
#include "res/GameSource.h"
#include <unordered_map>
#include <list>
#include <mutex>

#define PLAYER_BOX_SIZE 1
#define NEARBY_ENEMY_DISTANCE 1200
#define LOST_ENEMY_DISTANCE 1600    // 丢失敌人位置
#define PLAYER_ANGLE_SPEED 3.f      // 基础角度转动速度
#define WIDE_ANGLE 105.f            // 广角，发现敌人的角度(*2)
#define ESSENTIAL_HEALTH 1000       // 基础生命值

namespace danMuGame
{
    class SystemManager;

    class PlayerManager
    {
    private:
        static PlayerManager *m_instance;
        static std::once_flag m_flag;

        std::vector<entt::entity> playerVector;
        GameSource *m_gameSource = nullptr;
        int32_t useIndex = 0;

        std::unordered_map<int32_t, entt::entity> uidEntityTable; // uid对应的玩家表
        std::vector<entt::entity> playerInGame;                   // 参加游戏的玩家
        std::list<entt::entity> remainingPlayerList;              // 剩余的玩家
        std::vector<std::list<entt::entity>> teamTable;           // 玩家队伍表
        std::list<entt::entity> remainingUserPlayerList;          // 没有电脑的剩余玩家

        std::vector<entt::entity> waitingToReturnPlayer; // 等待返回战场的玩家

    private:
        PlayerManager();
        ~PlayerManager();
        PlayerManager(const PlayerManager &) = delete;
        PlayerManager(PlayerManager &&) = delete;
        PlayerManager &operator=(const PlayerManager &) = delete;
        PlayerManager &operator=(PlayerManager &&) = delete;

        class GC
        {
        public:
            ~GC()
            {
                if (PlayerManager::m_instance)
                {
                    delete PlayerManager::m_instance;
                    PlayerManager::m_instance = nullptr;
                }
            }
        };

    public:
        static PlayerManager *GetInstance();
        
        void initialization();

    public:
        inline std::vector<entt::entity> &getPlayerVector()
        {
            return playerVector;
        }

        inline std::unordered_map<int32_t, entt::entity> &getUidEntityTable()
        {
            return uidEntityTable;
        }

        inline std::vector<entt::entity> &getPlayerInGame()
        {
            return this->playerInGame;
        }

        inline std::list<entt::entity> &getRemainingPlayerList()
        {
            return this->remainingPlayerList;
        }

        inline std::list<entt::entity> &getRemainingUserPlayerList()
        {
            return this->remainingUserPlayerList;
        }

        inline std::vector<std::list<entt::entity>> &getTeamTable()
        {
            return this->teamTable;
        }

        inline std::vector<entt::entity> &getWaitingToReturnPlayer()
        {
            return this->waitingToReturnPlayer;
        }

        void playerReturnBattlefield(); // 玩家返回战场

        inline void addReturnTable(entt::entity entity)
        {
            waitingToReturnPlayer.push_back(entity);
        }

        void createPlayers(int32_t count);
        void resetPlayer(entt::entity &entity);
        void resetAllPlayer();

        inline int32_t getUseIndex()
        {
            return this->useIndex;
        }

        entt::entity usePlayer();

        entt::entity usePlayerByUidAndUsername(int32_t uid, const std::string &username);
    };
}

#endif