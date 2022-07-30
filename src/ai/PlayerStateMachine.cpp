#include "PlayerStateMachine.h"
#include "ECS/Components/ComponentAnimation.hpp"
#include "ECS/Components/ComponentAngle.h"
#include "ECS/Components/ComponentBase.hpp"
#include "ECS/Components/ComponentHealth.h"
#include "ECS/IntermediateProcess.h"
#include "Managers/GameManager.h"
#include "res/GameSource.h"
#include "managers/PlayerManager.h"
#include <stdint.h>
#include <iostream>
#include <random>
#include "CxhUtils.h"
#include "fmt/format.h"

using namespace danMuGame::component;

static std::random_device rd;

#define FIND_ROUND_RADIUS 30
#define RECOVER_PROPORTION 0.7f

#define DELEGATE()

namespace danMuGame // 空闲状态 Idle
{

    /************************** Player_Idle *********************************/
    void Player_Idle::enter(Control &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &moveStack = registry.get<ComponentMoveStack>(entity);
        auto &componentAngle = registry.get<ComponentAngle>(entity);
        moveStack.Clear();
        componentAngle.gotoAngle = componentAngle.angle;
    }
    void Player_Idle::update(FullControl &control) noexcept
    {

        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &componentHealth = registry.get<ComponentHealth>(entity);
        if (componentHealth.health <= 0)
        {
            control.changeTo<Player_Death>();
            return;
        }

        auto &player = registry.get<ComponentPlayer>(entity);
        auto &move = registry.get<ComponentMove>(entity);
        auto &moveStack = registry.get<ComponentMoveStack>(entity);
        auto &findRound = registry.get<ComponentFindRound>(entity);
        auto &crisis = registry.get<ComponentCrisis>(entity);
        auto &health = registry.get<ComponentHealth>(entity);
        auto &componentAngle = registry.get<ComponentAngle>(entity);

        if (player.skill == PlayerSkill::RECOVER && health.health > 0)
        {
            auto &healthCount = control.context().recoveryHealthCount;
            if (healthCount > 15)
            {
                healthCount = 0;
                health.Health(1);
            }
            else
            {
                ++healthCount;
            }
        }

        switch (crisis.situation)
        {
        case PlayerSituation::NORMAL:
            DELEGATE()
            {
                if (crisis.targetEnemy != (entt::entity)UINT32_MAX)
                {
                    control.changeTo<Player_FindTheEnemy>();
                    return;
                }
                // 寻路
                if (moveStack.moveRoundStack.empty() == true && findRound.needFindRound == false)
                {

                    GameMap *gameMap = GameManager::GetInstance()->getNowGameMap();

                    int moveX = FIND_ROUND_RADIUS - rd() % (FIND_ROUND_RADIUS * 2);
                    int moveY = FIND_ROUND_RADIUS - rd() % (FIND_ROUND_RADIUS * 2);

                    findRound.needFindRound = true;
                    findRound.startRelativePosition = gameMap->RealPositionToRelativePosition(move.position.toPoint2());

                    Point2 movePoint(findRound.startRelativePosition.x() + moveX, findRound.startRelativePosition.y() + moveY);

                    findRound.endRelativePosition = movePoint;
                }
                if (crisis.nearestEnemy != (entt::entity)UINT32_MAX)
                {
                    auto &enemyMove = registry.get<ComponentMove>(crisis.nearestEnemy);
                    auto angle = CxhUtils::getAngle(move.position, enemyMove.position);
                    if ((abs(componentAngle.angle - angle) < WIDE_ANGLE && IntermediateProcess::isCanMoveByLine(move.position, enemyMove.position, 1)) ||
                        CxhUtils::getEuclideanDistance(move.position, enemyMove.position) < 150)
                    {
                        // player.name = "find";
                        crisis.targetEnemy = crisis.nearestEnemy;
                        /**
                         *
                         * 通知队友
                         *
                         */
                        crisis.situation = PlayerSituation::FIND_ENEMY;
                        control.changeTo<Player_FindTheEnemy>();
                        return;
                    }
                }
                for (auto it = crisis.nearbyEnemies.begin(); it != crisis.nearbyEnemies.end(); ++it)
                {
                    auto &enemyMove = registry.get<ComponentMove>(*it);
                    auto angle = CxhUtils::getAngle(move.position, enemyMove.position);
                    if (abs(componentAngle.angle - angle) < WIDE_ANGLE && IntermediateProcess::isCanMoveByLine(move.position, enemyMove.position, 1))
                    {
                        // player.name = "find";
                        crisis.targetEnemy = *it;
                        /**
                         *
                         * 通知队友
                         *
                         */
                        crisis.situation = PlayerSituation::FIND_ENEMY;
                        control.changeTo<Player_FindTheEnemy>();
                        return;
                    }
                }
                if (player.uid < 0 && componentHealth.health < componentHealth.maxHealth * RECOVER_PROPORTION)
                {
                    control.changeTo<Player_Treat>();
                }
            }
            break;
        case PlayerSituation::BE_ATTACKED:
            /**
             *
             * 通知队友，自己被攻击了
             *
             */
            control.changeTo<Player_BeAttacked>();
            break;
        }
    }

