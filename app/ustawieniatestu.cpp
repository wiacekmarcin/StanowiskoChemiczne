#include "ustawieniatestu.h"
#include "ui_ustawieniatestu.h"
#include <QAbstractButton>
#include <QPushButton>

#include "ustawienia.h"

#define setDB(P, U) do { P->setMin(0.1); P->setPrec(1); P->setMax(1000); P->setValue(U); } while(false)

#define setEKRAN3(W, P, M, U) do { W->setMin(0); W->setPrec(P); W->setMax(M); W->setValue(U); } while(false)

#define setEKRAN4(W, U) do { W->setMin(0); W->setPrec(0); W->setMax(10000); W->setValue(U); } while(false)

#define setStezenia(W, U) do { W->setMin(0); W->setPrec(0); W->setMax(10000); W->setValue(U); } while(false)

#define setEkran6Zap(W, P, M, U) do { W->setMin(0); W->setPrec(P); W->setMax(M); W->setValue(U); } while(false)

#define setOpozKam(W, U) do { W->setMin(0); W->setPrec(0); W->setMax(100000); W->setValue(U); } while(false)

#define setZaplon(W, U) do { W->setMin(0); W->setPrec(0); W->setMax(100000); W->setValue(U); } while(false)

UstawieniaTestu::UstawieniaTestu(Ustawienia & ust, const UserPrivilige &user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UstawieniaTestu),
    ustawienia(ust)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &UstawieniaTestu::on_buttonBox_clicked);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &UstawieniaTestu::on_buttonBox_accepted);
    
    ui->test_1->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->test_2->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->test_3->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->test_4->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->oczekiwanieNaZaplon->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->pomiarStezen->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->opoznienieKamery->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->Ekran6Zaplon->setEnabled((user & U_SERVISANT) == U_SERVISANT);

    setDB(ui->ekran1_acetan, ustawienia.getMaxAceton());
    setDB(ui->ekran1_benzyna, ustawienia.getMaxBenzyna());
    setDB(ui->ekran1_etanol, ustawienia.getMaxEtanol());
    setDB(ui->ekran1_izopropanol, ustawienia.getMaxIzopropanol());
    setDB(ui->ekran1_toulen, ustawienia.getMaxToluen());

    ui->ekran2_sBCykle->setValue(ustawienia.getNrInitializeCycles());

    setEKRAN3(ui->ekran3_minCisnienie, 1, 120, ustawienia.getMinCisnieniePomProz());
    setEKRAN3(ui->ekran3_downHisterza, 2, 100, ustawienia.getDownLevelHistPomProz());
    setEKRAN3(ui->ekran3_upHisteraz, 2, 100, ustawienia.getDownLevelHistPomProz());
    setEKRAN3(ui->ekran3_nrHisterezy, 0, 10, ustawienia.getNumberHistPomProz());
    setEKRAN3(ui->ekran3_firstTime, 1, 1000, 0.1*ustawienia.getFirsTimeWaitPomProz());
    setEKRAN3(ui->ekran3_nextTime, 1, 1000, 0.1*ustawienia.getSecondTimeWaitPomProz());
    setEKRAN3(ui->ekran3_timeBetweenPompa, 1, 1000, 0.1*ustawienia.getMinTimeBetweenRunPomProz());
    setEKRAN3(ui->ekran3_maxtime, 1, 10000, 0.1*ustawienia.getAllTimeRunPomProz());

    setEKRAN4(ui->ekran4_dozowanietime1, ustawienia.getMinTimeAfterDozowanie());
    setEKRAN4(ui->ekran4_powietrzetime2, ustawienia.getMinTimeAfterPowietrze());

    setStezenia(ui->pomiarstezen_minpompa, ustawienia.getMinTimePompaMebramowa());
    setStezenia(ui->pomiarstezen_time2, ustawienia.getMinTimeAfterPompaOff());

    setEkran6Zap(ui->oczekiwaniezaplon_deltacisnienia, 3, 100, ustawienia.getMinDeltaCisnZaplon());
    setEkran6Zap(ui->oczekiwanienazaplon_timeIskra, 0, 1000, ustawienia.getMinTimeZaplonIskra());
    setEkran6Zap(ui->oczekiwanienazaplon_plomien, 0, 1000, ustawienia.getMinTimeZaplonPlomien());

    setOpozKam(ui->opoznieniekamery_plomien, ustawienia.getDelayTimeTriggerPlomien());

    setZaplon(ui->zaplon_czasPlomienia, ustawienia.getRunTimePlomien());
    setZaplon(ui->zaplon_czasIksryMechanicznej, ustawienia.getRunTimeIskraMechaniczna());
    setZaplon(ui->zaplon_czasWysokieNapiecie, ustawienia.getDelayTimeIskraElektrycznaHV());
    setZaplon(ui->zaplon_czasIskryElektrycznej, ustawienia.getDelayTimeIskraElektrycznaIskra());

    QPushButton * btn;
    btn = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    if (btn)
        btn->setEnabled((user & U_SERVISANT) == U_SERVISANT);

    btn = ui->buttonBox->button(QDialogButtonBox::Ok);
    if (btn)
        btn->setEnabled((user & U_SERVISANT) == U_SERVISANT);
}

