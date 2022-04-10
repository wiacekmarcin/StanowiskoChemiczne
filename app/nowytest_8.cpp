#include "nowytest_8.h"
#include "ui_nowytest_8.h"
#include "createtestwizard.h"

#include <QTimer>
#include <QMessageBox>

NowyTest_8::NowyTest_8(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_8)
{
    ui->setupUi(this);
}

NowyTest_8::~NowyTest_8()
{
    delete ui;
}

void NowyTest_8::initializePage()
{
    ui->frame_1->setVisible(true);
    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
    ui->frame_4->setVisible(false);

    ui->arrow_1->setVisible(true);
    ui->arrow_2->setVisible(true);
    ui->arrow_3->setVisible(true);
    ui->arrow_4->setVisible(true);

    ui->pbOK_1->setEnabled(true);
    ui->pbOK_2->setEnabled(true);
    ui->pbOK_3->setEnabled(true);
    ui->pbOK_4->setEnabled(true);

    setZ_criticalMask(drzwi_lewe | drzwi_prawe | proznia | wlot_powietrza | wentylacja_prawa | wentylacja_lewa );
    setZ_warningMask(0);
    //emit pomiary(true);
}

void NowyTest_8::runDone()
{
    ui->pbOK_2->setEnabled(true);
    updateOutput(pompa_powietrza, false);
    //ui->pbStep2->setEnabled(true);
    //ui->pbStep2->setDone(true);
    //ui->lStep3->setEnabled(true);
    //ui->pbStep3->setEnabled(true);
}

void NowyTest_8::on_pbOK_1_clicked()
{
    if (!sprawdzOtwarteZaworStezenia())
        return;
    setZ_warningMask(pom_stez_1 | pom_stez_2 );
    ui->arrow_1->setVisible(false);
    ui->frame_2->setVisible(true);
    ui->pbOK_1->setEnabled(false);
    updateOutput(pompa_powietrza, true);

    ui->pbOK_2->setEnabled(false);
    QTimer::singleShot(5000, this, &NowyTest_8::runDone);
}


void NowyTest_8::on_pbOK_2_clicked()
{
    //pomiar

    setZ_warningMask(0);
    ui->pbOK_2->setEnabled(false);
    ui->arrow_2->setVisible(false);
    ui->frame_3->setVisible(true);
}


void NowyTest_8::on_pbOK_3_clicked()
{
    if (!sprawdzOtwarteZawory(pom_stez_1 | pom_stez_2 | drzwi_lewe | drzwi_prawe | pom_stez_1 | pom_stez_2 | proznia | wlot_powietrza))
       return;
    ui->pbOK_3->setEnabled(false);
    ui->arrow_3->setVisible(false);
    ui->frame_4->setVisible(true);
    setZ_criticalMask(drzwi_lewe | drzwi_prawe | pom_stez_1 | pom_stez_2 | proznia | wlot_powietrza);
    setZ_warningMask(0);
}


void NowyTest_8::on_pbOK_4_clicked()
{
    if (!sprawdzOtwarteZawor2Calowe())
       return;

    setZ_criticalMask(0);
    setZ_warningMask(0);

   nextPage(nextPageId());

}


