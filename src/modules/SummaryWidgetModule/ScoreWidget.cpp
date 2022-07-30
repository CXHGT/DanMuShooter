#include "ScoreWidget.h"

ScoreWidget::ScoreWidget(QWidget *parent)
    : QWidget{parent}
{
    QFont font;
    font.setPointSize(12);

    this->rankingLabel = new QLabel(this);
    this->rankingLabel->setGeometry(0, 0, 30, 30);
    this->rankingLabel->setAlignment(Qt::AlignCenter);
    this->rankingLabel->setFont(font);
    this->rankingLabel->setStyleSheet("color:white");

    this->nameLabel = new QLabel(this);
    this->nameLabel->setGeometry(40, 0, 210, 30);
    this->nameLabel->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);
    this->nameLabel->setFont(font);
    this->nameLabel->setStyleSheet("color:white");

    this->injureScoreLabel = new QLabel(this);
    this->injureScoreLabel->setGeometry(260, 0, 70, 30);
    this->injureScoreLabel->setAlignment(Qt::AlignCenter);
    this->injureScoreLabel->setFont(font);
    this->injureScoreLabel->setStyleSheet("color:white");

    this->killScoreLabel = new QLabel(this);
    this->killScoreLabel->setGeometry(340, 0, 60, 30);
    this->killScoreLabel->setAlignment(Qt::AlignCenter);
    this->killScoreLabel->setFont(font);
    this->killScoreLabel->setStyleSheet("color:white");

    this->setGeometry(0,0,400,30);
}

void ScoreWidget::setData(const ScoreData &data)
{
    this->rankingLabel->setText(QString::number(data.ranking));
    this->nameLabel->setText(QString::fromStdString(data.name));
    this->injureScoreLabel->setText(QString::number(data.injureScore));
    this->killScoreLabel->setText(QString::number(data.killScore));
}

void ScoreWidget::clear()
{
    this->rankingLabel->setText("");
    this->nameLabel->setText("");
    this->injureScoreLabel->setText("");
    this->killScoreLabel->setText("");
}