    /************************** Player_Stop *********************************/

    void Player_Stop::enter(Control &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &move = registry.get<ComponentMove>(entity);
        auto &moveStack = registry.get<ComponentMoveStack>(entity);
        auto &animation = registry.get<ComponentAnimation>(entity);
        moveStack.Clear();
        GameSource::GetInstance()->initializationAnimation(animation, "PlayerStand_Basic");
        move.speed = 0.f;
    }

    void Player_Stop::update(FullControl &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &moveStack = registry.get<ComponentMoveStack>(entity);
        if (moveStack.moveRoundStack.getSize() > 0)
        {
            if (GameManager::GetInstance()->getGameStatue() == GameStatue::Prepare || GameManager::GetInstance()->getGameStatue() == GameStatue::Distribute)
            {
                std::cout << "???" << std::endl;
            }
            control.changeTo<Player_Walk>();
        }
    }

    /************************** Player_Walk *********************************/
    void Player_Walk::enter(Control &control) noexcept
    {
        // std::cout << "walk enter" << std::endl;
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &move = registry.get<ComponentMove>(entity);
        auto &componentAngle = registry.get<ComponentAngle>(entity);

        auto &animation = registry.get<ComponentAnimation>(entity);
        GameSource::GetInstance()->initializationAnimation(animation, "PlayerWalk_Basic");
        move.speed = 1.f;
        componentAngle.speed = PLAYER_ANGLE_SPEED;
    }

    void Player_Walk::update(FullControl &control) noexcept
    {
        auto &count = control.context().runTestCount;
        if (count > 800)
        {
            count = 0;
            control.changeTo<Player_Run>();
        }
        ++count;
    }

    /************************** Player_Run *********************************/
    void Player_Run::enter(Control &control) noexcept
    {
        // std::cout << "run enter" << std::endl;
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &move = registry.get<ComponentMove>(entity);
        auto &componentAngle = registry.get<ComponentAngle>(entity);
        auto &animation = registry.get<ComponentAnimation>(entity);
        GameSource::GetInstance()->initializationAnimation(animation, "PlayerRun_Basic");
        move.speed = 2.f;
        componentAngle.speed = 5.f;
        componentAngle.weight = 0;
    }

    void Player_Run::update(FullControl &control) noexcept
    {
        auto &count = control.context().runTestCount;
        if (count > 800)
        {
            count = 0;
            control.changeTo<Player_Walk>();
        }
        ++count;
    }

    /************************** Player_Treat *********************************/
    void Player_Treat::enter(Control &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &move = registry.get<ComponentMove>(entity);
        auto &moveStack = registry.get<ComponentMoveStack>(entity);
        auto &animation = registry.get<ComponentAnimation>(entity);
        GameSource::GetInstance()->initializationAnimation(animation, "PlayerStand_Basic");
        move.speed = 0.f;
        moveStack.Clear();
    }

