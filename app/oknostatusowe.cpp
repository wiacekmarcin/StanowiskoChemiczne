#include "oknostatusowe.h"
#include "ui_oknostatusowe.h"

#include "urzadzenia.h"
OknoStatusowe::OknoStatusowe(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::OknoStatusowe),
    timer(this),
    bZawor1(false),
    bZawor2(false),
    bZawor3(false),
    bZawor4(false),
    bZawor5(false),
    bZawor6(false),
    bZawor7(false),
    bZawor8(false),
    bpilot(false),
    busb6210(false),
    busb6501(false),
    bdozownik(false)
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
    ui->zawor1->setText(set.wejscie(1));
    ui->zawor2->setText(set.wejscie(2));
    ui->zawor3->setText(set.wejscie(3));
    ui->zawor4->setText(set.wejscie(4));
    ui->zawor5->setText(set.wejscie(5));
    ui->zawor6->setText(set.wejscie(6));
    ui->zawor7->setText(set.wejscie(7));
    ui->zawor8->setText(set.wejscie(8));
    ui->pilot->setText(set.wejscie(pilot));
    adjustSize();
}

void OknoStatusowe::setDigitalValue(int id, bool val)
{
    //qDebug("OknoStatusowe::setDigitalValue(int id, bool val) %d %d", id, val);
    /*
    switch(id) {
    case 1: ui->zawor1->setOk(val); break;
    case 2: ui->zawor2->setOk(val); break;
    case 3: ui->zawor3->setOk(val); break;
    case 4: ui->zawor4->setOk(val); break;
    case 5: ui->zawor5->setOk(val); break;
    case 6: ui->zawor6->setOk(val); break;
    case 7: ui->zawor7->setOk(val); break;
    case 8: ui->zawor8->setOk(val); break;
    case 9: ui->pilot->setOk(val); break;
    default:
        break;
    }
    */

    switch(id) {
    case 1: bZawor1 = val; break;
    case 2: bZawor1 = val; break;
    case 3: bZawor1 = val; break;
    case 4: bZawor1 = val; break;
    case 5: bZawor1 = val; break;
    case 6: bZawor1 = val; break;
    case 7: bZawor1 = val; break;
    case 8: bZawor1 = val; break;
    case 9: bpilot = val; break;
    default:
        break;
    }
}

void OknoStatusowe::setDozownik(bool ok)
{
    bdozownik = ok;
    //ui->dozownik->setOk(ok);
    //ui->dozownik->update();
}

void OknoStatusowe::setUSB6210(bool ok)
{
    //ui->usb6210->setOk(ok);
    //ui->usb6210->update();
    busb6210 = ok;
}

void OknoStatusowe::setUSB6501(bool ok)
{
    //ui->usb6501->setOk(ok);
    //ui->usb6501->update();
    busb6501 = ok;
}

void OknoStatusowe::setDrzwiKomory(bool prawe, bool otwarte)
{
    if (prawe)
        bZawor1 = otwarte;
    else
        bZawor2 = otwarte;
}

void OknoStatusowe::setZawor(int idZ, bool otwarte)
{
    switch(idZ) {
    case Urzadzenia::digitalIn::wentylacja_lewa:
        bZawor3 = otwarte; break;
    case Urzadzenia::digitalIn::proznia:
        bZawor8 = otwarte; break;
    case Urzadzenia::digitalIn::pom_stez_1:
        bZawor5 = otwarte; break;
    case Urzadzenia::digitalIn::wentylacja_prawa:
        bZawor4 = otwarte; break;
    case Urzadzenia::digitalIn::wlot_powietrza:
        bZawor7 = otwarte; break;
    case Urzadzenia::digitalIn::pom_stez_2:
        bZawor6 = otwarte; break;
    default:
        break;
    };
}

void OknoStatusowe::setPilot(bool przycisk)
{
    bpilot = przycisk;
}

void OknoStatusowe::timeout()
{
    ui->zawor1->setOk(bZawor1);
    ui->zawor2->setOk(bZawor2);
    ui->zawor3->setOk(bZawor3);
    ui->zawor4->setOk(bZawor4);
    ui->zawor5->setOk(bZawor5);
    ui->zawor6->setOk(bZawor6);
    ui->zawor7->setOk(bZawor7);
    ui->zawor8->setOk(bZawor8);
    ui->pilot->setOk(bpilot);
    ui->usb6210->setOk(busb6210);
    ui->usb6501->setOk(busb6501);
    ui->dozownik->setOk(bdozownik);


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
