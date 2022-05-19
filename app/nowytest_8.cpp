#include "nowytest_8.h"
#include "ui_nowytest_8.h"
#include "createtestwizard.h"

#include <QTimer>
#include <QMessageBox>

NowyTest_8::NowyTest_8(unsigned int czasPracyPompki_, unsigned int czasNaStezenie_, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_8),
    czasPracyPompki(1000*czasPracyPompki_),
    czasNaStezenie(1000*czasNaStezenie_)
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
    updateOutput(o_pompa_powietrza, false);
    QTimer::singleShot(czasNaStezenie, this, &NowyTest_8::runDone2);
}

void NowyTest_8::runDone2()
{
    ui->pbOK_2->setEnabled(true);
}

void NowyTest_8::on_pbOK_1_clicked()
{
    wizard()->setDebug(QString("PAGE8::OK1"));
    if (!sprawdzOtwarteZaworStezenia())
        return;
    setZ_warningMask(i_pom_stez_1 | i_pom_stez_2 );
    ui->arrow_1->setVisible(false);
    ui->frame_2->setVisible(true);
    ui->pbOK_1->setEnabled(false);
    updateOutput(o_pompa_powietrza, true);

    ui->pbOK_2->setEnabled(false);
    QTimer::singleShot(czasPracyPompki, this, &NowyTest_8::runDone);
}


void NowyTest_8::on_pbOK_2_clicked()
{
    wizard()->setDebug(QString("PAGE8::OK2"));
    //pomiar
    TestData * dt = getTestData();
    dt->setTemperaturaKomoryWarunkiKoncowe(getCzujnik(a_temp_komory));
    dt->setCisnienieKomoryWarunkiKoncowe(getCzujnik(a_cisn_komora));
    updateOutput(o_pompa_powietrza, false);
    setZ_warningMask(0);
    dt->setStezeniaPoZaplonie(getCzujnik(a_voc1), getCzujnik(a_voc2), getCzujnik(a_o2),
                   getCzujnik(a_co2), getCzujnik(a_8));
    ui->pbOK_2->setEnabled(false);
    ui->arrow_2->setVisible(false);
    ui->frame_4->setVisible(true);
    setZ_criticalMask(0);
    setZ_warningMask(0);
}


void NowyTest_8::on_pbOK_3_clicked()
{
    wizard()->setDebug(QString("PAGE8::OK3"));
    ui->pbOK_3->setEnabled(false);
    ui->arrow_3->setVisible(false);
    ui->frame_4->setVisible(true);
    setZ_criticalMask(0);
    setZ_warningMask(0);
}


void NowyTest_8::on_pbOK_4_clicked()
{
    wizard()->setDebug(QString("PAGE8::OK4"));
    if (!sprawdzOtwarteZawor2Calowe())
       return;

   nextPage(nextPageId());

}