    void Player_Treat::update(FullControl &control) noexcept
    {
        auto &count = control.context().runTestCount;
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &componentHealth = registry.get<ComponentHealth>(entity);
        if (count > 40)
        {
            count = 0;
            componentHealth.Health(10);
        }
        ++count;
        if (componentHealth.health > componentHealth.maxHealth * RECOVER_PROPORTION)
        {
            control.changeTo<Player_Walk>();
        }
    }

}

//=========================================================================================================================================//
//=========================================================================================================================================//
//=========================================================================================================================================//
//=========================================================================================================================================//

namespace danMuGame // Alert
{
    /************************** Player_Alert *********************************/
    void Player_Alert::enter(Control &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &moveStack = registry.get<ComponentMoveStack>(entity);
        auto &componentAngle = registry.get<ComponentAngle>(entity);
        moveStack.Clear();
        componentAngle.gotoAngle = componentAngle.angle;

        auto &animation = registry.get<ComponentAnimation>(entity);
        auto &move = registry.get<ComponentMove>(entity);
        GameSource::GetInstance()->initializationAnimation(animation, "PlayerWalk_Basic");
        move.speed = 1.f;
        componentAngle.speed = PLAYER_ANGLE_SPEED;
        componentAngle.weight = 0;
    }

    void Player_Alert::update(FullControl &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &crisis = registry.get<ComponentCrisis>(entity);

        auto &player = registry.get<ComponentPlayer>(entity);
        auto &componentHealth = registry.get<ComponentHealth>(entity);
        if (player.skill == PlayerSkill::RECOVER && componentHealth.health > 0)
        {
            auto &healthCount = control.context().recoveryHealthCount;
            if (healthCount > 15)
            {
                healthCount = 0;
                componentHealth.Health(1);
            }
            else
            {
                ++healthCount;
            }
        }

        if (componentHealth.health <= 0)
        {
            control.changeTo<Player_Death>();
            return;
        }

        if (crisis.situation == PlayerSituation::BE_ATTACKED)
        {
            control.changeTo<Player_BeAttacked>();
            return;
        }
    }

    /************************** Player_DetectDanger *********************************/
    void Player_DetectDanger::enter(Control &control) noexcept
    {
    }

    void Player_DetectDanger::update(FullControl &control) noexcept
    {
    }

    /************************** Player_SearchEnemy *********************************/
    void Player_SearchEnemy::enter(Control &control) noexcept
    {
    }

    void Player_SearchEnemy::update(FullControl &control) noexcept
    {
    }

    /************************** Player_BeAttacked *********************************/
    void Player_BeAttacked::enter(Control &control) noexcept
    {
    }

    void Player_BeAttacked::update(FullControl &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &crisis = registry.get<ComponentCrisis>(entity);
        // auto &testCount = control.context().testCount;
        if (crisis.beAttackedAtEnemy != (entt::entity)UINT32_MAX)
        {
            auto &player = registry.get<ComponentPlayer>(entity);
            // player.name = "beAttacked";
            crisis.targetEnemy = crisis.beAttackedAtEnemy;
            crisis.situation = PlayerSituation::WAR;
            control.changeTo<Player_OpenFire>();
            return;
            // if (testCount > 3)
            // {
            //     testCount = 0;
            //     for (auto it = crisis.nearbyEnemies.begin(); it != crisis.nearbyEnemies.end(); ++it)
            //     {
            //         auto &enemyMove = registry.get<ComponentMove>(*it);
            //         auto angle = CxhUtils::getAngle(move.position, enemyMove.position);
            //         if (abs(componentAngle.angle - angle) < WIDE_ANGLE && IntermediateProcess::isCanMoveByLine(move.position, enemyMove.position, 1))
            //         {
            //             crisis.targetEnemy = *it;
            //             /**
            //              *
            //              * 通知队友
            //              *
            //              */
            //             crisis.situation = PlayerSituation::FIND_ENEMY;
            //             control.changeTo<Player_FindTheEnemy>();
            //             return;
            //         }
            //     }
            // }
            // else
            // {
            //     ++testCount;
            // }
        }
    }

