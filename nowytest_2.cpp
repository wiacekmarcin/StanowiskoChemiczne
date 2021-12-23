#include "nowytest_2.h"
#include "ui_nowytest_2.h"
#include "createtestwizard.h"
#include <QTimer>

NowyTest_2::NowyTest_2(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_2)
{
    ui->setupUi(this);
    ign = false;
}

NowyTest_2::~NowyTest_2()
{
    delete ui;
}

void NowyTest_2::initializePage()
{
    qDebug("initializePage 2");
    ign = true;
    ui->frame->setEnabled(false);
    ui->cbStep3No->setChecked(false);
    ui->cbStep3Yes->setChecked(false);
    ui->cbStep3No->setEnabled(false);
    ui->cbStep3Yes->setEnabled(false);
    ui->dozowniknr->setText(field("Dozownik").toString());
    ui->dozowniknr_2->setText(field("Dozownik").toString());
    ui->cbNo->setEnabled(true);
    ui->cbYes->setEnabled(true);
    ui->cbNo->setChecked(false);
    ui->cbYes->setChecked(false);
    ign = false;
    dozownikFull = false;
    emit completeChanged();
}

bool NowyTest_2::isComplete() const
{
    if (wizard()->currentPage() != this)
        return true;

    if (!TestPage::isComplete())
        return false;

    if (ui->cbYes->isChecked())
        return true;

    if (ui->cbStep3Yes->isChecked())
        return true;

    if (dozownikFull)
        return true;

    return false;
}

void NowyTest_2::on_cbYes_toggled(bool checked)
{
    if (ign)
        return;
    ui->cbNo->setChecked(!checked);
    ui->frame->setEnabled(!checked);
    if (!checked)
        step1();
    emit completeChanged();
}

void NowyTest_2::on_cbNo_toggled(bool checked)
{
    if (ign)
        return;
    ui->cbYes->setChecked(!checked);
    ui->frame->setEnabled(checked);
    if (checked)
        step1();
    emit completeChanged();
}

void NowyTest_2::step1()
{
    ui->lstep1->setEnabled(true);
    ui->pbStep1->setEnabled(true);

    ui->lstep2->setEnabled(false);
    ui->pbStep2->setEnabled(false);
    ui->dozowniknr_2->setEnabled(false);

    ui->lstep3->setEnabled(false);
    ui->pbStep3aOk->setEnabled(false);
    ui->pbStep3aRun->setEnabled(false);

    ui->lstep4->setEnabled(false);
    ui->pbStep4->setEnabled(false);
}

void NowyTest_2::step3()
{
    ui->pbStep3aOk->setEnabled(true);
    ui->pbStep3aRun->setEnabled(true);
    ui->cbStep3No->setEnabled(false);
    ui->cbStep3Yes->setEnabled(false);
}

void NowyTest_2::on_pbStep1_clicked()
{
    ui->cbNo->setEnabled(false);
    ui->cbYes->setEnabled(false);
    ui->lstep2->setEnabled(true);
    ui->pbStep2->setEnabled(true);
    ui->pbStep1->setEnabled(false);
    ui->dozowniknr_2->setEnabled(true);
}

void NowyTest_2::on_pbStep2_clicked()
{
    ui->lstep3->setEnabled(true);
    ui->cbStep3Yes->setEnabled(true);
    ui->cbStep3No->setEnabled(true);
    ui->pbStep2->setEnabled(false);
}

void NowyTest_2::on_cbStep3Yes_toggled(bool checked)
{
    ui->cbStep3No->setChecked(!checked);
    if (!checked)
        step3();
    emit completeChanged();
}

void NowyTest_2::on_cbStep3No_toggled(bool checked)
{
    ui->cbStep3Yes->setChecked(!checked);
    if (checked)
        step3();
    emit completeChanged();
}

void NowyTest_2::on_pbStep3aRun_clicked()
{
    ui->pbStep3aRun->setEnabled(false);
    ui->pbStep3aOk->setEnabled(false);
    QTimer::singleShot(1000,this, &NowyTest_2::runDone);
}

void NowyTest_2::on_pbStep3aOk_clicked()
{
    ui->pbStep3aRun->setEnabled(false);
    ui->pbStep3aOk->setEnabled(false);

    ui->pbStep4->setEnabled(true);
    ui->lstep4->setEnabled(true);
}

void NowyTest_2::runDone()
{
    ui->pbStep3aRun->setEnabled(true);
    ui->pbStep3aOk->setEnabled(true);
}

void NowyTest_2::on_pbStep4_clicked()
{
    ui->pbStep4->setEnabled(false);
    dozownikFull = true;
    emit completeChanged();
}

