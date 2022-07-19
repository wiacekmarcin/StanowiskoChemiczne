#include "oknostanowewyjscia.h"
#include "ui_oknostanowewyjscia.h"

#include "stanwyjscia.h"
#include "ustawienia.h"

#define SETSTANWYJSCIA(N,M,V,R,C,S,I) do {\
    StanWyjscia * w##N = new StanWyjscia(I,this);\
    w##N->setObjectName(QString::fromUtf8("w"#N));\
    /*w##N->setFrameShape(QFrame::StyledPanel);*/\
    /*w##N->setFrameShadow(QFrame::Raised);*/\
    w##N->setMaskId(M, V);\
    w##N->setVisible(S);\
    wyjscia[M] = w##N;\
    ui->gridLayout->addWidget(w##N, R, C, 1, 1);\
    connect(w##N, &StanWyjscia::writeValue, this, &OknoStanoweWyjscia::writeValue);\
} while(false)


OknoStanoweWyjscia::OknoStanoweWyjscia(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OknoStanoweWyjscia)
{
    ui->setupUi(this);

    
    //VIS (VISIBLE BTN ON/OFF FOR USER)
    //SHOW (VISIBLE STATE FOR USER) 
    //ONEBTN ([ON],[OFF] <=> [ON/OFF])
    //            NR  ID                VIS  ROW COL SHOW  SHOWONEBTN
    SETSTANWYJSCIA(1, o_wentylator,     true,  0, 0, true,  false);
    SETSTANWYJSCIA(2, o_mieszadlo,      true,  0, 1, true,  false);
    SETSTANWYJSCIA(3, o_pompa_powietrza,true,  0, 2, true,  false);
    SETSTANWYJSCIA(4, o_pompa_prozniowa,true,  0, 3, true,  false);
    SETSTANWYJSCIA(5, o_trigger,        true,  0, 4, true,  true);
    SETSTANWYJSCIA(6, o_grzalka,        false, 0, 5, false, false);
    SETSTANWYJSCIA(7, o_hv_onoff,       false, 0, 6, false, false);
    SETSTANWYJSCIA(8, o_hv_bezpiecznik, false, 0, 7, false, false);
    SETSTANWYJSCIA(9, o_hv_iskra,       false, 0, 8, false, false);
    SETSTANWYJSCIA(10, o_mech_iskra,    false, 0, 9, false, false);
}

OknoStanoweWyjscia::~OknoStanoweWyjscia()
{
    delete ui;
}

void OknoStanoweWyjscia::setLabels(const Ustawienia & ust)
{
    wyjscia[o_hv_onoff]->setLabel(ust.wyjscie(o_hv_onoff));
    wyjscia[o_hv_bezpiecznik]->setLabel(ust.wyjscie(o_hv_bezpiecznik));
    wyjscia[o_hv_iskra]->setLabel(ust.wyjscie(o_hv_iskra));
    wyjscia[o_mech_iskra]->setLabel(ust.wyjscie(o_mech_iskra));
    wyjscia[o_grzalka]->setLabel(ust.wyjscie(o_grzalka));
    wyjscia[o_pompa_prozniowa]->setLabel(ust.wyjscie(o_pompa_prozniowa));
    wyjscia[o_pompa_powietrza]->setLabel(ust.wyjscie(o_pompa_powietrza));
    wyjscia[o_wentylator]->setLabel(ust.wyjscie(o_wentylator));
    wyjscia[o_mieszadlo]->setLabel(ust.wyjscie(o_mieszadlo));
    wyjscia[o_trigger]->setLabel(ust.wyjscie(o_trigger));

}

void OknoStanoweWyjscia::setOnOff(uint16_t mask)
{
    for ( QMap<uint16_t, StanWyjscia*>::iterator it = wyjscia.begin();
          it != wyjscia.end(); ++it) {
        it.value()->digitalWrite(mask);
    }
}

