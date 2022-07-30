#ifndef DANMUDATA_H
#define DANMUDATA_H

#include <string>
#include <stdint.h>
#include <vector>

typedef struct
{
    int32_t uid;
    std::string username;             // 玩家名称
    std::vector<std::string> message; // 玩家消息
} DanMuMessage;

typedef struct
{
    int32_t uid;
    int32_t giftId;
    std::string username; // 玩家名称
    std::string giftName; // 礼物名称
    int32_t value;        // 礼物价值
    int32_t count;        // 礼物数量
} DanMuGift;

#endif // DANMUDATA_H
