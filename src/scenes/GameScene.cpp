#include "GameScene.h"
#include <iostream>
#include "fmt/format.h"
#include <QPainter>
#include "res/GameSource.h"
#include "res/GameMap.h"
#include <QSize>
#include "ecs/components/ComponentCamera.hpp"
#include "ecs/components/ComponentBase.hpp"
#include "ecs/components/ComponentAnimation.hpp"
#include "ecs/components/ComponentAngle.h"
#include "ecs/components/ComponentHealth.h"
#include "ecs/components/ComponentBullet.h"
#include "ecs/components/ComponentFirearmsWeapon.h"
#include "managers/GameManager.h"

using namespace danMuGame;
using namespace danMuGame::component;

GameScene::GameScene(QWidget *parent)
    : QWidget{parent}
{
    std::cout << "GameScene create " << std::endl;
}

GameScene::~GameScene()
{
}

void GameScene::paintEvent(QPaintEvent *)
{
    if (m_registry != nullptr)
    {
        QPainter painter(this);
        QFont font;
        font.setPointSize(10);
        painter.setFont(font);
        paintBackground(painter);
        paintPlayer(painter);
        paintBullet(painter);
    }
}

void GameScene::paintBackground(QPainter &painter)
{
    const GameMap *map = GameManager::GetInstance()->getNowGameMap();
    const Size2 &mapTitleSize = map->getPixmapSize();

    // std::cout << fmt::format("[pixmap]width: {} height: {}",mapTitleSize.width(), mapTitleSize.height()) << std::endl;

    int drawWidthCount = (this->width() / mapTitleSize.width()) + 2;
    int drawHeightCount = (this->height() / mapTitleSize.height()) + 2;

    // std::cout << fmt::format("[pixmap]drawCount: {} height: {}",drawWidthCount, drawHeightCount) << std::endl;

    auto cameraEntity = m_registry->view<ComponentCamera>();
    auto &camera = cameraEntity.get<ComponentCamera>(*(cameraEntity.begin()));

    int cameraViewPointX = camera.viewPoint.x() - (this->width() / 2);
    int cameraViewPointY = camera.viewPoint.y() - (this->height() / 2);

    // std::cout << fmt::format("x:{},y:{}", cameraViewPointX, cameraViewPointY) << std::endl;

    int cameraPointX = cameraViewPointX / mapTitleSize.width();
    int cameraPointY = cameraViewPointY / mapTitleSize.height();

    Point2 sourcePoint;

    // 偏移画板
    int painterOffsetX = cameraViewPointX % mapTitleSize.width();
    int painterOffsetY = cameraViewPointY % mapTitleSize.height();
    painter.translate(-painterOffsetX - mapTitleSize.width() / 2, -painterOffsetY - mapTitleSize.height() / 2);

    // 计算并绘制背景图
    for (int posY = (cameraPointY < 0 ? -cameraPointY : 0); posY <= drawHeightCount; ++posY)
    {
        if (cameraPointY + posY >= map->getMapSize().height())
        {
            break;
        }
        for (int posX = (cameraPointX < 0 ? -cameraPointX : 0); posX <= drawWidthCount; ++posX)
        {
            if (cameraPointX + posX >= map->getMapSize().width())
            {
                break;
            }
            for (int layer = 0; layer < map->getLayerCount(); ++layer)
            {
                sourcePoint.setX(posX + cameraPointX);
                sourcePoint.setY(posY + cameraPointY);
                int sourceId = map->getSourceId(layer, sourcePoint);
                if (sourceId != -1)
                {
                    painter.drawPixmap(posX * mapTitleSize.width(), posY * mapTitleSize.height(), GameSource::GetInstance()->getMapSource(sourceId));
                }
            }
        }
    }
    painter.resetTransform();
}

