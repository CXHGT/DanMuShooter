#include "GameSource.h"
#include <mutex>
#include <memory>
#include "SourceUtils.h"
#include <QDebug>
#include <iostream>
#include <string>
#include <QImage>
#include "base64.h"
#include "fmt/format.h"
#include "CxhUtils.h"
#include "ecs/components/ComponentAnimation.hpp"
#include <QFile>

namespace danMuGame
{

    GameSource *GameSource::m_instance = nullptr;
    std::once_flag GameSource::m_flag;

    GameSource::GameSource()
    {
    }

    GameSource *GameSource::GetInstance()
    {
        std::call_once(m_flag,
                       []
                       {
                           GameSource::m_instance = new GameSource();
                           static GC gc;
                       });
        return GameSource::m_instance;
    }

    void GameSource::initialization()
    {
        // 加载资源json文件
        bool isOk = false;
        std::string jsonStr = CxhUtils::LoadTextFile("./Resources/sources.json", isOk);

        assert(isOk == true);
        nlohmann::json jsonData = nlohmann::json::parse(jsonStr);

        LoadAnimationSource(jsonData);
        LoadMapSource(jsonData);
        LoadGiftSource(jsonData);
        LoadBgmBuffer(jsonData);
        LoadIntroduceSource(jsonData);

        std::string weaponStr = CxhUtils::LoadTextFile("./Resources/FirearmsWeapon.json", isOk);
        assert(isOk == true);
        nlohmann::json weaponJson = nlohmann::json::parse(weaponStr);
        LoadFirearmsWeapon(weaponJson);
    }

    void GameSource::LoadFirearmsWeapon(const nlohmann::json &jsnoData)
    {
        for (auto it = jsnoData.begin(); it != jsnoData.end(); ++it)
        {
            std::cout << fmt::format("load gun : {}", it.key()) << std::endl;
            this->m_firearmsWeaponTable[it.key()] = FirearmsWeaponBase{
                .name = it.key(),
                .injure = it.value()["injure"].get<int32_t>(),
                .numberOfBullets = it.value()["numberOfBullets"].get<int32_t>(),
                .shootBulletCount = it.value()["shootBulletCount"].get<int32_t>(),
                .recoveryFrame = it.value()["recoveryFrame"].get<int32_t>(),
                .attackDistance = it.value()["attackDistance"].get<int32_t>(),
                .attackIntervalFrame = it.value()["attackIntervalFrame"].get<int32_t>(),
                .speed = it.value()["speed"].get<int32_t>(),
                .accuracy = it.value()["accuracy"].get<int32_t>(),
                .discoveryDistance = it.value()["discoveryDistance"].get<int32_t>(),
                .bulletType = (BulletType)(it.value()["bulletType"].get<int32_t>()),
                .behavior = (FirearmsWeaponBehavior)(it.value()["behavior"].get<int32_t>())};
        }
    }

    void GameSource::LoadAnimationSource(const nlohmann::json &jsonData)
    {
        m_animationNameTable["no animation"] = 0;
        int32_t number = 1;
        for (auto it = jsonData["animation"].begin(); it != jsonData["animation"].end(); ++it)
        {
            for (auto subIt = it.value().begin(); subIt != it.value().end(); ++subIt)
            {
                std::string tableName = fmt::format("{}_{}", it.key(), subIt.key());

                std::cout << fmt::format("loading Animation : {}", tableName) << std::endl;

                this->m_animationNameTable[tableName] = number;

                int32_t sizeX = subIt.value()["imageSize"]["x"].get<int32_t>();
                int32_t sizeY = subIt.value()["imageSize"]["y"].get<int32_t>();
                int32_t cutX = subIt.value()["cutSize"]["x"].get<int32_t>();
                int32_t cutY = subIt.value()["cutSize"]["y"].get<int32_t>();
                int32_t count = subIt.value()["count"].get<int32_t>();
                int32_t intervalFrame = subIt.value()["intervalFrame"].get<int32_t>();
                QPixmap pixmap(subIt.value()["path"].get<std::string>().c_str());

                this->m_animationSource[number] = Animation{std::vector<QPixmap>(), intervalFrame};
                SourceUtils::SplitQPixmap(pixmap, this->m_animationSource[number].source, sizeX, sizeY, cutX, cutY, count);

                ++number;
            }
        }
    }

