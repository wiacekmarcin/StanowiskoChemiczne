#include "nowytest_6.h"
#include "ui_nowytest_6.h"
#include "createtestwizard.h"
#include "testpage.h"
#include "oczekiwanienazaplon.h"
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
    QString zaplon = field(TestPage::zaplon).toString();
    QString info("Wciśnięcie przycisku zdalnego sterowania spowoduje uruchomienie : ");
    info += zaplon;
    info += ".";
    ui->lStep2->setText(info);

    ui->frame_2->setVisible(false);
    ui->arrow_1->setVisible(true);
    ui->pbOK_1->setEnabled(true);
    runDialog = false;
    step2 = false;

    setZ_warningMask(0);
}

void NowyTest_6::updateWejscia()
{
    //qDebug"%s%d %d",__FILE__,__LINE__, z_pilot());
    if (step2 && zi_pilot() && !runDialog) {
        runZaplon(field(TestPage::rodzajZaplonu).toInt());
        //qDebug"%s%d",__FILE__,__LINE__);
        runDialog = true;
        //qDebug"%s%d",__FILE__,__LINE__);
        OczekiwanieNaZaplon * dlg = new OczekiwanieNaZaplon(this);
        //qDebug"%s%d",__FILE__,__LINE__);
        dlg->exec();
        //qDebug"%s%d",__FILE__,__LINE__);
        delete dlg;
        nextPage(nextPageId());
    }
}

void NowyTest_6::on_pushButton_clicked()
{
    OczekiwanieNaZaplon * dlg = new OczekiwanieNaZaplon(this);
    dlg->exec();
    delete dlg;
    nextPage(nextPageId());
}


void NowyTest_6::on_pbOK_1_clicked()
{
    if (sprawdzZawory(ui->pbOK_1, ui->arrow_1, ui->frame_2)) {
        step2 = true;
        updateOutput(o_mieszadlo, false);
        setZ_criticalMask(i_drzwi_lewe | i_drzwi_prawe | i_wentylacja_prawa | i_wentylacja_lewa | i_proznia | i_wlot_powietrza | i_pom_stez_1 | i_pom_stez_2);
    }
}

