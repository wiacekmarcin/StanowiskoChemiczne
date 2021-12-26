#include "nowytest_4.h"
#include "ui_nowytest_4.h"

#include "createtestwizard.h"
#include "testpage.h"
#include <QTimer>

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
    ui->lciecz->setText(field("ciecz").toString());
    ui->ldozownik->setText(field("dozownik").toString());
    ui->lobjetosc->setText(field("objetosc").toString());
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

void NowyTest_4::on_pbStep2OK_clicked()
{
    ui->pbStep2OK->setEnabled(false);
    emit dozownik(field("dozownik").toInt(), field("objetosc").toInt());
    QTimer::singleShot(1000, this, &NowyTest_4::runDone2);
}

void NowyTest_4::on_pbStep3OK_clicked()
{
    ui->pbStep3OK->setEnabled(false);
    emit mieszadlo(true);
    QTimer::singleShot(1000, this, &NowyTest_4::runDone3);
}

void NowyTest_4::on_pbStep4OK_clicked()
{
    ui->pbStep4OK->setEnabled(false);
    emit zaworPowietrza(true);
    QTimer::singleShot(1000, this, &NowyTest_4::runDone4);
}

void NowyTest_4::runDone2()
{
    ui->pbStep2OK->setEnabled(true);
    ui->pbStep2OK->setDone(true);
    ui->pbStep3OK->setEnabled(true);
    ui->lStep3->setEnabled(true);
}

void NowyTest_4::runDone3()
{
    ui->pbStep3OK->setEnabled(true);
    ui->pbStep3OK->setDone(true);
    if (field("zaworPompy").toBool()) {
        ui->lStep4->setEnabled(true);
        ui->pbStep4OK->setEnabled(true);
    } else {
        valid = true;
        emit completeChanged();
    }
}

void NowyTest_4::runDone4()
{
    ui->pbStep4OK->setEnabled(true);
    ui->pbStep4OK->setDone(true);
    valid = true;
    emit completeChanged();
}
