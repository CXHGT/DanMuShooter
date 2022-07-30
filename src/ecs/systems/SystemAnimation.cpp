#include "SystemAnimation.h"
#include "ECS/Components/ComponentCamera.hpp"
#include "ECS/Components/ComponentBase.hpp"
#include "ECS/Components/ComponentAnimation.hpp"
#include "Managers/PlayerManager.h"

using namespace danMuGame::component;

namespace danMuGame::system
{
    SystemAnimation::SystemAnimation(entt::registry &registry)
        : System(registry)
    {
    }

    void SystemAnimation::update(float dt)
    {
        auto view = m_registry.view<ComponentAnimation>();
        for (auto entity : view)
        {
            auto &animation = view.get<ComponentAnimation>(entity);
            if (animation.animationKey != 0 && animation.isPlaying == true)
            {
                if (animation.elapsedFrame >= animation.delayFrame)
                {
                    animation.elapsedFrame = 0;
                    ++(animation.currentFrameIndex);
                    if (animation.currentFrameIndex >= animation.frameCount)
                    {
                        if (animation.isLoop)
                        {
                            animation.currentFrameIndex = 0;
                        }
                        else
                        {
                            animation.currentFrameIndex = animation.frameCount - 1;
                            animation.isPlaying = false;
                        }
                        // animation.isPlaying = animation.isLoop;
                    }
                }
                else
                {
                    ++(animation.elapsedFrame);
                }
            }
        }
    }
}
