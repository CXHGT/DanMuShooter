#include "CxhUtils.h"
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include "zlib.h"
#include <vector>
#include <cmath>
#include "fmt/format.h"
#include <mutex>
#include <stdlib.h>

#define UNZIP_BUFF_SIZE 1048576

namespace danMuGame::CxhUtils
{

    std::string LoadTextFile(const std::string &path, bool &isOK)
    {
        //加载n文件
        std::ifstream inputFile(path);
        if (inputFile)
        {
            std::stringstream buffer;
            buffer << inputFile.rdbuf();
            isOK = true;
            inputFile.close();
            return buffer.str();
        }
        inputFile.close();
        isOK = false;
        return "";
    }

    static unsigned char unZipBuff[UNZIP_BUFF_SIZE];
    static std::mutex single_unzip_mutex;

    const unsigned char *Single_UnZip(const std::string &zlibData, unsigned long &zipSize)
    {
        std::lock_guard<std::mutex> lock(single_unzip_mutex); // 防止多线程竞争
        uLong unzip_size = UNZIP_BUFF_SIZE;
        int unzipType = uncompress(unZipBuff, &unzip_size, (unsigned char *)zlibData.data(), zlibData.length());
        if (unzipType == 0)
        {
            zipSize = unzip_size;
            return unZipBuff;
        }
        return nullptr;
    }

    const unsigned char *UnZip(const std::string &zlibData, unsigned char *buffer, unsigned long &bufferSize)
    {
        uLong unzip_size = bufferSize;
        int unzipType = uncompress(buffer, &unzip_size, (unsigned char *)zlibData.data(), zlibData.length());
        if (unzipType == 0)
        {
            bufferSize = unzip_size;
            return unZipBuff;
        }
        return nullptr;
    }

    float getAngle(float startX, float startY, float endX, float endY)
    {
        float x = endX - startX;
        float y = endY - startY;
        float angle = atan2(y, x) * 180 / PI;
        if (std::isnan(angle))
        {
            angle = 0;
        }
        return angle;
    }

    float getManhattanDistance(float startX, float startY, float endX, float endY)
    {
        float x_distance = startX - endX;
        float y_distance = startY - endY;
        return abs(x_distance) + abs(y_distance);
    }

    float getEuclideanDistance(float startX, float startY, float endX, float endY)
    {
        float x_distance = startX - endX;
        float y_distance = startY - endY;
        return sqrt((x_distance * x_distance) + (y_distance * y_distance));
    }

    void getLinePoint2(std::vector<Point2> &savePoint2, int32_t startX, int32_t startY, int32_t endX, int32_t endY)
    {
        int32_t valueX = startX - endX;
        int32_t valueY = startY - endY;

        int32_t length = std::max(abs(valueX), abs(valueY));

        for (int num = 0; num <= length; ++num)
        {
            float proportion = (float)num / length;

            savePoint2.push_back(Point2(startX + (int)round(proportion * (-valueX)), startY + (int)round(proportion * (-valueY))));
        }
    }

    void stringSplit(const std::string &str, const std::string &pattern, std::vector<std::string> &res)
    {
        if (str != "")
        {
            //在字符串末尾也加入分隔符，方便截取最后一段
            std::string strs = str + pattern;

            size_t pos = strs.find(pattern);
            while (pos != std::string::npos)
            {
                std::string temp = strs.substr(0, pos);
                res.push_back(temp);
                //去掉已分割的字符串,在剩下的字符串中进行分割
                strs = strs.substr(pos + pattern.length(), strs.size());
                pos = strs.find(pattern);
            }
        }
    }

}
