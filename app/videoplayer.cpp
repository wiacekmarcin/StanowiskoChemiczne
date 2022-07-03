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
#include <QSpacerItem>

 VideoPlayer::VideoPlayer(QWidget *parent)
     : QWidget(parent)
     , playButton(0)
     , positionSlider(0)
 {

     VideoWidget *videoWidget = new VideoWidget(this);

     /*
     connect(&movie, SIGNAL(stateChanged(QMovie::MovieState)),
             this, SLOT(movieStateChanged(QMovie::MovieState)));
     connect(&movie, SIGNAL(frameChanged(int)),
             this, SLOT(frameChanged(int)));

    */
/*
     QAbstractButton *openButton = new QPushButton(tr("Open..."), this);
     connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));

     playButton = new QPushButton(this);
     playButton->setEnabled(false);
     playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

     connect(playButton, SIGNAL(clicked()),
             this, SLOT(play()));

     positionSlider = new QSlider(Qt::Horizontal);
     positionSlider->setRange(0, 0);

     connect(positionSlider, SIGNAL(sliderMoved(int)),
             this, SLOT(setPosition(int)));


     QBoxLayout *controlLayout = new QHBoxLayout(this);
     controlLayout->setMargin(0);
     controlLayout->addWidget(openButton);
     controlLayout->addWidget(playButton);
     controlLayout->addWidget(positionSlider);
*/
     QBoxLayout *layout = new QVBoxLayout(this);
     layout->addWidget(videoWidget);

 /*
     layout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
     layout->addLayout(controlLayout);
*/
     setLayout(layout);
/*
     videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
*/
     show();
 }

 VideoPlayer::~VideoPlayer()
 {
 }

 void VideoPlayer::openFile()
 {
     /*
     QStringList supportedFormats;
     foreach (QString fmt, QMovie::supportedFormats())
         supportedFormats << fmt;
     foreach (QString fmt, QImageReader::supportedImageFormats())
         supportedFormats << fmt;

     //supportedFormats << "mp4" << "MP4";
     QString filter = "Videos (";
     foreach ( QString fmt, supportedFormats) {
         filter.append(QString("*.%1 ").arg(fmt));
         //qDebug() << fmt;
     }
     filter.append(")");

     QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),
             QDir::homePath(), filter);

     if (!fileName.isEmpty()) {
         surface->stop();

         movie.setFileName(fileName);
         qDebug() << "\nmovie is= " << movie.isValid();
         qDebug() << "\nframeCount=" << movie.frameCount();
         qDebug() << "\ncurrent=" << movie.currentFrameNumber();
         //qDebug() << "\ncurrent=" << movie.
         playButton->setEnabled(true);
         positionSlider->setMaximum(movie.frameCount());

         movie.jumpToFrame(0);
     }
     */
 }

 void VideoPlayer::play()
 {

 }

  void VideoPlayer::setPosition(int frame)
 {

 }


