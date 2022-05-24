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
        czujki[i].minVal = settings.value(QString("czujnikianalogowe/%1/minval").arg(i), 1.0).toDouble();
        czujki[i].percentStab = settings.value(QString("czujnikianalogowe/%1/percent").arg(i), 1.0).toDouble();
    }

    for (int i = 0; i < maxCzujekCyfrIn; ++i) {
        wejscia[(digitalIn)(0x1 << i)] = settings.value(QString("wejsciacyfrowe/%1/name").arg(0x1 << i), QString("Wejscie cyfrowe %1").arg(i)).toString();
    }

    for (int i = 0; i < maxCzujekCyfrOut; ++i) {
        wyjscia[(digitalOut)(0x1 << i)] = settings.value(QString("wyjsciacyfrowe/%1/name").arg(0x1 << i), QString("Wyjscie cyfrowe %1").arg(i)).toString();
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


    ekran1.maxAceton = settings.value(QString("liquid/Aceton/max"), 10.0).toDouble();
    ekran1.maxEtanol = settings.value(QString("liquid/Etanol/max"), 10.0).toDouble();
    //maxOlej_napedowy;
    ekran1.maxIzopropanol = settings.value(QString("liquid/Izopropanol/max"), 10.0).toDouble();
    ekran1.maxBenzyna = settings.value(QString("liquid/Benzyna/max"), 10.0).toDouble();
    ekran1.maxToluen = settings.value(QString("liquid/Toluen/max"), 10.0).toDouble();

    ekran3.minCisnieniePomProz = settings.value(QString("cisnienie/minimum"), 85).toFloat();
    ekran3.downLevelHistPomProz = settings.value(QString("cisnienie/downLevel"), 1).toFloat();
    ekran3.upLevelHistPomProz = settings.value(QString("cisnienie/upLevel"), 1).toFloat();
    ekran3.firsTimeWaitPomProz = settings.value(QString("cisnienie/firstTime"),300).toUInt();
    ekran3.secondTimeWaitPomProz = settings.value(QString("cisnienie/secondTime"), 150).toUInt();
    ekran3.allTimeRunPomProz = settings.value(QString("cisnienie/maxTimeRun"), 1200).toULongLong();
    ekran3.numberHistPomProz = settings.value(QString("cisnienie/numberHisterez"), 3).toUInt();
    ekran3.minTimeBetweenRunPomProz = settings.value(QString("cisnienie/minTimeRun"), 100).toUInt();

    ekran4.minTimeAfterPowietrze = settings.value(QString("czasy/powietrze"), 15).toUInt();
    ekran4.minTimeAfterDozowanie = settings.value(QString("czasy/dozowanie"), 15).toUInt();

    pstezen.minTimeAfterPompaOff = settings.value(QString("czasy/cisnienie"), 30).toUInt();
    pstezen.minTimePompaMebramowa = settings.value(QString("czasy/pompamebramowa"), 30).toUInt();

    ekran6.minTimeZaplonIskra = settings.value(QString("czasy/maxCzekaniaIska"), 5).toUInt();
    ekran6.minTimeZaplonPlomien = settings.value(QString("czasy/maxCzekaniaPlomien"), 30).toUInt();
    ekran6.minDeltaCisnZaplon = settings.value(QString("cisnienie/deltaZaplon"), 5).toUInt();

    delayTimeTriggerPlomien = settings.value(QString("czasy/opoznieTrigerKameryPlomien"), 5000).toUInt();

    runTimePlomien = settings.value(QString("czasy/CzasPlomienia"), 6000).toUInt();
    runTimeIskraMechaniczna = settings.value(QString("czasy/CzasIskryMechanicznej"), 2000).toUInt();
    delayTimeIskraElektrycznaIskra = settings.value(QString("czasy/CzasIskryElektrcyznej"), 1000).toUInt();
    delayTimeIskraElektrycznaHV = settings.value(QString("czasy/IskraElektrcyznaWysokieNapiecie"), 2000).toUInt();
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
    settings.sync();

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

void Ustawienia::setWejscie(digitalIn id, const QString &name)
{
    wejscia[id] = name;
    settings.setValue(QString("wejsciacyfrowe/%1/name").arg((int)id), QVariant::fromValue(name));
    settings.sync();
}

QString Ustawienia::wejscie(digitalIn id) const
{
    return wejscia[id];
}

void Ustawienia::setWyjscie(digitalOut id, const QString &name)
{
    wyjscia[id] = name;
    settings.setValue(QString("wyjsciacyfrowe/%1/name").arg((int)id), QVariant::fromValue(name));
    settings.sync();
}

QString Ustawienia::wyjscie(digitalOut id) const
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
    settings.sync();
}

