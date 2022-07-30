#ifndef __CXH_UTILS_H__
#define __CXH_UTILS_H__

#define PI 3.14159265
#define DEC (PI / 180)

#include "Point2.hpp"
#include "Vector2.hpp"
#include <vector>
#include <string>

#define BIT(X) ((int32_t)1 << (X))
#define REVERSE_BIT(X) (((int32_t)0xFFFFFFFF << (X)) ^ 0xFFFFFFFF)

namespace danMuGame
{

    namespace CxhUtils
    {
        std::string LoadTextFile(const std::string &path, bool &isOK);

        // 解压zip数据，不需要提供缓存，内部缓存100k，解压超过此数值时数据将不可靠，并且只能单一线程使用，多线程会有数据重叠
        const unsigned char *Single_UnZip(const std::string &zlibData, unsigned long &zipSize);
        // 解压zip数据，需要提供缓存，返回的是缓存的指针，当指针为nullptr时，表示解压数据失败
        const unsigned char *UnZip(const std::string &zlibData, unsigned char *buffer, unsigned long &bufferSize);
        
        // 2个点，计算点1到点2的相对角度
        float getAngle(float startX, float startY, float endX, float endY);
        // 曼哈顿距离，也就是posX+posY
        float getManhattanDistance(float startX, float startY, float endX, float endY);
        // 欧几里得距离，也就是(0,0)到(posX,posY)的直线距离
        float getEuclideanDistance(float startX, float startY, float endX, float endY);

        void getLinePoint2(std::vector<Point2> &savePoint2, int32_t startX, int32_t startY, int32_t endX, int32_t endY);

        void stringSplit(const std::string &str, const std::string &pattern,std::vector<std::string> &res);
    }

    namespace CxhUtils
    {
        inline float getAngle(const Vector2 &left, const Vector2 &right)
        {
            return getAngle(left.x(), left.y(), right.x(), right.y());
        }
        inline float getAngle(const Point2 &left, const Point2 &right)
        {
            return getAngle(left.x(), left.y(), right.x(), right.y());
        }

        inline float getManhattanDistance(const Vector2 &left, const Vector2 &right)
        {
            return getManhattanDistance(left.x(), left.y(), right.x(), right.y());
        }
        inline float getManhattanDistance(const Point2 &left, const Point2 &right)
        {
            return getManhattanDistance(left.x(), left.y(), right.x(), right.y());
        }

        inline float getEuclideanDistance(const Vector2 &left, const Vector2 &right)
        {
            return getEuclideanDistance(left.x(), left.y(), right.x(), right.y());
        }
        inline float getEuclideanDistance(const Point2 &left, const Point2 &right)
        {
            return getEuclideanDistance(left.x(), left.y(), right.x(), right.y());
        }

        inline void getLinePoint2(std::vector<Point2> &savePoint2, const Point2 &left, const Point2 &right)
        {
            getLinePoint2(savePoint2, left.x(), left.y(), right.x(), right.y());
        }

    }
}
#endif
