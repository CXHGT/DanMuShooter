#include "GameMap.h"
#include <memory>
#include "GameSource.h"
#include <cmath>
#include <iostream>
#include "fmt/format.h"

namespace danMuGame
{
    GameMap::GameMap(int layerCount, const Size2 &mapSize, const Size2 &pixmapSize)
        : m_mapSize(mapSize),
          m_pixmapSize(pixmapSize)
    {
        // 先创建空图层，后面在进行内存的分配和写入
        for (int i = 0; i < layerCount; ++i)
        {
            m_layerVec.push_back(nullptr);
        }
        this->m_numberOfMapTiles = m_mapSize.width() * m_mapSize.height();

        this->g_AStarBuff = new uint8_t[m_numberOfMapTiles];
        memset(g_AStarBuff, 0, m_numberOfMapTiles);

        for (int i = 0; i < m_numberOfMapTiles; ++i)
        {
            m_AStarNodeCatch.emplace_back(AStarNode());
        }
        m_AStarNodeStack.setSize(m_numberOfMapTiles);
    }

    GameMap::~GameMap()
    {
        if (this->g_AStarBuff != nullptr)
        {
            delete[] this->g_AStarBuff;
            this->g_AStarBuff = nullptr;
        }
        for (int i = 0; i < m_layerVec.size(); ++i)
        {
            if (m_layerVec[i] != nullptr)
            {
                delete[] m_layerVec[i];
                m_layerVec[i] = nullptr;
            }
        }
    }

    // 设置图层的内存
    void GameMap::setLayerMemory(int layerIndex, const char *memory, uint32_t memorySize)
    {
        if (layerIndex < this->m_layerVec.size())
        {
            int32_t *layerMemory = new int32_t[memorySize];
            memcpy(layerMemory, memory, memorySize);
            m_layerVec[layerIndex] = layerMemory;
        }
    }

    const int32_t *GameMap::getLayerMemory(int index)
    {
        if (index < this->m_layerVec.size())
        {
            return this->m_layerVec[index];
        }
        return nullptr;
    }

    // 以块位置来寻路
    AStarNode *GameMap::getRelativePositionRoute(const Point2 &startPoint, const Point2 &endPoint, int32_t boxSize)
    {
        if (isCanMove(endPoint, boxSize) == false || startPoint == endPoint)
        {
            return nullptr;
        }
        int32_t aStarItemIndex = 0;

        m_AStarNodeStack.clear();
        m_temporaryAStarNodeStack.clear();

        auto buffId = getAStarBuffId();

        int pointX, pointY;

        Point2 movePoint;

        int32_t pointValue = getPointValue(startPoint);
        g_AStarBuff[pointValue] = buffId;

        AStarNode *nowAStarNode = &(m_AStarNodeCatch[aStarItemIndex]);
        nowAStarNode->setValue(startPoint, endPoint, 0);
        ++aStarItemIndex;

        for (;;)
        {
            for (int offsetX = -1; offsetX <= 1; ++offsetX)
            {
                for (int offsetY = -1; offsetY <= 1; ++offsetY)
                {
                    // if(abs(offsetX) + abs(offsetY) == 0){
                    //     continue;
                    // }
                    if (offsetX + offsetY == 0 || abs(offsetX + offsetY) == 2)
                    {
                        continue;
                    }
                    movePoint.setX(nowAStarNode->point.x() + offsetX);
                    movePoint.setY(nowAStarNode->point.y() + offsetY);
                    pointValue = getPointValue(movePoint);

                    if (isCanMove(movePoint, boxSize) == true && buffId != g_AStarBuff[pointValue])
                    {
                        g_AStarBuff[pointValue] = buffId;
                        AStarNode *astarItem = &(m_AStarNodeCatch[aStarItemIndex]);
                        astarItem->setValue(movePoint, endPoint, nowAStarNode->gCost + 1, nowAStarNode);
                        ++aStarItemIndex;

                        // assert(aStarItemIndex < 10000);

                        //  到达目的地了，那么直接返回
                        if (astarItem->point == endPoint)
                        {
                            // std::cout << "xiao hao : " << aStarItemIndex << std::endl;
                            return astarItem;
                        }
                        // 先入栈，并进行排列操作
                        if (m_AStarNodeStack.empty() == true)
                        {
                            m_AStarNodeStack.push(astarItem);
                        }
                        else if (m_AStarNodeStack.top()->getFCost() < astarItem->getFCost())
                        {
                            AStarNode *temporaryAStarItem;
                            // 对消耗进行排列操作，将最低消耗的放在最前方，这种操作类似于插入排序
                            while (m_AStarNodeStack.empty() == false && m_AStarNodeStack.top()->getFCost() < astarItem->getFCost())
                            {
                                temporaryAStarItem = m_AStarNodeStack.top();
                                m_AStarNodeStack.pop();
                                m_temporaryAStarNodeStack.push(temporaryAStarItem);
                            }
                            // 已经找到适合的位置了，插入节点
                            m_AStarNodeStack.push(astarItem);
                            // 将之前从队列中取出的节点都插回去
                            while (m_temporaryAStarNodeStack.empty() == false)
                            {
                                temporaryAStarItem = m_temporaryAStarNodeStack.top();
                                m_temporaryAStarNodeStack.pop();
                                m_AStarNodeStack.push(temporaryAStarItem);
                            }
                        }
                        else
                        {
                            m_AStarNodeStack.push(astarItem);
                        }
                    }
                }
            }

            // 已经无路可走，证明没有路可以走到规定的目标，那么直接返回nullptr
            if (m_AStarNodeStack.empty() == true)
            {
                return nullptr;
            }
            else
            {
                nowAStarNode = m_AStarNodeStack.top();
                m_AStarNodeStack.pop();
            }
        }
        return nullptr;
    }

