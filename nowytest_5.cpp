#include "nowytest_5.h"
#include "ui_nowytest_5.h"
#include "createtestwizard.h"
NowyTest_5::NowyTest_5(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_5)
{
    ui->setupUi(this);

    valid = false;
}

NowyTest_5::~NowyTest_5()
{
    delete ui;
}

bool NowyTest_5::isComplete() const
{
    if (wizard()->currentPage() != this)
        return true;

    if (!TestPage::isComplete())
        return false;

    return valid;
}

void NowyTest_5::initializePage()
{
    qDebug("initializePage 5");
    valid = false;
    emit completeChanged();
}

void NowyTest_5::on_pbStep2_clicked()
{
    ui->pbStep2->setEnabled(false);
    ui->lStep3->setEnabled(true);
    ui->pbStep3->setEnabled(true);
}

void NowyTest_5::on_pbStep3_clicked()
{
    ui->pbStep3->setEnabled(false);
    valid = true;
    emit completeChanged();
}

