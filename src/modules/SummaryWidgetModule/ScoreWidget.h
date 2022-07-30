#ifndef SCOREWIDGET_H
#define SCOREWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include "ScoreData.hpp"

class ScoreWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel * rankingLabel;
    QLabel * nameLabel;
    QLabel * injureScoreLabel;
    QLabel * killScoreLabel;

public:
    explicit ScoreWidget(QWidget *parent = nullptr);

    void setData(const ScoreData& data);

    void clear();

signals:

};

#endif // SCOREWIDGET_H
