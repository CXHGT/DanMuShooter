#include "IntermediateProcess.h"
#include "ECS/Components/ComponentFirearmsWeapon.h"
#include "ECS/Components/ComponentBullet.h"
#include "ECS/Components/ComponentAngle.h"
#include "ECS/Components/ComponentBase.hpp"
#include "Managers/BulletManager.h"
#include "Managers/GameManager.h"
#include "CxhUtils.h"
#include <random>
#include <iostream>

using namespace danMuGame::component;

namespace danMuGame::IntermediateProcess
{

    static std::random_device m_random;

    void FirearmsWeaponShoot(FirearmsWeapon &firearmsWeapon, entt::entity attacker, entt::entity target)
    {
        auto *weaponBase = firearmsWeapon.weaponBase;
        if (firearmsWeapon.isCanShoot == false || weaponBase->speed <= 0)
        {
            return;
        }
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &attackerMove = registry.get<ComponentMove>(attacker);
        auto &targetMove = registry.get<ComponentMove>(target);
        auto angle = CxhUtils::getAngle(attackerMove.position, targetMove.position);
        auto &componentAngle = registry.get<ComponentAngle>(attacker);

        // 与目标的角度大于20度则不能攻击
        if (abs(componentAngle.angle - angle) > 20.f)
        {
            return;
        }

        // 获取与目标的距离
        auto distance = CxhUtils::getEuclideanDistance(attackerMove.position, targetMove.position);
        // 超过枪械限制的攻击距离，则不攻击
        if (distance > weaponBase->attackDistance)
        {
            return;
        }

        // 子弹路线有障碍物挡着
        if (isCanMoveByLine(attackerMove.position, targetMove.position, 0) == false)
        {
            return;
        }

        auto relativelyVector2 = Vector2(cos(angle * DEC), sin(angle * DEC));

        int32_t accuracy = 100 - weaponBase->accuracy;
        if (accuracy < 1)
        {
            accuracy = 1;
        }

        for (int bulletNum = 0; bulletNum < weaponBase->shootBulletCount; ++bulletNum)
        {
            auto bulletEntity = BulletManager::GetInstance()->getBulletEntity();
            auto &bullet = registry.get<ComponentBullet>(bulletEntity);

            bullet.moveRoute.clear();
            bullet.endIndex = 0;
            bullet.startIndex = 0;
            bullet.injure = weaponBase->injure;
            bullet.attacker = attacker;
            bullet.targetEntity = target;

            int32_t offsetX = (100 - (m_random() % 200)) % accuracy;
            int32_t offsetY = (100 - (m_random() % 200)) % accuracy;

            auto startVector2 = attackerMove.position + relativelyVector2 * 45;
            auto endVector2 = Vector2(targetMove.position.x() + offsetX, targetMove.position.y() + offsetY);

            // 重新计算角度和距离
            angle = CxhUtils::getAngle(startVector2, endVector2);
            distance = CxhUtils::getEuclideanDistance(startVector2, endVector2);

            int32_t num = distance / weaponBase->speed;
            bullet.moveRoute.push_back(startVector2);

            float rx = cos(angle * DEC);
            float ry = sin(angle * DEC);

            for (int32_t index = 1; index < num - 1; ++index)
            {
                bullet.moveRoute.push_back(Vector2(rx * weaponBase->speed * index + startVector2.x(), ry * weaponBase->speed * index + startVector2.y()));
            }
            bullet.moveRoute.push_back(endVector2);
            bullet.isUse = true;
        }

        // 枪械数据设置
        firearmsWeapon.isCanShoot = false;
        firearmsWeapon.currentBullets--;
        firearmsWeapon.currentIntervalFrame = 0;
    }

    void FirearmsWeaponShoot2(FirearmsWeapon &firearmsWeapon, const Vector2 &startVector, const Vector2 &endVector, entt::entity attacker, entt::entity target)
    {
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto *weaponBase = firearmsWeapon.weaponBase;
        if (firearmsWeapon.isCanShoot == false)
        {
            return;
        }
        // 子弹路线有障碍物挡着
        if (isCanMoveByLine(startVector, endVector, 0) == false)
        {
            return;
        }

        int32_t accuracy = 100 - weaponBase->accuracy;
        if (accuracy < 1)
        {
            accuracy = 1;
        }

        for (int bulletNum = 0; bulletNum < weaponBase->shootBulletCount; ++bulletNum)
        {
            auto bulletEntity = BulletManager::GetInstance()->getBulletEntity();
            auto &bullet = registry.get<ComponentBullet>(bulletEntity);

            bullet.moveRoute.clear();
            bullet.endIndex = 0;
            bullet.startIndex = 0;
            bullet.injure = weaponBase->injure;
            bullet.attacker = attacker;
            bullet.targetEntity = target;

            Vector2 endVector2(endVector.x() + (100 - (m_random() % 200)) % accuracy, endVector.y() + (100 - (m_random() % 200)) % accuracy);

            float angle = CxhUtils::getAngle(startVector, endVector2);
            float distance = CxhUtils::getEuclideanDistance(startVector, endVector2);

            int num = distance / weaponBase->speed;
            bullet.moveRoute.push_back(startVector);

            float rx = cos(angle * DEC);
            float ry = sin(angle * DEC);

            for (int index = 1; index < num - 1; ++index)
            {
                bullet.moveRoute.push_back(Vector2(rx * weaponBase->speed * index + startVector.x(), ry * weaponBase->speed * index + startVector.y()));
            }
            bullet.moveRoute.push_back(endVector2);
            bullet.isUse = true;
        }

        // 枪械数据设置
        firearmsWeapon.isCanShoot = false;
        firearmsWeapon.currentBullets--;
        firearmsWeapon.currentIntervalFrame = 0;
    }

    bool isCanMoveByLine(const Vector2 &startVector, const Vector2 &endVector, int32_t boxSize)
    {
        GameMap *gameMap = GameManager::GetInstance()->getNowGameMap();
        // static std::vector<Point2> catchMoveRoute;
        // catchMoveRoute.clear();

        std::vector<Point2> catchMoveRoute;

        CxhUtils::getLinePoint2(catchMoveRoute, gameMap->RealVecotr2ToRelativePoint2(startVector), gameMap->RealVecotr2ToRelativePoint2(endVector));
        for (const auto &point : catchMoveRoute)
        {
            if (gameMap->isCanMove(point, boxSize) == false)
            {
                return false;
            }
        }
        return true;
    }
}