    /************************** Player_FindTheEnemy *********************************/
    void Player_FindTheEnemy::enter(Control &control) noexcept
    {
    }

    void Player_FindTheEnemy::update(FullControl &control) noexcept
    {
        entt::entity entity = control.context().entity;

        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &crisis = registry.get<ComponentCrisis>(entity);
        if (crisis.targetEnemy != (entt::entity)UINT32_MAX)
        {
            auto &move = registry.get<ComponentMove>(entity);
            auto &moveStack = registry.get<ComponentMoveStack>(entity);
            auto &findRound = registry.get<ComponentFindRound>(entity);

            auto &componentFirearmsWeapon = registry.get<ComponentFirearmsWeapon>(entity);

            auto &targetMove = registry.get<ComponentMove>(crisis.targetEnemy);

            auto &targetHealth = registry.get<ComponentHealth>(crisis.targetEnemy);
            if (targetHealth.health <= 0)
            {
                crisis.targetEnemy = (entt::entity)UINT32_MAX;
                crisis.situation = PlayerSituation::NORMAL;
                control.changeTo<Player_Walk>();
            }

            if (componentFirearmsWeapon.mainWeapon.weaponBase != nullptr)
            {
                auto distance = CxhUtils::getEuclideanDistance(move.position, targetMove.position);
                if (distance < componentFirearmsWeapon.mainWeapon.weaponBase->attackDistance - 50) // 保证在打击范围内
                {
                    crisis.situation = PlayerSituation::WAR;
                    auto &player = registry.get<ComponentPlayer>(entity);
                    // player.name = "FindTheEnemy";
                    control.changeTo<Player_OpenFire>();
                    return;
                }
            }

            // 这里或许要判断敌人是否是卡死状态
            if (moveStack.moveRoundStack.empty() == true && findRound.needFindRound == false)
            {
                GameMap *gameMap = GameManager::GetInstance()->getNowGameMap();
                findRound.needFindRound = true;
                findRound.startRelativePosition = gameMap->RealPositionToRelativePosition(move.position.toPoint2());
                findRound.endRelativePosition = gameMap->RealPositionToRelativePosition(targetMove.position.toPoint2());
            }
        }
        else
        {
            crisis.situation = PlayerSituation::NORMAL;
            control.changeTo<Player_Walk>();
            // crisis.situation = PlayerSituation::FIND_ENEMY;
            // control.changeTo<Player_SearchEnemy>();
            // return;
        }
    }
}

//=========================================================================================================================================//
//=========================================================================================================================================//
//=========================================================================================================================================//
//=========================================================================================================================================//

namespace danMuGame
{
    /************************** Player_War *********************************/
    void Player_War::enter(Control &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &moveStack = registry.get<ComponentMoveStack>(entity);
        auto &componentAngle = registry.get<ComponentAngle>(entity);
        moveStack.Clear();
        componentAngle.gotoAngle = componentAngle.angle;

        auto &animation = registry.get<ComponentAnimation>(entity);
        auto &move = registry.get<ComponentMove>(entity);
        GameSource::GetInstance()->initializationAnimation(animation, "PlayerWalk_Basic");
        move.speed = 1.f;
        componentAngle.speed = PLAYER_ANGLE_SPEED;
        componentAngle.weight = 0;

        auto &player = registry.get<ComponentPlayer>(entity);
        auto &componentHealth = registry.get<ComponentHealth>(entity);
        if (player.skill == PlayerSkill::RECOVER && componentHealth.health > 0)
        {
            auto &healthCount = control.context().recoveryHealthCount;
            if (healthCount > 15)
            {
                healthCount = 0;
                componentHealth.Health(1);
            }
            else
            {
                ++healthCount;
            }
        }
    }

