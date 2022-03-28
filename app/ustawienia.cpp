#include "ustawienia.h"

#include <QVariant>
#include <QTime>
#include <QCoreApplication>

void delay(unsigned int time)
{
    QTime dieTime= QTime::currentTime().addSecs(time);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void delayMs(unsigned int ms)
{
    QTime dieTime= QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
}

#define REVERSE_INIT(N,D) do { reverse_##N = settings.value(QString("dozownik/rerverse_%1").arg(N), D).toBool(); } while(false)
Ustawienia::Ustawienia()
{
    initialSetting();

    for (int i = 0; i < maxCzujekAnal; ++i) {
        czujki[i].name = settings.value(QString("czujnikianalogowe/%1/name").arg(i), QString("Czujnik analogowy %1").arg(i+1)).toString();
        czujki[i].unit = settings.value(QString("czujnikianalogowe/%1/unit").arg(i), QString("jedn.")).toString();
        czujki[i].ratio = settings.value(QString("czujnikianalogowe/%1/ratio").arg(i), 1.0).toDouble();
        qDebug("%s:%d %s %s %f", __FILE__,__LINE__,czujki[i].name.toStdString().c_str(),czujki[i].unit.toStdString().c_str(),czujki[i].ratio);
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
    impTime = settings.value(QString("dozownik/maxImpMotor"), 200).toUInt();
    stepsOnMl = settings.value(QString("dozownik/stepsOnMl"), 1.0).toDouble();
    nrInitializeCycles = settings.value(QString("dozownik/numberOfInitializeCycles"), 10).toUInt();

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
    if (settings.value("initial", false).toBool()) return;

    settings.setValue("initial",QVariant::fromValue(true));
    setCzujka(a_cisn_komora,   QString::fromUtf8("Ci\305\233nenie w komorze"), QString::fromUtf8("kPa"), 1.0);
    setCzujka(a_vol1,          QString::fromUtf8("St\304\231\305\274enie VOC 1"), QString::fromUtf8("%"), 1.0);
    setCzujka(a_o2,            QString::fromUtf8("St\304\231\305\274enie O2"), QString::fromUtf8("%"), 1.0);
    setCzujka(a_co2,           QString::fromUtf8("St\304\231\305\274enie CO2"), QString::fromUtf8("%"), 1.0);
    setCzujka(a_temp_komory,   QString::fromUtf8("Temperatura w komorze"), QString::fromUtf8("st C"), 1.0);
    setCzujka(a_temp_parownik, QString::fromUtf8("Temperatura parownika"), QString::fromUtf8("st C"), 1.0);
    setCzujka(a_vol2,          QString::fromUtf8("St\304\231\305\274enie VOC 2"), QString::fromUtf8("%"), 1.0);
    setCzujka(a_8,             QString::fromUtf8("Syg. analogowy 8"), QString::fromUtf8("mV"), 1.0);

    setWejscie(drzwi_lewe,         QString::fromUtf8("Komora drzwi lewe"));
    setWejscie(drzwi_prawe,        QString::fromUtf8("Komora drzwi prawe"));
    setWejscie(wentylacja_lewa,    QString::fromUtf8("Wentylacja lewa"));
    setWejscie(wentylacja_prawa,   QString::fromUtf8("Wentylacja prawa"));
    setWejscie(pom_stez_1,         QString::fromUtf8("Pomiar st\304\231\305\274enia 1"));
    setWejscie(pom_stez_2,         QString::fromUtf8("Pomiar st\304\231\305\274enia 2"));
    setWejscie(wlot_powietrza,     QString::fromUtf8("Powietrze"));
    setWejscie(proznia,            QString::fromUtf8("Pr\303\263\305\272nia"));
    setWejscie(pilot,              QString::fromUtf8("Pilot zdalnego sterowania"));

    setWyjscie(hv_onoff,           QString::fromUtf8("Iskra elektryczna ON/OFF"));
    setWyjscie(hv_bezpieczenstwa,          QString::fromUtf8("Iskra elektryczna Bezpiecznik"));
    setWyjscie(hw_iskra,           QString::fromUtf8("Iskra elektryczna Zap\305\202on"));
    setWyjscie(mech_iskra,         QString::fromUtf8("Iskra mechaniczna (ON/OFF silnik DC)"));
    setWyjscie(plomien,            QString::fromUtf8("P\305\202omie\305\204 (ON/OFF grza\305\202ki)"));
    setWyjscie(pompa_prozniowa,    QString::fromUtf8("Pompa pr\303\263\305\274niowa"));
    setWyjscie(pompa_powietrza,    QString::fromUtf8("Pompka mebramowa"));
    setWyjscie(wentylator,         QString::fromUtf8("Wentylator do przedmuchu"));
    setWyjscie(mieszadlo,          QString::fromUtf8("Mieszad\305\202o"));
    setWyjscie(trigger,            QString::fromUtf8("Trigger kamery"));
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
}
