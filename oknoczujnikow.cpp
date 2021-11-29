#include "oknoczujnikow.h"
#include "ui_oknoczujnikow.h"

OknoCzujnikow::OknoCzujnikow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OknoCzujnikow)
{
    ui->setupUi(this);
}

OknoCzujnikow::~OknoCzujnikow()
{
    delete ui;
}