void GameScene::paintPlayer(QPainter &painter)
{
    auto cameraEntity = m_registry->view<ComponentCamera>();
    const auto &camera = cameraEntity.get<ComponentCamera>(*(cameraEntity.begin()));
    int cameraViewPointX = camera.viewPoint.x() - (this->width() / 2);
    int cameraViewPointY = camera.viewPoint.y() - (this->height() / 2);

    auto view = m_registry->view<ComponentPlayer, ComponentHealth, ComponentAnimation, ComponentFirearmsWeapon, ComponentMove, ComponentAngle>();
    for (auto entity : view)
    {
        const auto &[player, animation, move, componentAngle] = view.get<ComponentPlayer, ComponentAnimation, ComponentMove, ComponentAngle>(entity);
        if (animation.animationKey == 0)
        {
            continue;
        }
        int drawPointX = move.position.x() - cameraViewPointX;
        int drawPointY = move.position.y() - cameraViewPointY;
        auto &AnimationPixmap = GameSource::GetInstance()->getAnimationSource(animation.animationKey, animation.currentFrameIndex);
        if (drawPointX + AnimationPixmap.width() > 0 &&
            drawPointY + AnimationPixmap.height() > 0 &&
            drawPointX - AnimationPixmap.width() < this->width() &&
            drawPointY - AnimationPixmap.height() < this->height())
        {
            painter.translate(drawPointX, drawPointY);
            int targetPointX = -(AnimationPixmap.width() / 2);
            int targetPointY = -(AnimationPixmap.height() / 2);

            /*
                    painter.setPen(Qt::white);
                    painter.drawText(targetPointX, targetPointY - 5, QString::fromStdString(fmt::format("{}:{}", player.team, player.name)));

                    Qt::GlobalColor healthDrawColor = Qt::green;
                    if (health.health < health.maxHealth * 0.3f)
                    {
                        healthDrawColor = Qt::red;
                    }
                    else if (health.health < health.maxHealth * 0.8f)
                    {
                        healthDrawColor = Qt::yellow;
                    }
                    painter.fillRect(targetPointX, targetPointY - 2, AnimationPixmap.width() * ((float)health.health / health.maxHealth), 3, healthDrawColor);
                    */
            // painter.fillRect(targetPointX, targetPointY + 2, playerPixmap.width() * ((float)m_player->getPrepare() / 100), 3, Qt::gray);

            /**************** 画贴图 **************/

            painter.rotate(componentAngle.angle);

            // std::cout << move.angle << std::endl;

            painter.translate(targetPointX, targetPointY);
            painter.drawPixmap(0, 0, AnimationPixmap.width(), AnimationPixmap.height(), AnimationPixmap);
            painter.resetTransform();

            /**************** 画名字 **************/

            // painter.translate(drawPointX, drawPointY);
            // painter.setPen(QPen(Qt::white, 1));
            // QFont font;
            // font.setPointSize(15);
            // painter.setFont(font);
            // painter.drawText(targetPointX, targetPointY - 5, QString::fromStdString(fmt::format("{}:{}", player.team, player.name)));

            // Qt::GlobalColor healthDrawColor = Qt::green;
            // if (health.health < health.maxHealth * 0.3f)
            // {
            //     healthDrawColor = Qt::red;
            // }
            // else if (health.health < health.maxHealth * 0.8f)
            // {
            //     healthDrawColor = Qt::yellow;
            // }
            // painter.fillRect(targetPointX, targetPointY - 2, AnimationPixmap.width() * ((float)health.health / health.maxHealth), 3, healthDrawColor);
            // painter.resetTransform();
        }
    }
    painter.setPen(QPen(Qt::white, 1));
    QFont font;
    font.setPointSize(11);
    painter.setFont(font);
    for (auto entity : view)
    {
        const auto &[player, health, animation, firearmsWeapon, move] = view.get<ComponentPlayer, ComponentHealth, ComponentAnimation, ComponentFirearmsWeapon, ComponentMove>(entity);
        if (animation.animationKey == 0)
        {
            continue;
        }
        int drawPointX = move.position.x() - cameraViewPointX;
        int drawPointY = move.position.y() - cameraViewPointY;
        auto &AnimationPixmap = GameSource::GetInstance()->getAnimationSource(animation.animationKey, animation.currentFrameIndex);
        if (drawPointX + AnimationPixmap.width() > 0 &&
            drawPointY + AnimationPixmap.height() > 0 &&
            drawPointX - AnimationPixmap.width() < this->width() &&
            drawPointY - AnimationPixmap.height() < this->height())
        {
            int targetPointX = -(AnimationPixmap.width() / 2);
            int targetPointY = -(AnimationPixmap.height() / 2);

            /**************** 画名字 **************/

            painter.translate(drawPointX, drawPointY);

            painter.fillRect(targetPointX, targetPointY - 16, AnimationPixmap.width(), 14, QColor(0x40, 0x40, 0x40, 100));

            painter.drawText(targetPointX, targetPointY - 5, QString::fromStdString(fmt::format("{}:{}", player.team, player.name)));

            Qt::GlobalColor healthDrawColor = Qt::green;
            if (health.health < health.maxHealth * 0.3f)
            {
                healthDrawColor = Qt::red;
            }
            else if (health.health < health.maxHealth * 0.8f)
            {
                healthDrawColor = Qt::yellow;
            }
            // 画生命值
            painter.fillRect(targetPointX, targetPointY - 2, AnimationPixmap.width() * ((float)health.health / health.maxHealth), 3, healthDrawColor);

            if (GameManager::GetInstance()->getGameStatue() == GameStatue::Prepare)
            {
                painter.fillRect(targetPointX, targetPointY, AnimationPixmap.width(), 54, QColor(0x40, 0x40, 0x40, 150));

                painter.drawText(targetPointX, targetPointY + 14, QString::fromStdString(fmt::format("{}/{}", health.health, health.maxHealth)));

                // painter.fillRect(targetPointX, targetPointY + 16, AnimationPixmap.width(), 18, QColor(0x40, 0x40, 0x40, 150));

                if (firearmsWeapon.mainWeapon.weaponBase != nullptr)
                {
                    painter.drawText(targetPointX, targetPointY + 30, QString::fromStdString(firearmsWeapon.mainWeapon.weaponBase->name));
                }
                else
                {
                    painter.drawText(targetPointX, targetPointY + 30, "无");
                }

                std::string skillName = "无";
                switch (player.skill)
                {
                case PlayerSkill::SUCK_BLOOD:
                    skillName = "吸血";
                    break;
                case PlayerSkill::RECOVER:
                    skillName = "回血";
                    break;
                case PlayerSkill::TRIUMPH:
                    skillName = "凯旋";
                    break;
                case PlayerSkill::DODGE:
                    skillName = "闪避";
                    break;
                case PlayerSkill::HARVEST:
                    skillName = "收割";
                    break;
                case PlayerSkill::HARM_PLUS:
                    skillName = "中伤";
                    break;
                }
                painter.drawText(targetPointX, targetPointY + 48, QString::fromStdString(skillName));
            }
            painter.resetTransform();
        }
    }
}

