#include "zdjeciaraport.h"
#include "ui_zdjeciaraport.h"
#include "pictureframe.h"

#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QMessageBox>


ZdjeciaRaport::ZdjeciaRaport(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ZdjeciaRaport)
{
    ui->setupUi(this);
}

ZdjeciaRaport::~ZdjeciaRaport()
{
    delete ui;
}

void ZdjeciaRaport::addImages(const QStringList &files, const QString & workDir)
{
    QDir testWorkDir(workDir);
    foreach(auto fName, files) {
        QFileInfo f(fName);
        QString fn = f.fileName();
        QString basename = f.baseName();
        QString fdst = testWorkDir.absoluteFilePath(fn);
        QFile fp(fName);
        if (!fp.copy(fdst)) {
            QMessageBox::warning(this, tr("Dodawanie zdjęc"), QString(tr("Nie udało się dodać zdjęcia : %1")).arg(fName));
            qDebug() << "nie udalo sie skopiowac: " << fName << " -> " << fdst;
            continue;
        }

        qDebug() << "fName" << fName << fdst;
        QImage im(fdst);
        if (im.isNull()) {
            qDebug() << "nie udalo sie otworzyć: " << fdst;
            QMessageBox::warning(this, tr("Dodawanie zdjęc"), QString(tr("Nie prawidłowy format pliku : %1")).arg(fName));
            continue;
        }

        //QSize is = im.size();
        QImage c = im.scaledToWidth(900);
        if (c.size().height() > 600) {
            QImage d = c.scaledToHeight(600);
            c = d;
        }
        qDebug() << "Image size=" << c.size();
        //QImage png = c.con

        QString fileNamePng = testWorkDir.absoluteFilePath(basename) + QString(".png");
        qDebug() <<  "save: " << fileNamePng;
        if(!c.save(fileNamePng, "PNG")) {
            QMessageBox::warning(this, tr("Dodawanie zdjęc"), QString(tr("Nie udało się zapisać pliku : %1")).arg(fileNamePng));
            qDebug() << "canot save" ;
            continue;
        }
        pictures[fileNamePng] = QSharedPointer<PictureFrame>(new PictureFrame(this, fileNamePng));

        ui->verticalLayout_all->addWidget(pictures[fileNamePng].data());
        connect(pictures[fileNamePng].data(), &PictureFrame::deleteFile , this, &ZdjeciaRaport::deleteFile);
        connect(pictures[fileNamePng].data(), &PictureFrame::moveFile , this, &ZdjeciaRaport::moveFile);
        connect(pictures[fileNamePng].data(), &PictureFrame::upFile , this, &ZdjeciaRaport::upFile);
        connect(pictures[fileNamePng].data(), &PictureFrame::downFile , this, &ZdjeciaRaport::downFile);
    }
}

QVector<QPair<QString, QString> > ZdjeciaRaport::getImagesDecription() const
{
    QVector<QPair<QString, QString> > ret (selected.size());
    for (int i = 0; i < selected.size(); i++) {
        ret[i] = selected[i].data()->getData2Pdf();
    }
    return ret;
}

void ZdjeciaRaport::deleteFile(const QString fileName)
{
    if (!pictures.contains(fileName))
        return;
    auto fr = pictures[fileName];
    ui->verticalLayout_all->removeWidget(fr.data());
    //TODO usunac plik z dysku ?
}

void ZdjeciaRaport::moveFile(bool raport, const QString fileName)
{
    if (!pictures.contains(fileName))
        return;

    QSharedPointer<PictureFrame> fr = pictures[fileName];
    if (raport) {
        ui->verticalLayout_all->removeWidget(fr.data());
        ui->selectedGridLayout->addWidget(fr.data(), selected.size(), 0);
        selected.push_back(fr);
    } else {
        bool remove = false;
        for (int i = 0; i < selected.size(); i++) {
            if (selected[i] == fr) {
                remove = true;
                ui->selectedGridLayout->removeWidget(selected[i].data());
            } else if (remove) {
                ui->selectedGridLayout->removeWidget(selected[i].data());
                ui->selectedGridLayout->addWidget(selected[i].data(), i-1, 0);
            }
        }
        ui->verticalLayout_all->addWidget(fr.data());
        selected.removeOne(fr);
    }
    for (int i = 0 ; i < selected.size(); ++i) {
        selected.at(i)->setFirst(i == 0);
        selected.at(i)->setLast(i == selected.size() - 1);
    }
}

void ZdjeciaRaport::upFile(const QString fileName)
{
    if (!pictures.contains(fileName))
        return;

    auto pic = pictures[fileName];
    int i = 0;
    for ( ; i < selected.size(); ++i) {
        if (selected[i] == pic)
            break;
    }

    if (i == selected.size() || i == 0)
        return;

    ui->selectedGridLayout->removeWidget(selected[i-1].data());
    ui->selectedGridLayout->removeWidget(selected[i].data());

    ui->selectedGridLayout->addWidget(selected[i].data(), i-1, 0);
    ui->selectedGridLayout->addWidget(selected[i-1].data(), i, 0);

    auto temp = selected[i-1];
    selected[i-1] = selected[i];
    selected[i] = temp;

    for (int i = 0 ; i < selected.size(); ++i) {
        selected.at(i)->setFirst(i == 0);
        selected.at(i)->setLast(i == selected.size() - 1);
    }
}

void ZdjeciaRaport::downFile(const QString fileName)
{
    if (!pictures.contains(fileName))
        return;

    auto pic = pictures[fileName];
    int i = 0;
    for ( ; i < selected.size(); ++i) {
        if (selected[i] == pic)
            break;
    }

    if (i == selected.size() || i == selected.size() - 1)
        return;

    ui->selectedGridLayout->removeWidget(selected[i+1].data());
    ui->selectedGridLayout->removeWidget(selected[i].data());

    ui->selectedGridLayout->addWidget(selected[i].data(), i+1, 0);
    ui->selectedGridLayout->addWidget(selected[i+1].data(), i, 0);

    auto temp = selected[i+1];
    selected[i+1] = selected[i];
    selected[i] = temp;

    for (int i = 0 ; i < selected.size(); ++i) {
        selected.at(i)->setFirst(i == 0);
        selected.at(i)->setLast(i == selected.size() - 1);
    }
}
