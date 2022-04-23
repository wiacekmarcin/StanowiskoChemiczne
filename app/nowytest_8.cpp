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

    setZ_criticalMask(i_drzwi_lewe | i_drzwi_prawe | i_proznia | i_wlot_powietrza | i_wentylacja_prawa | i_wentylacja_lewa );
    setZ_warningMask(0);
    //emit pomiary(true);
}

void NowyTest_8::runDone()
{
    ui->pbOK_2->setEnabled(true);

    //ui->pbStep2->setEnabled(true);
    //ui->pbStep2->setDone(true);
    //ui->lStep3->setEnabled(true);
    //ui->pbStep3->setEnabled(true);
}

void NowyTest_8::on_pbOK_1_clicked()
{
    if (!sprawdzOtwarteZaworStezenia())
        return;
    setZ_warningMask(i_pom_stez_1 | i_pom_stez_2 );
    ui->arrow_1->setVisible(false);
    ui->frame_2->setVisible(true);
    ui->pbOK_1->setEnabled(false);
    updateOutput(o_pompa_powietrza, true);

    ui->pbOK_2->setEnabled(false);
    QTimer::singleShot(2000, this, &NowyTest_8::runDone);
}


void NowyTest_8::on_pbOK_2_clicked()
{
    //pomiar
    TestData & dt = testData();
    dt.setTemperaturaKomory(TestData::FT_koniec, getCzujnik(a_temp_komory));
    dt.setCisnienieKomory(TestData::FT_koniec, getCzujnik(a_cisn_komora));
    updateOutput(o_pompa_powietrza, false);
    setZ_warningMask(0);
    ui->pbOK_2->setEnabled(false);
    ui->arrow_2->setVisible(false);
    ui->frame_3->setVisible(true);
}


void NowyTest_8::on_pbOK_3_clicked()
{
    if (!sprawdzOtwarteZawory(i_pom_stez_1 | i_pom_stez_2 | i_drzwi_lewe | i_drzwi_prawe | i_pom_stez_1 | i_pom_stez_2 | i_proznia | i_wlot_powietrza))
       return;
    TestData & dt = testData();
    dt.setStezenia(TestData::FT_koniec, getCzujnik(a_vol1), getCzujnik(a_vol2), getCzujnik(a_o2),
                   getCzujnik(a_co2), getCzujnik(a_8));
    ui->pbOK_3->setEnabled(false);
    ui->arrow_3->setVisible(false);
    ui->frame_4->setVisible(true);
    setZ_criticalMask(i_drzwi_lewe | i_drzwi_prawe | i_pom_stez_1 | i_pom_stez_2 | i_proznia | i_wlot_powietrza);
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