void GameScene::paintBullet(QPainter &painter)
{
    auto view = m_registry->view<ComponentBullet>();
    auto cameraEntity = m_registry->view<ComponentCamera>();
    const auto &camera = cameraEntity.get<ComponentCamera>(*(cameraEntity.begin()));

    int halfWidth = this->width() / 2;
    int halfHeight = this->height() / 2;
    Vector2 cameraViewVector = Vector2(camera.viewPoint.x() - halfWidth, camera.viewPoint.y() - halfHeight);

    for (auto entity : view)
    {
        auto &bullet = view.get<ComponentBullet>(entity);
        if (bullet.isUse == true)
        {
            painter.setPen(QPen(QColor(bullet.color.r(), bullet.color.g(), bullet.color.b()), 1));

            auto startVector = bullet.moveRoute[bullet.startIndex] - cameraViewVector;
            auto endVector = bullet.moveRoute[bullet.endIndex] - cameraViewVector;

            if ((abs(bullet.moveRoute[bullet.startIndex].x() - camera.viewPoint.x()) <= halfWidth && abs(bullet.moveRoute[bullet.startIndex].y() - camera.viewPoint.y()) <= halfHeight) ||
                (abs(bullet.moveRoute[bullet.endIndex].x() - camera.viewPoint.x()) <= halfWidth && abs(bullet.moveRoute[bullet.endIndex].y() - camera.viewPoint.y()) <= halfHeight))
            {
                painter.drawLine(startVector.x(), startVector.y(), endVector.x(), endVector.y());
            }
            // else
            // {
            //     // std::cout << "no draw " << fmt::format("cx:{} cy:{} sx:{}, sy:{}, ex:{}, ey:{} count: {}", camera.viewPoint.x(), camera.viewPoint.y(), startVector.x(), startVector.y(), endVector.x(), endVector.y(), count) << std::endl;

            //     // std::cout << fmt::format("moveRount sx:{}, sy:{}, ex:{}, ey:{}", bullet.moveRoute[0].x(), bullet.moveRoute[0].y(), bullet.moveRoute[bullet.moveRoute.size() - 1].x(), bullet.moveRoute[bullet.moveRoute.size() - 1].y()) << std::endl;
            // }
        }
    }
}
