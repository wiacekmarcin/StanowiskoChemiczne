#include "stanczujnika.h"
#include "ui_stanczujnika.h"

StanCzujnika::StanCzujnika(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StanCzujnika)
{
    ui->setupUi(this);
    setOk(true);
    red = true;
}

StanCzujnika::~StanCzujnika()
{
    delete ui;
}

void StanCzujnika::setText(const QString & text)
{
    ui->status->setText(text);
}

void StanCzujnika::setOk(bool ok)
{
    isOk = ok;
    ui->green->setVisible(ok);
    ui->red->setVisible(!ok);
    ui->red->setMargin(red ? 0 : -25);

}

void StanCzujnika::update()
{
    red = !red;
    if (isOk)
        return;
    ui->red->setMargin(red ? 0 : -25);
}
