#ifndef __COMPONENT_HEALTH_H__
#define __COMPONENT_HEALTH_H__

#include <stdint.h>

namespace danMuGame::component
{
    struct ComponentHealth
    {
        int32_t health;
        int32_t maxHealth;

        void Injure(int32_t injure);
        void Health(int32_t health);
        void AddMaxHealth(int32_t health);
        inline void InjurePercentage(float percentage)
        {
            Injure(percentage * maxHealth);
        }
        inline void HealthPercentage(float percentage)
        {
            Health(percentage * maxHealth);
        }
    };
}

#endif