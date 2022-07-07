#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H


#include "videowidgetsurface.h"

#include <QStringList>
#include <QWidget>
#include <QTimer>

class VideoImage;
class QLabel;

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    VideoWidget(QWidget *parent = 0);
    ~VideoWidget();

    VideoImage *videoSurface() const { return surface; }

    //QSize sizeHint() const;

    const QStringList &getFiles() const;
    void setFiles(const QStringList &newFiles);
    void up(const unsigned long & diff);
    void down(const unsigned long & diff);
    void play(bool play);
    void first();
    void last();

    QString getCurrentImage() const { return files.at(actPos); }
    QString getActFrame() const { return QString("%1 / %2").arg(actPos).arg(files.size()); };

    void setFrameNr(QLabel *newFrameNr);

private slots:
    void timeout();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    VideoImage *surface;
    QStringList files;
    QTimer t;
    bool needPaint;
    unsigned long actPos;
    QLabel * frameNr;
};


#endif // VIDEOWIDGET_H
