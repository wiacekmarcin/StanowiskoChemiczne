
#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QtGui/QMovie>
#include <QWidget>

class QAbstractButton;
class QAbstractVideoSurface;
class QSlider;

class VideoPlayer : public QWidget
{
    Q_OBJECT
public:
    VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();

public slots:
    void openFile();
    void play();

private slots:
    // void frameChanged(int frame);
    void setPosition(int frame);

private:
    bool presentImage(const QImage &image);

    QAbstractButton *playButton;
    QSlider *positionSlider;
};

#endif
