#include "oknostatusowe.h"
#include "ui_oknostatusowe.h"

#include "urzadzenia.h"
#include "ustawienia.h"
#include "stanczujnika.h"
#define _STR(X) #X
#define STR(X) _STR(X)
#define CON(X, Y) X ## Y
#define INIT_MAP(N) \
    bMapZawor[mapDigitalOrder[N]] = false; \
    sMapZawor[mapDigitalOrder[N]] = ui->zawor##N;\
    ui->zawor##N->setText("Zawór "#N);\
    ui->zawor##N->setObjectName("Zawor"#N);\
    ui->zawor##N->setOk(false);

#define SETZAWOR(N)

OknoStatusowe::OknoStatusowe(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OknoStatusowe),
    timer(this),
    bOpenUsb6210(false),
    bOpenUsb6501(false),
    bOpenDozownik(false),
    mapDigitalOrder{ 0, i_drzwi_lewe, i_wentylacja_lewa, i_proznia, i_pom_stez_1, i_drzwi_prawe, i_wentylacja_prawa, i_wlot_powietrza, i_pom_stez_2, i_pilot},
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

    ui->pilot->setText("Pilot");
    ui->pilot->setObjectName("Pilot");
    ui->pilot->setOk(false);


    ui->usb6210->setText("Karta NI USB6210");
    ui->usb6501->setText("Karta NI USB6501");
    ui->dozownik->setText(QString::fromUtf8("Sterownik dozowników"));

    ui->usb6210->setState(true, false);
    ui->usb6501->setState(true, false);
    ui->dozownik->setState(true, false);

    timer.setInterval(1000);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer.start();

    adjustSize();
}

OknoStatusowe::~OknoStatusowe()
{
    timer.stop();
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

void OknoStatusowe::setDozownik(bool open, bool conf)
{
    bOpenDozownik = open;
    bConfDozownik = conf;
}

void OknoStatusowe::setUSB6210(bool open, bool conf)
{
    
    bOpenUsb6210 = open;
    bConfUsb6210 = conf;
}

void OknoStatusowe::setUSB6501(bool open, bool conf)
{
    
    bOpenUsb6501 = open;
    bConfUsb6501 = conf;
}

void OknoStatusowe::timeout()
{
    for(short id = 1; id < 10; ++id) {
        sMapZawor[mapDigitalOrder[id]]->setOk(bMapZawor[mapDigitalOrder[id]]);
        sMapZawor[mapDigitalOrder[id]]->update();
    }
    ui->usb6210->setState(bOpenUsb6210, bConfUsb6210);
    ui->usb6501->setState(bOpenUsb6501, bConfUsb6501);
    ui->dozownik->setState(bOpenDozownik, bConfDozownik);

    ui->usb6210->update();
    ui->usb6501->update();
    ui->dozownik->update();
}
