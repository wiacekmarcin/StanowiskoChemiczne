#include "oknostatusowe.h"
#include "ui_oknostatusowe.h"

OknoStatusowe::OknoStatusowe(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OknoStatusowe),
    timer(this)
{
    ui->setupUi(this);

    ui->zawor1->setText("Zawór 1");
    ui->zawor2->setText("Zawór 2");
    ui->zawor3->setText("Zawór 3");
    ui->zawor4->setText("Zawór 4");
    ui->zawor5->setText("Zawór 5");
    ui->zawor6->setText("Zawór 6");
    ui->zawor7->setText("Zawór 7");
    ui->zawor8->setText("Zawór 8");
    ui->pilot->setText("Pilot");
    ui->usb6210->setText("USB6210");
    ui->usb6501->setText("USB6501");
    ui->dozownik->setText("Dozownik");

    ui->zawor1->setOk(false);
    ui->zawor2->setOk(false);
    ui->zawor3->setOk(false);
    ui->zawor4->setOk(false);
    ui->zawor5->setOk(false);
    ui->zawor6->setOk(false);
    ui->zawor7->setOk(false);
    ui->zawor8->setOk(false);
    ui->pilot->setOk(false);
    ui->usb6210->setOk(false);
    ui->usb6501->setOk(false);
    //ui->dozownik->setOk(false);

    timer.setInterval(1000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start();
    adjustSize();
}

OknoStatusowe::~OknoStatusowe()
{
    delete ui;
}

void OknoStatusowe::timeout()
{
    ui->zawor1->update();
    ui->zawor2->update();
    ui->zawor3->update();
    ui->zawor4->update();
    ui->zawor5->update();
    ui->zawor6->update();
    ui->zawor7->update();
    ui->zawor8->update();
    ui->pilot->update();
    ui->usb6210->update();
    ui->usb6501->update();
    ui->dozownik->update();
}
