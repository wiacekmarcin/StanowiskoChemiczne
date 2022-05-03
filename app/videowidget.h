#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H


#include "videowidgetsurface.h"

#include <QWidget>

class QAbstractVideoSurface;

class VideoWidgetSurface;

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    VideoWidget(QWidget *parent = 0);
    ~VideoWidget();

    QAbstractVideoSurface *videoSurface() const { return surface; }

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    VideoWidgetSurface *surface;
};

#if 0

#include <QWidget>
namespace Ui {
class VideoWidget;
}

class QMediaPlayer;
class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);

signals:

private slots:
    void on_rewind_clicked();
    void on_rewind5_clicked();
    void on_rewind1_clicked();
    void on_play25_clicked();
    void on_play50_clicked();
    void on_play150_clicked();
    void on_play200_clicked();
    void on_play_clicked();
    void on_pause_clicked();
    void on_stop_clicked();

    void updateTime(const qint64 & pos);
    void updateDuration(const qint64 & pos);

private:
    Ui::VideoWidget *ui;
    QMediaPlayer *player;
    qint64 duration;
};
#endif //if 0
#endif // VIDEOWIDGET_H
