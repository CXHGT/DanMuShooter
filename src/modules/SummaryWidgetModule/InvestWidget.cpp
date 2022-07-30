#include "InvestWidget.h"

InvestWidget::InvestWidget(QWidget *parent)
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
    this->nameLabel->setGeometry(40, 0, 270, 30);
    this->nameLabel->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);
    this->nameLabel->setFont(font);
    this->nameLabel->setStyleSheet("color:white");

    this->InvestLabel = new QLabel(this);
    this->InvestLabel->setGeometry(310, 0, 90, 30);
    this->InvestLabel->setAlignment(Qt::AlignCenter);
    this->InvestLabel->setFont(font);
    this->InvestLabel->setStyleSheet("color:white");

    this->setGeometry(0, 0, 400, 30);
}

void InvestWidget::setData(const InvestData &data)
{
    this->rankingLabel->setText(QString::number(data.ranking));
    this->nameLabel->setText(QString::fromStdString(data.name));
    this->InvestLabel->setText(QString::number(data.investScore));
}

void InvestWidget::clear()
{
    this->rankingLabel->setText("");
    this->nameLabel->setText("");
    this->InvestLabel->setText("");
}
