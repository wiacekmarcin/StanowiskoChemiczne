#include "oknostatusowe.h"
#include "ui_oknostatusowe.h"

OknoStatusowe::OknoStatusowe(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OknoStatusowe)
{
    ui->setupUi(this);
}

OknoStatusowe::~OknoStatusowe()
{
    delete ui;
}
