#include "nowytest_5.h"
#include "ui_nowytest_5.h"
#include "createtestwizard.h"

#include <QTimer>
#include <QMessageBox>

NowyTest_5::NowyTest_5(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_5)
{
    ui->setupUi(this);

    valid = false;
}

NowyTest_5::~NowyTest_5()
{
    delete ui;
}

bool NowyTest_5::isComplete() const
{
    if (wizard()->currentPage() != this)
        return true;

    if (!TestPage::isComplete())
        return false;

    return valid;
}

void NowyTest_5::initializePage()
{
    valid = false;
    emit completeChanged();
    ui->frame_2->setVisible(false);
    ui->arrow_1->setVisible(true);
    ui->pbOK_1->setEnabled(true);
    ui->pbOK_2->setEnabled(true);
    //emit pomiary(true);
}

void NowyTest_5::runDone()
{
    ui->pbOK_2->setEnabled(true);
    //ui->pbStep2->setEnabled(true);
    //ui->pbStep2->setDone(true);
    //ui->lStep3->setEnabled(true);
    //ui->pbStep3->setEnabled(true);
}


void NowyTest_5::on_pbOK_1_clicked()
{
    ui->pbOK_1->setEnabled(false);
    if (!isZamknietyZawor()) {
            QMessageBox msgBox;
            msgBox.setText("Wykryto nie prawidłowe ustawienie zaworów.");
            msgBox.setInformativeText("Wszystkie zawory X i Y powinny byc otwarte. \n Czy chcesz kontynuować");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();
            if (ret == QMessageBox::No)
                return;
    }
    ui->arrow_1->setVisible(false);
    ui->frame_2->setVisible(true);
    ui->pbOK_2->setEnabled(false);

    //emit pomiary(true);
    //emit pompaMembramowa(true);
    //emit mieszadlo(true);
    QTimer::singleShot(2000, this, &NowyTest_5::runDone);
}


void NowyTest_5::on_pbOK_2_clicked()
{
    nextPage(nextPageId());
}

