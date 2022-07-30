#ifndef __ASTAR_NODE_H__
#define __ASTAR_NODE_H__

#include "Point2.hpp"
#include <stdint.h>

namespace danMuGame
{
    typedef struct AStarNode_Type
    {
        // 点的位置i
        Point2 point;
        // 移动成本
        int gCost;
        // 到终点的距离
        int hCost;
        struct AStarNode_Type *m_parent;

    public:
        inline int getFCost()
        {
            return this->hCost + this->gCost;
        }
        void setValue(const Point2 &nowPoint, const Point2 &targetPoint, int walkCost, AStarNode_Type *parent = nullptr);

        /* data */
    } AStarNode;
}

#endif
