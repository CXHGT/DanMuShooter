#include "ComponentAngle.h"
#include <cmath>

namespace danMuGame::component
{
    void ComponentAngle::setGotoAngle(float mAngle, int32_t mWeight)
    {
        if (mWeight >= weight)
        {
            weight = mWeight;
            if (abs(mAngle - this->angle) > 180)
            {
                if (this->angle <= 0)
                {
                    this->angle += 360.f;
                    while (this->angle < -360.f)
                    {
                        this->angle += 360.f;
                    }
                }
                else
                {
                    this->angle -= 360.f;
                    while (this->angle > 360.f)
                    {
                        this->angle -= 360.f;
                    }
                }
            }
            gotoAngle = mAngle;
        }
    }
}