#include "PlayerManager.h"
#include <mutex>
#include "GameSettings.h"
#include "SystemManager.h"
#include "ecs/components/ComponentBase.hpp"
#include "ecs/components/ComponentAnimation.hpp"
#include "ecs/components/ComponentAngle.h"
#include "ecs/components/ComponentHealth.h"
#include "ecs/components/ComponentFirearmsWeapon.h"
#include <tuple>
#include "fmt/format.h"
#include <random>
#include "scenes/GameGui.h"
#include "managers/GameManager.h"

namespace danMuGame
{
    using namespace component;

    static std::random_device rd;

    PlayerManager *PlayerManager::m_instance = nullptr;
    std::once_flag PlayerManager::m_flag;

    PlayerManager::PlayerManager()
    {
    }

    PlayerManager::~PlayerManager()
    {
    }

    PlayerManager *PlayerManager::GetInstance()
    {
        std::call_once(m_flag,
                       []
                       {
                           PlayerManager::m_instance = new PlayerManager();
                           static GC gc;
                       });
        return PlayerManager::m_instance;
    }

    void PlayerManager::initialization()
    {
        createPlayers(GameSettings::GetInstance()->getPlayerMaxCount());

        this->m_gameSource = GameSource::GetInstance();
        for (int index = 0; index < 10; index++)
        {
            teamTable.push_back(std::list<entt::entity>());
        }
    }

