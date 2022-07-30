#include "BulletManager.h"
#include <mutex>
#include "GameSettings.h"
#include "res/GameSource.h"
#include "SystemManager.h"
#include "ecs/components/ComponentBase.hpp"
#include "ecs/components/ComponentAnimation.hpp"
#include "ecs/components/ComponentAngle.h"
#include "ecs/components/ComponentBullet.h"
#include <tuple>
#include <iostream>

#define PLAYER_BOX_SIZE 1

#define PLAYER_ANGLE_SPEED 3.f

namespace danMuGame
{
    using namespace component;

    BulletManager *BulletManager::m_instance = nullptr;
    std::once_flag BulletManager::m_flag;

    BulletManager::BulletManager()
    {
    }

    BulletManager::~BulletManager()
    {
    }

    //单例获取
    BulletManager *BulletManager::GetInstance()
    {
        std::call_once(m_flag,
                       []
                       {
                           BulletManager::m_instance = new BulletManager();
                           static GC gc;
                       });
        return BulletManager::m_instance;
    }

    void BulletManager::initialization()
    {
        auto &registry = GameManager::GetInstance()->getRegistry();
        for (int i = 0; i < 10000; ++i)
        {
            auto entity = registry.create();
            registry.emplace_or_replace<ComponentBullet>(entity, false, 0, 0, 0, (entt::entity)0, (entt::entity)0, std::vector<Vector2>(), FastColor3::black);
            this->bulletVector.push_back(entity);
            this->idleBulletQueue.push(entity);
        }
    }

    entt::entity BulletManager::createBulletEntity()
    {
        auto &registry = GameManager::GetInstance()->getRegistry();

        auto entity = registry.create();
        registry.emplace_or_replace<ComponentBullet>(entity, false, 0, 0, 0, (entt::entity)0, (entt::entity)0, std::vector<Vector2>(), FastColor3::black);

        this->bulletVector.push_back(entity);

        std::cout << this->bulletVector.size() << std::endl;
        return entity;
    }

    entt::entity BulletManager::getBulletEntity()
    {
        if (this->idleBulletQueue.empty() == true)
        {
            auto entity = createBulletEntity();
            this->bulletVector.push_back(entity);
            return entity;
        }
        auto entity = this->idleBulletQueue.front();
        this->idleBulletQueue.pop();
        return entity;
    }

    void BulletManager::addIdleBulletEntity(entt::entity entity)
    {
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &bullet = registry.get<ComponentBullet>(entity);
        bullet.isUse = false;
        bullet.color = FastColor3::black;
        this->idleBulletQueue.push(entity);
    }

    void BulletManager::clearInActionBullet()
    {
        auto &registry = GameManager::GetInstance()->getRegistry();
        for (auto entity : this->bulletVector)
        {
            auto &bullet = registry.get<ComponentBullet>(entity);
            if (bullet.isUse == true)
            {
                bullet.isUse = false;
                bullet.color = FastColor3::black;
                this->idleBulletQueue.push(entity);
            }
        }
    }

}
