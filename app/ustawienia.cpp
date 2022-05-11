#include "ustawienia.h"

#include <QVariant>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>

#define REVERSE_INIT(N,D) do { reverse_##N = settings.value(QString("dozownik/rerverse_%1").arg(N), D).toBool(); } while(false)
Ustawienia::Ustawienia()
{
    initialSetting();

    for (int i = 0; i < maxCzujekAnal; ++i) {
        czujki[i].name = settings.value(QString("czujnikianalogowe/%1/name").arg(i), QString("Czujnik analogowy %1").arg(i+1)).toString();
        czujki[i].unit = settings.value(QString("czujnikianalogowe/%1/unit").arg(i), QString("jedn.")).toString();
        czujki[i].ratio = settings.value(QString("czujnikianalogowe/%1/ratio").arg(i), 1.0).toDouble();
        czujki[i].baseUnit = settings.value(QString("czujnikianalogowe/%1/base_unit").arg(i), QString("jedn.")).toString();
        czujki[i].convert = settings.value(QString("czujnikianalogowe/%1/convert").arg(i), 1.0).toDouble();
        
    }

    for (int i = 0; i < maxCzujekCyfrIn; ++i) {
        wejscia[0x1 << i] = settings.value(QString("wejsciacyfrowe/%1/name").arg(0x1 << i), QString("Wejscie cyfrowe %1").arg(i)).toString();
    }

    for (int i = 0; i < maxCzujekCyfrOut; ++i) {
        wyjscia[0x1 << i] = settings.value(QString("wyjsciacyfrowe/%1/name").arg(0x1 << i), QString("Wyjscie cyfrowe %1").arg(i)).toString();
    }

    REVERSE_INIT(1,false);
    REVERSE_INIT(2,false);
    REVERSE_INIT(3,false);
    REVERSE_INIT(4,false);
    REVERSE_INIT(5,true);
    maxImp = settings.value(QString("dozownik/maxImpMotor"), 50000).toUInt();
    impTime = settings.value(QString("dozownik/impMotorTime"), 200).toUInt();
    stepsOnMl = settings.value(QString("dozownik/stepsOnMl"), 14000.0).toDouble();
    nrInitializeCycles = settings.value(QString("dozownik/numberOfInitializeCycles"), 10).toUInt();
    cisnienieProzni = settings.value(QString("testy/cisnienieWprozni"), 0.01).toDouble();

}

void Ustawienia::setCzujka(analogIn id, const Ustawienia::CzujnikAnalogowy & czAnal)
{
    if ((short)id >= maxCzujekAnal || (short)id < 0)
        return;
    czujki[id] = czAnal;
    settings.setValue(QString("czujnikianalogowe/%1/name").arg(id), QVariant::fromValue(czAnal.name));
    settings.setValue(QString("czujnikianalogowe/%1/unit").arg(id), QVariant::fromValue(czAnal.unit));
    settings.setValue(QString("czujnikianalogowe/%1/base_unit").arg(id), QVariant::fromValue(czAnal.baseUnit));
    settings.setValue(QString("czujnikianalogowe/%1/ratio").arg(id), QVariant::fromValue(czAnal.ratio));
    settings.setValue(QString("czujnikianalogowe/%1/convert").arg(id), QVariant::fromValue(czAnal.convert));
    settings.setValue(QString("czujnikianalogowe/%1/minval").arg(id), QVariant::fromValue(czAnal.minVal));
    settings.setValue(QString("czujnikianalogowe/%1/percent").arg(id), QVariant::fromValue(czAnal.percentStab));


}

Ustawienia::CzujnikAnalogowy Ustawienia::getCzujnikAnalogowyUstawienia(analogIn id) const
{
    if ((short)id >= maxCzujekAnal || (short)id < 0)
        return Ustawienia::CzujnikAnalogowy();
    return czujki[id];
}

QString Ustawienia::getCzAnalName(analogIn id) const
{
    if ((short)id >= maxCzujekAnal || (short)id < 0)
        return QString("-");
    return czujki[id].name;
}

double Ustawienia::getRatio(analogIn id) const
{
    if ((short)id >= maxCzujekAnal || (short)id < 0)
        return 0.0;
    return czujki[id].ratio;
}

void Ustawienia::setWejscie(int id, const QString &name)
{
    wejscia[id] = name;
    settings.setValue(QString("wejsciacyfrowe/%1/name").arg(id), QVariant::fromValue(name));
}

QString Ustawienia::wejscie(int id) const
{
    return wejscia[id];
}

void Ustawienia::setWyjscie(int id, const QString &name)
{
    wyjscia[id] = name;
    settings.setValue(QString("wyjsciacyfrowe/%1/name").arg(id), QVariant::fromValue(name));
}

QString Ustawienia::wyjscie(int id) const
{
    return wyjscia[id];
}

#define REVERSE_FUN(N) bool Ustawienia::getReverse_##N() const \
{\
    return reverse_##N;\
}\
\
void Ustawienia::setReverse_##N(bool newReverse)\
{\
    reverse_##N = newReverse;\
    settings.setValue(QString("dozownik/rerverse_%1").arg(N), QVariant::fromValue(newReverse));\
}

REVERSE_FUN(1)
REVERSE_FUN(2)
REVERSE_FUN(3)
REVERSE_FUN(4)
REVERSE_FUN(5)

int Ustawienia::getMaxImp() const
{
    return maxImp;
}

void Ustawienia::setMaxImp(int newMaxImp)
{
    maxImp = newMaxImp;
    settings.setValue(QString("dozownik/maxImpMotor"), QVariant::fromValue(newMaxImp));
}

