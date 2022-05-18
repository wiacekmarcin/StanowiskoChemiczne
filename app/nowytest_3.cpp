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


NowyTest_3::NowyTest_3(const double & mnoznik_, const QString & unit_, uint64_t timePompaProzniowa_, short maxHisterez_,
                       double wspolczynnikDolny_, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_3),
    mnoznik(mnoznik_),
    timePompaProzniowaMax(timePompaProzniowa_),
    maxHisterez(maxHisterez_),
    wspolczynnikDolny(wspolczynnikDolny_),
    unit(unit_)
{
    ui->setupUi(this);
    task = 0;

    cisnienieTimer.setInterval(100);
    connect(&cisnienieTimer, &QTimer::timeout, this, &NowyTest_3::updateCisnieie);
    ui->unit_1->setText(unit);
    //ui->unit_2->setText(unit);

    czas_wylaczeniaPompy = 0;
    czas_przerwyPompy = 50;

    ui->cisnienie->setMin(700);
    ui->cisnienie->setMax(1000);
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
     ui->frame_6->setVisible(false);
     ui->frame_7->setVisible(false);
     setZ_criticalMask(0);
     actWork = OP_IDLE;
}

void NowyTest_3::updateWejscia()
{

}

void NowyTest_3::updateCisnieie()
{
    static short nrUpdate = 0;
    double val = getCisnKomory();
    debugStr(QString("<b>actWork</b> Czas = (%1) actWork = %2 val = %3").arg(timePompaProzniowa).arg(actWork).arg(val));
    if (actWork == OP_IDLE)
        return;

    //double avg = getAvgCisnienie();

    //debugStr(QString("<b>actWork</b> Czas = (%1) actWork = %2 val = %3").arg(timePompaProzniowa).arg(actWork).arg(val));
    
    if (actWork == FIRST_WORK) {
        timePompaProzniowa = timePompaProzniowaMax;
    }

    --timePompaProzniowa;
    if (timePompaProzniowa == 0 && actWork != FINISH_STABLE) {
        actWork = OP_IDLE;
        updateOutput(o_pompa_prozniowa, false);
        cisnienieTimer.stop();
        int ret = QMessageBox::information(this, "Ustawianie podciśnienia",
                                 "Nie udało się uzyskać żądanego podciśnienie. Układ prawdopodobnie nieszczelny. Czy chcesz kontynuować",
                                 QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::Yes) {
                on_pbOk_5_clicked();
        } else if (ret == QMessageBox::No) {
            setFinished(false);
        }
    }
    switch(actWork) {
    case FIRST_WORK: {
        czasWork[actWork] = timePompaProzniowa;
        if (val < cisnienie_zad) {
            actWork = OP_IDLE;
            ui->frame_6->setVisible(true);
            ui->cisnienieKomora_6->setText(getCisnienie(val));
            ui->cisnienie_zad_6->setText(getCisnienie(cisnienie_zad));
            ui->arrow_4->setVisible(false);
            return;
        }
        actWork = FIRST_POMPA_RUN;
        czasWork[actWork] = timePompaProzniowa;
        updateOutput(o_pompa_prozniowa, true);
        return;
    }
    case FIRST_POMPA_RUN: {
        if (val < cisnienie_zad*(1-wspolczynnikDolny)) {
            actWork = FIRST_WAIT;
            updateOutput(o_pompa_prozniowa, false);
            czasWork[actWork] = timePompaProzniowa;
            nrHisterezy = maxHisterez-1;
        }
        return;
    }
    case FIRST_WAIT:
    {
        debugStr(QString("<b>F WAIT</b> Czas = (%1) [50] val = %2  <  %3").arg(czasWork[actWork] - timePompaProzniowa)
                 .arg(val).arg(cisnienie_zad*(1+wspolczynnikDolny)));
        if (czasWork[actWork] - timePompaProzniowa > 500) {
            actWork = FINISH_STABLE;
            ui->arrow_4->setVisible(false);
            ui->frame_5->setVisible(true);
            ui->uzyskane_cisnienie_5->setText(QString::number(val));
            return;
        }
        if (val > cisnienie_zad*(1+wspolczynnikDolny)) {
            if (czasWork[FIRST_WAIT] - timePompaProzniowa > 50) {
                actWork = NEXT_WORK;
                czasWork[actWork] = timePompaProzniowa;
                updateOutput(o_pompa_prozniowa, true);
            }
            return;
        }
        return;
    }
    case NEXT_WORK:
    {
        if (val < cisnienie_zad*(1-wspolczynnikDolny)) {
            actWork = NEXT_WAIT;
            updateOutput(o_pompa_prozniowa, false);
            czasWork[actWork] = timePompaProzniowa;
        }
        return;
    }
    case NEXT_WAIT:
    {
        if (val > cisnienie_zad*(1+wspolczynnikDolny)) {
            if (czasWork[NEXT_WAIT] - timePompaProzniowa > 50) {
                nrHisterezy--;
                if (nrHisterezy == 0) {
                    actWork = OP_IDLE;
                    updateOutput(o_pompa_prozniowa, false);
                    cisnienieTimer.stop();
                    int ret = QMessageBox::information(this, "Ustawianie podciśnienia",
                                             "Nie udało się uzyskać żądanego podciśnienie. Układ prawdopodobnie nieszczelny. Czy chcesz kontynuować",
                                             QMessageBox::Yes, QMessageBox::No);
                    if (ret == QMessageBox::Yes) {
                            on_pbOk_5_clicked();
                    } else if (ret == QMessageBox::No) {
                        setFinished(false);
                    }
                    return;
                }
                actWork = NEXT_WORK;
                updateOutput(o_pompa_prozniowa, true);
                czasWork[actWork] = timePompaProzniowa;
            }

        }
        return;
    }
    case FINISH_STABLE:
    {
        nrUpdate++;
        if (nrUpdate == 10) {
            nrUpdate = 0;
            ui->uzyskane_cisnienie_5->setText(getCisnienie(val));
        }

        return;
    }

    default:
        return;
    }
/*
    if (czas_brakuWzrostuCisnienia && czas_brakuWzrostuCisnienia - timePompaProzniowa > )
    if (--timePompaProzniowa == 0) {
        debugStr("Koniec ustalania");
        ui->frame_5->setVisible(true);
        ui->arrow_4->setVisible(false);
        ustaloneCisnienie = val;
        ui->uzyskane_cisnienie_5->setText(QString::number(val));
        cisnienieTimer.stop();

        updateOutput(o_pompa_prozniowa, false);
        ustalanieCisnienia = false;
    }

    if (oczekiwanieNaWzrost && val > cisnienie_zad*(1+wspolczynnikDolny) ) {
        if (czas_wylaczeniaPompy - timePompaProzniowa < czas_przerwyPompy) {
            debugStr(QString("Za krotka przerwa pompy"));
            return;
        }
        debugStr(QString("Wzrost cisnienia %1  %2").arg(val).arg(cisnienie_zad*(1+wspolczynnikDolny)));
        oczekiwanieNaWzrost = false;

        updateOutput(o_pompa_prozniowa, true);
        return;
    }

    if (!oczekiwanieNaWzrost && val < cisnienie_zad*(1-wspolczynnikDolny)) {
        debugStr(QString("Spadek cisnienia %1  %2. Nr histerezy %3").arg(val).arg(cisnienie_zad*(1-wspolczynnikDolny)).arg(nrHisterezy));
        --nrHisterezy;
        oczekiwanieNaWzrost = true;
        czas_wylaczeniaPompy = timePompaProzniowa;
        updateOutput(o_pompa_prozniowa, false);
        if (nrHisterezy == 0) {
            ui->frame_5->setVisible(true);
            ui->arrow_4->setVisible(false);
            ustaloneCisnienie = val;
            ui->uzyskane_cisnienie_5->setText(QString::number(val));
            cisnienieTimer.stop();

            updateOutput(o_pompa_prozniowa, false);
            ustalanieCisnienia = false;
        }
    }
    */
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

void NowyTest_3::debugStr(const QString &debug)
{
    //ui->debug->appendHtml(QString("<p>%1</p>").arg(debug));
}

QString NowyTest_3::getCisnienie(double val)
{
    return QString("%1 %2").arg(QString::number(val, 'f', 1), unit);
}
void NowyTest_3::setCisnKomory(const double & newCisnKomory)
{
    if (!ustalanieCisnienia)
        return;
    short id;
    double actCisn;
    {
        QMutexLocker lock(&mutexCisnienie);
        actCisn = cisnKomory = newCisnKomory * mnoznik;
        idPrev = (idPrev + 1) & 0xf;
        id = idPrev;
        prevCisnienie[idPrev] = cisnKomory;
    }

    if (id == 0) {
        ui->act_cisnienie->setText(getCisnienie(actCisn));
        if (cisnienie_zad != 0)
            ui->delta_percent->setText(QString::number(100.0*(cisnienie_zad - actCisn)/cisnienie_zad,'g',2));
    }
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
    //ui->pbOk_1->setEnabled(false);
    //ui->arrow_1->setVisible(false);
    //ui->frame_2->setVisible(true);
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
    
    //updateOutput(o_pompa_prozniowa, true);
    actWork = FIRST_WORK;
    ustalanieCisnienia = true;
    timePompaProzniowa = timePompaProzniowaMax;
    czas_wylaczeniaPompy = timePompaProzniowaMax + czas_przerwyPompy;
    czas_brakuWzrostuCisnienia = 0;
    nrHisterezy = maxHisterez;

    cisnienieTimer.start();
    setField(czyPompaMebr, QVariant::fromValue((bool)true));

    ui->arrow_3->setVisible(false);
    ui->frame_4->setVisible(true);
    ui->cisnienie_zad->setText(getCisnienie(cisnienie_zad));

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
    //uruchomienie pompy recznie

    wizard()->setDebug(QString("PAGE3:RUN5"));
    if (!sprawdzOtwarteZaworProzni())
        return;

    ui->frame_5->setVisible(false);
    ui->arrow_4->setVisible(true);

    //timePompaProzniowa = timePompaProzniowaMax;
    //czas_wylaczeniaPompy = timePompaProzniowaMax + czas_przerwyPompy;
    //czas_brakuWzrostuCisnienia = 0;
    //nrHisterezy = maxHisterez;
    updateOutput(o_pompa_prozniowa, true);
    //ustalanieCisnienia = true;
    //cisnienieTimer.start();
    setField(czyPompaMebr, QVariant::fromValue((bool)true));

    //ui->arrow_3->setVisible(false);
    ui->frame_4->setVisible(true);
    //ui->cisnienie_zad->setText(QString::number(cisnienie_zad,' ',2));
}
