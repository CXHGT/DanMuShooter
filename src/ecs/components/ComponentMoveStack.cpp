#include "ComponentMoveStack.h"

namespace danMuGame::component
{
    void ComponentMoveStack::Clear()
    {
        this->moveRoundStack.clear();
        this->walkCount = 0;
    }
}