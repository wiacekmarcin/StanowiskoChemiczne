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

    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, this, &DigitalOutWidget::timeout);
    setMinimumHeight(20);
    setMaximumHeight(20);
    setMaximumWidth(lenHistory + 2);
    setMinimumWidth(lenHistory + 2);
    for (int i =0; i< lenHistory; ++i) {
        points[i] = QPoint(i+1, 0);
    }
    timer->start();
}

DigitalOutWidget::~DigitalOutWidget()
{

}

void DigitalOutWidget::setLevel(bool high)
{
    val = high;
}

void DigitalOutWidget::paintEvent(QPaintEvent *paint)
{
    (void)paint;
    QPainter painter(this);
    painter.save();

    painter.setBrush(Qt::white);

    QRect rect = QRect(0, 0, lenHistory, 19);
    painter.fillRect(rect, QBrush(Qt::white));
    painter.drawRect(rect);//5 radius apiece
    painter.setBrush(Qt::black);
    painter.drawPoints(points, lenHistory);

    painter.restore();
}

void DigitalOutWidget::timeout()
{
    vals.push_back(val);
    if (vals.size() > lenHistory)
        vals.pop_front();
    for (int i=lenHistory-1, j = 0; i && j < vals.size(); --i, j++) {
        points[i].setY(vals[i - (lenHistory-vals.size())] ? 5 : 15);
    }
    update();
}