#define SET_CZUJKA(id, N, B, U, R, C, M, P) setCzujka(id, QString::fromUtf8(N), QString(B), QString(U), R, C, M, P)

void Ustawienia::initialSetting()
{
    if (settings.value("initial", false).toBool()) return;

    settings.setValue("initial",QVariant::fromValue(true));
    SET_CZUJKA(a_cisn_komora,   "Ci\305\233nienie w komorze", "kPa", "mBar", 0.04, 10, 0.05, 2);
    SET_CZUJKA(a_temp_komory,   "Temperatura w komorze", "st C", "st C", 0.05, 1, 0.05, 2);
    SET_CZUJKA(a_temp_parownik, "Temperatura parownika", "st C", "st C", 0.05, 1, 0.05, 2);
    SET_CZUJKA(a_voc1,          "St\304\231\305\274enie VOC 1", "%", "%", 3.03, 1, 0.05, 2);
    SET_CZUJKA(a_voc2,          "St\304\231\305\274enie VOC 2", "%", "%", 3.846, 1, 0.05, 2);
    SET_CZUJKA(a_8,             "Wirtualny czujnik", "%", "%", 3.03, 1, 0.05, 2);
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
    settings.sync();

    setMaxAceton(10.0);
    setMaxEtanol(10.0);
    setMaxIzopropanol(10.0);
    setMaxBenzyna(10.0);
    setMaxToluen(10.0);

    setMinCisnieniePomProz(85); //kPa
    setDownLevelHistPomProz(1);
    setUpLevelHistPomProz(1);
    setFirsTimeWaitPomProz(300);
    setSecondTimeWaitPomProz(150);
    setAllTimeRunPomProz(1200);
    setNumberHistPomProz(3);
    setMinTimeBetweenRunPomProz(100);

    setMinTimeAfterPowietrze(15);
    setMinTimeAfterDozowanie(15);

    setMinTimeAfterPompaOff(30);
    setMinTimePompaMebramowa(30);

    setMinTimeZaplonIskra(5);
    setMinTimeZaplonPlomien(30);
    setMinTimeZaplonPlomien(5);

    setDelayTimeTriggerPlomien(5000);

    setRunTimePlomien(6000);
    setRunTimeIskraMechaniczna(2000);

    setDelayTimeIskraElektrycznaHV(2000);
    setDelayTimeIskraElektrycznaIskra(1000);

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

unsigned int Ustawienia::getRunTimePlomien() const
{
    return runTimePlomien;
}

void Ustawienia::setRunTimePlomien(unsigned int newRunTimePlomien)
{
    runTimePlomien = newRunTimePlomien;
    settings.setValue(QString("czasy/CzasPlomienia"), QVariant::fromValue(newRunTimePlomien));
    settings.sync();
}

unsigned int Ustawienia::getRunTimeIskraMechaniczna() const
{
    return runTimeIskraMechaniczna;
}

void Ustawienia::setRunTimeIskraMechaniczna(unsigned int newRunTimeIskraMechaniczna)
{
    runTimeIskraMechaniczna = newRunTimeIskraMechaniczna;
    settings.setValue(QString("czasy/CzasIskryMechanicznej"), QVariant::fromValue(newRunTimeIskraMechaniczna));
    settings.sync();
}

unsigned int Ustawienia::getDelayTimeIskraElektrycznaIskra() const
{
    return delayTimeIskraElektrycznaIskra;
}

void Ustawienia::setDelayTimeIskraElektrycznaIskra(unsigned int newDelayTimeIskraElektrycznaIskra)
{
    delayTimeIskraElektrycznaIskra = newDelayTimeIskraElektrycznaIskra;
    settings.setValue(QString("czasy/CzasIskryElektrcyznej"), QVariant::fromValue(newDelayTimeIskraElektrycznaIskra));
    settings.sync();
}

unsigned int Ustawienia::getDelayTimeIskraElektrycznaHV() const
{
    return delayTimeIskraElektrycznaHV;
}

void Ustawienia::setDelayTimeIskraElektrycznaHV(unsigned int newDelayTimeIskraElektrycznaHV)
{
    delayTimeIskraElektrycznaHV = newDelayTimeIskraElektrycznaHV;
    settings.setValue(QString("czasy/IskraElektrcyznaWysokieNapiecie"), QVariant::fromValue(newDelayTimeIskraElektrycznaHV));
    settings.sync();
}

unsigned int Ustawienia::getDelayTimeTriggerPlomien() const
{
    return delayTimeTriggerPlomien;
}

void Ustawienia::setDelayTimeTriggerPlomien(unsigned int newDelayTimeTriggerPlomien)
{
    delayTimeTriggerPlomien = newDelayTimeTriggerPlomien;
    settings.setValue(QString("czasy/opoznieTrigerKameryPlomien"), QVariant::fromValue(newDelayTimeTriggerPlomien));
    settings.sync();
}

unsigned int Ustawienia::getMinTimeZaplonIskra() const
{
    return ekran6.minTimeZaplonIskra;
}

void Ustawienia::setMinTimeZaplonIskra(unsigned int newMinTimeZaplonIskra)
{
    ekran6.minTimeZaplonIskra = newMinTimeZaplonIskra;
    settings.setValue(QString("czasy/maxCzekaniaIska"), QVariant::fromValue(newMinTimeZaplonIskra));
    settings.sync();
}

unsigned int Ustawienia::getMinTimeZaplonPlomien() const
{
    return ekran6.minTimeZaplonPlomien;
}

void Ustawienia::setMinTimeZaplonPlomien(unsigned int newMinTimeZaplonPlomien)
{
    ekran6.minTimeZaplonPlomien = newMinTimeZaplonPlomien;
    settings.setValue(QString("czasy/maxCzekaniaPlomien"), QVariant::fromValue(newMinTimeZaplonPlomien));
    settings.sync();
}

float Ustawienia::getMinDeltaCisnZaplon() const
{
    return ekran6.minDeltaCisnZaplon;
}

void Ustawienia::setMinDeltaCisnZaplon(float newMinDeltaCisnZaplon)
{
    ekran6.minDeltaCisnZaplon = newMinDeltaCisnZaplon;
    settings.setValue(QString("cisnienie/deltaZaplon"), QVariant::fromValue(newMinDeltaCisnZaplon));
    settings.sync();
}

const UEkran6 &Ustawienia::getEkran6() const
{
    return ekran6;
}

void Ustawienia::setEkran6(const UEkran6 &newEkran6)
{
    setMinDeltaCisnZaplon(newEkran6.minDeltaCisnZaplon);
    setMinTimeZaplonPlomien(newEkran6.minTimeZaplonPlomien);
    setMinTimeZaplonIskra(newEkran6.minTimeZaplonIskra);
}

unsigned int Ustawienia::getMinTimeAfterPompaOff() const
{
    return pstezen.minTimeAfterPompaOff;
}

void Ustawienia::setMinTimeAfterPompaOff(unsigned int newMinTimeAfterPompaOff)
{
    pstezen.minTimeAfterPompaOff = newMinTimeAfterPompaOff;
    settings.setValue(QString("czasy/cisnienie"), QVariant::fromValue(newMinTimeAfterPompaOff));
    settings.sync();
}

unsigned int Ustawienia::getMinTimePompaMebramowa() const
{
    return pstezen.minTimePompaMebramowa;
}

void Ustawienia::setMinTimePompaMebramowa(unsigned int newMinTimePompaMebramowa)
{
    pstezen.minTimePompaMebramowa = newMinTimePompaMebramowa;
    settings.setValue(QString("czasy/pompamebramowa"), QVariant::fromValue(newMinTimePompaMebramowa));
    settings.sync();
}

const UPomiarStezen &Ustawienia::getPstezen() const
{
    return pstezen;
}

void Ustawienia::setPstezen(const UPomiarStezen &newPstezen)
{
    pstezen = newPstezen;
}

unsigned int Ustawienia::getMinTimeAfterPowietrze() const
{
    return ekran4.minTimeAfterPowietrze;
}

void Ustawienia::setMinTimeAfterPowietrze(unsigned int newMinTimeAfterPowietrze)
{
    ekran4.minTimeAfterPowietrze = newMinTimeAfterPowietrze;
    settings.setValue(QString("czasy/powietrze"), QVariant::fromValue(newMinTimeAfterPowietrze));
    settings.sync();
}

const UEkran4 &Ustawienia::getEkran4() const
{
    return ekran4;
}

void Ustawienia::setEkran4(const UEkran4 &newEkran4)
{
    setMinTimeAfterPowietrze(newEkran4.minTimeAfterPowietrze);
    setMinTimeAfterDozowanie(newEkran4.minTimeAfterDozowanie);
}

unsigned int Ustawienia::getMinTimeAfterDozowanie() const
{
    return ekran4.minTimeAfterDozowanie;
}

void Ustawienia::setMinTimeAfterDozowanie(unsigned int newMinTimeAfterDozowanie)
{
    ekran4.minTimeAfterDozowanie = newMinTimeAfterDozowanie;
    settings.setValue(QString("czasy/dozowanie"), QVariant::fromValue(newMinTimeAfterDozowanie));
    settings.sync();
}

unsigned int Ustawienia::getMinTimeBetweenRunPomProz() const
{
    return ekran3.minTimeBetweenRunPomProz;
}

void Ustawienia::setMinTimeBetweenRunPomProz(unsigned int newMinTimeBetweenRunPomProz)
{
    ekran3.minTimeBetweenRunPomProz = newMinTimeBetweenRunPomProz;
    settings.setValue(QString("cisnienie/minTimeRun"), QVariant::fromValue(newMinTimeBetweenRunPomProz));
    settings.sync();
}

unsigned short Ustawienia::getNumberHistPomProz() const
{
    return ekran3.numberHistPomProz;
}

void Ustawienia::setNumberHistPomProz(unsigned short newNumberHistPomProz)
{
    ekran3.numberHistPomProz = newNumberHistPomProz;
    settings.setValue(QString("cisnienie/numberHisterez"), QVariant::fromValue(newNumberHistPomProz));
    settings.sync();
}

unsigned long Ustawienia::getAllTimeRunPomProz() const
{
    return ekran3.allTimeRunPomProz;
}

void Ustawienia::setAllTimeRunPomProz(unsigned long newAllTimeRunPomProz)
{
    ekran3.allTimeRunPomProz = newAllTimeRunPomProz;
    settings.setValue(QString("cisnienie/maxTimeRun"), QVariant::fromValue(newAllTimeRunPomProz));
    settings.sync();
}

unsigned int Ustawienia::getSecondTimeWaitPomProz() const
{
    return ekran3.secondTimeWaitPomProz;
}

void Ustawienia::setSecondTimeWaitPomProz(unsigned int newSecondTimeWaitPomProz)
{
    ekran3.secondTimeWaitPomProz = newSecondTimeWaitPomProz;
    settings.setValue(QString("cisnienie/secondTime"), QVariant::fromValue(newSecondTimeWaitPomProz));
    settings.sync();
}

unsigned int Ustawienia::getFirsTimeWaitPomProz() const
{
    return ekran3.firsTimeWaitPomProz;
}

void Ustawienia::setFirsTimeWaitPomProz(unsigned int newFirsTimeWaitPomProz)
{
    ekran3.firsTimeWaitPomProz = newFirsTimeWaitPomProz;
    settings.setValue(QString("cisnienie/firstTime"), QVariant::fromValue(newFirsTimeWaitPomProz));
    settings.sync();
}

float Ustawienia::getUpLevelHistPomProz() const
{
    return ekran3.upLevelHistPomProz;
}

void Ustawienia::setUpLevelHistPomProz(float newUpLevelHistPomProz)
{
    ekran3.upLevelHistPomProz = newUpLevelHistPomProz;
    settings.setValue(QString("cisnienie/upLevel"), QVariant::fromValue(newUpLevelHistPomProz));
    settings.sync();
}

const UEkran3 &Ustawienia::getEkran3() const
{
    return ekran3;
}

void Ustawienia::setEkran3(const UEkran3 &newEkran3)
{
    setMinCisnieniePomProz(newEkran3.minCisnieniePomProz);
    setDownLevelHistPomProz(newEkran3.downLevelHistPomProz);
    setUpLevelHistPomProz(newEkran3.upLevelHistPomProz);
    setFirsTimeWaitPomProz(newEkran3.firsTimeWaitPomProz);
    setSecondTimeWaitPomProz(newEkran3.secondTimeWaitPomProz);
    setAllTimeRunPomProz(newEkran3.allTimeRunPomProz);
    setNumberHistPomProz(newEkran3.numberHistPomProz);
    setMinTimeBetweenRunPomProz(newEkran3.minTimeBetweenRunPomProz);
}

float Ustawienia::getDownLevelHistPomProz() const
{
    return ekran3.downLevelHistPomProz;
}

void Ustawienia::setDownLevelHistPomProz(float newDownLevelHistPomProz)
{
    ekran3.downLevelHistPomProz = newDownLevelHistPomProz;
    settings.setValue(QString("cisnienie/downLevel"), QVariant::fromValue(newDownLevelHistPomProz));
    settings.sync();
}

float Ustawienia::getMinCisnieniePomProz() const
{
    return ekran3.minCisnieniePomProz;
}

void Ustawienia::setMinCisnieniePomProz(float newMinCisnieniePomProz)
{
    ekran3.minCisnieniePomProz = newMinCisnieniePomProz;
    settings.setValue(QString("cisnienie/minimum"), QVariant::fromValue(newMinCisnieniePomProz));
    settings.sync();
}

const UEkran1 &Ustawienia::getEkran1() const
{
    return ekran1;
}

void Ustawienia::setEkran1(const UEkran1 &newEkran1)
{
    setMaxAceton(newEkran1.maxAceton);
    setMaxEtanol(newEkran1.maxEtanol);
    setMaxIzopropanol(newEkran1.maxIzopropanol);
    setMaxBenzyna(newEkran1.maxBenzyna);
    setMaxToluen(newEkran1.maxToluen);
}

float Ustawienia::getMaxToluen() const
{
    return ekran1.maxToluen;
}

void Ustawienia::setMaxToluen(float newMaxToluen)
{
    ekran1.maxToluen = newMaxToluen;
    settings.setValue(QString("liquid/Toluen/max"), QVariant::fromValue(newMaxToluen));
    settings.sync();
}

float Ustawienia::getMaxBenzyna() const
{
    return ekran1.maxBenzyna;
}

void Ustawienia::setMaxBenzyna(float newMaxBenzyna)
{
    ekran1.maxBenzyna = newMaxBenzyna;
    settings.setValue(QString("liquid/Benzyna/max"), QVariant::fromValue(newMaxBenzyna));
    settings.sync();
}

float Ustawienia::getMaxIzopropanol() const
{
    return ekran1.maxIzopropanol;
}

void Ustawienia::setMaxIzopropanol(float newMaxIzopropanol)
{
    ekran1.maxIzopropanol = newMaxIzopropanol;
    settings.setValue(QString("liquid/Izopropanol/max"), QVariant::fromValue(newMaxIzopropanol));
    settings.sync();
}

float Ustawienia::getMaxEtanol() const
{
    return ekran1.maxEtanol;
}

void Ustawienia::setMaxEtanol(float newMaxEtanol)
{
    ekran1.maxEtanol = newMaxEtanol;
    settings.setValue(QString("liquid/Etanol/max"), QVariant::fromValue(newMaxEtanol));
    settings.sync();
}

float Ustawienia::getMaxAceton() const
{
    return ekran1.maxAceton;
}

void Ustawienia::setMaxAceton(float newMaxAceton)
{
    ekran1.maxAceton = newMaxAceton;
    settings.setValue(QString("liquid/Aceton/max"), QVariant::fromValue(newMaxAceton));
    settings.sync();
}

unsigned short Ustawienia::getNrInitializeCycles() const
{
    return nrInitializeCycles;
}

void Ustawienia::setNrInitializeCycles(unsigned short newNrInitializeCycles)
{
    nrInitializeCycles = newNrInitializeCycles;
    settings.setValue(QString("dozownik/numberOfInitializeCycles"), QVariant::fromValue(newNrInitializeCycles));
    settings.sync();
}

double Ustawienia::getStepsOnMl() const
{
    return stepsOnMl;
}

void Ustawienia::setStepsOnMl(double newStepsOnMl)
{
    stepsOnMl = newStepsOnMl;
    settings.setValue(QString("dozownik/stepsOnMl"), QVariant::fromValue(newStepsOnMl));
    settings.sync();
}

int Ustawienia::getImpTime() const
{
    return impTime;
}

void Ustawienia::setImpTime(int newImpTime)
{
    impTime = newImpTime;
    settings.setValue(QString("dozownik/impMotorTime"), QVariant::fromValue(newImpTime));
    settings.sync();
}
