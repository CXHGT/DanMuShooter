#ifndef __SYSTEM_FIND_ROUND_H__
#define __SYSTEM_FIND_ROUND_H__

#include "System.h"
#include "AStarNode.h"

namespace danMuGame::component
{
    class ComponentMoveStack;
}

namespace danMuGame::system
{
    class SystemFindRound : public System
    {
    private:
        static void PathSmoothing(AStarNode* aStarItem, int32_t boxSize);

    public:
        explicit SystemFindRound(entt::registry &);
        virtual void update(float dt) override;
    };
}

#endif