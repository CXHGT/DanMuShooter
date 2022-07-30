#ifndef __COMPONENT_ANIMATION_HPP__
#define __COMPONENT_ANIMATION_HPP__

#include <stdint.h>

namespace danMuGame::component
{
    struct ComponentAnimation
    {
        int32_t animationKey;       // 动画key
        int32_t delayFrame;         // 帧间隔
        int32_t elapsedFrame;       // 第几个间隔
        int32_t currentFrameIndex;  // 当前帧的位置
        int32_t frameCount;         // 这个动画总共有多少帧数
        bool isPlaying;             // 是否可播放
        bool isLoop;                // 是否循环播放
    };

}

#endif