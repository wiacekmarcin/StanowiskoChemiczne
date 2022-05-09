#include "videowidget.h"

#include "videowidgetsurface.h"

#include <QtMultimedia>

 VideoWidget::VideoWidget(QWidget *parent)
     : QWidget(parent)
     , surface(0)
 {
     setAutoFillBackground(false);
     setAttribute(Qt::WA_NoSystemBackground, true);
     setAttribute(Qt::WA_PaintOnScreen, true);

     QPalette palette = this->palette();
     palette.setColor(QPalette::Background, Qt::black);
     setPalette(palette);

     setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

     surface = new VideoWidgetSurface(this);
 }

 VideoWidget::~VideoWidget()
 {
     delete surface;
 }

 QSize VideoWidget::sizeHint() const
 {
     return surface->surfaceFormat().sizeHint();
 }

 void VideoWidget::paintEvent(QPaintEvent *event)
 {
     QPainter painter(this);

     if (surface->isActive()) {
         const QRect videoRect = surface->videoRect();

         if (!videoRect.contains(event->rect())) {
             QRegion region = event->region();
             region.subtracted(videoRect);

             QBrush brush = palette().background();

             foreach (const QRect &rect, region.rects())
                 painter.fillRect(rect, brush);
         }

         surface->paint(&painter);
     } else {
         painter.fillRect(event->rect(), palette().background());
     }
 }

 void VideoWidget::resizeEvent(QResizeEvent *event)
 {
     QWidget::resizeEvent(event);

     surface->updateVideoRect();
 }



#if 0
#include "ui_videowidget.h"

#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QVideoWidget>

VideoWidget::VideoWidget(QWidget *parent) : QWidget(parent),
    ui(new Ui::VideoWidget)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    QVideoWidget *vw = new QVideoWidget;
    ui->verticalLayout->addWidget(vw);
    player->setVideoOutput(vw);
    /*
    player->setMedia(QUrl::fromLocalFile("/home/marcin/Dokumenty/apka-2021-12-18_14.27.34.mp4"));
    player->setVolume(50);
    duration = player->duration();
    ui->progressBar->setMaximum(1000);
    connect(player, &QMediaPlayer::positionChanged, this, &VideoWidget::updateTime);
    connect(player, &QMediaPlayer::durationChanged, this, &VideoWidget::updateDuration);
    player->play();
*/
    setVisible(false);
}

void VideoWidget::on_rewind_clicked()
{
    player->pause();
    player->setPosition(0);
    player->play();
}


void VideoWidget::on_rewind5_clicked()
{
    player->pause();
    player->setPosition(player->position() - 5000);
    player->play();
}

void VideoWidget::on_rewind1_clicked()
{
    player->pause();
    player->setPosition(player->position() - 1000);
    player->play();
}

void VideoWidget::on_play25_clicked()
{
    player->pause();
    player->setPlaybackRate(0.25);
    player->play();
}

void VideoWidget::on_play50_clicked()
{
    player->pause();
    player->setPlaybackRate(0.5);
    player->play();
}

void VideoWidget::on_play150_clicked()
{
    player->pause();
    player->setPlaybackRate(1.5);
    player->play();
}

void VideoWidget::on_play200_clicked()
{
    player->pause();
    player->setPlaybackRate(2.0);
    player->play();
}

void VideoWidget::on_play_clicked()
{
    player->pause();
    player->setPlaybackRate(1.0);
    player->play();
}

void VideoWidget::on_pause_clicked()
{
    player->pause();
}

void VideoWidget::on_stop_clicked()
{
    player->pause();
    player->stop();
}
#include <QDebug>
void VideoWidget::updateTime(const qint64 & pos)
{

    if (duration > 0)
        ui->progressBar->setValue(1000*pos/duration);
}

void VideoWidget::updateDuration(const qint64 &pos)
{
    duration = pos;
}
#endif
