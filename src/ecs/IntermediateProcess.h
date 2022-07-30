#ifndef __INTERMEDIATE_PROCESS_H__
#define __INTERMEDIATE_PROCESS_H__

#include "entt/entt.hpp"
#include "Vector2.hpp"
#include "Point2.hpp"
#include <vector>
#include <stdint.h>

namespace danMuGame
{
    namespace component
    {
        struct ComponentBullet;
        struct ComponentFirearmsWeapon;
    }
    struct FirearmsWeapon;

    using namespace component;

    namespace IntermediateProcess
    {
        void FirearmsWeaponShoot(FirearmsWeapon &firearmsWeapon, entt::entity attacker, entt::entity target);

        void FirearmsWeaponShoot2(FirearmsWeapon &firearmsWeapon, const Vector2 &startVector, const Vector2 &endVector, entt::entity attacker, entt::entity target);

        bool isCanMoveByLine(const Vector2 &startVector, const Vector2 &endVector, int32_t boxSize);
    }
}

#endif