#include "DanMuManager.h"
#include <mutex>
#include <thread>
#include <chrono>
#include <sstream>
#include "fmt/format.h"
#include <iostream>
#include "managers/GameSettings.h"
#include "GameManager.h"
#include "PlayerManager.h"
#include "scenes/GameGui.h"
#include <random>
#include "ecs/components/ComponentBase.hpp"
#include "base64.h"

static const std::string KeySecret = "在这里填上你开发者的KeySecert";
static const std::string KeyId = "在这里填上你开发者的KeyId";
static const std::string app_id = "在这里填上你开发者的AppId";

#define DELEGATE()

namespace danMuGame
{

    DanMuManager *DanMuManager::m_instance = nullptr;
    std::once_flag DanMuManager::m_flag;

    DanMuManager::DanMuManager() : QObject(nullptr)
    {
    }

    bool DanMuManager::DanMuMessageProcess(const DanMuMessage &danMu)
    {
        auto &messageCmdVec = danMu.message;
        if (messageCmdVec[0] == "点歌" && messageCmdVec.size() > 1)
        {
        }
        else if (messageCmdVec[0] == "加入" || messageCmdVec[0] == "加入游戏")
        {
            switch (GameManager::GetInstance()->getGameStatue())
            {
            case GameStatue::Running:
                DELEGATE()
                {
                    if (PlayerManager::GetInstance()->getUidEntityTable().find(danMu.uid) != PlayerManager::GetInstance()->getUidEntityTable().end())
                    {
                        GameGui::GetInstance()->addMainMessage(fmt::format("{}你已经在游戏中，寄了的话请等待自动复活", danMu.username));
                    }
                    else
                    {
                        auto entity = PlayerManager::GetInstance()->usePlayerByUidAndUsername(danMu.uid, danMu.username);
                        if (entity != (entt::entity)UINT32_MAX)
                        {
                            auto &move = GameManager::GetInstance()->getRegistry().get<component::ComponentMove>(entity);
                            auto *gameMap = GameManager::GetInstance()->getNowGameMap();
                            while (true)
                            {
                                static std::random_device rd;
                                int x = rd() % gameMap->getMapSize().width();
                                int y = rd() % gameMap->getMapSize().height();
                                if (gameMap->isCanMove(Point2(x, y), PLAYER_BOX_SIZE) == true)
                                {
                                    move.position.setX(x * gameMap->getPixmapSize().width());
                                    move.position.setY(y * gameMap->getPixmapSize().height());
                                    break;
                                }
                            }
                            GameGui::GetInstance()->addMainMessage(fmt::format("{}空降进入游戏中", danMu.username));
                        }
                        else
                        {
                            GameGui::GetInstance()->addMainMessage(fmt::format("{}空降失败", danMu.username));
                        }
                    }
                }
                break;
            case GameStatue::Prepare:
                PlayerManager::GetInstance()->usePlayerByUidAndUsername(danMu.uid, danMu.username);
                break;
            }
        }
        else if (messageCmdVec[0] == "人机" && danMu.username == "CXHGT")
        {
            PlayerManager::GetInstance()->usePlayer();
        }
        else if (messageCmdVec[0] == "强制开始" && danMu.username == "CXHGT")
        {
            GameManager::GetInstance()->setGameStatue(GameStatue::Running);
        }
        else if (messageCmdVec[0] == "重开" && danMu.username == "CXHGT")
        {
            GameManager::GetInstance()->gameRestart();
        }
        else
        {
            return false;
        }
        return true;
    }

    void DanMuManager::ReceivedDanmaKu(const DanmaData &danMu)
    {
        DanMuMessage danMuMessage;
        danMuMessage.uid = danMu.uid;
        danMuMessage.username = danMu.uname;

        //        std::cout << "[receive danmu]" << danMu.uname << " : " << danMu.msg << std::endl;

        std::istringstream danmuIss(danMu.msg);
        std::string tmp;
        while (std::getline(danmuIss, tmp, ' '))
        {
            danMuMessage.message.emplace_back(tmp);
        }
        if (DanMuMessageProcess(danMuMessage) == false)
        {
            this->m_danMuMessageQueue.push(danMuMessage);
        }
    }

    void DanMuManager::ReceivedGift(const GiftData &gift)
    {

        DanMuGift danMuGift;
        danMuGift.uid = gift.uid;
        danMuGift.count = gift.gift_num;
        danMuGift.giftId = gift.gift_id;
        danMuGift.giftName = gift.gift_name;
        danMuGift.username = gift.uname;
        danMuGift.value = gift.price;
        this->m_danMuGiftQueue.push(danMuGift);

        // this->m_danMuGiftQueue.push(DanMuGift{
        //     .uid = gift.uid,
        //     .count = gift.gift_num,
        //     .giftId = gift.gift_id,
        //     .giftName = gift.gift_name,
        //     .username = gift.uname,
        //     .value = gift.price,
        // });
    }
    void DanMuManager::ReceivedGuardBuy(const GuardBuyData &guardBuy)
    {
    }
    void DanMuManager::ReceivedSuperChat(const SuperChatData &superChat)
    {
    }
    void DanMuManager::ReceivedSuperChatDel(const SuperChatDelData &superChatDel)
    {
    }

    DanMuManager::~DanMuManager()
    {
        Stop();
    }

    void DanMuManager::Stop()
    {
        if (bsdk != nullptr)
        {
            bsdk->Stop();
            delete bsdk;
            bsdk = nullptr;
        }
    }

    //单例获取
    DanMuManager *DanMuManager::GetInstance()
    {
        std::call_once(m_flag,
                       []
                       {
                           DanMuManager::m_instance = new DanMuManager();
                           static GC gc;
                       });
        return DanMuManager::m_instance;
    }

    void DanMuManager::initialization()
    {
        if (this->bsdk != nullptr)
        {
            disconnect(this->bsdk, &BSdk::ReceivedDanmaKu, this, &DanMuManager::ReceivedDanmaKu);
            disconnect(this->bsdk, &BSdk::ReceivedGift, this, &DanMuManager::ReceivedGift);
            this->bsdk->Stop();
            delete this->bsdk;
        }
        this->bsdk = new BSdk(
            KeyId,
            KeySecret,
            app_id,
            GameSettings::GetInstance()->getGameCode());

        connect(this->bsdk, &BSdk::ReceivedDanmaKu, this, &DanMuManager::ReceivedDanmaKu);
        connect(this->bsdk, &BSdk::ReceivedGift, this, &DanMuManager::ReceivedGift);

        this->bsdk->Start();
    }
}
