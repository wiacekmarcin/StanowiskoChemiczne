#include "nowytest_1.h"
#include "ui_nowytest_1.h"
#include <QDebug>

NowyTest_1::NowyTest_1(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_1)
{
    ui->setupUi(this);

}


NowyTest_1::~NowyTest_1()
{
    delete ui;
}

void NowyTest_1::initializePage()
{
    //qDebug("Initializa page 1");
    ui->name->setText(field("nazwa").toString());
    ui->dozownik->setText(QString::number(field("dozownik").toInt()));
    ui->ciecz->setText(field("ciecz").toString());
    ui->ilosc->setText(QString::number(field("objetosc").toDouble()));
    QString zaplon = field("zaplon").toString();
    if (zaplon == "Iskra elektryczna")
        zaplon += QString(" (%1)").arg(field("zaplonExt").toString());
    ui->zaplon->setText(zaplon);
}
