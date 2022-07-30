#ifndef __DANMU_MANAGER_H__
#define __DANMU_MANAGER_H__

#include "BSdk.h"
#include "ThreadSafeQueue.hpp"
#include "DanMuData.h"
#include <mutex>
#include <queue>

namespace danMuGame
{
    class DanMuManager : public QObject
    {
        Q_OBJECT
    private:
        static DanMuManager *m_instance;
        static std::once_flag m_flag;

        BSdk *bsdk = nullptr;

        ThreadSafeQueue<DanMuMessage> m_danMuMessageQueue;
        ThreadSafeQueue<DanMuGift> m_danMuGiftQueue;

    private:
        DanMuManager();
        ~DanMuManager();
        class GC
        {
        public:
            ~GC()
            {
                if (DanMuManager::m_instance)
                {
                    delete DanMuManager::m_instance;
                    DanMuManager::m_instance = nullptr;
                }
            }
        };

    public:
        static DanMuManager *GetInstance();

        void initialization();

        void Stop();

    public:
        inline ThreadSafeQueue<DanMuMessage> &getDanMuMessageQueue()
        {
            return this->m_danMuMessageQueue;
        }
        inline ThreadSafeQueue<DanMuGift> &getDanMuGiftQueue()
        {
            return this->m_danMuGiftQueue;
        }

    private:
        bool DanMuMessageProcess(const DanMuMessage &danmu);
        void ReceivedDanmaKu(const DanmaData &danMu);
        void ReceivedGift(const GiftData &gift);
        void ReceivedGuardBuy(const GuardBuyData &guardBuy);
        void ReceivedSuperChat(const SuperChatData &superChat);
        void ReceivedSuperChatDel(const SuperChatDelData &superChatDel);
    };
}

#endif // DANMUMANAGER_H
