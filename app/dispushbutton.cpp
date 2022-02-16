#include "dispushbutton.h"
#include <QPainter>
#include <QWidget>
DisPushButton::DisPushButton(QWidget * parent): QPushButton(parent),
  dis(false)
{

}

void DisPushButton::paintEvent(QPaintEvent *paint)
{
    if (!dis) {
        QPushButton::paintEvent(paint);
    } else {
        QPainter painter(this);
        painter.save();

        QRect rect = QRect(0,0,size().width()-1,size().height()-1);
        painter.drawRoundedRect(rect, 5,5);//5 radius apiece

        painter.drawText(rect, text(), Qt::AlignHCenter | Qt::AlignVCenter);
        painter.restore();
    }
}

void DisPushButton::setDone(bool en)
{
    dis = en;
    update();
}

bool DisPushButton::isDone() const
{
    return dis;
}
