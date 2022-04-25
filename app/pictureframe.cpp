#include "pictureframe.h"
#include "ui_pictureframe.h"
#include <QPixmap>
#include <QFile>
#include <QFileInfo>

PictureFrame::PictureFrame(QWidget *parent, const QString & srcFileName) :
    QFrame(parent),
    ui(new Ui::PictureFrame)
{
    ui->setupUi(this);
    QFileInfo fsrc(srcFileName);
    QString fname = fsrc.baseName();
    ui->filename->setText(fname);
    ui->image->setPixmap(QPixmap(srcFileName).scaledToWidth(400));

}

PictureFrame::~PictureFrame()
{
    delete ui;
}
