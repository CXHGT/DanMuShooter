#ifndef __GAME_GUI_H__
#define __GAME_GUI_H__

#include "fmt/format.h"
#include <vector>
#include <QPixmap>
#include <QLabel>
#include <QString>
#include <string>
#include <queue>
#include "MusicPlayModule.h"
#include "SummaryWidgetModule.h"
#include "managers/GameManager.h"
#include <QBuffer>
#include <mutex>

class MusicPlayModule;

namespace danMuGame
{
    class GameGui
    {
    private:
        static GameGui *m_instance;
        static std::once_flag m_flag;

        QLabel *frameLabel = nullptr;
        int32_t timeCount = 0;
        std::queue<std::string> mainMessageQueue;
        std::queue<std::string> subMessageQueue;

    public:
        // QLabel *remainingPlayersLabel = nullptr;
        QLabel *mainMessage1Label = nullptr;
        QLabel *mainMessage2Label = nullptr;
        QLabel *mainMessage3Label = nullptr;
        MusicPlayModule *musicPlayModule = nullptr;

        QLabel *subMessage1Label = nullptr;
        QLabel *subMessage2Label = nullptr;
        QLabel *subMessage3Label = nullptr;
        QLabel *subMessage4Label = nullptr;
        QLabel *subMessage5Label = nullptr;
        QLabel *subMessage6Label = nullptr;
        QLabel *subMessage7Label = nullptr;
        QLabel *subMessage8Label = nullptr;

        QLabel *playerCountLabel = nullptr;

        QLabel *playerNameLabel = nullptr;
        QLabel *playerHealthLabel = nullptr;
        QLabel *playerFirearmsWeaponLabel = nullptr;
        QLabel *playerDisuseLabel = nullptr;
        QLabel *playerSkillLabel = nullptr;

        QLabel *timeLabel = nullptr;

        QLabel *giftLabel = nullptr;
        
        SummaryWidgetModule *summaryWidgetModule = nullptr;

        QLabel *introduceLabel = nullptr;

    private:
        GameGui();
        ~GameGui();
        GameGui(const GameGui &) = delete;
        GameGui(GameGui &&) = delete;
        GameGui &operator=(const GameGui &) = delete;
        GameGui &operator=(GameGui &&) = delete;

        class GC
        {
        public:
            ~GC()
            {
                if (GameGui::m_instance)
                {
                    delete GameGui::m_instance;
                    GameGui::m_instance = nullptr;
                }
            }
        };

        void mainMessageScrolling();
        void subMessageScrolling();

    public:
        static GameGui *GetInstance();

        void update_1_second();
        void update();

        inline void setFrameLabel(QLabel *label)
        {
            this->frameLabel = label;
        }

        inline void setFrameValue(int32_t frameValue)
        {
            frameLabel->setText(QString::fromStdString(fmt::format("Frame : {}, testCount : {}", frameValue, GameManager::GetInstance()->gameTestCount)));
        }

        void addMainMessage(const std::string &message);
        void addSubMainMessage(const std::string &message);

        void playMusic(const std::string &musicName, const std::string &musicPath, int32_t level = 0);

        void playMusicByBuffer(const std::string &musicName, QBuffer *buffer, int32_t level = 0);
    };
}

#endif
