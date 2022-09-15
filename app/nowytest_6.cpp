#include "nowytest_6.h"
#include "ui_nowytest_6.h"
#include "createtestwizard.h"
#include "testpage.h"
#include "oczekiwanienazaplon.h"
#include "ustawienia.h"

NowyTest_6::NowyTest_6(const UEkran6 &ust_, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_6),
    ust(ust_)
{
    ui->setupUi(this);
    connect(this, &NowyTest_6::showDglSignal, this, &NowyTest_6::showDlgSlot, Qt::QueuedConnection);
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

    if (!field(TestPage::wybranyPlomien).toBool()) {
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
    setZ_criticalMask(i_drzwi_lewe | i_drzwi_prawe | i_wentylacja_prawa | i_wentylacja_lewa | i_proznia | i_wlot_powietrza );
}

void NowyTest_6::updateWejscia()
{
    
    if (step2 && zi_pilot() && !runDialog) {
        setField(TestPage::rodzajZaplonu, ui->cbZaplon_2->currentIndex());
        if (ui->cbZaplon_2->currentIndex() == z_iskra_plomien)
            setField(TestPage::wybranyPlomien, true);
        runZaplon((ZaplonRodzaj)(field(TestPage::rodzajZaplonu).toInt()));
        runDialog = true;
        emit showDglSignal();
        wizard()->setDebug(QString("PAGEG:ZAPLON"));
    }
}

void NowyTest_6::on_pbOK_1_clicked()
{
    wizard()->setDebug(QString("PAGE6:OK1"));
    if (sprawdzZawory(ui->pbOK_1, ui->arrow_1, ui->frame_2)) {
        ui->arrow_1->setVisible(false);
        ui->frame_2->setVisible(true);
        ui->pbOK_2->setEnabled(false);
        setZ_criticalMask(i_drzwi_lewe | i_drzwi_prawe | i_wentylacja_prawa | i_wentylacja_lewa | i_proznia | i_wlot_powietrza | i_pom_stez_1 | i_pom_stez_2);
    }
}


void NowyTest_6::on_cbZaplon_2_currentTextChanged(const QString &arg1)
{
    ui->pbOK_2->setEnabled(arg1 != "--");
}


void NowyTest_6::on_pbOK_2_clicked()
{
    wizard()->setDebug(QString("PAGE6:OK2"));
    step2 = true;
    ui->pbOK_2->setEnabled(false);
    updateOutput(o_mieszadlo, false);
    qInfo() << "Zapis do fielda:" << ui->cbZaplon_2->currentIndex();
    setField(rodzajZaplonu, QVariant::fromValue(ui->cbZaplon_2->currentIndex()));
    setField(zaplon, QVariant::fromValue(ui->cbZaplon_2->currentText()));
    getTestData()->setZrodloZaplonu(ui->cbZaplon_2->currentText());
    if (!field(TestPage::wybranyPlomien).toBool() && ui->cbZaplon_2->currentIndex() == z_iskra_plomien)
        setField(TestPage::wybranyPlomien, true);
    ui->arriw_2->setVisible(false);
    ui->frame_3->setVisible(true);

    QString info("Wciśnięcie przycisku zdalnego sterowania spowoduje uruchomienie : ");
    info += ui->cbZaplon_2->currentText() + QString(" .");
    ui->lStep3->setText(info);

#ifdef SYMULATOR
    setField(TestPage::rodzajZaplonu, ui->cbZaplon_2->currentIndex());
    if (ui->cbZaplon_2->currentIndex() == z_iskra_plomien)
        setField(TestPage::wybranyPlomien, true);
    runZaplon((ZaplonRodzaj)(field(TestPage::rodzajZaplonu).toInt()));
    runDialog = true;
    emit showDglSignal();
    wizard()->setDebug(QString("PAGEG:ZAPLON"));
#endif
}

void NowyTest_6::showDlgSlot()
{
    float tmp = getCzujnik(a_temp_komory);
    float cisn = getCzujnik(a_cisn_komora);
    OczekiwanieNaZaplon * dlg = new OczekiwanieNaZaplon(this, wizard(),
                                tmp, cisn, ui->cbZaplon_2->currentIndex() == z_iskra_plomien, ust);

    bool wasZaplon = dlg->exec() == QDialog::Accepted;
    qDebug() << "exec " << wasZaplon;
    setField(TestPage::bylWybuch, wasZaplon);
    delete dlg;
    runDialog = false;
    nextPage(nextPageId());
}

