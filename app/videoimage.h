#ifndef VIDEOIMAGE_H
#define VIDEOIMAGE_H

#include <QImage>
#include <QSize>
#include <QWidget>

class VideoImage : public QObject
{
    Q_OBJECT
public:
    explicit VideoImage(QWidget *Widget, QObject *parent = nullptr);

    bool present(const QImage &frame);

    void paint(QPainter *painter);

    void updateVideoRect();
private:
    QWidget *widget;
    QImage image;
    QRect targetRect;
    QSize imageSize;

};

#endif // VIDEOIMAGE_H
