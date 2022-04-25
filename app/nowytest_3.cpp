#include "nowytest_3.h"
#include "ui_nowytest_3.h"
#include <QMutexLocker>

#include <QCheckBox>
//#include "windows.h"
#include <QTimer>

#include "createtestwizard.h"
#include "testpage.h"
#include "ustawienia.h"
#include "urzadzenia.h"

#include <QMessageBox>


NowyTest_3::NowyTest_3(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_3)
{
    ui->setupUi(this);
    task = 0;

    cisnienieTimer.setInterval(100);
    connect(&cisnienieTimer, &QTimer::timeout, this, &NowyTest_3::updateCisnieie);

}

NowyTest_3::~NowyTest_3()
{
    cisnienieTimer.stop();
    delete ui;

}

void NowyTest_3::initializePage()
{
     setField(czyPompaMebr, QVariant::fromValue((bool)false));
     ui->frame_2->setVisible(false);
     ui->frame_3->setVisible(false);
     ui->frame_4->setVisible(false);
     ui->frame_5->setVisible(false);
     setZ_criticalMask(0);
}

void NowyTest_3::updateWejscia()
{

}

void NowyTest_3::updateCisnieie()
{
    if (!ustalanieCisnienia)
        return;

    double avg = getAvgCisnienie();
    double val = getCisnKomory();

    
    if (--timePompaProzniowa == 0) {
        ui->frame_5->setVisible(true);
        ui->arrow_4->setVisible(false);
        ustaloneCisnienie = avg;
        ui->text5->setText(QString("Uzyskano podciśnienie %1 kPa.").arg(avg));
        cisnienieTimer.stop();

        updateOutput(o_pompa_prozniowa, false);
        ustalanieCisnienia = false;
    }

    
    if (0.95*val > avg && 1.05*val < avg /*&& avg < cisnieWProzni*/) {
        ui->frame_5->setVisible(true);
        ui->arrow_4->setVisible(false);
        cisnienieTimer.stop();
        ustaloneCisnienie = avg;
        ui->text5->setText(QString("Uzyskano podciśnienie %1 kPa.").arg(ustaloneCisnienie));
        ui->frame_5->setVisible(true);
        ui->arrow_4->setVisible(false);
        ustalanieCisnienia = false;
        updateOutput(o_pompa_prozniowa, false);
    }

}

double NowyTest_3::getCisnKomory()
{
    QMutexLocker lock(&mutexCisnienie);
    return cisnKomory;
}

double NowyTest_3::getAvgCisnienie()
{
    QMutexLocker lock(&mutexCisnienie);
    double avg = 0.0;
    /*
    
           __FILE__, __LINE__, prevCisnienie[0], prevCisnienie[1],
           prevCisnienie[3], prevCisnienie[4], prevCisnienie[5],
           prevCisnienie[6], prevCisnienie[7], prevCisnienie[8],
           prevCisnienie[10],prevCisnienie[11], prevCisnienie[12],
           prevCisnienie[13], prevCisnienie[14],prevCisnienie[15]);
    */
    for (unsigned short id = 0; id < 16 ; ++id) {
        avg += prevCisnienie[id];
    }
    
    return avg/16.0;
}
void NowyTest_3::setCisnKomory(const double & newCisnKomory)
{
    if (!ustalanieCisnienia)
        return;
    QMutexLocker lock(&mutexCisnienie);
    cisnKomory = newCisnKomory;
    idPrev = (idPrev + 1) & 0xf;
    //
    prevCisnienie[idPrev] = newCisnKomory;

    ui->act_cisnienie->setText(QString::number(newCisnKomory));
    if (cisnienie_zad != 0)
        ui->delta_percent->setText(QString::number(100.0*(cisnienie_zad - newCisnKomory)/cisnienie_zad,'g',2));
    /*
    
           __FILE__, __LINE__, prevCisnienie[0], prevCisnienie[1],
           prevCisnienie[3], prevCisnienie[4], prevCisnienie[5],
           prevCisnienie[6], prevCisnienie[7], prevCisnienie[8],
           prevCisnienie[10],prevCisnienie[11], prevCisnienie[12],
           prevCisnienie[13], prevCisnienie[14],prevCisnienie[15]);
           */
}

void NowyTest_3::on_pbOk_1_clicked()
{
    wizard()->setDebug(QString("PAGE3:OK1"));
    sprawdzZawory(ui->pbOk_1, ui->arrow_1, ui->frame_2);
    setZ_criticalMask(0);
    setZ_warningMask(0);
}


void NowyTest_3::on_pb_Skip2_clicked()
{
    wizard()->setDebug(QString("PAGE3:SKIP2"));
    nextPage(nextPageId());
}

void NowyTest_3::on_pbUstaw_2_clicked()
{
    wizard()->setDebug(QString("PAGE3:USTAW2"));
    if (!sprawdzOtwarteZawory(i_drzwi_lewe | i_drzwi_prawe | i_pom_stez_1 | i_pom_stez_2 | i_proznia | i_wlot_powietrza | i_wentylacja_lewa | i_wentylacja_prawa))
        return;
    ui->arrow_2->setVisible(false);
    ui->pb_Skip2->setEnabled(false);
    ui->pbUstaw_2->setEnabled(false);
    ui->cisnienie->setEnabled(false);
    cisnienie_zad = ui->cisnienie->value();
    ui->frame_3->setVisible(true);
}

void NowyTest_3::on_pbOk_3_clicked()
{
    wizard()->setDebug(QString("PAGE3:OK3"));
   if (!sprawdzOtwarteZaworProzni())
       return;

    if (!sprawdzOtwarteZawory(i_drzwi_lewe | i_drzwi_prawe | i_pom_stez_1 | i_pom_stez_2 | i_wlot_powietrza | i_wentylacja_lewa | i_wentylacja_prawa))
       return;

    setZ_criticalMask(i_drzwi_lewe | i_drzwi_prawe | i_pom_stez_1 | i_pom_stez_2 | i_wlot_powietrza | i_wentylacja_lewa | i_wentylacja_prawa);
    setZ_warningMask(i_proznia);

    ui->pbOk_3->setEnabled(false);
    
    updateOutput(o_pompa_prozniowa, true);
    ustalanieCisnienia = true;
    timePompaProzniowa = 100;
    cisnienieTimer.start();
    setField(czyPompaMebr, QVariant::fromValue((bool)true));

    ui->arrow_3->setVisible(false);
    ui->frame_4->setVisible(true);
    ui->cisnienie_zad->setText(QString::number(cisnienie_zad,' ',2));

}

void NowyTest_3::on_pbOk_5_clicked()
{
    wizard()->setDebug(QString("PAGE3:O5"));
    updateOutput(o_pompa_prozniowa, false);
    TestData * dt = getTestData();
    dt->setCisnienieKomoryDozowanie(getCzujnik(a_cisn_komora));
    setZ_warningMask(0);
    nextPage(nextPageId());
}

void NowyTest_3::on_pbRun_5_clicked()
{
    wizard()->setDebug(QString("PAGE3:RUN5"));
    if (!sprawdzOtwarteZaworProzni())
        return;

    ui->frame_5->setVisible(false);
    ui->arrow_4->setVisible(true);

    timePompaProzniowa = 100;
    updateOutput(o_pompa_prozniowa, true);
    ustalanieCisnienia = true;
    cisnienieTimer.start();
    setField(czyPompaMebr, QVariant::fromValue((bool)true));

    //ui->arrow_3->setVisible(false);
    ui->frame_4->setVisible(true);
    //ui->cisnienie_zad->setText(QString::number(cisnienie_zad,' ',2));
}
