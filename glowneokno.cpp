#include "glowneokno.h"
#include "ui_glowneokno.h"

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
