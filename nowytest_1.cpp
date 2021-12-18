#include "nowytest_1.h"
#include "ui_nowytest_1.h"
#include <QDebug>

NowyTest_1::NowyTest_1(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NowyTest_1)
{
    ui->setupUi(this);

    registerField("testName*", ui->nameTest);
    validatePage();
}


NowyTest_1::~NowyTest_1()
{
    delete ui;
}

bool NowyTest_1::isComplete() const
{
    return QWizardPage::isComplete() && !ui->nameTest->text().isEmpty();
}

