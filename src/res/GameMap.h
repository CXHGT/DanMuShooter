#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include <vector>
#include "AStarNode.h"
#include "GameStack.hpp"
#include "Size2.hpp"
#include "Point2.hpp"
#include "Vector2.hpp"
#include <stdint.h>

namespace danMuGame
{
    class GameMap
    {
    private:
        // 每个地图层可容纳的块数量 width * height
        int32_t m_numberOfMapTiles;
        // 每个块的像素大小
        Size2 m_pixmapSize;
        // 地图的块大小
        Size2 m_mapSize;
        // 第图层
        std::vector<int32_t *> m_layerVec;
        // 寻路时的id
        uint8_t m_AStarBuffId = 0x01;
        // 地图有多少个块，就会生成多少个节点
        std::vector<AStarNode> m_AStarNodeCatch;
        // 寻路时的临时栈
        GameStack<AStarNode *> m_AStarNodeStack;

        GameStack<AStarNode *> m_temporaryAStarNodeStack;

        // 寻路时要保证相同的位置不再重复行走，所以开辟一段内存来记录
        uint8_t *g_AStarBuff = nullptr;

    private:
        int32_t getPointValue(const Point2 &point) const;
        uint8_t getAStarBuffId();

    public:
        GameMap(int layerCount, const Size2 &mapSize, const Size2 &pixmapSize);
        ~GameMap();

        // 设置图层的内存
        void setLayerMemory(int layerIndex, const char *memory, uint32_t memorySize);

        const int32_t *getLayerMemory(int index);

        // 以块位置来寻路
        AStarNode *getRelativePositionRoute(const Point2 &startPoint, const Point2 &endPoint, int32_t boxSize = 0);
        // 以真实的像素位置寻路
        AStarNode *getRealPositionRoute(const Point2 &startPoint, const Point2 &endPoint, int32_t boxSize = 0);

        bool isCanMove(const Point2 &point, int32_t boxSize);
        bool isCanMove(const Point2 &point);

        // 使用的是相对位置
        int32_t getSourceId(int32_t layerIndex, const Point2 &point) const;

        inline Point2 RealVecotr2ToRelativePoint2(const Vector2 &point) const
        {
            return Point2((int32_t)point.x() / m_pixmapSize.width(), (int32_t)point.y() / m_pixmapSize.height());
        }

        inline Point2 RealPositionToRelativePosition(const Point2 &point) const
        {
            return Point2(point.x() / m_pixmapSize.width(), point.y() / m_pixmapSize.height());
        }

        inline int32_t getLayerCount() const
        {
            return m_layerVec.size();
        }

        inline const Size2 &getPixmapSize() const
        {
            return m_pixmapSize;
        }

        inline const Size2 &getMapSize() const
        {
            return m_mapSize;
        }
    };
}

#endif
