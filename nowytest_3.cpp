#include "nowytest_3.h"
#include "ui_nowytest_3.h"

#include <QCheckBox>
//#include "windows.h"
#include <QTimer>

#include "createtestwizard.h"
#include "testpage.h"

NowyTest_3::NowyTest_3(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_3)
{
    ui->setupUi(this);

    ui->lStep2B->setEnabled(false);

    ui->lstep2A->setEnabled(false);
    ui->pbStep2A_OK->setEnabled(false);
    ui->pbStep2A_Next->setEnabled(false);

    ui->pbStep2B_OK->setEnabled(false);


    ui->pbStep2_Skip->setEnabled(true);
    ui->pbStep2_OK->setEnabled(true);
    ui->lstep2->setEnabled(true);
    ui->podcisnienie->setEnabled(false);
    ui->podcisnienie->setMinimum(0);
    ui->podcisnienie->setMaximum(10000);
}

NowyTest_3::~NowyTest_3()
{
    delete ui;
}

bool NowyTest_3::isComplete() const
{
    if (wizard()->currentPage() != this)
        return true;

    if (!TestPage::isComplete())
        return false;

    return valid;
}

void NowyTest_3::initializePage()
{
    qDebug("initializePage 3");
     setField("Podcisnienie", QVariant::fromValue((int)0));
     valid = false;
     emit completeChanged();
}

void NowyTest_3::on_pbStep2_OK_clicked()
{
    ui->podcisnienie->setEnabled(true);
    ui->lstep2A->setEnabled(true);
    ui->pbStep2A_OK->setEnabled(true);
    ui->pbStep2A_Next->setEnabled(true);

    ui->pbStep2_OK->setEnabled(false);
    ui->pbStep2_Skip->setEnabled(false);
}

void NowyTest_3::on_pbStep2_Skip_clicked()
{
    ui->pbStep2_OK->setEnabled(false);
    ui->pbStep2_Skip->setEnabled(false);

    valid = true;
    emit completeChanged();
}

void NowyTest_3::on_pbStep2A_OK_clicked()
{
    ui->pbStep2A_OK->setEnabled(false);
    ui->pbStep2A_Next->setEnabled(false);
    valid = true;
    emit completeChanged();
}

void NowyTest_3::on_pbStep2A_Next_clicked()
{
    ui->pbStep2A_OK->setEnabled(false);
    ui->pbStep2A_Next->setEnabled(false);
    QTimer::singleShot(1000,this, &NowyTest_3::runDone3);
}

void NowyTest_3::on_pbStep2B_OK_clicked()
{
    ui->pbStep2B_OK->setEnabled(false);
    QTimer::singleShot(1000,this, &NowyTest_3::runDone4);
}

void NowyTest_3::runDone3()
{
    ui->pbStep2A_OK->setEnabled(true);
    ui->pbStep2A_Next->setEnabled(true);
}

void NowyTest_3::runDone4()
{
    //ui->pbStep2B_OK->setEnabled(true);
    valid = true;
    emit completeChanged();
}

void NowyTest_3::on_podcisnienie_valueChanged(int arg1)
{
    setField("Podcisnienie", QVariant::fromValue((int)arg1));
}
