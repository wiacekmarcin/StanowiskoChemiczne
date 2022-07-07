#include "videowidget.h"
#include "videowidgetsurface.h"

#include <QtMultimedia>
#include <QRegion>
#include <QPalette>
#include <QWidget>
#include <QPainter>
#include <QGridLayout>
#include <QLabel>

#include <QtMultimedia/QAbstractVideoSurface>
#include <QtMultimedia/QVideoFrame>


#include "videoimage.h"

 VideoWidget::VideoWidget(QWidget *parent)
     : QWidget(parent)
     , surface(0)
     , t(this)
     , needPaint(false)
     , actPos(0)
     , frameNr(nullptr)
 {

     setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

     surface = new VideoImage(this);

     t.setInterval(50);
     connect(&t, &QTimer::timeout, this, &VideoWidget::timeout);

 }

 VideoWidget::~VideoWidget()
 {
     t.stop();
     delete surface;
 }

 void VideoWidget::timeout()
 {
     if (files.size() == 0)
         return;

     if (++actPos >= (unsigned int) files.size())
         actPos = 0;

     surface->present(QImage(files.at(actPos), "tiff"));
     repaint();

 }

 /*
 QSize VideoWidget::sizeHint() const
 {
     //return surface->surfaceFormat().sizeHint();
     return surf
 }
*/
 void VideoWidget::paintEvent(QPaintEvent *event)
 {
     qInfo() << "paintEvent" << actPos << needPaint;
     if (needPaint) {
        QPainter painter(this);
        surface->paint(&painter);
        //needPaint = false;
        if (frameNr)
            frameNr->setText(getActFrame());
     }

 }

 void VideoWidget::resizeEvent(QResizeEvent *event)
 {
     QWidget::resizeEvent(event);
     surface->updateVideoRect();
 }

 void VideoWidget::setFrameNr(QLabel *newFrameNr)
 {
     frameNr = newFrameNr;
 }

 const QStringList &VideoWidget::getFiles() const
 {
     return files;
 }

 void VideoWidget::setFiles(const QStringList &newFiles)
 {
     files = newFiles;
     if (files.size() == 0)
         return;
     surface->present(QImage(files.at(actPos), "tiff"));
     needPaint = true;
     actPos = 0;
     surface->updateVideoRect();
 }

 void VideoWidget::up(const unsigned long &diff)
 {
     actPos+= diff;
     if (actPos > (unsigned long)files.size())
         actPos = files.size()-1;
     surface->present(QImage(files.at(actPos), "tiff"));
     repaint();
 }

 void VideoWidget::down(const unsigned long &diff)
 {
     qInfo() << diff << actPos;
     actPos = (diff > actPos) ? 0 : actPos - diff;
     qInfo() << actPos;
     surface->present(QImage(files.at(actPos), "tiff"));
     repaint();
 }

 void VideoWidget::play(bool play)
 {
     if (play) t.start();
     else t.stop();

 }

 void VideoWidget::first()
 {
     actPos = 0;
     surface->present(QImage(files.at(actPos), "tiff"));
     repaint();
 }

 void VideoWidget::last()
 {
     actPos = files.size() - 1;
     surface->present(QImage(files.at(actPos), "tiff"));
     repaint();
 }



