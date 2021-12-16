#include "nowytest_4.h"
#include "ui_nowytest_4.h"

NowyTest_4::NowyTest_4(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NowyTest_4)
{
    ui->setupUi(this);
}

NowyTest_4::~NowyTest_4()
{
    delete ui;
}
