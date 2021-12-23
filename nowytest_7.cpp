#include "nowytest_7.h"
#include "ui_nowytest_7.h"

NowyTest_7::NowyTest_7(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_7)
{
    ui->setupUi(this);
    ui->gbZaplon->setEnabled(false);
    ui->gbBrakZaplonu->setEnabled(false);
    valid = false;
    emit completeChanged();
}

NowyTest_7::~NowyTest_7()
{
    delete ui;
}

void NowyTest_7::initializePage()
{
    qDebug("initializePage 7");
    TestPage::initializePage();
}

bool NowyTest_7::isComplete() const
{
    return TestPage::isComplete();
}

int NowyTest_7::nextPage() const
{
    return next;
}

void NowyTest_7::on_rbZaplon_clicked()
{
    ui->rbZaplon->setChecked(true);
    ui->gbZaplon->setEnabled(true);
    ui->rbBrakZaplonu->setChecked(false);
    ui->gbBrakZaplonu->setEnabled(false);
}

void NowyTest_7::on_rbBrakZaplonu_clicked()
{
    ui->rbZaplon->setChecked(false);
    ui->gbZaplon->setEnabled(false);
    ui->rbBrakZaplonu->setChecked(true);
    ui->gbBrakZaplonu->setEnabled(true);
}

void NowyTest_7::on_pbStep1OK_clicked()
{
    valid = true;
    emit completeChanged();
}


void NowyTest_7::on_pbBrakZaplonu_clicked()
{

}

void NowyTest_7::on_pbYes_clicked()
{
    //emit changePage(6-1);
    valid = true;
    next = 6-1;
}

void NowyTest_7::on_pbNo_clicked()
{
    //emit changePage(8-1);
    valid = true;
    next = 6-1;
}

void NowyTest_7::on_pbZaplon_clicked()
{

}
