#include "MusicPlayModule.h"
#include "ui_MusicPlayModule.h"
#include "CommonUtils.h"
#include <QDebug>
#include <iostream>
#include <QMediaMetaData>

MusicPlayModule::MusicPlayModule(QWidget *parent) : QWidget(parent), ui(new Ui::MusicPlayModule)
{
    ui->setupUi(this);

    audioOutput = new QAudioOutput(this);
    mediaPlayer = new QMediaPlayer(this);
    audioOutput->setVolume(50);
    mediaPlayer->setAudioOutput(audioOutput);

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayModule::positionChanged);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, ui->musicSlider, &QSlider::setValue);
    connect(ui->musicSlider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &MusicPlayModule::durationChanged);
    connect(mediaPlayer, &QMediaPlayer::errorChanged, this, &MusicPlayModule::musicError);
}

void MusicPlayModule::positionChanged(qint64 position)
{
    if (position < musicDuration)
    {
        ui->timeLabel->setText(CommonUtils::formatDuration(position) + " / " + CommonUtils::formatDuration(musicDuration));
    }
    else
    {
        mediaPlayer->stop();
        this->isPlaying = false;
        // this->mediaPlayer->metaData().clear();
    }
}

void MusicPlayModule::PlayMusic(MusicData musicData, int level)
{
    if (this->playLevel <= level || this->isPlaying == false)
    {
        this->musicData = musicData;
        this->playLevel = level;
        mediaPlayer->stop();

        mediaPlayer->setSource(musicData.musicUrl);
        mediaPlayer->play();

        this->isPlaying = true;
        ui->musicName->setText(musicData.musicName);
    }
}

void MusicPlayModule::PlayMusicByBuffer(const std::string &musicName, QBuffer *buffer, int level)
{
    if (this->playLevel <= level || this->isPlaying == false)
    {
        this->playLevel = level;
        mediaPlayer->stop();

        mediaPlayer->setSourceDevice(buffer, QUrl(musicName.c_str()));
        mediaPlayer->play();

        this->isPlaying = true;
        ui->musicName->setText(musicName.c_str());
    }
}

void MusicPlayModule::durationChanged()
{
    this->musicDuration = mediaPlayer->duration();
    std::cout << this->musicDuration << std::endl;
    ui->timeLabel->setText("00:00 / " + CommonUtils::formatDuration(musicDuration));
    ui->musicSlider->setMaximum(musicDuration);
}

void MusicPlayModule::musicError()
{
    mediaPlayer->stop();
    this->isPlaying = false;
}

MusicPlayModule::~MusicPlayModule()
{
    delete ui;
}
