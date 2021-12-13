#include "videowidget.h"
#include "ui_videowidget.h"

#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QVideoWidget>

VideoWidget::VideoWidget(QWidget *parent) : QWidget(parent),
    ui(new Ui::VideoWidget)
{
    ui->setupUi(this);
/*
    player = new QMediaPlayer(this);
    QVideoWidget *vw = new QVideoWidget;
    ui->verticalLayout->addWidget(vw);
    player->setVideoOutput(vw);
    player->setMedia(QUrl::fromLocalFile("/home/marcin/Wideo/MVI_1629.MP4"));
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
    qDebug("%lld / %lld" , pos, duration);
    if (duration > 0)
        ui->progressBar->setValue(1000*pos/duration);
}

void VideoWidget::updateDuration(const qint64 &pos)
{
    duration = pos;
}