#define SET_CZUJKA(id, N, B, U, R, C, M, P) setCzujka(id, QString::fromUtf8(N), QString(B), QString(U), R, C, M, P)

void Ustawienia::initialSetting()
{
    //if (settings.value("initial", false).toBool()) return;

    settings.setValue("initial",QVariant::fromValue(true));
    SET_CZUJKA(a_cisn_komora,   "Ci\305\233nienie w komorze", "kPa", "kPa", 0.04, 1, 0.05, 2);
    SET_CZUJKA(a_temp_komory,   "Temperatura w komorze", "st C", "st C", 0.05, 1, 0.05, 2);
    SET_CZUJKA(a_temp_parownik, "Temperatura parownika", "st C", "st C", 0.05, 1, 0.05, 2);
    SET_CZUJKA(a_voc1,          "St\304\231\305\274enie VOC 1", "%", "%", 3.03, 1, 0.05, 2);
    SET_CZUJKA(a_voc2,          "St\304\231\305\274enie VOC 2", "%", "%", 3.846, 1, 0.05, 2);
    SET_CZUJKA(a_8,             "St\304\231\305\274enie virtualnego czujnika", "%", "%", 3.03, 1, 0.05, 2);
    SET_CZUJKA(a_o2,            "St\304\231\305\274enie O2", "%", "%", 0.4, 1, 0.05, 2);
    SET_CZUJKA(a_co2,           "St\304\231\305\274enie CO2", "%", "%", 0.5, 1, 0.05, 2);

    setWejscie(i_drzwi_lewe,         QString::fromUtf8("Drzwi komory LEWE"));
    setWejscie(i_drzwi_prawe,        QString::fromUtf8("Drzwi komory PRAWE"));
    setWejscie(i_wentylacja_lewa,    QString::fromUtf8("Zaw\303\263r went. WYLOT"));
    setWejscie(i_wentylacja_prawa,   QString::fromUtf8("Zaw\303\263r went. WLOT"));
    setWejscie(i_pom_stez_1,         QString::fromUtf8("Zaw\303\263r st\304\231\305\274e\305\204 - IN"));
    setWejscie(i_pom_stez_2,         QString::fromUtf8("Zaw\303\263r st\304\231\305\274e\305\204 - OUT"));
    setWejscie(i_wlot_powietrza,     QString::fromUtf8("Zaw\303\263r powietrza"));
    setWejscie(i_proznia,            QString::fromUtf8("Zaw\303\263r pr\303\263\305\274ni"));
    setWejscie(i_pilot,              QString::fromUtf8("Pilot"));

    setWyjscie(o_hv_onoff,           QString::fromUtf8("Iskra elektryczna ON/OFF"));
    setWyjscie(o_hv_bezpiecznik,     QString::fromUtf8("Iskra elektryczna Bezpiecznik"));
    setWyjscie(o_hv_iskra,           QString::fromUtf8("Iskra elektryczna Zap\305\202on"));
    setWyjscie(o_mech_iskra,         QString::fromUtf8("Iskra mechaniczna"));
    setWyjscie(o_grzalka,            QString::fromUtf8("P\305\202omie\305\204"));
    setWyjscie(o_pompa_prozniowa,    QString::fromUtf8("Pompa pr\303\263\305\274niowa"));
    setWyjscie(o_pompa_powietrza,    QString::fromUtf8("Pompka mebranowa"));
    setWyjscie(o_wentylator,         QString::fromUtf8("Wentylator do przedmuchu"));
    setWyjscie(o_mieszadlo,          QString::fromUtf8("Mieszad\305\202o"));
    setWyjscie(o_trigger,            QString::fromUtf8("Trigger kamery"));

    setReverse_1(false);
    setReverse_2(false);
    setReverse_3(false);
    setReverse_4(false);
    setReverse_5(true);

    setMaxImp(50000);
    setImpTime(130);
    setNrInitializeCycles(4);

    setStepsOnMl(14000);
}

void Ustawienia::setCzujka(analogIn id, const QString &name, const QString &baseUnit, const QString &unit,
                           const double &ratio, const double &convert, const double &minVal, const double &percent)
{
    CzujnikAnalogowy czA;
    czA.name = name;
    czA.baseUnit = baseUnit;
    czA.unit = unit;
    czA.ratio = ratio;
    czA.convert = convert;
    czA.minVal = minVal;
    czA.percentStab = percent;
    setCzujka(id, czA);
}

double Ustawienia::getCisnienieProzni() const
{
    return cisnienieProzni;
}

void Ustawienia::setCisnienieProzni(double newCisnienieProzni)
{
    cisnienieProzni = newCisnienieProzni;
    settings.setValue(QString("testy/cisnienieWprozni"), QVariant::fromValue(newCisnienieProzni));
}

unsigned short Ustawienia::getNrInitializeCycles() const
{
    return nrInitializeCycles;
}

void Ustawienia::setNrInitializeCycles(unsigned short newNrInitializeCycles)
{
    nrInitializeCycles = newNrInitializeCycles;
    settings.setValue(QString("dozownik/numberOfInitializeCycles"), QVariant::fromValue(newNrInitializeCycles));
}

double Ustawienia::getStepsOnMl() const
{
    return stepsOnMl;
}

void Ustawienia::setStepsOnMl(double newStepsOnMl)
{
    stepsOnMl = newStepsOnMl;
    settings.setValue(QString("dozownik/stepsOnMl"), QVariant::fromValue(newStepsOnMl));
}

int Ustawienia::getImpTime() const
{
    return impTime;
}

void Ustawienia::setImpTime(int newImpTime)
{
    impTime = newImpTime;
    settings.setValue(QString("dozownik/impMotorTime"), QVariant::fromValue(newImpTime));
}
