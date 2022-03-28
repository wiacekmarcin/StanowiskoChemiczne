#include "nowytest_2.h"
#include "ui_nowytest_2.h"
#include "createtestwizard.h"
#include <QTimer>
#include "urzadzenia.h"
#include "ustawienia.h"

NowyTest_2::NowyTest_2(Urzadzenia * u, unsigned short initC, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_2),
    initCykle(initC)
{
    ui->setupUi(this);
    ign = false;
    czyZamkKom = false;
    dozownikPodParow = true;
    showPb6Ok = false;

    connect(this, &NowyTest_2::cykleDozownik, u, &Urzadzenia::setCykle);
    connect(u, &Urzadzenia::setCykleDone, this, &NowyTest_2::dozownikDone);

 }

NowyTest_2::~NowyTest_2()
{
    delete ui;
}

void NowyTest_2::initializePage()
{
    ign = true;
    dozownikNr = field("dozownik").toUInt()-1;
    ui->lQuestion1->setText(QString("Czy uład dozownika %1 jest napełniony").arg(dozownikNr+1));
    ui->lQuestion3->setText(QString("Czy rozpocząć napełnianie układu dozownika %1").arg(dozownikNr+1));
    ui->l1Nie->setVisible(false);
    ui->cbFull1No->setEnabled(true);
    ui->cbFull1Yes->setEnabled(true);
    ui->cbFull1No->setChecked(false);
    ui->cbFull1Yes->setChecked(false);

    isInitFiling(false);

    ui->lQuestion3->setEnabled(false);
    ui->pb3Run->setEnabled(false);
    ui->lQuestion4->setEnabled(false);
    ui->cbFull4No->setChecked(false);
    ui->cbFull4Yes->setChecked(false);
    ui->cbFull4No->setEnabled(false);
    ui->cbFull4Yes->setEnabled(false);
    ui->l4No->setVisible(false);
    ui->l4Yes->setVisible(false);
    ui->pb5Full->setEnabled(false);
    ui->pb5Run->setEnabled(false);
    ui->lQuestion6->setEnabled(false);
    ui->pb6Ok->setEnabled(false);

    ign = false;
    dozownikFull = false;
    emit completeChanged();
}

void NowyTest_2::isInitFiling(bool yes)
{
    ui->frame->setEnabled(yes);
    ui->lQuestion2->setEnabled(yes);
    ui->pbQuestion2->setEnabled(yes);
}

bool NowyTest_2::isComplete() const
{
    if (wizard()->currentPage() != this)
        return false;

    if (!TestPage::isComplete())
        return false;

    if (!dozownikFull)
        return false;

    if (!dozownikPodParow)
        return false;

    if (!czyZamkKom)
        return false;

    return true;
}

void NowyTest_2::on_cbFull1Yes_toggled(bool checked)
{
    if (ign)
        return;
    ui->cbFull1No->setChecked(!checked);
    ui->frame->setEnabled(!checked);
    isInitFiling(!checked);
    dozownikFull = true;
    dozownikPodParow = checked;
    emit completeChanged();
}

void NowyTest_2::on_cbFull1No_toggled(bool checked)
{
    if (ign)
        return;
    ui->cbFull1Yes->setChecked(!checked);
    ui->frame->setEnabled(checked);
    isInitFiling(checked);
    dozownikFull = false;
    dozownikPodParow = !checked;
    emit completeChanged();
}

void NowyTest_2::on_pbQuestion2_clicked()
{
    ui->cbFull1No->setEnabled(false);
    ui->cbFull1No->setText("");
    ui->l1Nie->setVisible(true);
    ui->cbFull1Yes->setEnabled(false);
    ui->pbQuestion2->setEnabled(true);
    ui->pbQuestion2->setDone(true);

    ui->lQuestion3->setEnabled(true);
    ui->dozowniknr_2->setEnabled(true);
    ui->pb3Run->setEnabled(true);
    showPb6Ok = false;
}



void NowyTest_2::on_pb3Run_clicked()
{
    dozownikPodParow = false;
    tempFiling = 1;
    qDebug("%s:%d runCycle", __FILE__,__LINE__);
    ui->pb3Run->setEnabled(false);
    //emit cykleDozownik(dozownikNr, initCykle);
    QTimer::singleShot(1000, this, &NowyTest_2::runDone1);


}

void NowyTest_2::dozownikDone(bool succes)
{
    if (succes) {
        if (tempFiling == 1) {
            ui->lQuestion4->setEnabled(true);
            ui->cbFull4No->setEnabled(true);
            ui->cbFull4Yes->setEnabled(true);
            ui->pb3Run->setDone(true);
        } else {
            ui->pb5Run->setEnabled(true);
            ui->pb5Full->setEnabled(true);
        }
    } else {
        ui->pb3Run->setEnabled(true);
    }
}

void NowyTest_2::zamknietaKomora(bool komora)
{
    czyZamkKom = komora;
    ui->pb6Ok->setEnabled(showPb6Ok && czyZamkKom);

    emit completeChanged();

}

void NowyTest_2::isFilling4(bool full)
{
    ui->pb5Full->setEnabled(full);
    ui->pb5Run->setEnabled(full);
}

void NowyTest_2::runDone1()
{
    dozownikDone(true);
}

void NowyTest_2::runDone2()
{
    dozownikDone(true);
}

void NowyTest_2::on_cbFull4Yes_toggled(bool checked)
{
    if (ign)
        return;
    ui->cbFull4No->setChecked(!checked);
    isFilling4(!checked);
    dozownikFull = true;
    if (checked) {
        ui->lQuestion6->setEnabled(true);
        showPb6Ok = true;
        ui->pb6Ok->setEnabled(czyZamkKom);
    } else {
        ui->lQuestion6->setEnabled(false);
        showPb6Ok = false;
        ui->pb6Ok->setEnabled(false);
    }
    emit completeChanged();
}

void NowyTest_2::on_cbFull4No_toggled(bool checked)
{
    if (ign)
        return;
    ui->cbFull4Yes->setChecked(!checked);
    isFilling4(checked);
    dozownikFull = false;
    emit completeChanged();
}

void NowyTest_2::on_pb5Run_clicked()
{
    ui->cbFull4No->setEnabled(false);
    ui->cbFull4No->setText("");
    ui->l4No->setVisible(true);
    ui->cbFull4Yes->setEnabled(false);
    ui->cbFull4Yes->setVisible(false);
    ui->l4Yes->setVisible(false);

    tempFiling = 2;
    ui->pb5Run->setEnabled(false);
    ui->pb5Full->setEnabled(false);
    //emit cykleDozownik(dozownikNr, 1);
    QTimer::singleShot(1000, this, &NowyTest_2::runDone2);

}

void NowyTest_2::on_pb5Full_clicked()
{
    ui->cbFull4No->setEnabled(false);
    ui->cbFull4No->setText("");
    ui->l4No->setVisible(true);
    ui->pb5Run->setEnabled(false);
    ui->pb5Full->setEnabled(false);
    dozownikFull = true;

    ui->lQuestion6->setEnabled(true);
    ui->pb6Ok->setEnabled(czyZamkKom);
}



void NowyTest_2::on_pb6Ok_clicked()
{
    ui->cbFull4No->setEnabled(false);
    ui->cbFull4No->setText("");
    ui->cbFull4Yes->setEnabled(false);
    ui->cbFull4Yes->setText("");
    ui->l4Yes->setEnabled(false);
    ui->l4Yes->setVisible(true);
    ui->pb6Ok->setDone(true);
    dozownikFull = true;
    dozownikPodParow = true;
    emit completeChanged();
}



