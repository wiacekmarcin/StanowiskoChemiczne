#include "stanczujnika.h"
#include "ui_stanczujnika.h"

StanCzujnika::StanCzujnika(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StanCzujnika),
    timer(this)
{
    ui->setupUi(this);
    timer.setInterval(1000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start();
    setOk(true);
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
    ui->red->setMargin(-25);
    red = true;
}

void StanCzujnika::timeout()
{
    if (isOk)
        return;
    ui->red->setMargin(red ? 0 : -25);
    red = !red;
    qDebug("timeout");
}
