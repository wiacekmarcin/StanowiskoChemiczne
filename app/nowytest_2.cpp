#include "nowytest_2.h"
#include "ui_nowytest_2.h"
#include "createtestwizard.h"
#include "urzadzenia.h"
#include "ustawienia.h"

#include <QTimer>
#include <QMessageBox>

NowyTest_2::NowyTest_2(unsigned short initC, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_2),
    initCykle(initC)
{
    ui->setupUi(this);
    ui->txt_1->setText(QString("Czy układ dozownika <DOZOWNIK> jest napełniony ?"));
    ui->text_3->setText(QString("Rozpocznij napełnianie układu dozownika <DOZOWNIK>"));


 }

NowyTest_2::~NowyTest_2()
{
    delete ui;
}

void NowyTest_2::initializePage()
{
    m_DozownikPelny = false;
    pojedynczyCykl = false;
    showWarning = false;
    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
    ui->frame_4->setVisible(false);
    ui->frame_5->setVisible(false);

    ui->rb1_yes->setChecked(true);
    ui->rb4_yes->setChecked(true);
    dozownik = field(dozownikNr).toUInt()-1;
    ui->txt_1->setText(QString("Czy układ dozownika <DOZOWNIK> jest napełniony ?").replace("<DOZOWNIK>", QString::number(dozownik+1)));
    ui->text_3->setText(QString("Rozpocznij napełnianie układu dozownika <DOZOWNIK>").replace("<DOZOWNIK>", QString::number(dozownik+1)));

}

void NowyTest_2::updateWejscia()
{
    //qDebug("%s:%d",__FILE__,__LINE__);
    if (showWarning) {
        if (z_drzwi_lewe() && z_drzwi_prawe()) {
            showWarning = false;
            QMessageBox::warning(this, tr("Komora"),
                                           tr("Zamknięto komorę.\n"
                                              "Otwórz komorę aby kontynuować"),
                                           QMessageBox::Ok);
            showWarning = true;
        }
    }
}





void NowyTest_2::dozownikDone(bool succes)
{
    if (!succes) {
        int ret = QMessageBox::warning(this, tr("Dozownik"),
                                       tr("Nie udało się zadozować cieczy.\n"
                                          "Czy chcesz kontynuować?"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);
        if (ret == QMessageBox::No) {
            setFinished(false);
            return;
        }
    }
    if (pojedynczyCykl) {
        m_DozownikPelny = true;
        ui->pbOk_4->setEnabled(true);
        ui->rb4_no->setChecked(false);
        ui->rb4_yes->setChecked(true);

    } else {
        pojedynczyCykl = true;
        ui->arrow_3->setVisible(false);
        ui->frame_4->setVisible(true);
    }
}

void NowyTest_2::runDone()
{
    dozownikDone(true);
}

//nowe
void NowyTest_2::on_rb1_yes_toggled(bool checked)
{
    m_DozownikPelny = checked;
}

void NowyTest_2::on_rb1_no_toggled(bool checked)
{
    m_DozownikPelny = !checked;
}

void NowyTest_2::on_pbOk_1_clicked()
{
    if (m_DozownikPelny) {
        nextPage(nextPageId());
    } else {
        ui->pbOk_1->setEnabled(false);
        ui->arrow_1->setVisible(false);
        ui->frame_2->setVisible(true);
        ui->rb1_no->setEnabled(false);
        ui->rb1_yes->setEnabled(false);
    }
}

void NowyTest_2::on_pbOK_2_clicked()
{
    ui->pbOK_2->setEnabled(false);
    showWarning = false;
    while (z_drzwi_lewe() && z_drzwi_prawe()) {
        int ret = QMessageBox::warning(this, tr("Dozownik"),
                                       tr("Nie wykryto otwartych drzwiczek.\n"
                                          "Czy chcesz kontynuować test otwierając drzwi - Tak, przerwij - Test Nie?"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);
        if (ret == QMessageBox::No) {
            setFinished(false);
            return;
        }
    }
    showWarning = true;

    ui->arrow_2->setVisible(false);
    ui->frame_3->setVisible(true);
}

void NowyTest_2::on_pbOk_3_clicked()
{
    qDebug("%s:%d runCycle", __FILE__,__LINE__);
    ui->pbOk_3->setEnabled(false);
    m_DozownikPelny = true;
    //emit cykleDozownik(dozownik, 4);
    QTimer::singleShot(1000, this, &NowyTest_2::runDone);

}

void NowyTest_2::on_rb4_yes_toggled(bool checked)
{
    qDebug("m_DozownikPelny");
    m_DozownikPelny = checked;
}

void NowyTest_2::on_rb4_no_toggled(bool checked)
{
    m_DozownikPelny = !checked;
}

void NowyTest_2::on_pbOk_4_clicked()
{
    if (!m_DozownikPelny) {
        qDebug("%s:%d runCycle", __FILE__,__LINE__);
        ui->pbOk_4->setEnabled(false);
        //emit cykleDozownik(dozownik, 1);
        QTimer::singleShot(1000, this, &NowyTest_2::runDone);
    } else {
        ui->pbOk_4->setEnabled(false);
        ui->arrow_4->setVisible(false);
        ui->frame_5->setVisible(true);
        ui->rb4_no->setEnabled(false);
        ui->rb4_yes->setEnabled(false);
    }
}


void NowyTest_2::on_pbOk_5_clicked()
{
    ui->pbOk_5->setEnabled(false);
    showWarning = false;
    while (!z_drzwi_lewe() || !z_drzwi_prawe()) {
        int ret = QMessageBox::warning(this, tr("Dozownik"),
                                       tr("Wykryto otwarte drzwi komory.\n"
                                          "Zamknij je w celu kontynuacji. Zamknięto drzwi OK, przewanie testu Anuluj"),
                                       QMessageBox::Ok | QMessageBox::Abort | QMessageBox::Cancel,
                                       QMessageBox::Ok);
        if (ret == QMessageBox::Abort) {
            setFinished(false);
            return;
        } else if (ret == QMessageBox::Cancel)
            break;
    }
    nextPage(nextPageId());
}

