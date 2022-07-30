#include "SystemAngle.h"
#include "ECS/Components/ComponentCamera.hpp"
#include "ECS/Components/ComponentBase.hpp"
#include "ECS/Components/ComponentAngle.h"
#include "Managers/PlayerManager.h"

using namespace danMuGame::component;

namespace danMuGame::system
{
    SystemAngle::SystemAngle(entt::registry &registry)
        : System(registry)
    {
    }

    void SystemAngle::update(float dt)
    {
        auto view = m_registry.view<ComponentAngle>();
        for (auto entity : view)
        {
            auto &componentAngle = view.get<ComponentAngle>(entity);

            if (componentAngle.angle > componentAngle.gotoAngle)
            {
                componentAngle.angle -= componentAngle.speed;
                if (componentAngle.angle < componentAngle.gotoAngle)
                {
                    componentAngle.angle = componentAngle.gotoAngle;
                    componentAngle.weight = 0;
                }
            }
            else if (componentAngle.angle < componentAngle.gotoAngle)
            {
                componentAngle.angle += componentAngle.speed;
                if (componentAngle.angle > componentAngle.gotoAngle)
                {
                    componentAngle.angle = componentAngle.gotoAngle;
                    componentAngle.weight = 0;
                }
            }
        }
    }
}