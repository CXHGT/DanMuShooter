#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <vector>
#include <stdint.h>
#include <QPixmap>

namespace danMuGame
{
    struct Animation
    {
        std::vector<QPixmap> source;
        int32_t intervalFrame;
    };
}

#endif
