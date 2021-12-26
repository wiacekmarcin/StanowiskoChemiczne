#include "nowytest_7.h"
#include "ui_nowytest_7.h"

NowyTest_7::NowyTest_7(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_7)
{
    ui->setupUi(this);
    ui->lStep1A->setEnabled(false);
    ui->pbStep1OK->setEnabled(false);
    initial = false;
    next = 8;
}

NowyTest_7::~NowyTest_7()
{
    delete ui;
}

void NowyTest_7::initializePage()
{
    qDebug("initializePage 7");
    TestPage::initializePage();

    ui->lStep1A->setEnabled(true);
    ui->pbStep1OK->setEnabled(true);
    ui->gbBrakZaplonu->setEnabled(false);
    ui->rbBrakZaplonu->setEnabled(true);
    ui->rbZaplon->setEnabled(true);
    ui->rbZaplon->setChecked(true);
    ui->pbNo->setEnabled(true);
    ui->pbYes->setEnabled(true);
    ui->pbBrakZaplonu->setEnabled(true);
    ui->pbZaplon->setEnabled(true);
    ui->lbrakZaplonuBrakaplonu->setEnabled(true);
    ui->lStep1A->setEnabled(true);
    initial = true;
    ui->rbBrakZaplonu->setChecked(false);
    ui->rbZaplon->setChecked(false);
    initial = false;
    ui->pbBrakZaplonu->setDone(false);
    ui->pbNo->setDone(false);
    ui->pbStep1OK->setDone(false);
    ui->pbYes->setDone(false);
    ui->pbZaplon->setDone(false);

    valid = false;
    emit completeChanged();
}

bool NowyTest_7::isComplete() const
{
    return TestPage::isComplete() && valid;
}

int NowyTest_7::nextPage() const
{
    return next;
}

void NowyTest_7::on_rbZaplon_clicked()
{
    if (initial)
        return;
    ui->rbZaplon->setChecked(true);
    ui->lStep1A->setEnabled(true);
    ui->pbStep1OK->setEnabled(true);
    ui->rbBrakZaplonu->setChecked(false);
    ui->gbBrakZaplonu->setEnabled(false);
}

void NowyTest_7::on_rbBrakZaplonu_clicked()
{
    if (initial)
        return;

    ui->rbZaplon->setChecked(false);
    ui->lStep1A->setEnabled(false);
    ui->pbStep1OK->setEnabled(false);
    ui->rbBrakZaplonu->setChecked(true);
    ui->gbBrakZaplonu->setEnabled(true);
    ui->lbrakZaplonuBrakaplonu->setEnabled(false);
    ui->pbNo->setEnabled(false);
    ui->pbYes->setEnabled(false);
}

void NowyTest_7::on_pbStep1OK_clicked()
{
    ui->rbBrakZaplonu->setEnabled(false);
    emit wentylator(true);
    ui->pbStep1OK->setDone(true);
    valid = true;
    emit completeChanged();
    next = 8;
}


void NowyTest_7::on_pbBrakZaplonu_clicked()
{
    ui->pbBrakZaplonu->setDone(true);
    ui->rbZaplon->setChecked(false);
    ui->lStep1A->setEnabled(false);
    ui->pbStep1OK->setEnabled(false);
    ui->rbBrakZaplonu->setChecked(true);
    ui->rbZaplon->setEnabled(false);
    ui->pbZaplon->setEnabled(false);
    ui->pbNo->setEnabled(true);
    ui->pbYes->setEnabled(true);
    ui->lbrakZaplonuBrakaplonu->setEnabled(true);
}

void NowyTest_7::on_pbYes_clicked()
{
    //emit changePage(6-1);
    ui->pbYes->setDone(true);
    ui->pbNo->setEnabled(false);
    valid = true;
    next = 6;
    emit completeChanged();
}

void NowyTest_7::on_pbNo_clicked()
{
    //emit changePage(8-1);
    ui->pbNo->setDone(true);
    ui->pbYes->setEnabled(false);
    valid = true;
    next = 8;
    emit completeChanged();
}

void NowyTest_7::on_pbZaplon_clicked()
{
    valid = true;
    next = 7;
    ui->rbBrakZaplonu->setEnabled(false);
    ui->rbZaplon->setEnabled(false);
    ui->lStep1A->setEnabled(false);
    ui->pbStep1OK->setEnabled(false);
    ui->pbZaplon->setDone(true);
    ui->pbBrakZaplonu->setEnabled(false);
    emit completeChanged();
}
