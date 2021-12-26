#include "nowytest_5.h"
#include "ui_nowytest_5.h"
#include "createtestwizard.h"

#include <QTimer>

NowyTest_5::NowyTest_5(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_5)
{
    ui->setupUi(this);

    valid = false;
    ui->pbStep3->setEnabled(false);
    ui->lStep3->setEnabled(false);
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
    emit pomiary(true);
}

void NowyTest_5::on_pbStep2_clicked()
{
    ui->pbStep2->setEnabled(false);
    emit pompaMembramowa(true);
    emit mieszadlo(true);
    QTimer::singleShot(1000, this, &NowyTest_5::runDone2);

}

void NowyTest_5::on_pbStep3_clicked()
{
    emit pompaMembramowa(false);
    emit pomiarSingle(1); //voc
    emit pomiarSingle(2); //o2
    emit pomiarSingle(3); //co2
    emit pomiarStezen();
    ui->pbStep3->setDone(true);
    valid = true;
    emit completeChanged();
}

void NowyTest_5::runDone2()
{
    ui->pbStep2->setEnabled(true);
    ui->pbStep2->setDone(true);
    ui->lStep3->setEnabled(true);
    ui->pbStep3->setEnabled(true);
}

