#include "oknostanowewyjscia.h"
#include "ui_oknostanowewyjscia.h"

#include "stanwyjscia.h"
#include "ustawienia.h"

#define SETSTANWYJSCIA(N,M,V,R,C) do {\
    StanWyjscia * w##N = new StanWyjscia(this);\
    w##N->setObjectName(QString::fromUtf8("w"#N));\
    /*w##N->setFrameShape(QFrame::StyledPanel);*/\
    /*w##N->setFrameShadow(QFrame::Raised);*/\
    w##N->setMaskId(M, V);\
    wyjscia[M] = w##N;\
    ui->gridLayout->addWidget(w##N, R, C, 1, 1);\
    connect(w##N, &StanWyjscia::writeValue, this, &OknoStanoweWyjscia::writeValue);\
} while(false)


OknoStanoweWyjscia::OknoStanoweWyjscia(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OknoStanoweWyjscia)
{
    ui->setupUi(this);

    SETSTANWYJSCIA(1, wentylator, true, 0, 0);
    SETSTANWYJSCIA(2, mieszadlo, true, 0, 1);
    SETSTANWYJSCIA(3, pompa_powietrza, true, 0, 2);
    SETSTANWYJSCIA(4, pompa_prozniowa, true, 0, 3);
    SETSTANWYJSCIA(5, trigger, true, 0, 4);
    SETSTANWYJSCIA(6, plomien, false, 0, 5);
    SETSTANWYJSCIA(7, hv_onoff, false, 0, 6);
    SETSTANWYJSCIA(8, hv_bezpieczenstwa, false, 0, 7);
    SETSTANWYJSCIA(9, hw_iskra, false, 0, 8);
    SETSTANWYJSCIA(10, mech_iskra, false, 0, 9);
}

OknoStanoweWyjscia::~OknoStanoweWyjscia()
{
    delete ui;
}

void OknoStanoweWyjscia::setLabels(const Ustawienia & ust)
{
    wyjscia[hv_onoff]->setLabel(ust.wyjscie(hv_onoff));
    wyjscia[hv_bezpieczenstwa]->setLabel(ust.wyjscie(hv_bezpieczenstwa));
    wyjscia[hw_iskra]->setLabel(ust.wyjscie(hw_iskra));
    wyjscia[mech_iskra]->setLabel(ust.wyjscie(mech_iskra));
    wyjscia[plomien]->setLabel(ust.wyjscie(plomien));
    wyjscia[pompa_prozniowa]->setLabel(ust.wyjscie(pompa_prozniowa));
    wyjscia[pompa_powietrza]->setLabel(ust.wyjscie(pompa_powietrza));
    wyjscia[wentylator]->setLabel(ust.wyjscie(wentylator));
    wyjscia[mieszadlo]->setLabel(ust.wyjscie(mieszadlo));
    wyjscia[trigger]->setLabel(ust.wyjscie(trigger));

}

void OknoStanoweWyjscia::setOnOff(uint16_t mask)
{
    for ( QMap<uint16_t, StanWyjscia*>::iterator it = wyjscia.begin();
          it != wyjscia.end(); ++it) {
        it.value()->digitalWrite(mask);
    }
}

