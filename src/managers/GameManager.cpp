#include "GameManager.h"
#include <mutex>
#include "GameSettings.h"
#include "res/GameSource.h"
#include "SystemManager.h"
#include "PlayerManager.h"
#include "BulletManager.h"
#include "DanMuManager.h"
#include <iostream>
#include "Scenes/GameGui.h"
#include <random>
#include "MusicPlayModule.h"
#include "ecs/components/ComponentBase.hpp"

using namespace danMuGame::component;

namespace danMuGame
{
    GameManager *GameManager::m_instance = nullptr;
    std::once_flag GameManager::m_flag;

    static std::random_device rd;

    static char introducePrepare[] = "[游戏还处于测试阶段]\n\n弹幕发送发送:\n\"加入\": 加入游戏\n\"队伍+(0-9)\": 加入特定队伍\n\t       0队为野人队\n\"步枪\",\"霰弹\",\"手枪\": 切换武器\n";
    static char introduceRunning[] = "[游戏还处于测试阶段]\n\n战斗阶段暂时只有2个指令\n\n\"治疗\": 停下缓慢恢复生命值\n\"停止治疗\": 字面意识\n\n直播有4-6秒的延时，注意预判";

    GameManager::GameManager()
    {
        m_registry.reserve(20000);
    }

    GameManager::~GameManager()
    {
    }

    GameManager *GameManager::GetInstance()
    {
        std::call_once(m_flag,
                       []
                       {
                           GameManager::m_instance = new GameManager();
                           static GC gc;
                       });
        return GameManager::m_instance;
    }

    void GameManager::initialization()
    {
        this->m_nowGameMap = GameSource::GetInstance()->getMap("test_map");

        for (int index = 0; index < GameSettings::GetInstance()->getPlayerMaxCount(); ++index)
        {
            scoreVecCatch.push_back(ScoreData{});
            investVecCatch.push_back(InvestData{});
        }
        gameRestart();
    }

    static std::string getFightingMusicBufferName()
    {
        int rdValue = rd() % 8;
        switch (rdValue)
        {
        case 0:
            return "fighting_1.mp3";
            break;
        case 1:
            return "fighting_2.mp3";
            break;
        case 2:
            return "fighting_3.mp3";
            break;
        case 3:
            return "fighting_4.mp3";
            break;
        case 4:
            return "fighting_5.mp3";
            break;
        case 5:
            return "fighting_6.mp3";
            break;
        case 6:
            return "fighting_7.mp3";
            break;
        case 7:
            return "fighting_8.mp3";
            break;
        }
        return "";
    }

    static std::string getPrepareMusicBufferName()
    {
        int rdValue = rd() % 2;
        switch (rdValue)
        {
        case 0:
            return "Prepare.mp3";
            break;
        case 1:
            return "Prepare2.mp3";
            break;
        }
        return "";
    }

#define DELEGATE()

    static bool scoreVecSortFun(const ScoreData &leftData, const ScoreData &rightData)
    {
        return (leftData.killScore * 500 + leftData.injureScore) > (rightData.killScore * 500 + rightData.injureScore);
    }

    static bool investVecSortFun(const InvestData &leftData, const InvestData &rightData)
    {
        return leftData.investScore > rightData.investScore;
    }

    void GameManager::setGameStatue(GameStatue gameStatue) noexcept
    {
        auto *gui = GameGui::GetInstance();
        this->timeCount = 9999;
        this->m_gameStatue = gameStatue;
        switch (this->m_gameStatue)
        {
        case GameStatue::Prepare:
            DELEGATE()
            {
                gameTestCount++;
                this->timeCount = 60;
                GameGui::GetInstance()->summaryWidgetModule->setHidden(true);

                std::string bufferName = getPrepareMusicBufferName();
                GameGui::GetInstance()->playMusicByBuffer(bufferName, GameSource::GetInstance()->m_bgmCatch[bufferName], 0);

                // gui->introduceLabel->setText(introducePrepare);
            }
            gui->giftLabel->setPixmap(*(GameSource::GetInstance()->gift1));
            gui->introduceLabel->setPixmap(*(GameSource::GetInstance()->introduce));
            break;
        case GameStatue::Running:
            DELEGATE()
            {
                this->playerReturnTime = 30;
                this->timeCount = 240;
                GameGui::GetInstance()->summaryWidgetModule->setHidden(true);

                std::string musicBufferName = getFightingMusicBufferName();
                GameGui::GetInstance()->playMusicByBuffer(musicBufferName, GameSource::GetInstance()->m_bgmCatch[musicBufferName], 0);

                // gui->introduceLabel->setText(introduceRunning);
            }
            gui->giftLabel->setPixmap(*(GameSource::GetInstance()->gift2));
            break;
        case GameStatue::FrontEnd:
            DELEGATE()
            {
                this->timeCount = 5;
                GameGui::GetInstance()->summaryWidgetModule->setHidden(true);
                std::string endBufferName = "end.mp3";
                GameGui::GetInstance()->playMusicByBuffer(endBufferName, GameSource::GetInstance()->m_bgmCatch[endBufferName], 0);
            }
            break;
        case GameStatue::Ending:
            DELEGATE()
            {
                auto view = m_registry.view<ComponentPlayer, ComponentPlayerData>();
                for (auto entity : view)
                {
                    auto &player = view.get<ComponentPlayer>(entity);
                    auto &playerData = view.get<ComponentPlayerData>(entity);
                    if (player.isUse == true)
                    {
                        this->scoreVecCatch[player.index].name = player.name;
                        this->scoreVecCatch[player.index].injureScore = playerData.damageValue;
                        this->scoreVecCatch[player.index].killScore = playerData.eliminations;

                        if (playerData.invesValue > 0)
                        {
                            this->investVecCatch[player.index].name = player.name;
                            this->investVecCatch[player.index].investScore = playerData.invesValue;
                        }
                    }
                }

                std::sort(this->scoreVecCatch.begin(), this->scoreVecCatch.end(), scoreVecSortFun);
                std::sort(this->investVecCatch.begin(), this->investVecCatch.end(), investVecSortFun);

                GameGui::GetInstance()->summaryWidgetModule->setValue(this->scoreVecCatch, this->investVecCatch);

                // GameGui::GetInstance()->playMusic("end", "./Resources/bgm/结算bgm.mp3", 0);
            }
            this->timeCount = 10;
            GameGui::GetInstance()->summaryWidgetModule->setHidden(false);
            break;
        }
    }

