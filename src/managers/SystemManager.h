#ifndef __SYSTEM_MANAGER_H__
#define __SYSTEM_MANAGER_H__

#include <vector>
#include "entt/entt.hpp"
#include "GameManager.h"
#include <memory>
#include <mutex>
#include <iostream>
#include <condition_variable>

namespace danMuGame
{
    namespace system
    {
        class SystemCamera;
        class SystemPlayer;
        class SystemFindRound;
        class SystemGameDanMu;
        class SystemGamePrepare;
        class SystemAngle;
        class SystemAnimation;
        class SystemBullet;
        class SystemMove;
        class SystemFirearmsWeapon;
    }

    class SystemManager
    {
    private:
        static SystemManager *m_instance;
        static std::once_flag m_flag;

        std::shared_ptr<system::SystemCamera> m_systemCamera = nullptr;
        std::shared_ptr<system::SystemPlayer> m_systemPlayer = nullptr;
        std::shared_ptr<system::SystemFindRound> m_systemFindRound = nullptr;
        std::shared_ptr<system::SystemGameDanMu> m_systemGameDanMu = nullptr;
        std::shared_ptr<system::SystemGamePrepare> m_systemGamePrepare = nullptr;
        std::shared_ptr<system::SystemAngle> m_systemAngle = nullptr;
        std::shared_ptr<system::SystemAnimation> m_systemAnimation = nullptr;
        std::shared_ptr<system::SystemBullet> m_systemBullet = nullptr;
        std::shared_ptr<system::SystemMove> m_systemMove = nullptr;
        std::shared_ptr<system::SystemFirearmsWeapon> m_systemFirearmsWeapon = nullptr;

        bool thread1isOK = true;
        bool thread2isOk = true;

        bool threadCanRun = true;
        float m_second = 0.f;

        std::thread thread1;
        std::thread thread2;
        std::mutex thread1_mutex;
        std::mutex thread2_mutex;
        std::condition_variable updateCond1;
        std::condition_variable updateCond2;

    private:
        SystemManager();
        ~SystemManager();
        SystemManager(const SystemManager &) = delete;
        SystemManager(SystemManager &&) = delete;
        SystemManager &operator=(const SystemManager &) = delete;
        SystemManager &operator=(SystemManager &&) = delete;

        class GC
        {
        public:
            ~GC()
            {
                if (SystemManager::m_instance)
                {
                    delete SystemManager::m_instance;
                    SystemManager::m_instance = nullptr;
                }
            }
        };

        void threadFunction1(void) noexcept;
        void threadFunction2(void) noexcept;

    public:
        // 获取单例
        static SystemManager *GetInstance();

        void initialization();

    public:
        void update(float second) noexcept;

        inline bool isUpdateOk()
        {
            return (this->thread1isOK == true && this->thread2isOk == true);
        }

        inline void resetUpdateValue()
        {
            thread1isOK = true;
            thread2isOk = true;
        }
    };
}

#endif
