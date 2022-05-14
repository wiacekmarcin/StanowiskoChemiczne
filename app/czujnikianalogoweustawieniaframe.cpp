#include "czujnikianalogoweustawieniaframe.h"
#include "czujnikianalogoweokno.h"
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QCoreApplication>

#include "sygnalanalogowyustawienia.h"
#include <QTimer>
#include <QDebug>

CzujnikiAnalogoweUstawieniaFrame::CzujnikiAnalogoweUstawieniaFrame(QWidget *parent) :
    QFrame( parent )
{




}

CzujnikiAnalogoweUstawieniaFrame::~CzujnikiAnalogoweUstawieniaFrame()
{

}

void CzujnikiAnalogoweUstawieniaFrame::setUstawienia(const UserPrivilige & user, const Ustawienia &sett)
{
    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    this->user = user;
    for (int i = 0; i < sett.maxCzujekAnal; i++) {
        createOneElement(i, QString("Czujnik %1").arg(i+1));
        czujniki[i]->setData(user, sett.getCzujnikAnalogowyUstawienia((analogIn)i));
        connect(czujniki[i], &SygnalAnalogowyUstawienia::updateCzujnik,
                this, &CzujnikiAnalogoweUstawieniaFrame::updateCzujnik);
    }

    //QMetaObject::connectSlotsByName(this);
    //setWindowTitle(QCoreApplication::translate("CzujnikAnalogowyUstawieniaOkno", "Ustawienia parametr\303\263w sygna\305\202\303\263w", nullptr));
}

void CzujnikiAnalogoweUstawieniaFrame::saveData(Ustawienia &ust)
{
    for (int i = 0; i < ust.maxCzujekAnal; ++i) {
        ust.setCzujka((analogIn)i, czujniki[i]->getUstawienia());
    }
}


#define SET_CZUJKA(ID,OP,BU,UU,RA,MN,MV,PS) do { \
    Ustawienia::CzujnikAnalogowy c;\
    c.name = OP; c.unit = BU; c.unit = UU; \
    c.ratio = RA; c.convert = MN; c.minVal = MV; c.percentStab = PS;\
    czujniki[ID]->setData(user, c);\
} while(false)

void CzujnikiAnalogoweUstawieniaFrame::restoreDefaults()
{
    SET_CZUJKA(a_cisn_komora,   "Ci\305\233nienie w komorze", "kPa", "mBar", 0.04, 10, 0.05, 2);
    SET_CZUJKA(a_temp_komory,   "Temperatura w komorze", "st C", "st C", 0.05, 1, 0.05, 2);
    SET_CZUJKA(a_temp_parownik, "Temperatura parownika", "st C", "st C", 0.05, 1, 0.05, 2);
    SET_CZUJKA(a_voc1,          "St\304\231\305\274enie VOC 1", "%", "%", 3.03, 1, 0.05, 2);
    SET_CZUJKA(a_voc2,          "St\304\231\305\274enie VOC 2", "%", "%", 3.846, 1, 0.05, 2);
    SET_CZUJKA(a_8,             "St\304\231\305\274enie virtualnego czujnika", "%", "%", 3.03, 1, 0.05, 2);
    SET_CZUJKA(a_o2,            "St\304\231\305\274enie O2", "%", "%", 0.4, 1, 0.05, 2);
    SET_CZUJKA(a_co2,           "St\304\231\305\274enie CO2", "%", "%", 0.5, 1, 0.05, 2);

}

void CzujnikiAnalogoweUstawieniaFrame::updateCzujnik()
{
    for (int i = 0; i < Ustawienia::maxCzujekAnal; i++) {
        if (!czujniki[i]->valid()) {
            break;
        }
    }
}

void CzujnikiAnalogoweUstawieniaFrame::createOneElement(int id, const QString & titleBox)
{
    QGroupBox *groupBox = new QGroupBox(titleBox, this);
    groupBox->setObjectName(QString("groupBox_%1").arg(id));

    QHBoxLayout *horizontalLayout = new QHBoxLayout(groupBox);;
    horizontalLayout->setObjectName(QString("horizontalLayout_%1").arg(id));

    SygnalAnalogowyUstawienia *widget = new SygnalAnalogowyUstawienia(groupBox);
    widget->setObjectName(QString::fromUtf8("widget_%1").arg(id));
    horizontalLayout->addWidget(widget);

    czujniki[id] = widget;

    gridLayout->addWidget(groupBox, id / 4, id % 4, 1, 1);
}
