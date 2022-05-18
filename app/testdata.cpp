#include "testdata.h"

#include <QDebug>
#include <QFloat16>
#include <QDataStream>
TestData::TestData()
{

}

void TestData::setTemperaturaKomory(FazaTestu ft, const float &temp)
{
    emit debug(QString("Ustaw Temp ft = %1 temp = %2").arg(ft).arg(temp));
    switch(ft) {
    case FT_poczatek :  tempKomoraPoczatek = temp; break;
    case FT_koniec:     tempKomoraKoniec = temp; break;
    case FT_dozowanie:  proby.last().tempKomoryDozowanie = temp; break;
    case FT_przedZaplon:  proby.last().tempKomoryZaplon = temp; break;
    case FT_zaplon: proby.last().tempKomoryZaplon = temp; break;
    default :  qInfo () << "Invalid ft=" << ft ; break ;
    }
}

void TestData::setCisnienieKomory(FazaTestu ft, const float &cisn)
{
    emit debug(QString("Ustaw ciśnienie ft = %1 cisn = %2").arg(ft).arg(cisn));
    switch(ft) {
    case FT_koniec: cisnienieKoniec = cisn; break;
    case FT_dozowanie: proby.last().cisnKomoryDozowanie = cisn; break;
    case FT_przedZaplon: proby.last().cisnKomoryZaplon = cisn; break;
    case FT_zaplon: proby.last().cisnKomoryZaplon = cisn; break;
    default :  qInfo () << "Invalid ft=" << ft ; break ;
    }
}

void TestData::setStezenia(FazaTestu ft, const float &voc1, const float &voc2, const float &o2, const float &co2, const float &cz8)
{
    emit debug(QString("Ustaw stezenia ft = %1 %2,%3,%4,%5,%6").arg(ft).arg(voc1).arg(voc2).arg(o2).arg(co2).arg(cz8));
    switch(ft) {
    case FT_koniec: {
        this->voc1 = voc1;
        this->voc2 = voc2;
        this->o2 = o2;
        this->co2 = co2;
        this->cz8 = cz8;
        break;
    }
    case FT_przedZaplon:
    {
        proby.last().voc1 = voc1;
        proby.last().voc2 = voc2;
        proby.last().o2 = o2;
        proby.last().co2 = co2;
        proby.last().cz8 = cz8;
        break;
    }
    default : qInfo () << "Invalid ft=" << ft ;  break ;
    
    }
}

float TestData::getCz8() const
{
    return cz8;
}

float TestData::getCo2() const
{
    return co2;
}

float TestData::getO2() const
{
    return o2;
}

float TestData::getVoc2() const
{
    return voc2;
}

float TestData::getVoc1() const
{
    return voc1;
}

float TestData::getCisnienieKoniec() const
{
    return cisnienieKoniec;
}

float TestData::getTempKomoraKoniec() const
{
    return tempKomoraKoniec;
}

const QList<float> &TestData::getIloscCieczy() const
{
    return iloscCieczy;
}

const QList<ProbaType> &TestData::getProby() const
{
    return proby;
}

float TestData::getWilgotnosc() const
{
    return wilgotnosc;
}

float TestData::getTempKomoraPoczatek() const
{
    return tempKomoraPoczatek;
}

const QString &TestData::getNazwaCieczy() const
{
    return nazwaCieczy;
}

const QStringList &TestData::getUczestnicy() const
{
    return uczestnicy;
}

const QDateTime &TestData::getDataTestu() const
{
    return dataTestu;
}

void TestData::setUdanaProba(bool success, bool powtarzaneDozowanie, bool powtarzanyZaplon)
{
    emit debug(QString("Udana proba = %1").arg(success));
    proby.last().success = success;
    proby.last().iloscCalkowitaCieczy = 0;
    foreach(auto doz, iloscCieczy) {
        proby.last().iloscCalkowitaCieczy += doz;
    }
    if (!success) {
        proby.append(ProbaType());
        proby.last().powtarzaneDozowanie = powtarzaneDozowanie;
        proby.last().powtarzanyZaplon =  powtarzanyZaplon;
    }
}

