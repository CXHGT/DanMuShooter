#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include <QWidget>
#include <QObject>
#include <QPainter>
#include "entt/entt.hpp"
#include "Point2.hpp"
#include "Vector2.hpp"

class GameScene : public QWidget
{
    Q_OBJECT
private:
    int m_frameCount = 0;
    entt::registry *m_registry = nullptr;

public:
    explicit GameScene(QWidget *parent = nullptr);
    ~GameScene();
    virtual void paintEvent(QPaintEvent *) override;
    inline void setRegistry(entt::registry *registry)
    {
        m_registry = registry;
    }

private:
    void paintBackground(QPainter &painter);
    void paintPlayer(QPainter &painter);
    void paintBullet(QPainter &painter);

signals:
};

#endif // GAMESCENE_H
