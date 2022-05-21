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

    //setMinimumHeight(20);
    //setMaximumHeight(20);
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
    //qDebug() << __FILE__ << __LINE__ << "value" << value;
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
    painter.setBrush(Qt::red);
    painter.setPen(QPen(Qt::red));
    painter.drawPolyline(points, lenHistory);
    //painter.drawPoints(points, lenHistory);
    painter.setPen(QPen(Qt::black));
    painter.drawText(5, 15, QString::number(maxVal));
    painter.drawText(5, height()-5, QString::number(minVal));
    painter.drawText(width()-70, 15, QString::number(maxVal));
    painter.drawText(width()-70, height()-5, QString::number(minVal));

    painter.drawText(80,15, title);
    painter.restore();
}

void DigitalOutWidget::setUnit(const QString &un)
{
    unit = un;
}

void DigitalOutWidget::setTitle(const QString &tl)
{
    title = tl;
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

    unsigned int h = height();
    if (h == 0)
        h = 1;


    for (int i=lenHistory-1, j = 0; i && j < vals.size(); --i, j++) {
        float val = vals[i - (lenHistory-vals.size())];
        float delta = maxVal - minVal;
        //if (unit == "mBar")
        //    qDebug() << __FILE__ << __LINE__ << "val= " << val << " delta= " << delta << " height=" << h << "p ="
        //             << (maxVal - minVal - (val - minVal)) / (( maxVal - minVal)/h);
        if (delta == 0)
            delta = 1;

        unsigned int point = (maxVal - minVal - (val - minVal)) / (delta / h) ;
        //if (unit == "mBar")
        //    qDebug() << point;
        points[i].setY( point );
        //if (unit == "mBar")
           //qDebug() << __FILE__ << __LINE__ << points[i];
    }

    update();
}