    void Player_War::update(FullControl &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &componentHealth = registry.get<ComponentHealth>(entity);
        auto &move = registry.get<ComponentMove>(entity);
        auto &componentAngle = registry.get<ComponentAngle>(entity);

        auto &player = registry.get<ComponentPlayer>(entity);

        if (componentHealth.health <= 0)
        {
            control.changeTo<Player_Death>();
            return;
        }
        auto &crisis = registry.get<ComponentCrisis>(entity);
        if (crisis.situation == PlayerSituation::BE_ATTACKED)
        {
            auto &attackEnemyMove = registry.get<ComponentMove>(crisis.beAttackedAtEnemy);
            if (crisis.targetEnemy != (entt::entity)UINT32_MAX)
            {
                auto &targetMove = registry.get<ComponentMove>(crisis.targetEnemy);
                if (CxhUtils::getEuclideanDistance(attackEnemyMove.position, move.position) < CxhUtils::getEuclideanDistance(targetMove.position, move.position))
                {
                    // player.name = "war be attacker";
                    crisis.targetEnemy = crisis.beAttackedAtEnemy;
                    crisis.situation = PlayerSituation::WAR;
                }
            }
            else
            {
                crisis.targetEnemy = crisis.beAttackedAtEnemy;
                crisis.situation = PlayerSituation::WAR;
            }
        }

        if (crisis.nearestEnemy != crisis.targetEnemy && crisis.nearestEnemy != (entt::entity)UINT32_MAX)
        {
            auto &nearestMove = registry.get<ComponentMove>(crisis.nearestEnemy);
            auto &nearestHealth = registry.get<ComponentHealth>(crisis.nearestEnemy);
            if (nearestHealth.health <= 0)
            {
                crisis.nearbyEnemies.remove(crisis.nearestEnemy);
                crisis.nearestEnemy = (entt::entity)UINT32_MAX;
            }
            else
            {
                if (IntermediateProcess::isCanMoveByLine(move.position, nearestMove.position, 0) == true &&
                    abs(componentAngle.angle - CxhUtils::getAngle(move.position, nearestMove.position)) < WIDE_ANGLE &&
                    CxhUtils::getEuclideanDistance(move.position, nearestMove.position) < NEARBY_ENEMY_DISTANCE)
                {
                    // player.name = "nearset enemy";
                    crisis.targetEnemy = crisis.nearestEnemy;
                }
            }
        }
        if (crisis.targetEnemy != (entt::entity)UINT32_MAX)
        {
            auto &enemyHealth = registry.get<ComponentHealth>(crisis.targetEnemy);
            auto &enemyMove = registry.get<ComponentMove>(crisis.targetEnemy);
            if (enemyHealth.health <= 0)
            {
                crisis.targetEnemy = (entt::entity)UINT32_MAX;
            }
            else
            {
                auto &count = control.context().lostEnemyCount;
                if (IntermediateProcess::isCanMoveByLine(move.position, enemyMove.position, 0) == true)
                {
                    count = 0;
                }
                else
                {
                    ++count;
                }
                if (count > 800)
                {
                    crisis.targetEnemy = (entt::entity)UINT32_MAX;
                }
                else if (CxhUtils::getEuclideanDistance(move.position, enemyMove.position) > LOST_ENEMY_DISTANCE)
                {
                    crisis.targetEnemy = (entt::entity)UINT32_MAX;
                }
            }
        }

        if (crisis.targetEnemy == (entt::entity)UINT32_MAX)
        {
            // 判断队友是否在战斗，是否需要支援
            crisis.situation = PlayerSituation::NORMAL;
            control.changeTo<Player_Walk>();
            return;
        }
    }

    /************************** Player_OpenFire *********************************/
    void Player_OpenFire::enter(Control &control) noexcept
    {
    }

#define WALK_AVOID 6

