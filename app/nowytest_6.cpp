#include "nowytest_6.h"
#include "ui_nowytest_6.h"
#include "createtestwizard.h"
#include "testpage.h"
#include "oczekiwanienazaplon.h"
#include "ustawienia.h"

NowyTest_6::NowyTest_6(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_6)
{
    ui->setupUi(this);

}

NowyTest_6::~NowyTest_6()
{
    delete ui;
}

void NowyTest_6::initializePage()
{

    ui->cbZaplon_2->clear();
    ui->cbZaplon_2->addItem("--");
    ui->cbZaplon_2->addItem(ISKRA_ELE);
    ui->cbZaplon_2->addItem(ISKRA_MECH);
    if (field(TestPage::wybranyPlomien).toBool()) {
        ui->cbZaplon_2->addItem(PLOMIEN);
    }

    ui->frame_2->setVisible(false);
    ui->arrow_1->setVisible(true);
    ui->pbOK_1->setEnabled(true);
    ui->pbOK_2->setEnabled(true);
    ui->frame_3->setVisible(false);
    ui->arriw_2->setVisible(true);
    runDialog = false;
    step2 = false;

    setZ_warningMask(0);
    setZ_criticalMask(i_drzwi_lewe | i_drzwi_prawe | i_wentylacja_prawa | i_wentylacja_lewa | i_proznia | i_wlot_powietrza | i_pom_stez_1 | i_pom_stez_2);
}

void NowyTest_6::updateWejscia()
{
    
    if (step2 && zi_pilot() && !runDialog) {
        runZaplon((ZaplonRodzaj)(field(TestPage::rodzajZaplonu).toInt()));
        
        runDialog = true;
        
        OczekiwanieNaZaplon * dlg = new OczekiwanieNaZaplon(this, wizard(),
                                    getCzujnik(a_temp_komory), getCzujnik(a_cisn_komora));
        
        dlg->exec();
        
        delete dlg;
        nextPage(nextPageId());
    }
}

void NowyTest_6::on_pbOK_1_clicked()
{
    if (sprawdzZawory(ui->pbOK_1, ui->arrow_1, ui->frame_2)) {
        ui->arrow_1->setVisible(false);
        ui->frame_2->setVisible(true);
        ui->pbOK_2->setEnabled(false);
    }
}


void NowyTest_6::on_cbZaplon_2_currentTextChanged(const QString &arg1)
{
    ui->pbOK_2->setEnabled(arg1 != "--");
}


void NowyTest_6::on_pbOK_2_clicked()
{
    step2 = true;
    updateOutput(o_mieszadlo, false);
    setField(zaplon, QVariant::fromValue(ui->cbZaplon_2->currentText()));
    TestData().setZrodloZaplonu(ui->cbZaplon_2->currentText());
    ui->arriw_2->setVisible(false);
    ui->frame_3->setVisible(true);

    QString info("Wciśnięcie przycisku zdalnego sterowania spowoduje uruchomienie : ");
    info += ui->cbZaplon_2->currentText() + QString(" .");
    ui->lStep3->setText(info);
}

