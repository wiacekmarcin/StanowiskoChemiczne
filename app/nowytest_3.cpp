#include "nowytest_3.h"
#include "ui_nowytest_3.h"
#include <QMutexLocker>

#include <QCheckBox>
//#include "windows.h"
#include <QTimer>

#include "createtestwizard.h"
#include "testpage.h"
#include "ustawienia.h"
#include "urzadzenia.h"

#include <QMessageBox>


NowyTest_3::NowyTest_3(Urzadzenia * u, double cisnienie, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_3),
    cisnieWProzni(cisnienie),
    urzadzenia(u)
{
    ui->setupUi(this);
    task = 0;

    cisnienieTimer.setInterval(100);
    connect(&cisnienieTimer, &QTimer::timeout, this, &NowyTest_3::updateCisnieie);

}

NowyTest_3::~NowyTest_3()
{
    cisnienieTimer.stop();
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
     ui->frame_2->setVisible(false);
     ui->frame_3->setVisible(false);
     ui->frame_4->setVisible(false);
     emit completeChanged();
}

void NowyTest_3::openZawor(unsigned int id, bool val)
{
    if (id == proznia) {
        zaworProzni = val;
        if (prozniaTask && !val) {
            //ui->pbProzniaDone->setEnabled(true);
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



void NowyTest_3::updateCisnieie()
{
    double avg = getAvgCisnienie();
    double val = getCisnKomory();

    qDebug("%s:%d avg = %f cisn = %f  [%f] <%f %f>",__FILE__, __LINE__, avg, val, cisnieWProzni, 0.95*val, 1.05*val);
    if (0.95*val < avg && 1.05*avg > val && avg < cisnieWProzni) {
        //ui->pbProzniaDone->setEnabled(true);
        //ui->pbProzniaDone->setDone(true);
        valid = true;
        cisnienieTimer.stop();
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

void NowyTest_3::on_pbOk_1_clicked()
{
    if (!isZamknietyZawor()) {
        QMessageBox msgBox;
        msgBox.setText("Wykryto nie prawidłowe ustawienie zaworów.");
        msgBox.setInformativeText("Zawory A i B powinny być otwarte a pozostałe zawory powinny być zamknięte");
        msgBox.setStandardButtons(QMessageBox::Ok);
        //msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
    } else {
        ui->frame_2->setVisible(true);
    }

}