UstawieniaTestu::~UstawieniaTestu()
{
    delete ui;
}

void UstawieniaTestu::on_buttonBox_clicked(QAbstractButton *button)
{
    
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults))
    {
        ui->ekran1_acetan->setValue(10.0);
        ui->ekran1_benzyna->setValue(10.0);
        ui->ekran1_etanol->setValue(10.0);
        ui->ekran1_izopropanol->setValue(10.0);
        ui->ekran1_toulen->setValue(10.0);

        ui->ekran2_sBCykle->setValue(4);

        ui->ekran3_minCisnienie->setValue(75);
        ui->ekran3_downHisterza->setValue(1);
        ui->ekran3_upHisteraz->setValue(1);
        ui->ekran3_nrHisterezy->setValue(3);
        ui->ekran3_firstTime->setValue(300/10);
        ui->ekran3_nextTime->setValue(150/10);
        ui->ekran3_timeBetweenPompa->setValue(50/10);
        ui->ekran3_maxtime->setValue(1200/10);

        ui->ekran4_dozowanietime1->setValue(15);
        ui->ekran4_powietrzetime2->setValue(15);

        ui->pomiarstezen_minpompa->setValue(30);
        ui->pomiarstezen_time2->setValue(30);

        ui->oczekiwaniezaplon_deltacisnienia->setValue(5);
        ui->oczekiwanienazaplon_timeIskra->setValue(5);
        ui->oczekiwanienazaplon_plomien->setValue(15);

        ui->opoznieniekamery_plomien->setValue(5000);

        ui->zaplon_czasPlomienia->setValue(6000);
        ui->zaplon_czasIksryMechanicznej->setValue(2000);
        ui->zaplon_czasWysokieNapiecie->setValue(2000);
        ui->zaplon_czasIskryElektrycznej->setValue(1000);

    }
    if ((QPushButton*)button == ui->buttonBox->button(QDialogButtonBox::Apply))
    {
        save();
    }
}

void UstawieniaTestu::on_buttonBox_accepted()
{
    
    save();
}

void UstawieniaTestu::save()
{
    ustawienia.setMaxAceton(ui->ekran1_acetan->value());
    ustawienia.setMaxBenzyna(ui->ekran1_benzyna->value());
    ustawienia.setMaxEtanol(ui->ekran1_etanol->value());
    ustawienia.setMaxIzopropanol(ui->ekran1_izopropanol->value());
    ustawienia.setMaxToluen(ui->ekran1_toulen->value());

    ustawienia.setNrInitializeCycles(ui->ekran2_sBCykle->value());

    ustawienia.setMinCisnieniePomProz(ui->ekran3_minCisnienie->value());
    ustawienia.setDownLevelHistPomProz(ui->ekran3_downHisterza->value());
    ustawienia.setDownLevelHistPomProz(ui->ekran3_upHisteraz->value());
    ustawienia.setNumberHistPomProz(ui->ekran3_nrHisterezy->value());
    ustawienia.setFirsTimeWaitPomProz(10*ui->ekran3_firstTime->value());
    ustawienia.setSecondTimeWaitPomProz(10*ui->ekran3_nextTime->value());
    ustawienia.setMinTimeBetweenRunPomProz(10*ui->ekran3_timeBetweenPompa->value());
    ustawienia.setAllTimeRunPomProz(10*ui->ekran3_maxtime->value());

    ustawienia.setMinTimeAfterDozowanie(ui->ekran4_dozowanietime1->value());
    ustawienia.setMinTimeAfterPowietrze(ui->ekran4_powietrzetime2->value());

    ustawienia.setMinTimePompaMebramowa(ui->pomiarstezen_minpompa->value());
    ustawienia.setMinTimeAfterPompaOff(ui->pomiarstezen_time2->value());

    ustawienia.setMinDeltaCisnZaplon(ui->oczekiwaniezaplon_deltacisnienia->value());
    ustawienia.setMinTimeZaplonIskra(ui->oczekiwanienazaplon_timeIskra->value());
    ustawienia.setMinTimeZaplonPlomien(ui->oczekiwanienazaplon_plomien->value());

    ustawienia.setDelayTimeTriggerPlomien(ui->opoznieniekamery_plomien->value());

    ustawienia.setRunTimePlomien(ui->zaplon_czasPlomienia->value());
    ustawienia.setRunTimeIskraMechaniczna(ui->zaplon_czasIksryMechanicznej->value());
    ustawienia.setDelayTimeIskraElektrycznaHV(ui->zaplon_czasWysokieNapiecie->value());
    ustawienia.setDelayTimeIskraElektrycznaIskra(ui->zaplon_czasIskryElektrycznej->value());

}
