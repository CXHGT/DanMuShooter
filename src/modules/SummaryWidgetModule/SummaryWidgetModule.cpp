#include "SummaryWidgetModule.h"
#include "./ui_SummaryWidgetModule.h"
#include <QLayout>
#include <QBoxLayout>

SummaryWidgetModule::SummaryWidgetModule(QWidget *parent)
    : QWidget(parent), ui(new Ui::SummaryWidgetModule)
{
    ui->setupUi(this);
    ScoreData scoreData{
        .ranking = 0,
        .name = "你好中文",
        .injureScore = 999,
        .killScore = 999};

    InvestData investData{
        .ranking = 0,
        .name = "这是中文",
        .investScore = 999};

    for (int index = 0; index < 22; ++index)
    {
        ScoreWidget *scoreWidget = new ScoreWidget(ui->score);
        scoreWidget->setGeometry(0, index * 30, 400, 30);
        scoreWidget->setData(scoreData);
        scoreData.ranking++;
        scoreWidgetVec.push_back(scoreWidget);

        InvestWidget *investWidget = new InvestWidget(ui->invest);
        investWidget->setGeometry(0, index * 30, 400, 30);
        investWidget->setData(investData);
        investData.ranking++;
        investWidgetVec.push_back(investWidget);
    }
    //    this->setStyleSheet("QWidget#SummaryWidgetModule{background:rgba(0, 169, 254, 150);}");
}

void SummaryWidgetModule::clearValue()
{
    for (int index = 0; index < this->scoreWidgetVec.size(); ++index)
    {
        this->scoreWidgetVec[index]->clear();
    }
    for (int index = 0; index < this->investWidgetVec.size(); ++index)
    {
        this->investWidgetVec[index]->clear();
    }
}

void SummaryWidgetModule::setValue(const std::vector<ScoreData> &scoreVec, const std::vector<InvestData> &investVec)
{
    clearValue();

    if (scoreVec.size() > 0)
    {
        ui->label_3->setText(QString::fromStdString(scoreVec[0].name));
    }

    int num = 0;
    for (auto scoreData : scoreVec)
    {
        scoreData.ranking = num + 1;
        if (num < this->scoreWidgetVec.size())
        {
            this->scoreWidgetVec[num]->setData(scoreData);
        }
        else
        {
            break;
        }
        num++;
    }
    num = 0;
    for (auto investData : investVec)
    {
        if(investData.investScore <= 0)
        {
            break;
        }
        investData.ranking = num + 1;
        if (num < this->investWidgetVec.size())
        {
            this->investWidgetVec[num]->setData(investData);
        }
        else
        {
            break;
        }
        num++;
    }
}

SummaryWidgetModule::~SummaryWidgetModule()
{
    delete ui;
}
