#include "glowneokno.h"
#include "ui_glowneokno.h"

GlowneOkno::GlowneOkno(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GlowneOkno)
{
    ui->setupUi(this);
    ui->widget->setText("Czujnik 1");
    ui->widget_2->setText("Czujnik 2");
    ui->widget_2->setOk(false);
}

GlowneOkno::~GlowneOkno()
{
    delete ui;
}
