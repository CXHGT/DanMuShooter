#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include "BSdk.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class GameWidget;
}
QT_END_NAMESPACE

class GameWidget : public QWidget
{
    Q_OBJECT
private:
    int updateTimeId;
    int update1SecondTimeId;

public:
    GameWidget(QWidget *parent = nullptr);
    ~GameWidget();
    virtual void closeEvent(QCloseEvent *) override;
    virtual void timerEvent(QTimerEvent *) override;

private:
    Ui::GameWidget *ui;
};
#endif // GAMEWIDGET_H
