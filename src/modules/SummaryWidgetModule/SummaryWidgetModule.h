#ifndef SUMMARYWIDGETMODULE_H
#define SUMMARYWIDGETMODULE_H

#include <QWidget>
#include "ScoreWidget.h"
#include "InvestWidget.h"
#include <vector>
#include "ScoreData.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class SummaryWidgetModule; }
QT_END_NAMESPACE

class SummaryWidgetModule : public QWidget
{
    Q_OBJECT

private:
    std::vector<ScoreWidget*> scoreWidgetVec;
    std::vector<InvestWidget*> investWidgetVec;

public:
    SummaryWidgetModule(QWidget *parent = nullptr);
    ~SummaryWidgetModule();

    void clearValue();
    void setValue(const std::vector<ScoreData> & scoreVec, const std::vector<InvestData> &investVec);

private:
    Ui::SummaryWidgetModule *ui;
};
#endif // SUMMARYWIDGETMODULE_H
