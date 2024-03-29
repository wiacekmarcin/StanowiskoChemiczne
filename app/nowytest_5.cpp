#include "nowytest_5.h"
#include "ui_nowytest_5.h"
#include "createtestwizard.h"
#include "common.h"
#include <QTimer>
#include <QMessageBox>

NowyTest_5::NowyTest_5(const UPomiarStezen & u, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_5),
    czasPracyPompki(1000*u.minTimePompaMebramowa),
    czasNaStezenie(1000*u.minTimeAfterPompaOff)
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
    setZ_criticalMask(i_drzwi_lewe | i_drzwi_prawe | i_pom_stez_1 | i_pom_stez_2 | i_proznia | i_wlot_powietrza | i_wentylacja_lewa | i_wentylacja_prawa);

}

void NowyTest_5::runDone()
{
    updateOutput(o_pompa_powietrza, false);
    QTimer::singleShot(czasNaStezenie, this, &NowyTest_5::runDone2);
}

void NowyTest_5::runDone2()
{
    ui->pbOK_3->setEnabled(true);
}

void NowyTest_5::on_pbOK_1_clicked()
{
    wizard()->setDebug(QString("PAGE5:OK1"));
    sprawdzZawory(ui->pbOK_1, ui->arrow_1, ui->frame_2);
    setZ_criticalMask(i_drzwi_lewe | i_drzwi_prawe | i_wentylacja_prawa | i_wentylacja_lewa | i_proznia | i_wlot_powietrza);
    setZ_warningMask(0);
#if SKIP_ZAWORY    
    ui->pbOK_1->setEnabled(false);
    ui->arrow_1->setVisible(false);
    ui->frame_2->setVisible(true);
#endif
}


void NowyTest_5::on_pbOK_2_clicked()
{
    wizard()->setDebug(QString("PAGE5:OK2"));
    if (!sprawdzOtwarteZaworStezenia())
        return;
    setZ_warningMask(i_pom_stez_1 | i_pom_stez_2);
    ui->pbOK_2->setEnabled(false);
    ui->arrow_2->setVisible(false);
    ui->frame_3->setVisible(true);
    ui->pbOK_3->setEnabled(false);

    updateOutput(o_pompa_powietrza, true);
    //emit pomiary(true);
    //emit pompaMembramowa(true);
    //emit o_mieszadlo(true);
    QTimer::singleShot(czasPracyPompki, this, &NowyTest_5::runDone);
}


void NowyTest_5::on_pbOK_3_clicked()
{
    wizard()->setDebug(QString("PAGE5:OK3"));
    if (!sprawdzOtwarteZaworStezenia())
        return;
    updateOutput(o_pompa_powietrza, false);
    TestData * dt = getTestData();
    float val = getCzujnik(a_temp_komory);
    wizard()->setDebug(QString("PAGE5:TemperaturaKomory %1").arg(val));
    dt->setTemperaturaKomoryPrzedZaplonem(val);
    //if (field(czyPompaProzn).toBool()) {
    dt->setCisnienieKomoryPrzedZaplonem(getCzujnik(a_cisn_komora));
    //}
    //setZ_warningMask(i_pom_stez_1 | i_pom_stez_2);

    dt->setStezeniaPrzedZaplonem(getCzujnik(a_voc1), getCzujnik(a_voc2), getCzujnik(a_o2),
                   getCzujnik(a_co2), getCzujnik(a_8));
    nextPage(nextPageId());
}

