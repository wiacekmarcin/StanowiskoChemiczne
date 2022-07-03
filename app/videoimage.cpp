#include "videoimage.h"

#include <QDebug>
#include <QPainter>

VideoImage::VideoImage(QWidget *Widget, QObject * parent)
    : QObject(parent), widget(Widget)
{
    image = QImage(widget->size(), QImage::Format_RGB32);
}

bool VideoImage::present(const QImage &frame)
{
    qInfo() << "present";
    image = frame;
    widget->repaint(targetRect);
    return true;
}

void VideoImage::paint(QPainter *painter)
{
    qInfo() << "VideoImage::paint";
    painter->drawImage(targetRect, image, image.rect());
}

void VideoImage::updateVideoRect()
{
    QSize size = image.size();
    size.scale(widget->size().boundedTo(size), Qt::KeepAspectRatio);

    targetRect = QRect(QPoint(0, 0), size);
    targetRect.moveCenter(widget->rect().center());
}
