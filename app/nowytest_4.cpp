#include "nowytest_4.h"
#include "ui_nowytest_4.h"

#include "createtestwizard.h"
#include "testpage.h"
#include "common.h"
#include <QTimer>
#include <QMessageBox>

NowyTest_4::NowyTest_4(const UEkran4 & u4_, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_4),
    u4(u4_),
    infoString("Rozpocznij dozowanie <b>[CIECZ]</b> z dozonika <b>[DOZOWNIK]</b> w objętości <b>[OBJETOSC]</b> ml.")
{
    ui->setupUi(this);

    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
    ui->frame_4->setVisible(false);
}

void NowyTest_4::initializePage()
{
    setZ_criticalMask(i_drzwi_lewe | i_drzwi_prawe | i_pom_stez_1 | i_pom_stez_2 | i_proznia | i_wlot_powietrza | i_wentylacja_lewa | i_wentylacja_prawa);
    
    QString info = infoString;
    info.replace("[CIECZ]", field(ciecz).toString());
    info.replace("[DOZOWNIK]", QString::number(field(dozownikNr).toUInt()));
    info.replace("[OBJETOSC]", QString::number(field(objetosc).toDouble()));
    if (field(calaObjetosc).toDouble() > 0) {
        info += QString(" Razem bedzie <b>%1<b> ml").arg(QString::number(
                            field(calaObjetosc).toDouble() + field(objetosc).toDouble()
                                                                       ));
        }


    ui->text_2->setTextFormat(Qt::RichText);
    ui->text_2->setText(info);
    ui->arrow_1->setVisible(true);
    ui->arrow_2->setVisible(true);
    ui->arrow_3->setVisible(true);
    ui->arrow_4->setVisible(true);

    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
    ui->frame_4->setVisible(false);

    ui->pbOk_1->setEnabled(true);
    ui->pbOk_2->setEnabled(true);
    ui->pbOk_3->setEnabled(true);
    ui->pbOk_4->setEnabled(true);

    TestData * td = getTestData();
    td->start();
    float val = getCzujnik(a_cisn_komora);
    wizard()->setDebug(QString("PAGE4:Cisnienie warunki poczatkowe:%1").arg(val));
    wizard()->setDebug(QString("PAGE4:P1:%1").arg(val));
    td->setCisnienieP1(val);
    td->setCisnienieKomoryWarunkiPoczatkowe(val);
}

NowyTest_4::~NowyTest_4()
{
    delete ui;
}

void NowyTest_4::dozownikDone(bool success)
{
    qInfo() << "DozownikDone=" << success;
    if (!success) {
        QMessageBox msgBox;
        msgBox.setText("Nie udało się zadozować cieczy.");
        msgBox.setInformativeText("Czy chcesz kontynuować");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if (ret == QMessageBox::No)
            return;
    } else {
        setField(calaObjetosc, QVariant::fromValue(field(calaObjetosc).toDouble() + field(objetosc).toDouble()));
        ui->pbOk_2->setEnabled(false);
        ui->arrow_2->setVisible(false);
        ui->frame_3->setVisible(true);
        ui->pbOk_2->setVisible(true);
        ui->pbOk_3->setEnabled(false);
        QTimer::singleShot(1000*u4.minTimeAfterDozowanie, this, &NowyTest_4::runDone);
    }

}

void NowyTest_4::on_pbOk_1_clicked()
{
    wizard()->setDebug(QString("PAGE4:OK1"));
    sprawdzZawory(ui->pbOk_1, ui->arrow_1, ui->frame_2);
#if SYMULATOR    
    ui->pbOk_1->setEnabled(false);
    ui->arrow_1->setVisible(false);
    ui->frame_2->setVisible(true);
#endif    
}

void NowyTest_4::on_pbOk_2_clicked()
{
    wizard()->setDebug(QString("PAGE4:OK2"));
    if (!sprawdzOtwarteZawory(i_drzwi_lewe | i_drzwi_prawe | i_pom_stez_1 | i_pom_stez_2 | i_proznia | i_wlot_powietrza | i_wentylacja_lewa | i_wentylacja_prawa))
        return;
    ui->pbOk_2->setEnabled(false);

    dozownikMl(field(dozownikNr).toUInt()-1, (unsigned int)10*field(objetosc).toDouble());
    updateOutput(o_mieszadlo, true);
#if SYMULATOR    
    dozownikDone(true);
#endif    
}

void NowyTest_4::on_pbOk_3_clicked()
{
    wizard()->setDebug(QString("PAGE4:OK3"));
    float val1 = getCzujnik(a_temp_parownik);
    float val2 = getCzujnik(a_temp_komory);
    float val3 = getCzujnik(a_cisn_komora);
    TestData * td = getTestData();
    wizard()->setDebug(QString("PAGE4:Temperatura Parownika:%1").arg(val1));
    wizard()->setDebug(QString("PAGE4:Temperatura Komory Dozowanie:%1").arg(val2));
    wizard()->setDebug(QString("PAGE4:P2:%1").arg(val3));
    td->setTemperaturaParownika(val1);
    td->setTemperaturaKomoryDozowanie(val2);
    td->setCisnienieP2(val3);

    if (field(czyPompaProzn).toBool()) {
        float val = getCzujnik(a_cisn_komora);
        wizard()->setDebug(QString("PAGE4:Cisnienie Przed Zaplonem %1").arg(val));
        getTestData()->setCisnienieKomoryZaplon(val);
        ui->pbOk_3->setEnabled(false);
        ui->arrow_3->setVisible(false);
        ui->frame_4->setVisible(true);
        ui->pbOk_4->setEnabled(false);
        QTimer::singleShot(1000*u4.minTimeAfterPowietrze, this, &NowyTest_4::runDone2);
        setZ_criticalMask(i_drzwi_lewe | i_drzwi_prawe | i_pom_stez_1 | i_pom_stez_2 | i_proznia | i_wentylacja_lewa | i_wentylacja_prawa);
    } else {

        nextPage(nextPageId());
    }
}

void NowyTest_4::runDone()
{
   ui->pbOk_3->setEnabled(true);
}

void NowyTest_4::runDone2()
{
   ui->pbOk_4->setEnabled(true);
}


void NowyTest_4::on_pbOk_4_clicked()
{
    wizard()->setDebug(QString("PAGE4:OK4"));
    float val = getCzujnik(a_cisn_komora);
    wizard()->setDebug(QString("PAGE4:OK4:P3:%1").arg(val));
    getTestData()->setCisnienieP3(val);
    getTestData()->setCisnienieKomoryPrzedZaplonem(val);
    if (!sprawdzOtwarteZaworPowietrza())
        return;

    nextPage(nextPageId());
}

