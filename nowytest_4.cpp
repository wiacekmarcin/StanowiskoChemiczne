#include "nowytest_4.h"
#include "ui_nowytest_4.h"

#include "createtestwizard.h"
#include "testpage.h"

NowyTest_4::NowyTest_4(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_4)
{
    ui->setupUi(this);

    ui->lStep3->setEnabled(false);
    ui->lStep4->setEnabled(false);
    ui->pbStep3OK->setEnabled(false);
    ui->pbStep4OK->setEnabled(false);

    valid = false;


}

void NowyTest_4::initializePage()
{
    qDebug("initializePage 4");
    ui->lciecz->setText(field("Ciecz").toString());
    ui->ldozownik->setText(field("Dozownik").toString());
    ui->lobjetosc->setText(field("Ilosc").toString());
    valid = false;
    emit completeChanged();
}

NowyTest_4::~NowyTest_4()
{
    delete ui;
}

bool NowyTest_4::isComplete() const
{
    if (wizard()->currentPage() != this)
        return true;

    if (!TestPage::isComplete())
        return false;

    return valid;
}

void NowyTest_4::on_pbStep1_clicked()
{

    ui->pbStep3OK->setEnabled(false);
    ui->lStep3->setEnabled(false);
    ui->lStep4->setEnabled(false);
    ui->pbStep4OK->setEnabled(false);

}

void NowyTest_4::on_pbStep2OK_clicked()
{
    ui->pbStep2OK->setEnabled(false);
    ui->pbStep3OK->setEnabled(true);
    ui->lStep3->setEnabled(true);
}

void NowyTest_4::on_pbStep3OK_clicked()
{
    ui->pbStep3OK->setEnabled(false);
    if (field("Podcisnienie").toBool()) {
        ui->lStep4->setEnabled(true);
        ui->pbStep4OK->setEnabled(true);
    } else {
        valid = true;
        emit completeChanged();
    }
}

void NowyTest_4::on_pbStep4OK_clicked()
{
    ui->pbStep4OK->setEnabled(false);
    valid = true;
    emit completeChanged();
}
