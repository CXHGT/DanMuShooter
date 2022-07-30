#include "GameWidget.h"
#include "managers/GameSettings.h"
#include "managers/GameManager.h"
#include "managers/DanMuManager.h"
#include "managers/PlayerManager.h"
#include "managers/BulletManager.h"
#include "managers/SystemManager.h"
#include "scenes/GameGui.h"
#include "res/GameSource.h"
#include "./ui_GameWidget.h"

using namespace danMuGame;

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    auto *gameGui = GameGui::GetInstance();
    gameGui->setFrameLabel(ui->frameLabel);

    gameGui->mainMessage1Label = ui->message1Label;
    gameGui->mainMessage2Label = ui->message2Label;
    gameGui->mainMessage3Label = ui->message3Label;

    gameGui->subMessage1Label = ui->sublabel1;
    gameGui->subMessage2Label = ui->sublabel2;
    gameGui->subMessage3Label = ui->sublabel3;
    gameGui->subMessage4Label = ui->sublabel4;
    gameGui->subMessage5Label = ui->sublabel5;
    gameGui->subMessage6Label = ui->sublabel6;
    gameGui->subMessage7Label = ui->sublabel7;
    gameGui->subMessage8Label = ui->sublabel8;

    gameGui->playerNameLabel = ui->playerNameLabel;
    gameGui->playerHealthLabel = ui->playerHealthLabel;
    gameGui->playerFirearmsWeaponLabel = ui->firearmsWeaponLabel;
    gameGui->playerDisuseLabel = ui->disuseLabel;
    gameGui->playerSkillLabel = ui->skillLabel;

    gameGui->playerCountLabel = ui->playerCountLabel;
    gameGui->timeLabel = ui->timeLabel;

    gameGui->musicPlayModule = ui->musicPlayModule;
    gameGui->giftLabel = ui->giftLabel;

    gameGui->summaryWidgetModule = ui->summaryWidgetModule;

    gameGui->introduceLabel = ui->introduceLabel;

    GameSource::GetInstance()->initialization();

    PlayerManager::GetInstance()->initialization();
    BulletManager::GetInstance()->initialization();
    GameManager::GetInstance()->initialization();
    GameManager::GetInstance()->setScene(ui->gameScene);

    SystemManager::GetInstance()->initialization();

    DanMuManager::GetInstance()->initialization();

    this->updateTimeId = this->startTimer(16);
    this->update1SecondTimeId = this->startTimer(1000);
}

void GameWidget::closeEvent(QCloseEvent *event)
{
    DanMuManager::GetInstance()->Stop();
    event->accept();
}

void GameWidget::timerEvent(QTimerEvent *event)
{
    auto *gameManager = GameManager::GetInstance();
    if (this->updateTimeId == event->timerId())
    {
        gameManager->update(1.0f / 60);
    }
    else if (this->update1SecondTimeId == event->timerId())
    {
        gameManager->update_1_second();
    }
}

GameWidget::~GameWidget()
{
    qDebug() << "~ GameWidget";
    delete ui;
}