    void PlayerManager::createPlayers(int32_t count)
    {
        auto &registry = GameManager::GetInstance()->getRegistry();
        for (int num = 0; num < count; ++num)
        {
            auto entity = registry.create();
            registry.emplace_or_replace<ComponentHealth>(entity, ESSENTIAL_HEALTH, ESSENTIAL_HEALTH);
            registry.emplace_or_replace<ComponentAnimation>(entity, 0, 0, 0, 0, 0, false, true);
            registry.emplace_or_replace<ComponentInventory>(entity, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
            registry.emplace_or_replace<ComponentMove>(entity, Vector2(-1000.f, -1000.f), 0.f);
            registry.emplace_or_replace<ComponentMoveStack>(entity, GameStack<Vector2>(), 0, 0.f);
            registry.emplace_or_replace<ComponentFindRound>(entity, false, Point2(), Point2(), PLAYER_BOX_SIZE);
            registry.emplace_or_replace<ComponentAngle>(entity, 0.f, 0.f, PLAYER_ANGLE_SPEED, 0);
            registry.emplace_or_replace<ComponentFirearmsWeapon>(entity, FirearmsWeapon{nullptr, 0, 0, 0, false});
            registry.emplace_or_replace<ComponentCrisis>(entity, (entt::entity)UINT32_MAX, 999999.f, Point2(-1000, -1000), (entt::entity)UINT32_MAX, (entt::entity)UINT32_MAX, std::list<entt::entity>(), std::list<entt::entity>(), PlayerAction::Normal, PlayerSituation::NORMAL);
            registry.emplace_or_replace<ComponentPlayerData>(entity, 0, 0, 0, 0);

            Context context{
                .entity = entity,
                .testCount = 0};

            auto fsmPtr = std::make_shared<FSM::Instance>(context);

            registry.emplace_or_replace<ComponentPlayer>(entity, false, (int32_t)playerVector.size(), "电脑", 0, 1, fsmPtr, PlayerSkill::NO_SKILL);
            this->playerVector.push_back(entity);
        }
    }

    void PlayerManager::resetPlayer(entt::entity &entity)
    {
        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &player = registry.get<ComponentPlayer>(entity);
        auto &health = registry.get<ComponentHealth>(entity);
        auto &animation = registry.get<ComponentAnimation>(entity);
        auto &inventory = registry.get<ComponentInventory>(entity);
        auto &move = registry.get<ComponentMove>(entity);
        auto &moveStack = registry.get<ComponentMoveStack>(entity);
        auto &findRound = registry.get<ComponentFindRound>(entity);
        auto &componentAngle = registry.get<ComponentAngle>(entity);
        auto &componentFirearmsWeapon = registry.get<ComponentFirearmsWeapon>(entity);
        auto &crisis = registry.get<ComponentCrisis>(entity);
        auto &playerData = registry.get<ComponentPlayerData>(entity);

        // auto &[player, health, animation, inventory, move] = view.get<ComponentPlayer, ComponentHealth, ComponentAnimation, ComponentInventory, ComponentMove>(entity);
        /************************/
        crisis.action = PlayerAction::Normal;
        crisis.nearbyDiscoveredEnemies.clear();
        crisis.nearbyEnemies.clear();
        crisis.questionPoint.setX(-1000);
        crisis.targetEnemy = (entt::entity)UINT32_MAX;
        crisis.nearestEnemy = (entt::entity)UINT32_MAX;
        crisis.beAttackedAtEnemy = (entt::entity)UINT32_MAX;
        crisis.nearestDistance = 999999.f;
        crisis.situation = PlayerSituation::NORMAL;
        /************************/
        player.name = "电脑";
        player.isUse = false;
        player.uid = -1;
        player.team = 0;
        player.stateMachine->changeTo<Player_NO>();
        player.stateMachine->update();
        player.stateMachine->context().testCount = 0;
        player.stateMachine->context().lostEnemyCount = 0;
        player.stateMachine->context().runTestCount = 0;
        player.stateMachine->context().recoveryHealthCount = 0;
        player.skill = PlayerSkill::NO_SKILL;
        /************************/
        health.health = ESSENTIAL_HEALTH;
        health.maxHealth = ESSENTIAL_HEALTH;
        /************************/
        animation.animationKey = 0;
        animation.currentFrameIndex = 0;
        animation.delayFrame = 0;
        animation.elapsedFrame = 0;
        animation.frameCount = 0;
        animation.isPlaying = false;
        animation.isLoop = true;
        /************************/
        inventory.shotGunBulletCount = 0;
        inventory.rifleBulletCount = 0;
        inventory.pistolBulletCount = 0;
        inventory.grenadeCount = 0;
        inventory.medicalKitCount = 0;
        inventory.shotGunBulletMaxCount = 0;
        inventory.rifleBulletMaxCount = 0;
        inventory.pistolBulletMaxCount = 0;
        inventory.grenadeMaxCount = 0;
        inventory.medicalKitMaxCount = 0;
        /************************/
        move.position.setX(-1000.f);
        move.position.setY(-1000.f);
        move.speed = 0.f;
        /************************/
        moveStack.moveRoundStack.clear();
        // moveStack.walkCount = 0;
        moveStack.targetAngle = 0.f;
        /************************/
        findRound.needFindRound = false;
        findRound.boxSize = PLAYER_BOX_SIZE;
        /************************/
        componentAngle.angle = 0.f;
        componentAngle.gotoAngle = 0.f;
        componentAngle.speed = PLAYER_ANGLE_SPEED;
        componentAngle.weight = 0;
        /************************/
        playerData.damageValue = 0;
        playerData.eliminations = 0;
        playerData.healthValue = 0;
        playerData.invesValue = 0;
        /************************/

        GameSource::GetInstance()->initFirearmsWeapon(componentFirearmsWeapon.mainWeapon, "烧火棍");
    }

    void PlayerManager::resetAllPlayer()
    {
        for (auto entity : this->playerVector)
        {
            resetPlayer(entity);
        }
        for (auto &table : this->teamTable)
        {
            table.clear();
        }
        this->uidEntityTable.clear();
        this->remainingPlayerList.clear();
        this->playerInGame.clear();
        this->remainingUserPlayerList.clear();
        this->waitingToReturnPlayer.clear();
        this->useIndex = 0;
    }

    entt::entity PlayerManager::usePlayer()
    {
        if (useIndex >= this->playerVector.size())
        {
            return (entt::entity)UINT32_MAX;
        }
        auto entity = this->playerVector[useIndex];
        ++useIndex;

        for (auto &teamList : this->teamTable)
        {
            teamList.clear();
        }

        auto &registry = GameManager::GetInstance()->getRegistry();
        auto &player = registry.get<ComponentPlayer>(entity);
        player.isUse = true;
        player.name = fmt::format("电脑{}", useIndex);
        // player.uid = -1;
        // player.team = 0;

        this->playerInGame.push_back(entity);
        this->remainingPlayerList.push_back(entity);
        this->teamTable[0].push_back(entity);

        player.stateMachine->changeTo<Player_Stop>();
        player.stateMachine->update();
        return entity;
    }

    void PlayerManager::playerReturnBattlefield()
    {
        auto &registry = GameManager::GetInstance()->getRegistry();
        for (auto entity : waitingToReturnPlayer)
        {
            auto &player = registry.get<ComponentPlayer>(entity);
            auto &health = registry.get<ComponentHealth>(entity);
            auto &move = registry.get<ComponentMove>(entity);
            auto &moveStack = registry.get<ComponentMoveStack>(entity);
            auto &findRound = registry.get<ComponentFindRound>(entity);
            auto &componentAngle = registry.get<ComponentAngle>(entity);
            auto &componentFirearmsWeapon = registry.get<ComponentFirearmsWeapon>(entity);
            auto &crisis = registry.get<ComponentCrisis>(entity);

            health.health = ESSENTIAL_HEALTH;
            health.maxHealth = ESSENTIAL_HEALTH;
            /************************/
            crisis.action = PlayerAction::Normal;
            crisis.nearbyDiscoveredEnemies.clear();
            crisis.nearbyEnemies.clear();
            crisis.questionPoint.setX(-1000);
            crisis.targetEnemy = (entt::entity)UINT32_MAX;
            crisis.nearestEnemy = (entt::entity)UINT32_MAX;
            crisis.beAttackedAtEnemy = (entt::entity)UINT32_MAX;
            crisis.nearestDistance = 999999.f;
            crisis.situation = PlayerSituation::NORMAL;
            /************************/
            moveStack.moveRoundStack.clear();
            moveStack.targetAngle = 0.f;
            /************************/
            findRound.needFindRound = false;
            findRound.boxSize = PLAYER_BOX_SIZE;
            /************************/
            componentAngle.angle = 0.f;
            componentAngle.gotoAngle = 0.f;
            componentAngle.speed = PLAYER_ANGLE_SPEED;
            componentAngle.weight = 0;
            /************************/
            GameSource::GetInstance()->initFirearmsWeapon(componentFirearmsWeapon.mainWeapon, "格洛克16");

            auto *gameMap = GameManager::GetInstance()->getNowGameMap();
            while (true)
            {
                int x = rd() % gameMap->getMapSize().width();
                int y = rd() % gameMap->getMapSize().height();
                if (gameMap->isCanMove(Point2(x, y), PLAYER_BOX_SIZE) == true)
                {
                    move.position.setX(x * gameMap->getPixmapSize().width());
                    move.position.setY(y * gameMap->getPixmapSize().height());
                    break;
                }
            }

            GameGui::GetInstance()->addMainMessage(fmt::format("{}返回战场", player.name));
            player.stateMachine->changeTo<Player_Walk>();

            this->remainingPlayerList.push_back(entity);
            this->remainingUserPlayerList.push_back(entity);
        }
        waitingToReturnPlayer.clear();
    }

    entt::entity PlayerManager::usePlayerByUidAndUsername(int32_t uid, const std::string &username)
    {
        auto &registry = GameManager::GetInstance()->getRegistry();
        if (useIndex >= this->playerVector.size() || this->uidEntityTable.find(uid) != this->uidEntityTable.end())
        {
            return (entt::entity)UINT32_MAX;
        }
        auto entity = this->playerVector[useIndex];
        ++useIndex;

        auto &player = registry.get<ComponentPlayer>(entity);
        auto &componentFirearmsWeapon = registry.get<ComponentFirearmsWeapon>(entity);
        player.isUse = true;
        player.name = username;
        player.uid = uid;
        player.team = 0;
        uidEntityTable[uid] = entity;

        GameSource::GetInstance()->initFirearmsWeapon(componentFirearmsWeapon.mainWeapon, "格洛克16");
        this->playerInGame.push_back(entity);
        this->remainingPlayerList.push_back(entity);
        this->remainingUserPlayerList.push_back(entity);
        this->teamTable[0].push_back(entity);

        player.stateMachine->changeTo<Player_Stop>();
        player.stateMachine->update();
        return entity;
    }

}
