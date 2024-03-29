#ifndef VIDEOWIDGETSURFACE_H
#define VIDEOWIDGETSURFACE_H

#include <QtMultimedia/QAbstractVideoSurface>
#include <QtMultimedia/QVideoFrame>
#include <QList>
#include <QRect>
#include <QImage>
#include <QSize>
#include <QWidget>

 class VideoWidgetSurface : public QAbstractVideoSurface
 {
     Q_OBJECT
 public:
     VideoWidgetSurface(QWidget *widget, QObject *parent = 0);

     QList<QVideoFrame::PixelFormat> supportedPixelFormats(
             QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;

     bool start(const QVideoSurfaceFormat &format);
     void stop();

     bool present(const QVideoFrame &frame);

     QRect videoRect() const { return targetRect; }
     void updateVideoRect();
     bool isFormatSupported( const QVideoSurfaceFormat &format, QVideoSurfaceFormat *similar) const;
     void paint(QPainter *painter);

 private:
     QWidget *widget;
     QImage::Format imageFormat;
     QRect targetRect;
     QSize imageSize;
     QRect sourceRect;
     QVideoFrame currentFrame;
 };
 #endif
