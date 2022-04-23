#include "nowytest_7.h"
#include "ui_nowytest_7.h"

#include "urzadzenia.h"
#include <QMessageBox>
#include "ustawienia.h"
#include <QMessageBox>

NowyTest_7::NowyTest_7(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_7)
{
    ui->setupUi(this);
    initial = false;
    next = PAGE_8;
    wykrytyZaplon = true;

    if (wykrytyZaplon) {
        ui->text_1->setText(QCoreApplication::translate("NowyTest_7",
   "<html><head/><body><p>Program <span style=\" font-weight:700;\">wykrył</span> zap\305\202on. Czy Potwierdzasz ?</p></body></html>", nullptr));
    } else {
        ui->text_1->setText(QCoreApplication::translate("NowyTest_7",
    "<html><head/><body><p>Program <span style=\" font-weight:700;\">nie wykrył</span> zap\305\202on. Czy Potwierdzasz ?</p></body></html>", nullptr));
    }

    //ui->pbOK_1->setEnabled(false);
}

NowyTest_7::~NowyTest_7()
{
    delete ui;
}

void NowyTest_7::initializePage()
{
    ui->frame_2->setVisible(false);
    ui->arrow_2->setVisible(true);
    ui->frame_3->setVisible(false);
    ui->arrow_3->setVisible(true);
    ui->frame_4->setVisible(false);
    ui->arrow_4->setVisible(true);
    ui->rb1_yes->setChecked(true);
    ui->pbOK_1->setEnabled(true);
    ui->pbOK_2->setEnabled(true);
    ui->pbOK_3->setEnabled(true);
    ui->pbOk_4->setEnabled(true);
    TestPage::initializePage();
    ui->rb3_zaplon->setEnabled(field(TestPage::rodzajZaplonu).toInt() != z_iskra_plomien);
}

TestPage::PageId NowyTest_7::nextPageId() const
{
    return next;
}

void NowyTest_7::on_pbOK_1_clicked()
{
    setField(bylWybuch, (wykrytyZaplon && rbYes) || (!wykrytyZaplon && !rbYes));
    if ((wykrytyZaplon && rbYes) || (!wykrytyZaplon && !rbYes)) {
        next = TestPage::PAGE_8;
        setZ_criticalMask(i_drzwi_lewe | i_drzwi_prawe | i_proznia | i_wlot_powietrza | i_pom_stez_1 | i_pom_stez_2 | i_wentylacja_prawa | i_wentylacja_lewa);
        QMessageBox::information(this, QString("Kamera"), QString("Pamiętaj o zapisaniu filmu na kartę"));
        testData().setUdanaProba(true);
        setField(TestPage::powtarzanyTest, QVariant::fromValue((bool)false));
        nextPage(next);
    } else if (field(TestPage::rodzajZaplonu).toInt() == z_iskra_plomien) {
        ui->arrow_1->setVisible(false);
        ui->pbOK_1->setEnabled(false);
        ui->frame_2->setVisible(true);
        next = TestPage::PAGE_9;
        setZ_criticalMask(0);
        setZ_warningMask(i_wentylacja_prawa | i_wentylacja_lewa);
        testData().setUdanaProba(false);
        setField(TestPage::powtarzanyTest, QVariant::fromValue((bool)true));
    } else {
        ui->pbOK_1->setEnabled(false);
        ui->arrow_1->setVisible(false);
        ui->frame_3->setVisible(true);
        testData().setUdanaProba(false);
        setField(TestPage::powtarzanyTest, QVariant::fromValue((bool)true));
    }

}

void NowyTest_7::on_rb1_yes_toggled(bool checked)
{
    rbYes = checked;
}

void NowyTest_7::on_rb1_no_toggled(bool checked)
{
    rbYes = !checked;
}

void NowyTest_7::on_pbOK_2_clicked()
{
    if (sprawdzOtwarteZawor2Calowe()) {


        updateOutput(o_wentylator, true);
        nextPage(nextPageId());
    }
}


void NowyTest_7::on_rb3_zaplon_toggled(bool checked)
{
    if (checked) {
        next = TestPage::PAGE_6;
        dozowanieCieczy = false;
    }
}


void NowyTest_7::on_rb3_ciecz_toggled(bool checked)
{
    if (checked) {
        dozowanieCieczy = true;
        next = TestPage::PAGE_4;
    }
}


void NowyTest_7::on_pbOK_3_clicked()
{
    if (!ui->rb3_ciecz->isChecked() && !ui->rb3_zaplon->isChecked()) {
        QMessageBox::information(this, QString("Wybór procedury postępowania"), QString("Nie wybrano żadnej opcji"));
        return;
    }
    if (!dozowanieCieczy) {
        nextPage(nextPageId());
        return;
    }
    ui->arrow_3->setVisible(false);
    ui->pbOK_3->setEnabled(false);
    ui->frame_4->setVisible(true);
}


void NowyTest_7::on_pbOk_4_clicked()
{
    if (ui->cbCiecz->value() == 0) {
        QMessageBox::information(this, QString("Dozowanie cieczy"), QString("Wybierz więcej niż 0ml"));
        return;
     }
    
    setField(TestPage::objetosc, QVariant::fromValue(ui->cbCiecz->value()));
    
    nextPage(nextPageId());
}

