#include "nowytest_8.h"
#include "ui_nowytest_8.h"
#include "createtestwizard.h"

#include <QTimer>
#include <QMessageBox>

NowyTest_8::NowyTest_8(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_8)
{
    ui->setupUi(this);
}

NowyTest_8::~NowyTest_8()
{
    delete ui;
}

void NowyTest_8::initializePage()
{
    ui->frame_2->setVisible(false);
    ui->frame_3->setVisible(false);
    ui->arrow_1->setVisible(true);
    ui->arrow_2->setVisible(true);
    ui->pbOK_1->setEnabled(true);
    ui->pbOK_2->setEnabled(true);
    ui->pbOK_3->setEnabled(true);
    //emit pomiary(true);
}

void NowyTest_8::runDone()
{
    ui->pbOK_3->setEnabled(true);
    updateOutput(pompa_powietrza, false);
    //ui->pbStep2->setEnabled(true);
    //ui->pbStep2->setDone(true);
    //ui->lStep3->setEnabled(true);
    //ui->pbStep3->setEnabled(true);
}

void NowyTest_8::on_pbOK_1_clicked()
{
    sprawdzZawory(ui->pbOK_1, ui->arrow_1, ui->frame_2);
}


void NowyTest_8::on_pbOK_2_clicked()
{
    if (!sprawdzOtwarteZaworStezenia())
        return;

    ui->pbOK_2->setEnabled(false);
    ui->arrow_2->setVisible(false);
    ui->frame_3->setVisible(true);
    ui->pbOK_3->setEnabled(false);

    updateOutput(pompa_powietrza, true);
    //emit pomiary(true);
    //emit pompaMembramowa(true);
    //emit mieszadlo(true);
    QTimer::singleShot(5000, this, &NowyTest_8::runDone);
}


void NowyTest_8::on_pbOK_3_clicked()
{
    if (!sprawdzOtwarteZaworStezenia())
        return;
    nextPage(nextPageId());
}

