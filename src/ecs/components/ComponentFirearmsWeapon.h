#ifndef __COMPONENT_FIREARMS_WEAPON_H__
#define __COMPONENT_FIREARMS_WEAPON_H__

#include <stdint.h>
#include <string>

namespace danMuGame
{

    enum BulletType
    {
        SHOT_GUN, // 霰弹
        RIFLE,    // 步枪
        PISTOL,   // 手枪
        // MACHINE_GUN, // 机枪
        // SNIPER_RIFLE // 狙击枪
    };

    enum FirearmsWeaponBehavior
    {
        NO_BEHAVIOR = 0,
        RUN_ATTACK = 1,
    };

    struct FirearmsWeaponBase
    {
        std::string name;
        int32_t injure;                  // 伤害
        int32_t numberOfBullets;         // 子弹数量
        int32_t shootBulletCount;        // 射出的子弹数量，默认为1，霰弹大于1
        int32_t recoveryFrame;           // 换弹时间
        int32_t attackDistance;          // 攻击距离
        int32_t attackIntervalFrame;     // 攻击间隔
        int32_t speed;                   // 子弹移动速度
        int32_t accuracy;                // 精准度, 100最大
        int32_t discoveryDistance;       // 多长的距离内会被发现
        BulletType bulletType;           // 子弹类型，也就是使用的是哪一些子弹
        FirearmsWeaponBehavior behavior; // 行为
    };

    struct FirearmsWeapon
    {
        struct FirearmsWeaponBase *weaponBase; // 使用到的枪械
        int32_t currentIntervalFrame;          // 当前间隔帧
        int32_t currentBullets;                // 当前子弹数量
        int32_t currentRecoveryFrame;          // 当前换弹帧
        bool isCanShoot;                       // 是否可以射击
    };

    namespace component
    {
        struct ComponentFirearmsWeapon
        {
            struct FirearmsWeapon mainWeapon;
        };
    }
}

#endif
