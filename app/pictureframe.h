#ifndef PICTUREFRAME_H
#define PICTUREFRAME_H

#include <QFrame>
#include <QString>

namespace Ui {
class PictureFrame;
}

class PictureFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PictureFrame(QWidget *parent, const QString & srcFileName);
    ~PictureFrame();

private:
    Ui::PictureFrame *ui;
};

#endif // PICTUREFRAME_H
