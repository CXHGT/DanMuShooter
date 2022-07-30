#ifndef INVESTWIDGET_H
#define INVESTWIDGET_H

#include <QObject>
#include <QWidget>
#include "ScoreData.hpp"
#include <QLabel>

class InvestWidget : public QWidget
{
private:
    QLabel *rankingLabel;
    QLabel *nameLabel;
    QLabel *InvestLabel;

    Q_OBJECT
public:
    explicit InvestWidget(QWidget *parent = nullptr);
    void setData(const InvestData &data);
    void clear();

signals:
};

#endif // INVESTWIDGET_H
