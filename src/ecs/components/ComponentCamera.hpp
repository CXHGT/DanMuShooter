#ifndef __COMPONENT_CAMERA_HPP__
#define __COMPONENT_CAMERA_HPP__

#include "Vector2.hpp"
#include "entt/entt.hpp"
#include <stdint.h>

namespace danMuGame::component
{
    struct ComponentCamera
    {
        float scale;
        Vector2 viewPoint;           // 观测点
        entt::entity viewEntity;    // 观测实体
        int32_t viewFrame;          // 剩余观察帧数，一般会观测1000帧后换其他人，除非遇到战事
    };
}

#endif