    void Player_OpenFire::update(FullControl &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &crisis = registry.get<ComponentCrisis>(entity);
        if (crisis.targetEnemy != (entt::entity)UINT32_MAX)
        {
            auto &move = registry.get<ComponentMove>(entity);
            auto &moveStack = registry.get<ComponentMoveStack>(entity);
            auto &findRound = registry.get<ComponentFindRound>(entity);

            auto &componentAngle = registry.get<ComponentAngle>(entity);
            auto &componentFirearmsWeapon = registry.get<ComponentFirearmsWeapon>(entity);

            auto &enemyMove = registry.get<ComponentMove>(crisis.targetEnemy);

            auto angle = CxhUtils::getAngle(move.position, enemyMove.position);
            componentAngle.setGotoAngle(angle, 2);

            // 寻路 走位
            if (moveStack.moveRoundStack.empty() == true && findRound.needFindRound == false)
            {
                GameMap *gameMap = GameManager::GetInstance()->getNowGameMap();

                int moveX = WALK_AVOID - rd() % (WALK_AVOID * 2);
                int moveY = WALK_AVOID - rd() % (WALK_AVOID * 2);

                findRound.needFindRound = true;
                findRound.startRelativePosition = gameMap->RealPositionToRelativePosition(move.position.toPoint2());

                Point2 movePoint(findRound.startRelativePosition.x() + moveX, findRound.startRelativePosition.y() + moveY);

                findRound.endRelativePosition = movePoint;
            }
            if (componentFirearmsWeapon.mainWeapon.weaponBase != nullptr)
            {
                IntermediateProcess::FirearmsWeaponShoot(componentFirearmsWeapon.mainWeapon, entity, crisis.targetEnemy);
                if (CxhUtils::getEuclideanDistance(move.position, enemyMove.position) > componentFirearmsWeapon.mainWeapon.weaponBase->attackDistance + 50)
                {
                    control.changeTo<Player_Chase>();
                }
            }
        }
    }

    /************************** Player_ReloadBullet *********************************/
    void Player_ReloadBullet::enter(Control &control) noexcept
    {
    }

    void Player_ReloadBullet::update(FullControl &control) noexcept
    {
    }

    /************************** Player_UrgentTreat *********************************/
    void Player_UrgentTreat::enter(Control &control) noexcept
    {
    }

    void Player_UrgentTreat::update(FullControl &control) noexcept
    {
    }

    /************************** Player_CloseCombat *********************************/
    void Player_CloseCombat::enter(Control &control) noexcept
    {
    }

    void Player_CloseCombat::update(FullControl &control) noexcept
    {
    }

    /************************** Player_ThrowGrenades *********************************/
    void Player_ThrowGrenades::enter(Control &control) noexcept
    {
    }

    void Player_ThrowGrenades::update(FullControl &control) noexcept
    {
    }

    /************************** Player_Chase *********************************/
    void Player_Chase::enter(Control &control) noexcept
    {
    }

    void Player_Chase::update(FullControl &control) noexcept
    {
        entt::entity entity = control.context().entity;

        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &crisis = registry.get<ComponentCrisis>(entity);
        if (crisis.targetEnemy != (entt::entity)UINT32_MAX)
        {
            auto &move = registry.get<ComponentMove>(entity);
            auto &moveStack = registry.get<ComponentMoveStack>(entity);
            auto &findRound = registry.get<ComponentFindRound>(entity);

            auto &componentFirearmsWeapon = registry.get<ComponentFirearmsWeapon>(entity);

            auto &targetMove = registry.get<ComponentMove>(crisis.targetEnemy);

            if (componentFirearmsWeapon.mainWeapon.weaponBase != nullptr)
            {
                auto distance = CxhUtils::getEuclideanDistance(move.position, targetMove.position);
                if (distance < componentFirearmsWeapon.mainWeapon.weaponBase->attackDistance - 50) // 保证在打击范围内
                {
                    crisis.situation = PlayerSituation::WAR;
                    auto &player = registry.get<ComponentPlayer>(entity);
                    // player.name = "FindTheEnemy";
                    control.changeTo<Player_OpenFire>();
                    return;
                }
            }

            // 这里或许要判断敌人是否是卡死状态
            if (moveStack.moveRoundStack.empty() == true && findRound.needFindRound == false)
            {
                GameMap *gameMap = GameManager::GetInstance()->getNowGameMap();
                findRound.needFindRound = true;
                findRound.startRelativePosition = gameMap->RealPositionToRelativePosition(move.position.toPoint2());
                findRound.endRelativePosition = gameMap->RealPositionToRelativePosition(targetMove.position.toPoint2());
            }
        }
        else
        {
            crisis.situation = PlayerSituation::NORMAL;
            control.changeTo<Player_Walk>();
            // crisis.situation = PlayerSituation::FIND_ENEMY;
            // control.changeTo<Player_SearchEnemy>();
            // return;
        }
    }

