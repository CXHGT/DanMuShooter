#ifndef __COMPONENT_BASE_HPP__
#define __COMPONENT_BASE_HPP__

#include "Point2.hpp"
#include "Vector2.hpp"
#include <stdint.h>
#include <string>
#include "GameStack.hpp"
#include "memory"
#include "Color3.hpp"
#include <list>
#include "ai/PlayerStateMachine.h"
#include "ComponentMoveStack.h"

namespace danMuGame::component
{

    enum PlayerSkill
    {
        NO_SKILL,   // 没有技能
        SUCK_BLOOD, // 吸血
        RECOVER,    // 恢复
        TRIUMPH,    // 凯旋
        DODGE,      // 闪避
        HARVEST,    // 收割
        HARM_PLUS,  // 中伤
    };

    struct ComponentPlayer
    {
        bool isUse;
        int32_t index;
        std::string name;
        int32_t uid;
        int32_t team;
        std::shared_ptr<FSM::Instance> stateMachine; // 状态机
        PlayerSkill skill;
    };

    enum PlayerAction
    {
        Normal,   // 正常
        RunAway,  // 胆小，容易逃跑
        Onslaught // 鲁莽，猛干
    };

    enum PlayerSituation
    {
        NORMAL,        // 正常
        TREAT,         // 治疗
        RELOAD_BULLET, // 空闲状态下换弹
        FIND_ENEMY,    // 发现敌人
        TRACK,         // 跟踪
        BE_ATTACKED,   // 被攻击
        WAR,           // 战斗
    };

    struct ComponentCrisis
    {
        entt::entity nearestEnemy;                       // 最近的敌人
        float nearestDistance;                           // 最近敌人的距离
        Point2 questionPoint;                            // 问题点，枪声点
        entt::entity beAttackedAtEnemy;                  // 哪个敌人在开枪
        entt::entity targetEnemy;                        // 目标敌人，攻击的敌人
        std::list<entt::entity> nearbyEnemies;           // 附近的敌人 1300以内
        std::list<entt::entity> nearbyDiscoveredEnemies; // 附近已发现的敌人
        PlayerAction action;                             // 玩家行为
        PlayerSituation situation;                       // 玩家情况
    };

    struct ComponentPlayerData
    {
        int32_t eliminations; // 淘汰人数
        int32_t damageValue;  // 造成的伤害
        int32_t healthValue;  // 治疗的数值
        int32_t invesValue;   // 投喂
    };

    struct ComponentAttributes
    {
        int32_t power;    // 力量
        int32_t accuracy; // 准确度
        // int32_t stamina;        // 耐力
    };

    struct ComponentMessage
    {
        std::string message;
        int32_t showFrame;
        int32_t currentFrame;
        Color3 color;
    };

    struct ComponentSkill
    {
    };

    struct ComponentInventory
    {
        int32_t shotGunBulletCount; // 霰弹
        int32_t rifleBulletCount;   // 步枪
        int32_t pistolBulletCount;  // 手枪
        int32_t grenadeCount;       // 手榴弹
        int32_t medicalKitCount;    // 医疗包

        int32_t shotGunBulletMaxCount;
        int32_t rifleBulletMaxCount;
        int32_t pistolBulletMaxCount;
        int32_t grenadeMaxCount;
        int32_t medicalKitMaxCount;
    };

    struct ComponentDefense
    {
        float armorDefense;    // 护甲防御值
        int32_t armorDurable;  // 护甲耐久值
        float helmetDefense;   // 头盔防御值
        int32_t helmetDurable; // 头盔耐久值
    };

    struct ComponentMove
    {
        Vector2 position;
        float speed;
    };

    struct ComponentFindRound
    {
        bool needFindRound;
        Point2 startRelativePosition;
        Point2 endRelativePosition;
        int32_t boxSize;
    };

}

#endif