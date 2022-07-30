#include "GameGui.h"
#include <mutex>
#include <memory>
#include "managers/PlayerManager.h"
#include "managers/GameManager.h"
#include "ecs/components/ComponentCamera.hpp"
#include "ecs/components/ComponentHealth.h"
#include "ecs/components/ComponentBase.hpp"
#include "ecs/components/ComponentFirearmsWeapon.h"

namespace danMuGame
{

    GameGui *GameGui::m_instance = nullptr;

    GameGui::GameGui()
    {
    }

    GameGui::~GameGui()
    {
    }

    //单例获取
    GameGui *GameGui::GetInstance()
    {
        if (m_instance == nullptr)
        {
            static std::mutex _mutex;
            std::lock_guard<std::mutex> lock(_mutex);
            if (m_instance == nullptr)
            {
                m_instance = new GameGui();
                static GC gc;
            }
        }
        return m_instance;
    }

    void GameGui::update_1_second()
    {
        ++timeCount;
        if (timeCount > 5)
        {
            timeCount = 0;
            mainMessageScrolling();
        }
    }

    void GameGui::update()
    {
        while (mainMessageQueue.empty() == false)
        {
            if (this->mainMessage1Label->text() != "")
            {
                mainMessageScrolling();
            }
            this->mainMessage1Label->setText(QString::fromStdString(mainMessageQueue.front()));
            mainMessageQueue.pop();
        }
        while (subMessageQueue.empty() == false)
        {
            if (this->subMessage1Label->text() != "")
            {
                subMessageScrolling();
            }
            this->subMessage1Label->setText(QString::fromStdString(subMessageQueue.front()));
            subMessageQueue.pop();
        }

        playerCountLabel->setText(QString::fromStdString(
            fmt::format("{}/{}",
                        PlayerManager::GetInstance()->getRemainingPlayerList().size(),
                        PlayerManager::GetInstance()->getPlayerInGame().size())));

        auto &registry = GameManager::GetInstance()->getRegistry();
        auto view = registry.view<component::ComponentCamera>();
        auto &camera = view.get<component::ComponentCamera>(*view.begin());
        auto entity = camera.viewEntity;
        if (entity != (entt::entity)UINT32_MAX)
        {
            auto &componentHealth = registry.get<component::ComponentHealth>(entity);
            auto &componentPlayer = registry.get<component::ComponentPlayer>(entity);
            auto &componentFirearmsWeapon = registry.get<component::ComponentFirearmsWeapon>(entity);
            auto &bulletData = registry.get<component::ComponentPlayerData>(entity);
            playerNameLabel->setText(QString::fromStdString(componentPlayer.name));
            playerHealthLabel->setText(QString::fromStdString(fmt::format("{}/{}", componentHealth.health, componentHealth.maxHealth)));
            if (componentFirearmsWeapon.mainWeapon.weaponBase != nullptr)
            {
                playerFirearmsWeaponLabel->setText(QString::fromStdString(componentFirearmsWeapon.mainWeapon.weaponBase->name));
            }
            else
            {
                playerFirearmsWeaponLabel->setText("无");
            }
            playerDisuseLabel->setText(QString::number(bulletData.eliminations));

            switch (componentPlayer.skill)
            {
            case component::PlayerSkill::DODGE:
                playerSkillLabel->setText("闪避");
                break;
            case component::PlayerSkill::HARM_PLUS:
                playerSkillLabel->setText("中伤");
                break;
            case component::PlayerSkill::SUCK_BLOOD:
                playerSkillLabel->setText("吸血");
                break;
            case component::PlayerSkill::RECOVER:
                playerSkillLabel->setText("回血");
                break;
            case component::PlayerSkill::TRIUMPH:
                playerSkillLabel->setText("凯旋");
                break;
            case component::PlayerSkill::HARVEST:
                playerSkillLabel->setText("收割");
                break;
            default:
                playerSkillLabel->setText("无技能");
                break;
            }
            // playerSkillLabel->setText(QString::number(bulletData.damageValue));
        }
        else
        {
            playerNameLabel->setText("-----");
            playerHealthLabel->setText("-----");
            playerFirearmsWeaponLabel->setText("-----");
            playerDisuseLabel->setText("-----");
            playerSkillLabel->setText("-----");
        }
    }

    void GameGui::addMainMessage(const std::string &message)
    {
        this->mainMessageQueue.push(message);
    }

    void GameGui::addSubMainMessage(const std::string &message)
    {
        this->subMessageQueue.push(message);
    }

    void GameGui::mainMessageScrolling()
    {
        this->mainMessage3Label->setText(this->mainMessage2Label->text());
        this->mainMessage2Label->setText(this->mainMessage1Label->text());
        this->mainMessage1Label->setText("");
    }

    void GameGui::subMessageScrolling()
    {
        this->subMessage8Label->setText(this->subMessage7Label->text());
        this->subMessage7Label->setText(this->subMessage6Label->text());
        this->subMessage6Label->setText(this->subMessage5Label->text());
        this->subMessage5Label->setText(this->subMessage4Label->text());
        this->subMessage4Label->setText(this->subMessage3Label->text());
        this->subMessage3Label->setText(this->subMessage2Label->text());
        this->subMessage2Label->setText(this->subMessage1Label->text());
        this->subMessage1Label->setText("");
    }

    void GameGui::playMusic(const std::string &musicName, const std::string &musicPath, int32_t level)
    {
        MusicData musicData{
            .musicName = QString::fromStdString(musicName),
            .musicUrl = QUrl(musicPath.c_str())};
        this->musicPlayModule->PlayMusic(musicData, level);
    }

    void GameGui::playMusicByBuffer(const std::string &musicName, QBuffer *buffer, int32_t level)
    {
        this->musicPlayModule->PlayMusicByBuffer(musicName, buffer, level);
    }
}
