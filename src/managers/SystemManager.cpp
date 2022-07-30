#include "SystemManager.h"
#include <mutex>
#include "GameSettings.h"
#include "res/GameSource.h"
#include "ecs/systems/SystemCamera.h"
#include "ecs/systems/SystemPlayer.h"
#include "ecs/systems/SystemFindRound.h"
#include "ecs/systems/SystemGameDanMu.h"
#include "ecs/systems/SystemGamePrepare.h"
#include "ecs/systems/SystemAngle.h"
#include "ecs/systems/SystemAnimation.h"
#include "ecs/systems/SystemBullet.h"
#include "ecs/systems/SystemMove.h"
#include "ecs/systems/SystemFirearmsWeapon.h"
#include <functional>
#include <thread>

namespace danMuGame
{
    using namespace system;

    SystemManager *SystemManager::m_instance = nullptr;
    std::once_flag SystemManager::m_flag;

    SystemManager::SystemManager()
    {
    }

    SystemManager::~SystemManager()
    {
        threadCanRun = false;
        thread1isOK = false;
        thread2isOk = false;
        updateCond1.notify_one();
        updateCond2.notify_one();
        thread1_mutex.unlock();
        thread2_mutex.unlock();
        thread1.join();
        thread2.join();
    }

    SystemManager *SystemManager::GetInstance()
    {
        std::call_once(m_flag,
                       []
                       {
                           SystemManager::m_instance = new SystemManager();
                           static GC gc;
                       });
        return SystemManager::m_instance;
    }

    void SystemManager::initialization()
    {
        auto &registry = GameManager::GetInstance()->getRegistry();

        this->m_systemCamera = std::make_shared<SystemCamera>(registry);
        this->m_systemPlayer = std::make_shared<SystemPlayer>(registry);
        this->m_systemFindRound = std::make_shared<SystemFindRound>(registry);
        this->m_systemGameDanMu = std::make_shared<SystemGameDanMu>(registry);
        this->m_systemGamePrepare = std::make_shared<SystemGamePrepare>(registry);
        this->m_systemAngle = std::make_shared<SystemAngle>(registry);
        this->m_systemAnimation = std::make_shared<SystemAnimation>(registry);
        this->m_systemBullet = std::make_shared<SystemBullet>(registry);
        this->m_systemMove = std::make_shared<SystemMove>(registry);
        this->m_systemFirearmsWeapon = std::make_shared<SystemFirearmsWeapon>(registry);

        thread1 = std::thread(std::bind(&SystemManager::threadFunction1, this));
        thread2 = std::thread(std::bind(&SystemManager::threadFunction2, this));
    }

    void SystemManager::update(float dt) noexcept
    {
        this->thread1isOK = false;
        this->thread2isOk = false;
        this->m_second = dt;
        updateCond1.notify_one();
        updateCond2.notify_one();
        // thread1_mutex.unlock();
        // thread2_mutex.unlock();
    }

    void SystemManager::threadFunction1(void) noexcept
    {
        while (threadCanRun)
        {
            std::unique_lock<std::mutex> lock(thread1_mutex);
            updateCond1.wait(lock, [&]
                             { return thread1isOK == false; });

            /********************************************/
            if (threadCanRun == false)
            {
                break;
            }
            /********************************************/

            switch (GameManager::GetInstance()->getGameStatue())
            {
            case GameStatue::Running:
                // std::cout << "update : 1" << std::endl;
                this->m_systemPlayer->update(m_second);
                // std::cout << "update : 2" << std::endl;
                this->m_systemMove->update(m_second);
                // std::cout << "update : 3" << std::endl;
                this->m_systemAnimation->update(m_second);
                // std::cout << "update : 4" << std::endl;
                this->m_systemAngle->update(m_second);
                // std::cout << "update : 5" << std::endl;
                this->m_systemCamera->update(m_second);
                // std::cout << "update : 6" << std::endl;
                this->m_systemBullet->update(m_second);
                // std::cout << "update : 7" << std::endl;
                this->m_systemFirearmsWeapon->update(m_second);
                // std::cout << "update : 8" << std::endl;
                break;
            case GameStatue::FrontEnd:
                // this->m_systemPlayer->update(m_second);
                this->m_systemMove->update(m_second);
                this->m_systemAnimation->update(m_second);
                this->m_systemAngle->update(m_second);
                this->m_systemCamera->update(m_second);
                this->m_systemBullet->update(m_second);
                // this->m_systemFirearmsWeapon->update(m_second);
                break;
            case GameStatue::Prepare:
                this->m_systemGamePrepare->update(m_second);
                this->m_systemAnimation->update(m_second);
                break;
            case GameStatue::Distribute:
                this->m_systemGamePrepare->update(m_second);
                break;
            }

            /********************************************/

            this->thread1isOK = true;
            thread1_mutex.unlock();
        }
    }

    void SystemManager::threadFunction2(void) noexcept
    {
        while (threadCanRun)
        {
            // thread2_mutex.lock();
            std::unique_lock<std::mutex> lock(thread2_mutex);
            updateCond2.wait(lock, [&]
                             { return thread2isOk == false; });

            /********************************************/
            if (threadCanRun == false)
            {
                break;
            }
            /********************************************/

            switch (GameManager::GetInstance()->getGameStatue())
            {
            case GameStatue::Running:
                // std::cout << "2 update : 1" << std::endl;
                this->m_systemGameDanMu->update(m_second);
                // std::cout << "2 update : 2" << std::endl;
                this->m_systemFindRound->update(m_second);
                // std::cout << "2 update : 3" << std::endl;
                break;
            case GameStatue::FrontEnd: // 结算前不再接受弹幕指令
                // this->m_systemGameDanMu->update(m_second);
                this->m_systemFindRound->update(m_second);
                break;
            case GameStatue::Prepare:
                this->m_systemGameDanMu->update(m_second);
                break;
            }

            /********************************************/

            this->thread2isOk = true;
            thread2_mutex.unlock();
        }
    }

}
