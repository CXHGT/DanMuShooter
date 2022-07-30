#include "ComponentHealth.h"

namespace danMuGame::component
{

    void ComponentHealth::Health(int32_t health)
    {
        this->health += health;
        if(this->health > this->maxHealth)
        {
            this->health = this->maxHealth;
        }
    }

    void ComponentHealth::Injure(int32_t injure)
    {
        if (this->health < injure)
        {
            this->health = 0;
        }
        else
        {
            this->health -= injure;
        }
    }

    void ComponentHealth::AddMaxHealth(int32_t health)
    {
        this->maxHealth += health;
        this->health += health;
    }
}