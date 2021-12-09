#include "glowneokno.h"
#include "ui_glowneokno.h"

#include "nowyprojectdlg.h"
#include "ustawieniasygnalowokno.h"

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFile>
#include <QVideoWidget>

#include "videowidget.h"

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
    UstawieniaSygnalowOkno *dlg = new UstawieniaSygnalowOkno(this);
    dlg->exec();
}

void GlowneOkno::on_actionNowy_projekt_triggered()
{
    auto * dlg = new NowyProjectDlg(this);
    if (dlg->exec() == QDialog::Accepted) {
        //ui->treeWidget->
    }
}
