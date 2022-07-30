#ifndef __COMPONENT_ANGLE_H__
#define __COMPONENT_ANGLE_H__

#include <stdint.h>

namespace danMuGame::component
{
    struct ComponentAngle
    {
        float angle;
        float gotoAngle;
        float speed;
        int32_t weight; // 权重，只有大于等于当前权重时才可以设置gotoAngle值
        void setGotoAngle(float mAngle, int32_t mWeight);
    };
}

#endif