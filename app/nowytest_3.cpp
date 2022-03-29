#include "nowytest_3.h"
#include "ui_nowytest_3.h"
#include <QMutexLocker>

#include <QCheckBox>
//#include "windows.h"
#include <QTimer>

#include "createtestwizard.h"
#include "testpage.h"
#include "ustawienia.h"

NowyTest_3::NowyTest_3(double cisnienie, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_3),
    cisnieWProzni(cisnienie)
{
    ui->setupUi(this);
    task = 0;

/*zawor prozni */
    ui->lstep2A->setEnabled(false);
    ui->pbProzniaDone->setEnabled(false);
/*pompa*/
    ui->pbStep2B2_OK->setEnabled(false);
    ui->lStep2B2->setEnabled(false);
/*podcienienie */
    ui->podcisnienie->setEnabled(false);
    ui->lStep2B1->setEnabled(false);
    ui->lStep2B1a->setEnabled(false);


    ui->pbSetSkip->setEnabled(true);
    ui->pbSetProznia->setEnabled(true);
    ui->lstep2->setEnabled(true);
    ui->pb100mBar->setEnabled(true);

    prozniaTimer.setInterval(100);
    connect(&prozniaTimer, &QTimer::timeout, this, &NowyTest_3::updateProznia);
}

NowyTest_3::~NowyTest_3()
{
    prozniaTimer.stop();
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
     prozniaTask = false;
     emit completeChanged();
}

void NowyTest_3::openZawor(unsigned int id, bool val)
{
    if (id == proznia) {
        zaworProzni = val;
        if (prozniaTask && !val) {
            ui->pbProzniaDone->setEnabled(true);
        }
    } else if (id == wlot_powietrza) {
        zaworPowietrza = val;
    }
}



void NowyTest_3::cisnienieKomory(double val)
{
    qDebug("%s:%d val =%f", __FILE__,__LINE__, val);
    setCisnKomory(val);
}

void NowyTest_3::on_pbSetProznia_clicked()
{
    ui->pbSetProznia->setDone(true);
    ui->lstep2A->setEnabled(true);
    //ui->pbProzniaDone->setEnabled(true);
    prozniaTask = true;

    ui->pbSetProznia->setDone(true);
    ui->pbSetSkip->setEnabled(false);
    ui->pb100mBar->setEnabled(false);
    prozniaTimer.start();

}

void NowyTest_3::runDone2()
{
    ui->pbProzniaDone->setEnabled(true);
    ui->pbProzniaDone->setDone(true);
    valid = true;
    //emit zaworProzni(false);
    emit completeChanged();
}

void NowyTest_3::on_pbSetSkip_clicked()
{
    ui->pbSetProznia->setEnabled(false);
    ui->pbSetSkip->setDone(true);
    ui->pb100mBar->setEnabled(false);

    valid = true;
    emit completeChanged();
}

void NowyTest_3::on_pbProzniaDone_clicked()
{
    ui->pbProzniaDone->setEnabled(false);
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
    ui->pbSetProznia->setEnabled(false);
    ui->pbSetSkip->setEnabled(false);
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

void NowyTest_3::updateProznia()
{
    double avg = getAvgCisnienie();
    double val = getCisnKomory();

    qDebug("%s:%d avg = %f cisn = %f  [%f] <%f %f>",__FILE__, __LINE__, avg, val, cisnieWProzni, 0.95*val, 1.05*val);
    if (0.95*val < avg && 1.05*avg > val && avg < cisnieWProzni) {
        ui->pbProzniaDone->setEnabled(true);
        ui->pbProzniaDone->setDone(true);
        valid = true;
        prozniaTimer.stop();
        emit completeChanged();

    }
}

double NowyTest_3::getCisnKomory()
{
    QMutexLocker lock(&mutexCisnienie);
    return cisnKomory;
}

double NowyTest_3::getAvgCisnienie()
{
    QMutexLocker lock(&mutexCisnienie);
    double avg = 0.0;
    /*
    qDebug("%s:%d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
           __FILE__, __LINE__, prevCisnienie[0], prevCisnienie[1],
           prevCisnienie[3], prevCisnienie[4], prevCisnienie[5],
           prevCisnienie[6], prevCisnienie[7], prevCisnienie[8],
           prevCisnienie[10],prevCisnienie[11], prevCisnienie[12],
           prevCisnienie[13], prevCisnienie[14],prevCisnienie[15]);
    */
    for (unsigned short id = 0; id < 16 ; ++id) {
        avg += prevCisnienie[id];
    }
    qDebug("%s:%d avg=%f", __FILE__, __LINE__, avg);
    return avg/16.0;
}
void NowyTest_3::setCisnKomory(double newCisnKomory)
{
    QMutexLocker lock(&mutexCisnienie);
    cisnKomory = newCisnKomory;
    idPrev = (idPrev + 1) & 0xf;
    //qDebug("%s:%d idprev = %d", __FILE__, __LINE__, idPrev);
    prevCisnienie[idPrev] = newCisnKomory;
    /*
    qDebug("%s:%d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
           __FILE__, __LINE__, prevCisnienie[0], prevCisnienie[1],
           prevCisnienie[3], prevCisnienie[4], prevCisnienie[5],
           prevCisnienie[6], prevCisnienie[7], prevCisnienie[8],
           prevCisnienie[10],prevCisnienie[11], prevCisnienie[12],
           prevCisnienie[13], prevCisnienie[14],prevCisnienie[15]);
           */
}