    void GameManager::gameRestart() noexcept
    {
        this->setGameStatue(GameStatue::Prepare);

        PlayerManager::GetInstance()->resetAllPlayer();
        BulletManager::GetInstance()->clearInActionBullet();

        for (auto &scoreData : this->scoreVecCatch)
        {
            scoreData.injureScore = 0;
            scoreData.name = "无";
            scoreData.killScore = 0;
            scoreData.ranking = 0;
        }
        for (auto &investData : this->investVecCatch)
        {
            investData.investScore = 0;
            investData.name = "无";
            investData.ranking = 0;
        }
    }

    void GameManager::update(float second) noexcept
    {
        auto *playerManager = PlayerManager::GetInstance();
        auto *systemManager = SystemManager::GetInstance();
        if (m_gameStatue == GameStatue::Prepare && timeCount <= 0)
        {
            if (playerManager->getUseIndex() < GameSettings::GetInstance()->getRobotCount())
            {
                playerManager->usePlayer();
            }
            else
            {
                setGameStatue(GameStatue::Distribute);
            }
        }

        switch (m_gameStatue)
        {
        case GameStatue::Pause:
            break;
        case GameStatue::Prepare:
        case GameStatue::Distribute:
        case GameStatue::Running:
        case GameStatue::FrontEnd:
            if (this->m_gameScene != nullptr && systemManager->isUpdateOk())
            {
                this->m_gameScene->update();
                systemManager->update(second);
                ++frameCount;
            }
            break;
        }

        GameGui::GetInstance()->update();
    }

    void GameManager::update_1_second() noexcept
    {
        auto *gui = GameGui::GetInstance();
        if (timeCount > 0)
        {
            --timeCount;
        }
        switch (this->m_gameStatue)
        {
        case GameStatue::Prepare:
            gui->timeLabel->setText(QString::fromStdString(fmt::format("准备时间: {}秒", timeCount)));

            break;
        case GameStatue::Running:
            if (playerReturnTime > 0)
            {
                --playerReturnTime;
            }

            gui->timeLabel->setText(QString::fromStdString(fmt::format("剩余时间: {}秒", timeCount)));

            if (gui->musicPlayModule->getIsPlaying() == false)
            {
                std::string musicBufferName = getFightingMusicBufferName();
                GameGui::GetInstance()->playMusicByBuffer(musicBufferName, GameSource::GetInstance()->m_bgmCatch[musicBufferName], 0);
            }

            if (playerReturnTime <= 0 && timeCount > 0)
            {

                if (PlayerManager::GetInstance()->getWaitingToReturnPlayer().size() > 0)
                {
                    PlayerManager::GetInstance()->playerReturnBattlefield();
                    gui->timeLabel->setText(QString::fromStdString(fmt::format("淘汰的玩家已经重新整装上阵")));
                    playerReturnTime = 30;
                }
            }

            break;
        case GameStatue::FrontEnd:
            gui->timeLabel->setText(QString::fromStdString(fmt::format("{}秒 后结算战斗", timeCount)));
            break;
        case GameStatue::Ending:
            gui->timeLabel->setText(QString::fromStdString(fmt::format("{}秒 后重开游戏", timeCount)));
            break;
        }
        if (timeCount <= 0)
        {
            switch (m_gameStatue)
            {
            case GameStatue::Running:
                setGameStatue(GameStatue::FrontEnd);
                break;
            case GameStatue::FrontEnd:
                setGameStatue(GameStatue::Ending);
                break;
            case GameStatue::Ending:
                gameRestart();
                break;
            }
        }

        // if (frameCount <= 0 && this->m_gameStatue == GameStatue::Running)
        // {
        //     m_systemManager->resetUpdateValue();
        // }
        // setGameStatue(GameStatue::Ending);r

        gui->setFrameValue(frameCount);
        gui->update_1_second();
        frameCount = 0;
        ++nowTimeValue;

        // gameRestart();
    }
}
