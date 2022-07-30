#include "SystemGameDanMu.h"
#include "ECS/Components/ComponentCamera.hpp"
#include "ECS/Components/ComponentBase.hpp"
#include "ECS/Components/ComponentHealth.h"
#include "Managers/PlayerManager.h"
#include "Managers/DanMuManager.h"
#include "Managers/GameManager.h"
#include "fmt/format.h"
#include <sstream>
#include <iostream>
#include "CxhUtils.h"
#include <random>
#include "GameGui.h"

using namespace danMuGame::component;

namespace danMuGame::system
{
    SystemGameDanMu::SystemGameDanMu(entt::registry &registry)
        : System(registry)
    {
    }

    static std::random_device rd;

    std::string SystemGameDanMu::getNormalWeapon()
    {
        int32_t rdValue = rd() % 1000;
        std::string weaponName = "m4a1";
        if (rdValue < 200)
        {
            weaponName = "s686";
        }
        else if (rdValue < 400)
        {
            weaponName = "akm";
        }
        else if (rdValue < 600)
        {
            weaponName = "m60";
        }
        else if (rdValue < 800)
        {
            weaponName = "ump45";
        }
        else if (rdValue < 860)
        {
            weaponName = "98k";
        }
        return weaponName;
    }
    std::string SystemGameDanMu::getHighLevelWeapon()
    {
        int32_t rdValue = rd() % 1000;
        std::string weaponName = "aug";
        if (rdValue < 100)
        {
            weaponName = "awm";
        }
        else if (rdValue < 150)
        {
            weaponName = "沙漠之鹰";
        }
        else if (rdValue < 300)
        {
            weaponName = "格洛克22";
        }
        else if (rdValue < 450)
        {
            weaponName = "大菠萝";
        }
        else if (rdValue < 600)
        {
            weaponName = "s12k";
        }
        else if (rdValue < 700)
        {
            weaponName = "qroza";
        }
        else if (rdValue < 800)
        {
            weaponName = "slr";
        }
        else if (rdValue < 880)
        {
            weaponName = "vss";
        }
        else if (rdValue > 995)
        {
            weaponName = "m82a1";
        }
        return weaponName;
    }

    static void toggleWeapon(ComponentPlayer &player, FirearmsWeapon &weapon, const std::string &weaponName)
    {
        GameGui::GetInstance()->addSubMainMessage(fmt::format("{}切换武器为:{}", player.name, weaponName));
        GameSource::GetInstance()->initFirearmsWeapon(weapon, weaponName);
    }

    static void toggleSkill(ComponentPlayer &player, PlayerSkill skill)
    {
        std::string skillName = "";
        switch (skill)
        {
        case PlayerSkill::NO_SKILL:
            skillName = "无技能";
            break;
        case PlayerSkill::DODGE:
            skillName = "闪避";
            break;
        case PlayerSkill::HARM_PLUS:
            skillName = "中伤";
            break;
        case PlayerSkill::SUCK_BLOOD:
            skillName = "吸血";
            break;
        case PlayerSkill::RECOVER:
            skillName = "回血";
            break;
        case PlayerSkill::TRIUMPH:
            skillName = "凯旋";
            break;
        case PlayerSkill::HARVEST:
            skillName = "收割";
            break;
        }
        GameGui::GetInstance()->addSubMainMessage(fmt::format("{}切换技能为:{}", player.name, skillName));
        player.skill = skill;
    }

