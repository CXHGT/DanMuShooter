#ifndef __COMPONENT_BULLET_H__
#define __COMPONENT_BULLET_H__

#include <stdint.h>
#include <vector>
#include "Vector2.hpp"
#include "Color3.hpp"
#include "entt/entt.hpp"

namespace danMuGame::component
{
    struct ComponentBullet
    {
        // 是否可用
        bool isUse;
        int startIndex;
        int endIndex;
        // 伤害
        int32_t injure;
        // 攻击者
        entt::entity attacker;
        // 目标实体
        entt::entity targetEntity;
        // 移动路线
        std::vector<Vector2> moveRoute;

        Color3 color;

        void bulletRun(const Vector2 &startVector, const Vector2 &endVector, float speed, float injure,entt::entity attacker, entt::entity target);
    };
}

#endif