void TestData::setStopTest(const QTime &newStopTest)
{
    stopTest = newStopTest;
}

void TestData::setStartTest(const QTime &newStartTest)
{
    startTest = newStartTest;
}

const QString &TestData::getNazwaTestu() const
{
    return nazwaTestu;
}

void TestData::setNazwaTestu(const QString &newNazwaTestu)
{
    nazwaTestu = newNazwaTestu;
}

void TestData::addValues(float voc1, float voc2, float o2, float co2, float a8, float tempPar, float tempKom, float cisnKom)
{
    AnalValType v;
    v.voc1 = voc1;
    v.voc2 = voc2;
    v.o2 = o2;
    v.co2 = co2;
    v.a8 = a8;
    v.tempKom = tempKom;
    v.tempPar = tempPar;
    v.cisnKom = cisnKom;
    values.append(v);
}


QDataStream & operator<<(QDataStream & ds, const TestData & item)
{
    qInfo() << "TestData begin";
    qInfo() << item.dataTestu.toString() << ",[";
    foreach (auto u, item.uczestnicy)
        qInfo() << "Ucz:" << u;
    qInfo() << "]," << item.nazwaCieczy << "," << item.tempKomoraPoczatek << "," << item.wilgotnosc << "," << item.proby.size();
    foreach(auto p, item.proby) {
        qInfo() << "p:[" << p.cisnKomoryDozowanie << "," << p.cisnKomoryZaplon << "," << p.co2 << "," << p.cz8 << "," << p.koncentracjaPar << ",";
        qInfo() << p.o2 << "," << p.powtarzaneDozowanie << "," << p.powtarzanyZaplon << "," << p.success << "," << p.tempKomoryDozowanie << ",";
        qInfo() << p.tempKomoryZaplon << "," << p.tempParownikaDozowanie << "," << p.voc1 << "," << p.voc2 << "," << p.zlaKoncetracja << ",";
        qInfo() << p.zrodloZaplonu << "," << p.iloscCalkowitaCieczy << "]";
    }
    qInfo() << "Ciezcz(" << item.iloscCieczy.size() << " :[";
    foreach (auto c, item.iloscCieczy) {
        qInfo() << c << ",";
    }
    qInfo() << "]," << item.tempKomoraKoniec << "," <<  item.cisnienieKoniec << ",";
    qInfo() << item.voc1 << "," << item.voc2 << "," << item.o2 << "," << item.co2 << "," << item.cz8 << ",";
    qInfo() << item.nazwaTestu << "values(" << item.values.size() << ")=[";
    foreach(auto v, item.values) {
        qInfo() << "(" << v.a8 << "," << v.o2 << "," << v.co2 << "," << v.voc1 << "," << v.voc2 << "," << v.cisnKom << "," << v.tempKom << "," << v.tempKom << "," << v.tempPar << "),";
    }
    qInfo() << "]," << item.startTest.toString() << "," << item.stopTest.toString();


    ds << item.dataTestu;
    ds << item.uczestnicy;
    ds << item.nazwaCieczy;
    ds << item.tempKomoraPoczatek;
    ds << item.wilgotnosc;
    ds << item.proby;
    ds << item.iloscCieczy;
    ds << item.tempKomoraKoniec;
    ds << item.cisnienieKoniec;
    ds << item.voc1;
    ds << item.voc2;
    ds << item.o2;
    ds << item.co2;
    ds << item.cz8;
    ds << item.nazwaTestu;
    ds << item.values;
    ds << item.startTest;
    ds << item.stopTest;

    qInfo() << "TestData end";
    return ds;
}

