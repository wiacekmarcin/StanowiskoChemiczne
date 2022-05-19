#include "oczekiwanienazaplon.h"
#include "ui_oczekiwanienazaplon.h"
#include "createtestwizard.h"
#include "ustawienia.h"
#include <QDebug>

OczekiwanieNaZaplon::OczekiwanieNaZaplon(QWidget *parent, CreateTestWizard * wiz_, float tempB, float cisnB, bool plomien_) :
    QDialog(parent),
    ui(new Ui::OczekiwanieNaZaplon),
    wiz(wiz_),
    tempPocz(tempB), cisnPocz(cisnB),
    tempPrev(tempB), cisnPrev(cisnB),
    plomien(plomien_), isZaplon(false)
{
    ui->setupUi(this);
    ui->pbTime->setMaximum(30);
    secs = 0;
    maxSecs = plomien ? 50 : 5;
    ui->pbTime->setMaximum(maxSecs);
    ui->pbTime->setValue(secs);
    connect(&timer, &QTimer::timeout, this, &OczekiwanieNaZaplon::updateTime);
    timer.start(1000);
}

OczekiwanieNaZaplon::~OczekiwanieNaZaplon()
{
    timer.stop();
    delete ui;
}

void OczekiwanieNaZaplon::updateTime()
{
    secs++;
    if (secs > maxSecs) {
        timer.stop();
        reject();
    }

    float actTemp = wiz->getCzujnik(a_temp_komory);
    float actCisn = wiz->getCzujnik(a_cisn_komora);
    ui->temperatura->setText(QString("%1/%2").arg(QString::number(actTemp-tempPrev,'f',1), QString::number(actTemp-tempPocz, 'f', 1)));
    ui->cisnienie->setText(QString("%1/%2").arg(QString::number(actCisn-cisnPrev,'f',2), QString::number(actCisn-cisnPocz, 'f', 2)));
    cisnPrev = actCisn;
    tempPrev = actTemp;
    ui->pbTime->setValue(secs);
    if (actCisn - cisnPocz > 5.0)
    {
        timer.stop();
        accept();
    }
}

bool OczekiwanieNaZaplon::getIsZaplon() const
{
    return isZaplon;
}

