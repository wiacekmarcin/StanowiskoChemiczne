#include "oknostatusowe.h"
#include "ui_oknostatusowe.h"

#include "urzadzenia.h"
#include "ustawienia.h"
#include "stanczujnika.h"
#define _STR(X) #X
#define STR(X) _STR(X)
#define CON(X, Y) X ## Y
#define INIT_MAP(N) bMapZawor[mapDigitalOrder[N]] = false; sMapZawor[mapDigitalOrder[N]] = ui->zawor##N;
OknoStatusowe::OknoStatusowe(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OknoStatusowe),
    timer(this),
    busb6210(false),
    busb6501(false),
    bdozownik(false),
    mapDigitalOrder{ 0, drzwi_lewe, wentylacja_lewa, proznia, pom_stez_1, drzwi_prawe, wentylacja_prawa, wlot_powietrza, pom_stez_2, pilot},
    maxZawors(10)
{
    

    ui->setupUi(this);

    INIT_MAP(1);
    INIT_MAP(2);
    INIT_MAP(3);
    INIT_MAP(4);
    INIT_MAP(5);
    INIT_MAP(6);
    INIT_MAP(7);
    INIT_MAP(8);
    bMapZawor[mapDigitalOrder[9]] = false;
    sMapZawor[mapDigitalOrder[9]] = ui->pilot;


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
    ui->dozownik->setOk(false);

    timer.setInterval(1000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start();
    adjustSize();
}

OknoStatusowe::~OknoStatusowe()
{
    delete ui;
}

void OknoStatusowe::setLabels(const Ustawienia &set)
{
    for(short id = 1; id < 10; ++id) {
        sMapZawor[mapDigitalOrder[id]]->setText(set.wejscie(mapDigitalOrder[id]));
    }
    adjustSize();
}

void OknoStatusowe::setDigitalValue(int id, bool val)
{
    bMapZawor[id] = val;
}

void OknoStatusowe::setDozownik(bool ok)
{
    bdozownik = ok;
}

void OknoStatusowe::setUSB6210(bool ok)
{
    busb6210 = ok;
}

void OknoStatusowe::setUSB6501(bool ok)
{
    busb6501 = ok;
}

void OknoStatusowe::setDrzwiKomory(bool prawe, bool otwarte)
{
    if (prawe)
        bMapZawor[drzwi_lewe] = otwarte;
    else
        bMapZawor[drzwi_prawe] = otwarte;
}

void OknoStatusowe::setZawor(int idZ, bool otwarte)
{
    bMapZawor[idZ] = otwarte;
}

void OknoStatusowe::setPilot(bool przycisk)
{
    bMapZawor[pilot] = przycisk;
}

void OknoStatusowe::timeout()
{
    for(short id = 1; id < 10; ++id) {
        sMapZawor[mapDigitalOrder[id]]->setOk(bMapZawor[mapDigitalOrder[id]]);
        sMapZawor[mapDigitalOrder[id]]->update();
    }
    ui->usb6210->setOk(busb6210);
    ui->usb6501->setOk(busb6501);
    ui->dozownik->setOk(bdozownik);

    ui->usb6210->update();
    ui->usb6501->update();
    ui->dozownik->update();
}
