#include "videoplayer.h"
#include "videowidget.h"

#include <QtMultimedia>
#include <QPushButton>
#include <QAbstractButton>
#include <QStyle>
#include <QSlider>
#include <QHBoxLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDir>

 VideoPlayer::VideoPlayer(QWidget *parent)
     : QWidget(parent)
     , surface(0)
     , playButton(0)
     , positionSlider(0)
 {
     qDebug() << __FILE__ << __LINE__ << "videoplayer";
     connect(&movie, SIGNAL(stateChanged(QMovie::MovieState)),
             this, SLOT(movieStateChanged(QMovie::MovieState)));
     connect(&movie, SIGNAL(frameChanged(int)),
             this, SLOT(frameChanged(int)));

     VideoWidget *videoWidget = new VideoWidget;
     surface = videoWidget->videoSurface();

     QAbstractButton *openButton = new QPushButton(tr("Open..."));
     connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));

     playButton = new QPushButton;
     playButton->setEnabled(false);
     playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

     connect(playButton, SIGNAL(clicked()),
             this, SLOT(play()));

     positionSlider = new QSlider(Qt::Horizontal);
     positionSlider->setRange(0, 0);

     connect(positionSlider, SIGNAL(sliderMoved(int)),
             this, SLOT(setPosition(int)));

     connect(&movie, SIGNAL(frameChanged(int)),
             positionSlider, SLOT(setValue(int)));

     QBoxLayout *controlLayout = new QHBoxLayout;
     controlLayout->setMargin(0);
     controlLayout->addWidget(openButton);
     controlLayout->addWidget(playButton);
     controlLayout->addWidget(positionSlider);

     QBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(videoWidget);
     layout->addLayout(controlLayout);

     setLayout(layout);
     show();
 }

 VideoPlayer::~VideoPlayer()
 {
 }

 void VideoPlayer::openFile()
 {
     QStringList supportedFormats;
     foreach (QString fmt, QMovie::supportedFormats())
         supportedFormats << fmt;
     foreach (QString fmt, QImageReader::supportedImageFormats())
         supportedFormats << fmt;

     supportedFormats << "mp4" << "MP4";
     QString filter = "Videos (";
     foreach ( QString fmt, supportedFormats) {
         filter.append(QString("*.%1 ").arg(fmt));
         qDebug() << fmt;
     }
     filter.append(")");

     QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),
             QDir::homePath(), filter);

     if (!fileName.isEmpty()) {
         surface->stop();

         movie.setFileName(fileName);
         qDebug() << movie.isValid();

         playButton->setEnabled(true);
         positionSlider->setMaximum(movie.frameCount());

         movie.jumpToFrame(0);
     }
 }

 void VideoPlayer::play()
 {
     switch(movie.state()) {
     case QMovie::NotRunning:
         movie.start();
         break;
     case QMovie::Paused:
         movie.setPaused(false);
         break;
     case QMovie::Running:
         movie.setPaused(true);
         break;
     }
 }

 void VideoPlayer::movieStateChanged(QMovie::MovieState state)
 {
     switch(state) {
     case QMovie::NotRunning:
     case QMovie::Paused:
         playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
         break;
     case QMovie::Running:
         playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
         break;
     }
 }

 void VideoPlayer::frameChanged(int frame)
 {
     if (!presentImage(movie.currentImage())) {
         movie.stop();
         playButton->setEnabled(false);
         positionSlider->setMaximum(0);
     } else {
         positionSlider->setValue(frame);
     }
 }

 void VideoPlayer::setPosition(int frame)
 {
     movie.jumpToFrame(frame);
 }

 bool VideoPlayer::presentImage(const QImage &image)
 {
     QVideoFrame frame(image);

     if (!frame.isValid())
         return false;

     QVideoSurfaceFormat currentFormat = surface->surfaceFormat();

     if (frame.pixelFormat() != currentFormat.pixelFormat()
             || frame.size() != currentFormat.frameSize()) {
         QVideoSurfaceFormat format(frame.size(), frame.pixelFormat());

         if (!surface->start(format))
             return false;
     }

     if (!surface->present(frame)) {
         surface->stop();

         return false;
     } else {
         return true;
     }
 }

