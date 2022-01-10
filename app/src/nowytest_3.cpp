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

/*zawor prozni */
    ui->lstep2A->setEnabled(false);
    ui->pbStep2A_OK->setEnabled(false);
/*pompa*/
    ui->pbStep2B2_OK->setEnabled(false);
    ui->lStep2B2->setEnabled(false);
/*podcienienie */
    ui->podcisnienie->setEnabled(false);
    ui->lStep2B1->setEnabled(false);
    ui->lStep2B1a->setEnabled(false);


    ui->pbStep2_Skip->setEnabled(true);
    ui->pbStep2_OK->setEnabled(true);
    ui->lstep2->setEnabled(true);
    ui->pb100mBar->setEnabled(true);
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
     setField("Podcisnienie", QVariant::fromValue((int)0));
     setField("zaworPompy", QVariant::fromValue((bool)false));
     valid = false;
     emit completeChanged();
}

void NowyTest_3::on_pbStep2_OK_clicked()
{
    ui->pbStep2_OK->setDone(true);
    ui->lstep2A->setEnabled(true);
    ui->pbStep2A_OK->setEnabled(true);

    ui->pbStep2_OK->setDone(true);
    ui->pbStep2_Skip->setEnabled(false);
    ui->pb100mBar->setEnabled(false);

}

void NowyTest_3::runDone2()
{
    ui->pbStep2A_OK->setEnabled(true);
    ui->pbStep2A_OK->setDone(true);
    valid = true;
    emit zaworProzni(false);
    emit completeChanged();
}

void NowyTest_3::on_pbStep2_Skip_clicked()
{
    ui->pbStep2_OK->setEnabled(false);
    ui->pbStep2_Skip->setDone(true);
    ui->pb100mBar->setEnabled(false);

    valid = true;
    emit completeChanged();
}

void NowyTest_3::on_pbStep2A_OK_clicked()
{
    ui->pbStep2A_OK->setEnabled(false);
    emit zaworProzni(true);
    QTimer::singleShot(1000,this, &NowyTest_3::runDone2);
}

void NowyTest_3::runDone3()
{
    valid = true;
    emit completeChanged();
}

void NowyTest_3::on_podcisnienie_valueChanged(int arg1)
{
    setField("Podcisnienie", QVariant::fromValue((int)arg1));
}

void NowyTest_3::on_pb100mBar_clicked()
{
    ui->pbStep2_OK->setEnabled(false);
    ui->pbStep2_Skip->setEnabled(false);
    ui->podcisnienie->setEnabled(true);
    ui->lStep2B1->setEnabled(true);
    ui->lStep2B1a->setEnabled(true);
    ui->lStep2B2->setEnabled(true);
    ui->pb100mBar->setDone(true);
    ui->pbStep2B2_OK->setEnabled(true);
}

void NowyTest_3::on_pbStep2B2_OK_clicked()
{
    ui->pbStep2B2_OK->setDone(true);
    ui->podcisnienie->setReadOnly(true);
    QTimer::singleShot(3000,this, &NowyTest_3::runDone3);
    setField("zaworPompy", QVariant::fromValue((bool)true));
    emit pompaProzniowa(true);
}