    /************************** Player_Guard *********************************/
    void Player_Guard::enter(Control &control) noexcept
    {
    }

    void Player_Guard::update(FullControl &control) noexcept
    {
    }

    /************************** Player_SuckerPunch *********************************/
    void Player_SuckerPunch::enter(Control &control) noexcept
    {
    }

    void Player_SuckerPunch::update(FullControl &control) noexcept
    {
    }
}

//=========================================================================================================================================//
//=========================================================================================================================================//
//=========================================================================================================================================//
//=========================================================================================================================================//

namespace danMuGame // Retreat
{
    /************************** Player_Retreat *********************************/
    void Player_Retreat::enter(Control &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &moveStack = registry.get<ComponentMoveStack>(entity);
        auto &componentAngle = registry.get<ComponentAngle>(entity);
        moveStack.Clear();
        componentAngle.gotoAngle = componentAngle.angle;
    }

    void Player_Retreat::update(FullControl &control) noexcept
    {
    }

    /************************** Player_RunAway *********************************/
    void Player_RunAway::enter(Control &control) noexcept
    {
    }

    void Player_RunAway::update(FullControl &control) noexcept
    {
    }
}

namespace danMuGame // End
{
    /************************** Player_End *********************************/
    void Player_End::enter(Control &control) noexcept
    {
        // std::cout << "end enter" << std::endl;
    }
    void Player_End::update(FullControl &control) noexcept
    {
        // std::cout << "end update" << std::endl;
    }

    /************************** Player_Death *********************************/
    void Player_Death::enter(Control &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &player = registry.get<ComponentPlayer>(entity);
        if (player.isUse == true)
        {
            auto &move = registry.get<ComponentMove>(entity);
            auto &moveStack = registry.get<ComponentMoveStack>(entity);
            auto &animation = registry.get<ComponentAnimation>(entity);
            GameSource::GetInstance()->initializationAnimation(animation, "PlayerDeath_Basic");
            animation.isLoop = false;
            moveStack.Clear();
            move.speed = 0.f;
            PlayerManager::GetInstance()->getRemainingPlayerList().remove(entity);
            PlayerManager::GetInstance()->getRemainingUserPlayerList().remove(entity);
            if (player.uid != -1)
            {
                PlayerManager::GetInstance()->addReturnTable(entity);
            }
        }
    }
    void Player_Death::update(FullControl &control) noexcept
    {
        entt::entity entity = control.context().entity;
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &player = registry.get<ComponentPlayer>(entity);
        if (player.isUse == true)
        {
            auto &testCount = control.context().testCount;
            if (testCount > 500)
            {
                auto &animation = registry.get<ComponentAnimation>(entity);
                animation.animationKey = 0;
            }
            else
            {
                ++testCount;
            }
        }
        // std::cout << "end update" << std::endl;
    }

    void Player_NO::enter(Control &control) noexcept
    {
    }
    void Player_NO::update(FullControl &control) noexcept
    {
    }
}
