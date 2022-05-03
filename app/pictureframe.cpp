#include "pictureframe.h"
#include "ui_pictureframe.h"
#include <QPixmap>
#include <QFile>
#include <QFileInfo>
#include <QToolButton>

PictureFrame::PictureFrame(QWidget *parent, const QString & srcFileName) :
    QFrame(parent),
    ui(new Ui::PictureFrame),
    fileName(srcFileName)
{
    ui->setupUi(this);
    QFileInfo fsrc(srcFileName);
    QString fname = fsrc.baseName();
    ui->filename->setText(fname);
    ui->image->setPixmap(QPixmap(srcFileName).scaledToWidth(400));

    connect(ui->deleteBtn, &QToolButton::clicked, this, &PictureFrame::on_deleteBtn);
    connect(ui->raport, &QToolButton::clicked, this, &PictureFrame::on_moveBtn);
    connect(ui->upBtn, &QToolButton::clicked, this, &PictureFrame::on_upBtn);
    connect(ui->downBtn, &QToolButton::clicked, this, &PictureFrame::on_downBtn);

    inRaport = false;
    ui->lineEdit->setVisible(inRaport);
    ui->upBtn->setVisible(inRaport);
    ui->downBtn->setVisible(inRaport);
    ui->raport->setText(inRaport ? QString("Usuń z raportu") : QString("Dodaj do raportu"));
    ui->frame->setVisible(inRaport);
    ui->deleteBtn->setVisible(!inRaport);
}

PictureFrame::~PictureFrame()
{
    delete ui;
}

void PictureFrame::setFirst(bool first)
{
    ui->upBtn->setDisabled(first);
}

void PictureFrame::setLast(bool last)
{
    ui->downBtn->setDisabled(last);
}

QPair<QString, QString> PictureFrame::getData2Pdf() const
{
    return QPair<QString,QString>(fileName, ui->lineEdit->text());
}

void PictureFrame::on_deleteBtn()
{
    emit deleteFile(fileName);
}

void PictureFrame::on_moveBtn()
{
    inRaport = !inRaport;
    ui->lineEdit->setVisible(inRaport);
    ui->upBtn->setVisible(inRaport);
    ui->downBtn->setVisible(inRaport);
    ui->raport->setText(inRaport ? QString("Usuń z raportu") : QString("Dodaj do raportu"));
    ui->frame->setVisible(inRaport);
    ui->deleteBtn->setVisible(!inRaport);
    emit moveFile(inRaport, fileName);
}

void PictureFrame::on_upBtn()
{
    emit upFile(fileName);
}

void PictureFrame::on_downBtn()
{
    emit downFile(fileName);
}
