#include "oczekiwanienazaplon.h"
#include "ui_oczekiwanienazaplon.h"

OczekiwanieNaZaplon::OczekiwanieNaZaplon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OczekiwanieNaZaplon)
{
    ui->setupUi(this);
    ui->pbTime->setMaximum(30);
    secs = 0;
    maxSecs = 10;
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
        accept();
    }
    ui->pbTime->setValue(secs);
}
