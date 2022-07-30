#ifndef __BULLET_MANAGER_H__
#define __BULLET_MANAGER_H__

#include <vector>
#include <stack>
#include <queue>
#include "entt/entt.hpp"
#include "GameStatue.h"
#include "scenes/GameScene.h"
#include <memory>
#include <vector>
#include "res/GameSource.h"
#include <mutex>

namespace danMuGame
{

    class SystemManager;

    class BulletManager
    {
    private:
        static BulletManager *m_instance;
        static std::once_flag m_flag;

        std::vector<entt::entity> bulletVector;
        std::queue<entt::entity> idleBulletQueue;

    private:
        BulletManager();
        ~BulletManager();
        BulletManager(const BulletManager &) = delete;
        BulletManager(BulletManager &&) = delete;
        BulletManager &operator=(const BulletManager &) = delete;
        BulletManager &operator=(BulletManager &&) = delete;

        class GC
        {
        public:
            ~GC()
            {
                if (BulletManager::m_instance)
                {
                    delete BulletManager::m_instance;
                    BulletManager::m_instance = nullptr;
                }
            }
        };

        entt::entity createBulletEntity();

    public:
        static BulletManager *GetInstance();

        void initialization();

    public:


        entt::entity getBulletEntity();
        void addIdleBulletEntity(entt::entity entity);
        void clearInActionBullet();

        inline const std::vector<entt::entity> &getBulletVector()
        {
            return this->bulletVector;
        }
    };
}

#endif