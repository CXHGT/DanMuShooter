#include "GameSettings.h"
#include <mutex>
#include <iomanip>

namespace danMuGame
{
    GameSettings *GameSettings::m_instance = nullptr;
    std::once_flag GameSettings::m_flag;

    GameSettings::GameSettings()
    {
    }

    GameSettings::~GameSettings()
    {
    }

    GameSettings *GameSettings::GetInstance()
    {
        std::call_once(m_flag,
                       []
                       {
                           GameSettings::m_instance = new GameSettings();
                           static GC gc;
                       });
        return GameSettings::m_instance;
    }

    void GameSettings::initialization()
    {
        //加载settings.json文件
        std::ifstream inputFile(fileName);
        if (inputFile)
        {
            nlohmann::json jsonData;
            inputFile >> jsonData;
            // 玩家最大数量
            if (jsonData.find("playerMaxCount") == jsonData.end())
            {
                m_playerMaxCount = 100;
            }
            else
            {
                m_playerMaxCount = jsonData["playerMaxCount"].get<int>();
                if (m_playerMaxCount < 0)
                {
                    m_playerMaxCount = 100;
                }
            }
            // 人机最大数量
            if (jsonData.find("robotMaxCount") == jsonData.end())
            {
                m_robotMaxCount = m_playerMaxCount / 2;
            }
            else
            {
                m_robotMaxCount = jsonData["robotMaxCount"].get<int>();
                if (m_robotMaxCount < 0)
                {
                    m_robotMaxCount = m_playerMaxCount / 2;
                }
            }
            // 身份码是否保存
            if (jsonData.find("gameCodeIsSave") == jsonData.end())
            {
                m_gameCodeIsSave == false;
            }
            else
            {
                m_gameCodeIsSave = jsonData["gameCodeIsSave"].get<bool>();
            }
            // 身份码
            if (jsonData.find("gameCode") == jsonData.end())
            {
                m_gameCode = "";
            }
            else
            {
                m_gameCode = jsonData["gameCode"].get<std::string>();
            }

            inputFile.close();
            return;
        }
        inputFile.close();

        m_playerMaxCount = 100;
        m_robotMaxCount = m_playerMaxCount / 2;
        m_gameCodeIsSave = false;
        m_gameCode = "";
        save();
    }

    void GameSettings::save()
    {
        std::ofstream outputFile(fileName);
        if (outputFile)
        {
            nlohmann::json jsonData;
            jsonData["playerMaxCount"] = m_playerMaxCount;
            jsonData["robotMaxCount"] = m_robotMaxCount;
            jsonData["gameCodeIsSave"] = m_gameCodeIsSave;
            if (m_gameCodeIsSave == true)
            {
                jsonData["gameCode"] = m_gameCode;
            }
            else
            {
                jsonData["gameCode"] = "";
            }
            // std::setw(4)用于格式化文件
            outputFile << std::setw(4) << jsonData;
        }
        outputFile.close();
    }
}