#include "nowytest_7.h"
#include "ui_nowytest_7.h"

#include "urzadzenia.h"
#include <QMessageBox>

NowyTest_7::NowyTest_7(Urzadzenia * /*u*/, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_7)
{
    ui->setupUi(this);
    initial = false;
    next = PAGE_8;
    wykrytyZaplon = true;

    if (wykrytyZaplon)
        ui->text_1->setText(QCoreApplication::translate("NowyTest_7",
   "<html><head/><body><p>Program <span style=\" font-weight:700;\">wykrył</span> zap\305\202on. Czy Potwierdzasz ?</p></body></html>", nullptr));
    else
        ui->text_1->setText(QCoreApplication::translate("NowyTest_7",
    "<html><head/><body><p>Program <span style=\" font-weight:700;\">nie wykrył</span> zap\305\202on. Czy Potwierdzasz ?</p></body></html>", nullptr));


    //ui->pbOK_1->setEnabled(false);
}

NowyTest_7::~NowyTest_7()
{
    delete ui;
}

void NowyTest_7::initializePage()
{
    ui->rb1_yes->setChecked(true);
    ui->frame_1->setVisible(false);
    TestPage::initializePage();

    valid = false;
    emit completeChanged();
}

bool NowyTest_7::isComplete() const
{
    return TestPage::isComplete() && valid;
}

TestPage::PageId NowyTest_7::nextPageId() const
{
    return next;
}

void NowyTest_7::changeData()
{
    qDebug("%s:%d %d-%d", __FILE__, __LINE__, b_wentylacja_prawa, b_wentylacja_lewa);
}


void NowyTest_7::on_pbOK_1_clicked()
{
    if ((wykrytyZaplon && rbYes) || (!wykrytyZaplon && !rbYes)) {
        nextPage(nextPageId());
        ui->arrow_1->setVisible(false);
        ui->pbOK_1->setEnabled(false);
        ui->frame_2->setVisible(true);
    } else {
        ui->arrow_1->setVisible(false);
        ui->frame_3->setVisible(true);
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

void NowyTest_7::on_pbOK_2yes_clicked()
{

   while (!(!b_wentylacja_lewa && !b_wentylacja_prawa &&
          b_drzwi_prawe && b_proznia && b_pom_stez_1 && b_drzwi_lewe &&
          b_wlot_powietrza && b_pom_stez_2)) {
       qDebug("--------------%d %d", b_wentylacja_lewa, wentylacja_prawa);
        QMessageBox msgBox;
        msgBox.setText("Wykryto nie prawidłowe ustawienie zaworów.");
        msgBox.setInformativeText("Zawory A i B powinny być otwarte a pozostałe zawory powinny być zamknięte.\n Czy chcesz kontynouwac?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if (ret == QMessageBox::No){
            setFinished(false);
            return;
        } else if (ret == QMessageBox::Cancel)
            break;

   }
   //emit zalacz wentylator
   nextPage(nextPageId());
}







void NowyTest_7::on_pBNo_3_clicked()
{
    nextPage(nextPageId());
}


void NowyTest_7::on_pbYes_3_clicked()
{
    ui->pbYes_3->setEnabled(false);
    ui->pBNo_3->setEnabled(false);
    ui->arrow_3->setVisible(false);
    ui->frame_4->setEnabled(true);
}

