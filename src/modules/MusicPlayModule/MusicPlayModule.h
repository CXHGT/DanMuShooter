#ifndef MUSICPLAYMODULE_H
#define MUSICPLAYMODULE_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QLabel>
#include <QSlider>
#include <stdint.h>
#include <QBuffer>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MusicPlayModule;
}
QT_END_NAMESPACE

struct MusicData
{
    QString musicName;
    QUrl musicUrl;
};

class MusicPlayModule : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPlayModule(QWidget *parent = nullptr);
    ~MusicPlayModule();

    void PlayMusic(MusicData musicData, int level = 0);

    void PlayMusicByBuffer(const std::string& musicName,QBuffer *buffer, int level = 0);

    inline void setSound(int sound)
    {
        this->audioOutput->setVolume(sound);
    }

    inline bool getIsPlaying()
    {
        return this->isPlaying;
    }

    inline int32_t getNowLevel()
    {
        return this->playLevel;
    }

public slots:
    void positionChanged(qint64 position);
    void durationChanged();
    void musicError();

signals:

private:
    Ui::MusicPlayModule *ui;
    QMediaPlayer *mediaPlayer = nullptr;
    QAudioOutput *audioOutput = nullptr;
    QLabel *songName;
    QSlider *slider;

    bool isPlaying;
    int playLevel;

    qint64 musicDuration;

    MusicData musicData;
};
#endif // MUSICPLAYMODEL_H
