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

void Ustawienia::setCzujka(short id, const QString &name, const QString &unit, const double &ratio)
{
    if (id >= maxCzujekAnal || id < 0)
        return;
    czujki[id].name = name;
    czujki[id].unit = unit;
    czujki[id].ratio = ratio;
    settings.setValue(QString("czujnikianalogowe/%1/name").arg(id), QVariant::fromValue(name));
    settings.setValue(QString("czujnikianalogowe/%1/unit").arg(id), QVariant::fromValue(unit));
    settings.setValue(QString("czujnikianalogowe/%1/ratio").arg(id), QVariant::fromValue(ratio));
}

Ustawienia::CzujnikAnalogowy Ustawienia::getCzujka(short id) const
{
    if (id >= maxCzujekAnal || id < 0)
        return Ustawienia::CzujnikAnalogowy();
    return czujki[id];
}

QString Ustawienia::getName(short id) const
{
    if (id >= maxCzujekAnal || id < 0)
        return QString();
    return czujki[id].name;
}

QString Ustawienia::getUnit(short id) const
{
    if (id >= maxCzujekAnal || id < 0)
        return QString();
    return czujki[id].unit;
}

double Ustawienia::getRatio(short id) const
{
    if (id >= maxCzujekAnal || id < 0)
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

void Ustawienia::initialSetting()
{
    //if (settings.value("initial", false).toBool()) return;

    settings.setValue("initial",QVariant::fromValue(true));
    setCzujka(a_cisn_komora,   QString::fromUtf8("Ci\305\233nienie w komorze"), QString::fromUtf8("kPa"), 25.0);
    setCzujka(a_vol1,          QString::fromUtf8("St\304\231\305\274enie VOC 1"), QString::fromUtf8("%"), 0.33);
    setCzujka(a_o2,            QString::fromUtf8("St\304\231\305\274enie O2"), QString::fromUtf8("%"), 2.5);
    setCzujka(a_co2,           QString::fromUtf8("St\304\231\305\274enie CO2"), QString::fromUtf8("%"), 2.0);
    setCzujka(a_temp_komory,   QString::fromUtf8("Temperatura w komorze"), QString::fromUtf8("st C"), 20.0);
    setCzujka(a_temp_parownik, QString::fromUtf8("Temperatura parownika"), QString::fromUtf8("st C"), 20.0);
    setCzujka(a_vol2,          QString::fromUtf8("St\304\231\305\274enie VOC 2"), QString::fromUtf8("%"), 0.33);
    setCzujka(a_8,             QString::fromUtf8("Stężenie"), QString::fromUtf8("ppm"), 10000);

    setWejscie(i_drzwi_lewe,         QString::fromUtf8("Drzwi komory LEWE"));
    setWejscie(i_drzwi_prawe,        QString::fromUtf8("Drzwi komory PRAWE"));
    setWejscie(i_wentylacja_lewa,    QString::fromUtf8("Zaw\303\263r went. WYLOT"));
    setWejscie(i_wentylacja_prawa,   QString::fromUtf8("Zaw\303\263r went. WLOT"));
    setWejscie(i_pom_stez_1,         QString::fromUtf8("Zaw\303\263r pom. st\304\231\305\274e\305\204 - IN"));
    setWejscie(i_pom_stez_2,         QString::fromUtf8("Zaw\303\263r pom. st\304\231\305\274e\305\204. - OUT"));
    setWejscie(i_wlot_powietrza,     QString::fromUtf8("Zaw\303\263r podawania powietrza"));
    setWejscie(i_proznia,            QString::fromUtf8("Zaw\303\263r pr\303\263\305\274ni"));
    setWejscie(i_pilot,              QString::fromUtf8("Pilot zdalnego sterowania"));

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