    void GameSource::LoadMapSource(const nlohmann::json &jsonData)
    {
        int32_t tiledWidth = jsonData["map"]["tiledWidth"].get<int32_t>();
        int32_t tiledHeight = jsonData["map"]["tiledHeight"].get<int32_t>();
        Size2 mapTitleSize = Size2(tiledWidth, tiledHeight);
        for (const auto &material : jsonData["map"]["materials"])
        {
            std::string materialName = material.get<std::string>();

            std::cout << fmt::format("loading material : {}", materialName) << std::endl;

            QPixmap mapMaterialPixmap(materialName.c_str());
            int32_t heightCount = mapMaterialPixmap.height() / tiledWidth;
            int32_t widthCount = mapMaterialPixmap.width() / tiledHeight;
            SourceUtils::SplitQPixmap(mapMaterialPixmap, m_mapTiledSourceVec, tiledWidth, tiledHeight, widthCount, heightCount);
        }

        for (auto it = jsonData["map"]["maps"].begin(); it != jsonData["map"]["maps"].end(); ++it)
        {
            bool isOk = false;
            std::string text = CxhUtils::LoadTextFile(it.value().get<std::string>(), isOk);
            assert(isOk == true);

            nlohmann::json mapJson = nlohmann::json::parse(text);
            Size2 mapSize(mapJson["width"].get<int32_t>(), mapJson["height"].get<int32_t>());
            int32_t layerCount = mapJson["layers"].size();

            std::cout << fmt::format("loading Map : {}", it.key()) << std::endl;

            GameMap *gameMap = new GameMap(layerCount, mapSize, mapTitleSize);
            this->m_gameMapTable[it.key()] = gameMap;

            for (int layer = 0; layer < layerCount; ++layer)
            {
                std::string layerData = base64_decode(mapJson["layers"][layer]["data"].get<std::string>(), true);
                unsigned long unzip_size = 0;
                auto *unzipBuffer = CxhUtils::Single_UnZip(layerData, unzip_size);
                gameMap->setLayerMemory(layer, (const char *)unzipBuffer, unzip_size);
            }
        }
    }

    void GameSource::LoadGiftSource(const nlohmann::json &jsonData)
    {
        this->gift1 = new QPixmap(jsonData["gift"]["gift1"].get<std::string>().c_str());
        this->gift2 = new QPixmap(jsonData["gift"]["gift2"].get<std::string>().c_str());
    }

    void GameSource::LoadIntroduceSource(const nlohmann::json &jsonData)
    {
        this->introduce = new QPixmap(jsonData["introduce"]["introduce1"].get<std::string>().c_str());
    }

    void GameSource::LoadBgmBuffer(const nlohmann::json &jsonData)
    {
        for (auto it = jsonData["bgm"].begin(); it != jsonData["bgm"].end(); ++it)
        {
            QFile file(it.value().get<std::string>().c_str());

            if (!file.open(QIODevice::ReadOnly))
                qDebug() << "File not opened";
            qDebug() << "File size:" << file.size(); // File size: 11181085

            QByteArray ba = file.readAll();
            qDebug() << "ByteArray size:" << ba.size(); // ByteArray size: 11181085

            QBuffer *buffer = new QBuffer;
            buffer->setData(ba);
            if (!buffer->open(QIODevice::ReadOnly))
                qDebug() << "Buffer not opened";
            qDebug() << "Buffer size:" << buffer->size(); // Buffer size: 11181085

            buffer->seek(qint64(0));

            this->m_bgmCatch[it.key()] = buffer;
        }
    }

    GameMap *GameSource::getMap(const std::string &mapName)
    {
        if (m_gameMapTable.find(mapName) != m_gameMapTable.end())
        {
            return m_gameMapTable[mapName];
        }
        return nullptr;
    }

    GameSource::~GameSource()
    {
        for (auto keyValue : m_gameMapTable)
        {
            delete keyValue.second;
        }
    }

    void GameSource::initializationAnimation(component::ComponentAnimation &animation, const std::string &animationName)
    {
        if (m_animationNameTable.find(animationName) != m_animationNameTable.end())
        {
            animation.currentFrameIndex = 0;

            int32_t key = m_animationNameTable[animationName];
            animation.animationKey = key;
            auto &animationVec = m_animationSource[key];
            animation.frameCount = (int32_t)animationVec.source.size();
            // std::cout << animationVec.size() << std::endl;

            animation.isPlaying = true;
            animation.delayFrame = animationVec.intervalFrame;
            animation.elapsedFrame = 0;
            animation.isLoop = true; // 默认循环播放
        }
        else
        {
            animation.isPlaying = false;
            animation.animationKey = 0;
        }
    }

    void GameSource::initFirearmsWeapon(FirearmsWeapon &firearmsWeapon, const std::string &firearmsWeaponName)
    {
        if (m_firearmsWeaponTable.find(firearmsWeaponName) != m_firearmsWeaponTable.end())
        {
            firearmsWeapon.weaponBase = &(m_firearmsWeaponTable[firearmsWeaponName]);
            firearmsWeapon.currentBullets = 0;
            firearmsWeapon.currentIntervalFrame = 0;
            firearmsWeapon.currentRecoveryFrame = 0;
            firearmsWeapon.isCanShoot = false;
        }
    }
}
