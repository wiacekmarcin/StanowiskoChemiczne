#include "videowidgetsurface.h"

#include <QtMultimedia>
#include <QWidget>
#include <QList>


 VideoWidgetSurface::VideoWidgetSurface(QWidget *widget, QObject *parent)
     : QAbstractVideoSurface(parent)
     , widget(widget)
     , imageFormat(QImage::Format_Invalid)
 {

 }

 QList<QVideoFrame::PixelFormat> VideoWidgetSurface::supportedPixelFormats(
         QAbstractVideoBuffer::HandleType handleType) const
 {
     if (handleType == QAbstractVideoBuffer::NoHandle) {
         return QList<QVideoFrame::PixelFormat>()
                 << QVideoFrame::Format_RGB32
                 << QVideoFrame::Format_ARGB32
                 << QVideoFrame::Format_ARGB32_Premultiplied
                 << QVideoFrame::Format_RGB565
                 << QVideoFrame::Format_RGB555;
     } else {
         return QList<QVideoFrame::PixelFormat>();
     }
 }

 bool VideoWidgetSurface::isFormatSupported(
         const QVideoSurfaceFormat &format, QVideoSurfaceFormat *similar) const
 {
     Q_UNUSED(similar);

     const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
     const QSize size = format.frameSize();

     return imageFormat != QImage::Format_Invalid
             && !size.isEmpty()
             && format.handleType() == QAbstractVideoBuffer::NoHandle;
 }

 bool VideoWidgetSurface::start(const QVideoSurfaceFormat &format)
 {
     qDebug() << __FILE__ << __LINE__ << format.isValid();
     const QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat());
     const QSize size = format.frameSize();

     if (imageFormat != QImage::Format_Invalid && !size.isEmpty()) {
         qDebug() << __FILE__ << __LINE__;
         this->imageFormat = imageFormat;
         imageSize = size;
         sourceRect = format.viewport();

         QAbstractVideoSurface::start(format);

         widget->updateGeometry();
         updateVideoRect();

         return true;
     } else {
         return false;
     }
 }

 void VideoWidgetSurface::stop()
 {
/*
     currentFrame = QVideoFrame();
     targetRect = QRect();

     QAbstractVideoSurface::stop();

     widget->update();
*/
 }

 bool VideoWidgetSurface::present(const QVideoFrame &frame)
 {
     qDebug() << __FILE__ << __LINE__;
     if (surfaceFormat().pixelFormat() != frame.pixelFormat()
             || surfaceFormat().frameSize() != frame.size()) {
         setError(IncorrectFormatError);
         stop();
         qDebug() << __FILE__ << __LINE__ << "IncorrectFormat";
         return false;
     } else {
         currentFrame = frame;
         qDebug() << __FILE__ << __LINE__;
         widget->repaint(targetRect);
         return true;
     }
 }

 void VideoWidgetSurface::updateVideoRect()
 {
     QSize size = surfaceFormat().sizeHint();
     qDebug() << __FILE__ << __LINE__ << size << widget->size();
     size.scale(widget->size().boundedTo(size), Qt::KeepAspectRatio);

     targetRect = QRect(QPoint(0, 0), size);
     targetRect.moveCenter(widget->rect().center());
 }

 void VideoWidgetSurface::paint(QPainter *painter)
 {
     qDebug() << __FILE__ << __LINE__;
     if (currentFrame.map(QAbstractVideoBuffer::ReadOnly)) {
         const QTransform oldTransform = painter->transform();
         qDebug() << __FILE__ << __LINE__;
         if (surfaceFormat().scanLineDirection() == QVideoSurfaceFormat::BottomToTop) {
            painter->scale(1, -1);
            painter->translate(0, -widget->height());
         }

         QImage image(
                 currentFrame.bits(),
                 currentFrame.width(),
                 currentFrame.height(),
                 currentFrame.bytesPerLine(),
                 imageFormat);

         painter->drawImage(targetRect, image, sourceRect);
         painter->setTransform(oldTransform);

         currentFrame.unmap();
     }
 }
