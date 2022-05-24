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


NowyTest_3::NowyTest_3(const double & mnoznik_, const QString & unit_, const UEkran3 & ust_, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_3),
    mnoznik(mnoznik_),
    unit(unit_),
    ust(ust_)
{
    ui->setupUi(this);
    task = 0;

    cisnienieTimer.setInterval(100);
    connect(&cisnienieTimer, &QTimer::timeout, this, &NowyTest_3::updateCisnieie);
    ui->unit_1->setText(unit);
    //ui->unit_2->setText(unit);



    ui->cisnienie->setMin(mnoznik*ust.minCisnieniePomProz);
    ui->cisnienie->setMax(1000);


    tdlg.hide();
    tdlg.setAllHisterezy(QString::number(ust.numberHistPomProz));
    tdlg.setNumHisterezy(QString::number(ust.numberHistPomProz));
    tdlg.setMaxTime(QString::number(ust.allTimeRunPomProz));
    tdlg.setFirstTimeWait(QString::number(ust.firsTimeWaitPomProz));
    tdlg.setSecondTimeWait(QString::number(ust.secondTimeWaitPomProz));

}

NowyTest_3::~NowyTest_3()
{
    cisnienieTimer.stop();
    delete ui;

}

void NowyTest_3::initializePage()
{
     setField(czyPompaProzn, QVariant::fromValue((bool)false));
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
    double val = getCisnKomory();

    if (actWork == OP_IDLE)
        return;


    
    if (actWork == FIRST_WORK) {
        tdlg.show();

        timePompaProzniowa = ust.allTimeRunPomProz;
        upLevel = (1+(ust.upLevelHistPomProz/100))*cisnienie_zad;
        downLevel = (1-(ust.downLevelHistPomProz/100))*cisnienie_zad;
        wizard()->setDebug(QString("PAGE3:Ruszam z odciaganiem prożni proznia = %1, max czas = %2s histereza [%3 %4]").arg(cisnienie_zad).
                           arg(ust.allTimeRunPomProz / 10).arg(downLevel).arg(upLevel));
        tdlg.setDownLevel(QString::number(downLevel, 'f', 3));
        tdlg.setUpLevel(QString::number(upLevel, 'f', 3));

        tdlg.setZadaneCisnie(QString::number(cisnienie_zad, 'f', 3));


    }

    --timePompaProzniowa;
    tdlg.setActTime(QString::number(ust.allTimeRunPomProz - timePompaProzniowa));
    tdlg.setActCisnie(QString::number(val, 'f', 3));
    if (timePompaProzniowa == 0 && actWork != FINISH_STABLE) {

        actWork = OP_IDLE;
        updateOutput(o_pompa_prozniowa, false);
        cisnienieTimer.stop();
        wizard()->setDebug(QString("PAGE3:Nie udalo sie uzyskac cisnienia = %1, max czas = %2s histereza [%3 %4] aktualne cisnieie %5").arg(cisnienie_zad).
                           arg(ust.allTimeRunPomProz / 10).arg(downLevel).arg(upLevel).arg(val));
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
            cisnienieTimer.stop();
            ui->frame_6->setVisible(true);
            ui->cisnienieKomora_6->setText(getCisnienie(val));
            ui->cisnienie_zad_6->setText(getCisnienie(cisnienie_zad));
            ui->arrow_4->setVisible(false);
            wizard()->setDebug(QString("PAGE3:Cisnienie nisze niz zadane = %1, max czas = %2s histereza [%3 %4] aktualne cisnieie %5").arg(cisnienie_zad).
                               arg(ust.allTimeRunPomProz / 10).arg(downLevel).arg(upLevel).arg(val));
            return;
        }
        actWork = FIRST_POMPA_RUN;
        czasWork[actWork] = timePompaProzniowa;
        updateOutput(o_pompa_prozniowa, true);
        return;
    }
    case FIRST_POMPA_RUN: {
        if (val < downLevel) {
            actWork = FIRST_WAIT;
            updateOutput(o_pompa_prozniowa, false);
            czasWork[actWork] = timePompaProzniowa;
            nrHisterezy = ust.numberHistPomProz-1;

            wizard()->setDebug(QString("PAGE3:Pierwsze uruchomienie pompy zadane cisnienie = %1, max czas = %2s histereza [%3 %4] aktualne cisnieie %5").arg(cisnienie_zad).
                               arg(ust.allTimeRunPomProz / 10).arg(downLevel).arg(upLevel).arg(val));
        }
        return;
    }
    case FIRST_WAIT:
    {
        tdlg.setBezczynnoscPompy(QString::number(czasWork[actWork] - timePompaProzniowa));
        if (val < upLevel && czasWork[actWork] - timePompaProzniowa > ust.firsTimeWaitPomProz) {
            wizard()->setDebug(QString("PAGE3:Pierwsze zatrzymanie pompy [Cisnienie stabilne] zadane cisnienie = %1, max czas = %2s histereza [%3 %4] aktualne cisnieie %5, czas stabilnego cisnienia %6s aktualny czas = %7").
                               arg(cisnienie_zad).arg(ust.allTimeRunPomProz / 10).arg(downLevel).arg(upLevel).arg(val)
                               .arg(ust.firsTimeWaitPomProz/10).arg((ust.allTimeRunPomProz-timePompaProzniowa)/10));
            actWork = FINISH_STABLE;
            ui->arrow_4->setVisible(false);
            ui->frame_5->setVisible(true);
            ui->uzyskane_cisnienie_5->setText(QString::number(val));
            cisnienieTimer.stop();
            return;
        }
        if (val > upLevel) {
            if (czasWork[FIRST_WAIT] - timePompaProzniowa > ust.minTimeBetweenRunPomProz) {
                wizard()->setDebug(QString("PAGE3:Pierwsze zatrzymanie pompy [Cisnienie Niestabilne] zadane cisnienie = %1, max czas = %2s histereza [%3 %4] aktualne cisnieie %5, czas stabilnego cisnienia %6s aktualny czas = %7").
                                   arg(cisnienie_zad).arg(ust.allTimeRunPomProz / 10).arg(downLevel).arg(upLevel).arg(val)
                                   .arg(ust.firsTimeWaitPomProz/10).arg((ust.allTimeRunPomProz-timePompaProzniowa)/10));

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
        if (val < downLevel) {
            actWork = NEXT_WAIT;
            updateOutput(o_pompa_prozniowa, false);
            czasWork[actWork] = timePompaProzniowa;
            wizard()->setDebug(QString("PAGE3:Kolejne zatrzymanie pompy zadane cisnienie = %1, max czas = %2s histereza [%3 %4] aktualne cisnieie %5, czas stabilnego cisnienia %6s aktualny czas = %7").
                               arg(cisnienie_zad).arg(ust.allTimeRunPomProz / 10).arg(downLevel).arg(upLevel).arg(val)
                               .arg(ust.secondTimeWaitPomProz/10).arg((ust.allTimeRunPomProz-timePompaProzniowa)/10));

        }
        return;
    }
    case NEXT_WAIT:
    {
        tdlg.setBezczynnoscPompy(QString::number(czasWork[actWork] - timePompaProzniowa));
        if (val < upLevel && czasWork[actWork] - timePompaProzniowa > ust.secondTimeWaitPomProz) {
            wizard()->setDebug(QString("PAGE3:Kolejne zatrzymanie pompy [Cisnienie stabilne] zadane cisnienie = %1, max czas = %2s histereza [%3 %4] aktualne cisnieie %5, czas stabilnego cisnienia %6s aktualny czas = %7").
                               arg(cisnienie_zad).arg(ust.allTimeRunPomProz / 10).arg(downLevel).arg(upLevel).arg(val)
                               .arg(ust.secondTimeWaitPomProz/10).arg((ust.allTimeRunPomProz-timePompaProzniowa)/10));
            actWork = FINISH_STABLE;
            ui->arrow_4->setVisible(false);
            ui->frame_5->setVisible(true);
            ui->uzyskane_cisnienie_5->setText(QString::number(val));
            cisnienieTimer.stop();
            return;
        }
        if (val > upLevel) {
            if (czasWork[NEXT_WAIT] - timePompaProzniowa > ust.minTimeBetweenRunPomProz) {
                nrHisterezy--;
                tdlg.setNumHisterezy(QString::number(nrHisterezy));
                if (nrHisterezy == 0) {
                    actWork = OP_IDLE;
                    updateOutput(o_pompa_prozniowa, false);
                    cisnienieTimer.stop();
                    wizard()->setDebug(QString("PAGE3:Ostatnie zatrzymanie pompy [CISNIENIE NIESTABILNE] zadane cisnienie = %1, max czas = %2s histereza [%3 %4] aktualne cisnieie %5, czas stabilnego cisnienia %6s aktualny czas = %7").
                                       arg(cisnienie_zad).arg(ust.allTimeRunPomProz / 10).arg(downLevel).arg(upLevel).arg(val)
                                       .arg(ust.secondTimeWaitPomProz/10).arg((ust.allTimeRunPomProz-timePompaProzniowa)/10));
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
        return;
    }

    default:
        return;
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

    for (unsigned short id = 0; id < 16 ; ++id) {
        avg += prevCisnienie[id];
    }
    
    return avg/16.0;
}

void NowyTest_3::debugStr(const QString &debug)
{
    qDebug() << debug;
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
    TestData * dt = getTestData();
    dt->start();
    dt->setPodcisnienie(false);
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

    cisnienieTimer.start();
    setField(czyPompaProzn, QVariant::fromValue((bool)true));

    ui->arrow_3->setVisible(false);
    ui->frame_4->setVisible(true);
    ui->cisnienie_zad->setText(getCisnienie(cisnienie_zad));

}

void NowyTest_3::on_pbOk_5_clicked()
{
    wizard()->setDebug(QString("PAGE3:O5"));
    updateOutput(o_pompa_prozniowa, false);
    TestData * dt = getTestData();
    dt->start();
    dt->setPodcisnienie(true);
    cisnienieTimer.stop();
    setZ_warningMask(0);
    nextPage(nextPageId());
}

void NowyTest_3::on_pbRun_5_clicked()
{
    //uruchomienie pompy recznietd->start();

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
    setField(czyPompaProzn, QVariant::fromValue((bool)true));

    //ui->arrow_3->setVisible(false);
    ui->frame_4->setVisible(true);
    //ui->cisnienie_zad->setText(QString::number(cisnienie_zad,' ',2));
}