    // 以真实的像素位置寻路
    AStarNode *GameMap::getRealPositionRoute(const Point2 &startPoint, const Point2 &endPoint, int32_t boxSize)
    {
        return getRelativePositionRoute(RealPositionToRelativePosition(startPoint), RealPositionToRelativePosition(endPoint), boxSize);
    }

    bool GameMap::isCanMove(const Point2 &point)
    {
        int32_t pointValue = getPointValue(point);
        if (this->m_layerVec.size() >= 2 && pointValue != -1 && this->m_layerVec[1][pointValue] != 0)
        {
            return false;
        }
        return true;
    }

    bool GameMap::isCanMove(const Point2 &point, int32_t boxSize)
    {
        if (false == isCanMove(point))
        {
            return false;
        }
        if (boxSize > 0)
        {
            for (int offsetX = -boxSize; offsetX <= boxSize; ++offsetX)
            {
                for (int offsetY = -boxSize; offsetY <= boxSize; ++offsetY)
                {
                    if (abs(offsetX) + abs(offsetY) == boxSize)
                    {
                        if (false == isCanMove(Point2(point.x() + offsetX, point.y() + offsetY)))
                        {
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    int32_t GameMap::getSourceId(int32_t layerIndex, const Point2 &point) const
    {
        int32_t pointValue = getPointValue(point);
        if (pointValue != -1 && layerIndex >= 0 && layerIndex < m_layerVec.size())
        {
            return this->m_layerVec[layerIndex][pointValue] - 1;
        }
        return -1;
    }

    int32_t GameMap::getPointValue(const Point2 &point) const
    {
        int32_t pointValue = point.y() * m_mapSize.width() + point.x();
        if (pointValue >= 0 && pointValue < m_numberOfMapTiles)
        {
            return pointValue;
        }
        return -1;
    }

    // 获得寻路区分id
    uint8_t GameMap::getAStarBuffId()
    {
        ++m_AStarBuffId;
        if (0x00 == m_AStarBuffId)
        {
            memset(g_AStarBuff, 0, m_numberOfMapTiles);
            m_AStarBuffId = 1;
        }
        return m_AStarBuffId;
    }
}
