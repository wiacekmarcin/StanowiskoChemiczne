#include "sygnalycyfroweustawienia.h"
#include "ui_sygnalycyfroweustawienia.h"
#include "stanwyjscia.h"

#define SETSTANWYJSCIA(N,M,V,R,C,S) do {\
    StanWyjscia * w##N = new StanWyjscia(this);\
    w##N->setObjectName(QString::fromUtf8("w"#N));\
    /*w##N->setFrameShape(QFrame::StyledPanel);*/\
    /*w##N->setFrameShadow(QFrame::Raised);*/\
    w##N->setMaskId(M, V);\
    w##N->setVisible(S);\
    wyjscia[M] = w##N;\
    ui->gridLayout->addWidget(w##N, R, C, 1, 1);\
    connect(w##N, &StanWyjscia::writeValue, this, &SygnalyCyfroweUstawienia::writeValue);\
} while(false)



SygnalyCyfroweUstawienia::SygnalyCyfroweUstawienia(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SygnalyCyfroweUstawienia)
{
    ui->setupUi(this);

    SETSTANWYJSCIA(1, o_wentylator, true, 0, 0, true);
    SETSTANWYJSCIA(2, o_mieszadlo, true, 0, 1, true);
    SETSTANWYJSCIA(3, o_pompa_powietrza, true, 0, 2, true);
    SETSTANWYJSCIA(4, o_pompa_prozniowa, true, 0, 3, true);
    SETSTANWYJSCIA(5, o_trigger, true, 0, 4, true);
    SETSTANWYJSCIA(6, o_grzalka, true, 1, 0, true);
    SETSTANWYJSCIA(7, o_hv_onoff, true, 1, 1, true);
    SETSTANWYJSCIA(8, o_hv_bezpiecznik, true, 1, 2, true);
    SETSTANWYJSCIA(9, o_hv_iskra, true, 1, 3, true);
    SETSTANWYJSCIA(10, o_mech_iskra, true, 1, 4, true);
}

SygnalyCyfroweUstawienia::~SygnalyCyfroweUstawienia()
{
    delete ui;
}

#define SETIN(N, M, S) do { \
        ui->inPin##N##Name->setText(ust.wejscie(M));\
        if ((user & U_SERVISANT) == U_SERVISANT) ui->linPin##N->setText(ui->linPin##N->text() + QString(S));\
    } while(false)

#define SETOUT(N, M, S) do { \
        ui->outPin##N##Name->setText(ust.wyjscie(M));\
        wyjscia[M]->setLabel(ust.wyjscie(M));\
        if ((user & U_SERVISANT) == U_SERVISANT) ui->loutPin##N->setText(ui->loutPin##N->text() + QString(S));\
    } while(false)

void SygnalyCyfroweUstawienia::setLabels(const Ustawienia &ust, const UserPrivilige &user)
{
    SETIN(1, i_drzwi_lewe,       " [0x001 = P2.0]");
    SETIN(2, i_wentylacja_lewa,  " [0x002 = P2.1]");
    SETIN(3, i_proznia,          " [0x004 = P2.2]");
    SETIN(4, i_pom_stez_1,       " [0x008 = P2.3]");
    SETIN(5, i_drzwi_prawe,      " [0x010 = P2.4]");
    SETIN(6, i_wentylacja_prawa, " [0x020 = P2.5]");
    SETIN(7, i_wlot_powietrza,   " [0x040 = P2.6]");
    SETIN(8, i_pom_stez_2,       " [0x080 = P2.7]");
    SETIN(9, i_pilot,            " [0x100 = P1.4]");

    SETOUT(1, o_hv_onoff,           " [0x001 = P0.0 = X1]");
    SETOUT(2, o_hv_bezpiecznik,     " [0x002 = P0.1 = X2]");
    SETOUT(3, o_hv_iskra,           " [0x004 = P0.2 = X3]");
    SETOUT(4, o_mech_iskra,         " [0x008 = P0.3 = X4]");
    SETOUT(5, o_grzalka,            " [0x010 = P0.4 = X5]");
    SETOUT(6, o_pompa_prozniowa,    " [0x020 = P0.5 = X6]");
    SETOUT(7, o_pompa_powietrza,    " [0x040 = P0.6 = X7]");
    SETOUT(8, o_wentylator,         " [0x080 = P0.7 = X8]");
    SETOUT(9, o_mieszadlo,          " [0x100 = P1.0 = X9]");
    SETOUT(10, o_trigger,           " [0x100 = P1.2 = X11]");
}

void SygnalyCyfroweUstawienia::setOnOff(uint16_t mask)
{

}
