#include "nowytest_8.h"
#include "ui_nowytest_8.h"
#include "createtestwizard.h"
#include "testpage.h"

NowyTest_8::NowyTest_8(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_8)
{
    ui->setupUi(this);

    valid = false;
}

NowyTest_8::~NowyTest_8()
{
    delete ui;
}

void NowyTest_8::initializePage()
{
    qDebug("initializePage 8");
}




