#include "testdata.h"

TestData::TestData(const QString &nazwa_, short doz, const QString &c, const double &obj, const QString &zaplon1,
                   const QString zaplon2)
    : dozownik(doz), ciecz(c), objetosc(obj), zaplon(zaplon1), zaplon_ext(zaplon2), nazwa(nazwa_)
{
    wizard = nullptr;
}

unsigned short TestData::getDozownik() const
{
    return dozownik;
}

void TestData::setDozownik(unsigned short value)
{
    dozownik = value;
}

QString TestData::getCiecz() const
{
    return ciecz;
}

void TestData::setCiecz(const QString &value)
{
    ciecz = value;
}

double TestData::getObjetosc() const
{
    return objetosc;
}

void TestData::setObjetosc(double value)
{
    objetosc = value;
}

QString TestData::getZaplon() const
{
    return zaplon;
}

void TestData::setZaplon(const QString &value)
{
    zaplon = value;
}

QString TestData::getZaplonExt() const
{
    return zaplon_ext;
}

void TestData::setZaplonExt(const QString &value)
{
    zaplon_ext = value;
}

QString TestData::getNazwa() const
{
    return nazwa;
}

void TestData::setNazwa(const QString &value)
{
    nazwa = value;
}

CreateTestWizard *TestData::createWizard(QWidget *parent)
{
    wizard = new CreateTestWizard(nazwa,dozownik,ciecz,objetosc,zaplon,zaplon_ext,parent);
    return wizard;
}

CreateTestWizard *TestData::getWizard()
{
    return wizard;
}

void TestData::setVisible(bool visible)
{
    if (wizard)
        wizard->setVisible(visible);
}