    void SystemGameDanMu::update(float dt)
    {
        auto *danMuManager = DanMuManager::GetInstance();
        auto &uidPlayerTable = PlayerManager::GetInstance()->getUidEntityTable();
        auto &danMuMessageQueue = danMuManager->getDanMuMessageQueue();
        auto &danMuGiftQueue = danMuManager->getDanMuGiftQueue();
        auto *gameGui = GameGui::GetInstance();

        while (danMuMessageQueue.empty() == false)
        {
            DanMuMessage danMuMessage;
            danMuMessageQueue.try_pop(danMuMessage);

            if (uidPlayerTable.find(danMuMessage.uid) != uidPlayerTable.end())
            {
                auto entity = uidPlayerTable[danMuMessage.uid];
                auto &player = m_registry.get<ComponentPlayer>(entity);
                auto &componentFirearmsWeapon = m_registry.get<ComponentFirearmsWeapon>(entity);
                auto &componentHealth = m_registry.get<ComponentHealth>(entity);
                // player.name = danMumessage.message[0];

                switch (GameManager::GetInstance()->getGameStatue())
                {
                case GameStatue::Prepare:
                    if (danMuMessage.message[0] == "手枪")
                    {
                        toggleWeapon(player, componentFirearmsWeapon.mainWeapon, "格洛克16");
                    }
                    else if (danMuMessage.message[0] == "步枪")
                    {
                        toggleWeapon(player, componentFirearmsWeapon.mainWeapon, "m16");
                    }
                    else if (danMuMessage.message[0] == "散弹" || danMuMessage.message[0] == "霰弹")
                    {
                        toggleWeapon(player, componentFirearmsWeapon.mainWeapon, "m1187");
                    }
                    else if (danMuMessage.message[0].find("队伍") != std::string::npos)
                    {
                        if (danMuMessage.message.size() == 1)
                        {
                            std::vector<std::string> messageVec;

                            CxhUtils::stringSplit(danMuMessage.message[0], "队伍", messageVec);

                            if (messageVec.size() >= 2)
                            {
                                std::istringstream iss(messageVec[1]);
                                int team;
                                iss >> team;
                                if (iss.fail() == false && team >= 0 && team < 10)
                                {
                                    if (player.team != team)
                                    {
                                        auto &teamTable = PlayerManager::GetInstance()->getTeamTable();
                                        if (teamTable[team].size() < 10)
                                        {
                                            teamTable[player.team].remove(entity);
                                            teamTable[team].push_back(entity);
                                            player.team = team;
                                            gameGui->addSubMainMessage(fmt::format("{}加入队伍:{}", player.name, player.team));
                                        }
                                        else
                                        {
                                            gameGui->addSubMainMessage(fmt::format("队伍{}人数已满", team));
                                        }
                                    }
                                }
                            }
                        }
                        else if (danMuMessage.message.size() == 2)
                        {
                            std::istringstream iss(danMuMessage.message[1]);
                            int team;
                            iss >> team;
                            if (iss.fail() == false && team >= 0 && team < 10)
                            {
                                if (player.team != team)
                                {
                                    auto &teamTable = PlayerManager::GetInstance()->getTeamTable();
                                    if (teamTable[team].size() < 10)
                                    {
                                        teamTable[player.team].remove(entity);
                                        teamTable[team].push_back(entity);
                                        player.team = team;
                                        gameGui->addSubMainMessage(fmt::format("{}加入队伍:{}", player.name, player.team));
                                    }
                                    else
                                    {
                                        gameGui->addSubMainMessage(fmt::format("队伍{}人数已满", team));
                                    }
                                }
                            }
                        }
                    }
                    else if (danMuMessage.message[0] == "吸血")
                    {
                        toggleSkill(player, PlayerSkill::SUCK_BLOOD);
                    }
                    else if (danMuMessage.message[0] == "回血")
                    {
                        toggleSkill(player, PlayerSkill::RECOVER);
                    }
                    else if (danMuMessage.message[0] == "凯旋")
                    {
                        toggleSkill(player, PlayerSkill::TRIUMPH);
                    }
                    else if (danMuMessage.message[0] == "闪避")
                    {
                        toggleSkill(player, PlayerSkill::DODGE);
                    }
                    else if (danMuMessage.message[0] == "收割")
                    {
                        toggleSkill(player, PlayerSkill::HARVEST);
                    }
                    else if (danMuMessage.message[0] == "中伤")
                    {
                        toggleSkill(player, PlayerSkill::HARM_PLUS);
                    }
                    else if (danMuMessage.message[0] == "无技能")
                    {
                        toggleSkill(player, PlayerSkill::NO_SKILL);
                    }
                    // else if (danMuMessage.message[0] == "普通枪")
                    // {
                    //     std::string weaponName = getNormalWeapon();
                    //     gameGui->addSubMainMessage(fmt::format("{}切换武器为:{}", player.name, weaponName));
                    //     GameSource::GetInstance()->initFirearmsWeapon(componentFirearmsWeapon.mainWeapon, weaponName);
                    // }
                    // else if (danMuMessage.message[0] == "高级枪")
                    // {
                    //     std::string weaponName = getHighLevelWeapon();
                    //     gameGui->addSubMainMessage(fmt::format("{}切换武器为:{}", player.name, weaponName));
                    //     GameSource::GetInstance()->initFirearmsWeapon(componentFirearmsWeapon.mainWeapon, weaponName);
                    // }
                    break;
                case GameStatue::Running:
                    if (componentHealth.health > 0)
                    {
                        if (danMuMessage.message[0] == "手枪")
                        {
                            toggleWeapon(player, componentFirearmsWeapon.mainWeapon, "格洛克16");
                        }
                        else if (danMuMessage.message[0] == "步枪")
                        {
                            toggleWeapon(player, componentFirearmsWeapon.mainWeapon, "m16");
                        }
                        else if (danMuMessage.message[0] == "散弹" || danMuMessage.message[0] == "霰弹")
                        {
                            toggleWeapon(player, componentFirearmsWeapon.mainWeapon, "m1187");
                        }
                        else if (danMuMessage.message[0] == "治疗")
                        {
                            player.stateMachine->changeTo<Player_Treat>();
                        }
                        else if (danMuMessage.message[0] == "停止治疗")
                        {
                            player.stateMachine->changeTo<Player_Walk>();
                        }
                        else if (danMuMessage.message[0] == "吸血")
                        {
                            toggleSkill(player, PlayerSkill::SUCK_BLOOD);
                        }
                        else if (danMuMessage.message[0] == "回血")
                        {
                            toggleSkill(player, PlayerSkill::RECOVER);
                        }
                        else if (danMuMessage.message[0] == "凯旋")
                        {
                            toggleSkill(player, PlayerSkill::TRIUMPH);
                        }
                        else if (danMuMessage.message[0] == "闪避")
                        {
                            toggleSkill(player, PlayerSkill::DODGE);
                        }
                        else if (danMuMessage.message[0] == "收割")
                        {
                            toggleSkill(player, PlayerSkill::HARVEST);
                        }
                        else if (danMuMessage.message[0] == "中伤")
                        {
                            toggleSkill(player, PlayerSkill::HARM_PLUS);
                        }
                        else if (danMuMessage.message[0] == "无技能")
                        {
                            toggleSkill(player, PlayerSkill::NO_SKILL);
                        }
                        break;
                    }
                }
            }
        }
        while (danMuGiftQueue.empty() == false)
        {
            DanMuGift gift;
            danMuGiftQueue.try_pop(gift);

            if (uidPlayerTable.find(gift.uid) != uidPlayerTable.end())
            {
                auto entity = uidPlayerTable[gift.uid];
                auto &player = m_registry.get<ComponentPlayer>(entity);
                auto &componentFirearmsWeapon = m_registry.get<ComponentFirearmsWeapon>(entity);
                auto &componentHealth = m_registry.get<ComponentHealth>(entity);
                auto &componentPlayerData = m_registry.get<ComponentPlayerData>(entity);

                // player.name = fmt::format("{}:{}:{}", gift.giftName, gift.value, gift.count);

                if (gift.giftName != "辣条")
                {
                    componentPlayerData.invesValue += gift.value / 100;
                }

                switch (GameManager::GetInstance()->getGameStatue())
                {
                case GameStatue::Prepare:
                    if (gift.giftName == "牛哇" || gift.giftName == "牛哇牛哇")
                    {
                        int32_t addMaxValue = gift.count * 100;
                        gameGui->addSubMainMessage(fmt::format("{}增加了{}点最大生命值", player.name, addMaxValue));
                        componentHealth.AddMaxHealth(addMaxValue);
                    }
                    else if (gift.giftName == "辣条")
                    {
                        if (gift.count == 2)
                        {
                            std::string weaponName = getNormalWeapon();
                            gameGui->addSubMainMessage(fmt::format("{}切换武器为:{}", player.name, weaponName));
                            GameSource::GetInstance()->initFirearmsWeapon(componentFirearmsWeapon.mainWeapon, weaponName);
                        }
                        int32_t addMaxValue = gift.count * 50;
                        gameGui->addSubMainMessage(fmt::format("{}增加了{}点最大生命值", player.name, addMaxValue));
                        componentHealth.AddMaxHealth(addMaxValue);
                    }
                    else if (gift.giftName == "打call")
                    {
                        int32_t addMaxValue = gift.count * 500;
                        gameGui->addSubMainMessage(fmt::format("{}增加了{}点最大生命值", player.name, addMaxValue));
                        componentHealth.AddMaxHealth(addMaxValue);

                        std::string weaponName = getHighLevelWeapon();
                        gameGui->addSubMainMessage(fmt::format("{}切换武器为:{}", player.name, weaponName));
                        GameSource::GetInstance()->initFirearmsWeapon(componentFirearmsWeapon.mainWeapon, weaponName);
                    }
                    else if (gift.giftName == "这个好诶")
                    {
                        int32_t addMaxValue = gift.count * 1250;
                        gameGui->addSubMainMessage(fmt::format("{}增加了{}点最大生命值", player.name, addMaxValue));
                        componentHealth.AddMaxHealth(addMaxValue);

                        std::string weaponName = getHighLevelWeapon();
                        gameGui->addSubMainMessage(fmt::format("{}切换武器为:{}", player.name, weaponName));
                        GameSource::GetInstance()->initFirearmsWeapon(componentFirearmsWeapon.mainWeapon, weaponName);
                    }
                    break;
                case GameStatue::Running:
                    if (gift.giftName == "牛哇" || gift.giftName == "牛哇牛哇")
                    {
                        if (componentHealth.health > 0)
                        {
                            if (gift.count == 5)
                            {
                                std::string weaponName = getHighLevelWeapon();
                                gameGui->addSubMainMessage(fmt::format("{}切换武器为:{}", player.name, weaponName));
                                GameSource::GetInstance()->initFirearmsWeapon(componentFirearmsWeapon.mainWeapon, weaponName);
                            }

                            int32_t addHealthValue = gift.count * 200;
                            componentHealth.Health(addHealthValue);
                            gameGui->addSubMainMessage(fmt::format("{}恢复了{}点生命值", player.name, addHealthValue));
                        }
                        else
                        {
                            gameGui->addSubMainMessage(fmt::format("{}已经寄了，回血失败", player.name));
                        }
                    }
                    else if (gift.giftName == "辣条")
                    {
                        if (componentHealth.health > 0)
                        {
                            if (gift.count == 2)
                            {
                                std::string weaponName = getNormalWeapon();
                                gameGui->addSubMainMessage(fmt::format("{}切换武器为:{}", player.name, weaponName));
                                GameSource::GetInstance()->initFirearmsWeapon(componentFirearmsWeapon.mainWeapon, weaponName);
                            }

                            int32_t addHealthValue = gift.count * 100;
                            componentHealth.Health(addHealthValue);
                            gameGui->addSubMainMessage(fmt::format("{}恢复了{}点生命值", player.name, addHealthValue));
                        }
                        else
                        {
                            gameGui->addSubMainMessage(fmt::format("{}已经寄了，回血失败", player.name));
                        }
                    }
                    else if (gift.giftName == "打call")
                    {
                        if (componentHealth.health > 0)
                        {
                            int32_t addHealthValue = gift.count * componentHealth.maxHealth * 0.5;
                            componentHealth.Health(addHealthValue);
                            gameGui->addSubMainMessage(fmt::format("{}恢复了{}点生命值", player.name, addHealthValue));
                        }
                        else
                        {
                            gameGui->addSubMainMessage(fmt::format("{}已经寄了，回血失败", player.name));
                        }
                    }
                    else if (gift.giftName == "这个好诶")
                    {
                        if (componentHealth.health > 0)
                        {
                            int32_t addHealthValue = gift.count * componentHealth.maxHealth * 0.8;
                            componentHealth.Health(addHealthValue);
                            gameGui->addSubMainMessage(fmt::format("{}恢复了{}点生命值", player.name, addHealthValue));

                            componentFirearmsWeapon.mainWeapon.currentBullets = 999;
                            componentFirearmsWeapon.mainWeapon.isCanShoot = true;
                            componentFirearmsWeapon.mainWeapon.currentRecoveryFrame = 0;
                            auto view = m_registry.view<ComponentCamera>();
                            auto &camera = view.get<ComponentCamera>(*view.begin());
                            if (1000 * gift.count >= camera.viewFrame)
                            {
                                camera.viewEntity = entity;
                                camera.viewFrame = 1000 * gift.count;
                            }
                        }
                        else
                        {
                            gameGui->addSubMainMessage(fmt::format("{}已经寄了，礼物效果失效", player.name));
                        }
                    }
                    break;
                }
            }
        }
    }
}
