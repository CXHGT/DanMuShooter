#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "entt/entity/registry.hpp"

namespace danMuGame::system
{
    class System
    {
    public:
        explicit System(entt::registry &registry);
        virtual void update(float dt) = 0;
        virtual ~System() {}

    protected:
        entt::registry &m_registry;
    };
}

#endif