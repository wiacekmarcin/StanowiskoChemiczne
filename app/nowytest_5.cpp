#include "nowytest_5.h"
#include "ui_nowytest_5.h"
#include "createtestwizard.h"

#include <QTimer>
#include <QMessageBox>

NowyTest_5::NowyTest_5(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_5)
{
    ui->setupUi(this);
}

NowyTest_5::~NowyTest_5()
{
    delete ui;
}

void NowyTest_5::initializePage()
{
    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
    ui->arrow_1->setVisible(true);
    ui->arrow_2->setVisible(true);
    ui->pbOK_1->setEnabled(true);
    ui->pbOK_2->setEnabled(true);
    ui->pbOK_3->setEnabled(true);
    //emit pomiary(true);
    setZ_criticalMask(drzwi_lewe | drzwi_prawe | pom_stez_1 | pom_stez_2 | proznia | wlot_powietrza | wentylacja_lewa | wentylacja_prawa);

}

void NowyTest_5::runDone()
{
    ui->pbOK_3->setEnabled(true);
    //updateOutput(pompa_powietrza, false);
    //ui->pbStep2->setEnabled(true);
    //ui->pbStep2->setDone(true);
    //ui->lStep3->setEnabled(true);
    //ui->pbStep3->setEnabled(true);
}

void NowyTest_5::on_pbOK_1_clicked()
{
    sprawdzZawory(ui->pbOK_1, ui->arrow_1, ui->frame_2);
    setZ_criticalMask(drzwi_lewe | drzwi_prawe | wentylacja_prawa | wentylacja_lewa | proznia | wlot_powietrza);
    setZ_warningMask(0);

}


void NowyTest_5::on_pbOK_2_clicked()
{
    if (!sprawdzOtwarteZaworStezenia())
        return;
    setZ_warningMask(pom_stez_1 | pom_stez_2);
    ui->pbOK_2->setEnabled(false);
    ui->arrow_2->setVisible(false);
    ui->frame_3->setVisible(true);
    ui->pbOK_3->setEnabled(false);

    updateOutput(pompa_powietrza, true);
    //emit pomiary(true);
    //emit pompaMembramowa(true);
    //emit mieszadlo(true);
    QTimer::singleShot(5000, this, &NowyTest_5::runDone);
}


void NowyTest_5::on_pbOK_3_clicked()
{
    if (!sprawdzOtwarteZaworStezenia())
        return;
    updateOutput(pompa_powietrza, false);
    //setZ_warningMask(pom_stez_1 | pom_stez_2);
    nextPage(nextPageId());
}

