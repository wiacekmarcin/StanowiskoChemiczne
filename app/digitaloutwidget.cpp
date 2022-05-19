#include "digitaloutwidget.h"
//#include "ui_digitaloutwidget.h"

#include <QTimer>

#include <QPainter>
#include <QBrush>
#include <QDebug>

DigitalOutWidget::DigitalOutWidget(QWidget *parent) :
    QWidget(parent)
{
    val = false;

    setMinimumHeight(20);
    setMaximumHeight(20);
    setMaximumWidth(lenHistory + 2);
    setMinimumWidth(lenHistory + 2);
    for (int i =0; i< lenHistory; ++i) {
        points[i] = QPoint(i+1, 0);
    }
}

DigitalOutWidget::~DigitalOutWidget()
{

}

void DigitalOutWidget::addValue(float value)
{
    vals.push_back(value);
    if (vals.size() >= lenHistory)
        vals.pop_front();
}

void DigitalOutWidget::paintEvent(QPaintEvent *paint)
{
    (void)paint;
    QPainter painter(this);
    painter.save();

    painter.setBrush(Qt::white);

    QRect rect = QRect(0, 0, lenHistory, height());
    painter.fillRect(rect, QBrush(Qt::white));
    painter.drawRect(rect);//5 radius apiece
    painter.setBrush(Qt::black);
    painter.drawPoints(points, lenHistory);

    painter.restore();
}

void DigitalOutWidget::setUnit(const QString &un)
{
    unit = un;
}

void DigitalOutWidget::updateVals()
{
    minVal = vals.at(0);
    maxVal = vals.at(0);
    for (int i = 0; i < vals.size(); i++) {
        float val = vals.at(i);
        if (val < minVal)
            minVal = val;
        if (val > maxVal)
            maxVal = val;
    }
    for (int i=lenHistory-1, j = 0; i && j < vals.size(); --i, j++) {
        float val = vals[i - (lenHistory-vals.size())];
        float delta = maxVal - minVal;
        if (delta == 0)
            delta = 1;
        delta = height() / delta;
        unsigned int point = (maxVal - minVal - (val - minVal)) / delta ;
        points[i].setY( point );
    }
    update();
}