QDataStream & operator>>(QDataStream & ds, TestData & item)
{
    qInfo() << "TestData begin";
    ds >> item.dataTestu;
    ds >> item.uczestnicy;
    ds >> item.nazwaCieczy;
    ds >> item.tempKomoraPoczatek;
    ds >> item.wilgotnosc;
    ds >> item.proby;
    ds >> item.iloscCieczy;
    ds >> item.tempKomoraKoniec;
    ds >> item.cisnienieKoniec;
    ds >> item.voc1;
    ds >> item.voc2;
    ds >> item.o2;
    ds >> item.co2;
    ds >> item.cz8;
    ds >> item.nazwaTestu;
    ds >> item.values;
    ds >> item.startTest;
    ds >> item.stopTest;


    qInfo() << item.dataTestu.toString() << ",[";
    foreach (auto u, item.uczestnicy)
        qInfo() << "Ucz:" << u;
    qInfo() << "]," << item.nazwaCieczy << "," << item.tempKomoraPoczatek << "," << item.wilgotnosc << "," << item.proby.size();
    foreach(auto p, item.proby) {
        qInfo() << "p:[" << p.cisnKomoryDozowanie << "," << p.cisnKomoryZaplon << "," << p.co2 << "," << p.cz8 << "," << p.koncentracjaPar << ",";
        qInfo() << p.o2 << "," << p.powtarzaneDozowanie << "," << p.powtarzanyZaplon << "," << p.success << "," << p.tempKomoryDozowanie << ",";
        qInfo() << p.tempKomoryZaplon << "," << p.tempParownikaDozowanie << "," << p.voc1 << "," << p.voc2 << "," << p.zlaKoncetracja << ",";
        qInfo() << p.zrodloZaplonu << "," << p.iloscCalkowitaCieczy << "]";
    }
    qInfo() << "Ciezcz(" << item.iloscCieczy.size() << " :[";
    foreach (auto c, item.iloscCieczy) {
        qInfo() << c << ",";
    }
    qInfo() << "]," << item.tempKomoraKoniec << "," <<  item.cisnienieKoniec << ",";
    qInfo() << item.voc1 << "," << item.voc2 << "," << item.o2 << "," << item.co2 << "," << item.cz8 << ",";
    qInfo() << item.nazwaTestu << "values(" << item.values.size() << ")=[";
    foreach(auto v, item.values) {
        qInfo() << "(" << v.a8 << "," << v.o2 << "," << v.co2 << "," << v.voc1 << "," << v.voc2 << "," << v.cisnKom << "," << v.tempKom << "," << v.tempPar << "),";
    }
    qInfo() << "]," << item.startTest.toString() << "," << item.stopTest.toString();


    qInfo() << "TestData end";
    return ds;
}

QDataStream & operator<<(QDataStream & ds, const ProbaType & item)
{
    ds << item.cisnKomoryDozowanie << item.cisnKomoryZaplon << item.co2 << item.cz8 << item.koncentracjaPar;
    ds << item.o2 << item.powtarzaneDozowanie << item.powtarzanyZaplon << item.success << item.tempKomoryDozowanie;
    ds << item.tempKomoryZaplon << item.tempParownikaDozowanie << item.voc1 << item.voc2 << item.zlaKoncetracja;
    ds << item.zrodloZaplonu << item.iloscCalkowitaCieczy;
    return ds;
}

QDataStream & operator>>(QDataStream & ds, ProbaType & item)
{
    ds >> item.cisnKomoryDozowanie >> item.cisnKomoryZaplon >> item.co2 >> item.cz8 >> item.koncentracjaPar;
    ds >> item.o2 >> item.powtarzaneDozowanie >> item.powtarzanyZaplon >> item.success >> item.tempKomoryDozowanie;
    ds >> item.tempKomoryZaplon >> item.tempParownikaDozowanie >> item.voc1 >> item.voc2 >> item.zlaKoncetracja;
    ds >> item.zrodloZaplonu >> item.iloscCalkowitaCieczy;
    return ds;
}

QDataStream & operator<<(QDataStream & ds, const AnalValType & item)
{
    ds << item.voc1 << item.voc2 << item.o2 << item.co2 << item.a8 << item.tempKom << item.tempPar << item.cisnKom;
    return ds;
}

QDataStream & operator>>(QDataStream & ds, AnalValType & item)
{
    ds >> item.voc1 >> item.voc2 >> item.o2 >> item.co2 >> item.a8 >> item.tempKom >> item.tempPar >> item.cisnKom;
    return ds;
}

