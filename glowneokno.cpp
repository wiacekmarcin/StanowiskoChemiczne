#include "glowneokno.h"
#include "ui_glowneokno.h"

#include "nowyprojectdlg.h"
#include "czujnikanalogowyustawieniaokno.h"

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFile>
#include <QVideoWidget>

#include "videowidget.h"
#include "nowytest_1.h"


GlowneOkno::GlowneOkno(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlowneOkno)
{
    ui->setupUi(this);

    VideoWidget * videowidget = new VideoWidget(ui->tabFoto);
    ui->verticalLayout_2->addWidget(videowidget);
}

GlowneOkno::~GlowneOkno()
{
    delete ui;
}

void GlowneOkno::on_actionUstawienia_sygna_w_triggered()
{
    CzujnikAnalogowyUstawieniaOkno *dlg = new CzujnikAnalogowyUstawieniaOkno(this);
    dlg->exec();
}

void GlowneOkno::on_actionNowy_projekt_triggered()
{
    auto * dlg = new NowyProjectDlg(this);
    qDebug("%d", dlg->exec());
    //if (dlg->exec() == QDialog::Accepted) {
    //        //ui->treeWidget->
    //}
}

void GlowneOkno::on_actionNowy_Test_triggered()
{
    auto * dlg = new NowyTest_1(this);
    if (dlg->exec() == QDialog::Accepted) {
        //ui->treeWidget->
    }
}
