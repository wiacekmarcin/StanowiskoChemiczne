#include "glowneokno.h"
#include "ui_glowneokno.h"

#include "ustawieniasygnalowokno.h"

GlowneOkno::GlowneOkno(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlowneOkno)
{
    ui->setupUi(this);

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
