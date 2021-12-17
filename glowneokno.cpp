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
#include "nowytest_2.h"
#include "nowytest_3.h"
#include "testwizard.h"

GlowneOkno::GlowneOkno(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlowneOkno)
{
    ui->setupUi(this);

    VideoWidget * videowidget = new VideoWidget(ui->tabFoto);
    ui->verticalLayout_2->addWidget(videowidget);

    ui->tabTest->setHidden(true);
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
    dlg->exec();
    if (dlg->exec() == QDialog::Rejected) {
    //        //ui->treeWidget->
    }
    dlg->exec();
}

void GlowneOkno::on_actionNowy_Test_triggered()
{
    TestWizard wizard(this);
    wizard.exec();
    //ui->tabTest->validateCurrentPage();

    /*
    auto * dlg = new NowyTest_1(this);
    dlg->exec();

    auto * dlg2 = new NowyTest_2(this);
    dlg2->exec();

    auto * dlg3 = new NowyTest_3(this);
    dlg3->exec();
    */

}
