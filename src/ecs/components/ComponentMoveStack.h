#ifndef __COMPONENT_MOVE_STACK_H__
#define __COMPONENT_MOVE_STACK_H__

#include <stdint.h>
#include "GameStack.hpp"
#include "Vector2.hpp"

namespace danMuGame::component
{

    struct ComponentMoveStack
    {
        GameStack<Vector2> moveRoundStack;
        int32_t walkCount;
        float targetAngle;

        void Clear();
    };
}

#endif