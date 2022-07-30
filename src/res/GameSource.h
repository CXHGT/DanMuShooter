#ifndef __GAME_SOURCE_H__
#define __GAME_SOURCE_H__

#include <vector>
#include <QPixmap>
#include <stdint.h>
#include "GameMap.h"
#include <unordered_map>
#include "nlohmann/json.hpp"
#include "ecs/components/ComponentFirearmsWeapon.h"
#include <string>
#include "Animation.h"
#include <QBuffer>
#include <mutex>

namespace danMuGame
{

    namespace component
    {
        class ComponentAnimation;
    }

    class GameSource
    {
    private:
        static GameSource *m_instance;
        static std::once_flag m_flag;

    private:
        GameSource();
        ~GameSource();
        GameSource(const GameSource &) = delete;
        GameSource(GameSource &&) = delete;
        GameSource &operator=(const GameSource &) = delete;
        GameSource &operator=(GameSource &&) = delete;

        void LoadMapSource(const nlohmann::json &jsonData);
        void LoadAnimationSource(const nlohmann::json &jsonData);
        void LoadGiftSource(const nlohmann::json &jsonData);
        void LoadBgmBuffer(const nlohmann::json &jsonData);
        void LoadIntroduceSource(const nlohmann::json &jsonData);

        void LoadFirearmsWeapon(const nlohmann::json &jsnoData);

        class GC
        {
        public:
            ~GC()
            {
                if (GameSource::m_instance)
                {
                    delete GameSource::m_instance;
                    GameSource::m_instance = nullptr;
                }
            }
        };

    public:
        std::vector<QPixmap> m_mapTiledSourceVec;

        // std::unordered_map<AnimationKey, std::vector<QPixmap>> m_animationSource;
        std::unordered_map<int32_t, Animation> m_animationSource;
        std::unordered_map<std::string, int32_t> m_animationNameTable;
        // 保管地图
        std::unordered_map<std::string, GameMap *> m_gameMapTable;
        // 保管枪械
        std::unordered_map<std::string, FirearmsWeaponBase> m_firearmsWeaponTable;

        std::unordered_map<std::string, QBuffer *> m_bgmCatch;

        QPixmap *gift1 = nullptr;
        QPixmap *gift2 = nullptr;

        QPixmap *introduce = nullptr;

    public:
        static GameSource *GetInstance();

        void initialization();

    public:


        GameMap *getMap(const std::string &mapName);

        inline QPixmap &getMapSource(int id)
        {
            return m_mapTiledSourceVec[id];
        }

        inline QPixmap &getAnimationSource(int key, int id)
        {
            return m_animationSource[key].source[id];
        }

        void initializationAnimation(component::ComponentAnimation &animation, const std::string &animationName);

        void initFirearmsWeapon(FirearmsWeapon &firearmsWeapon, const std::string &firearmsWeaponName);
    };
}

#endif
