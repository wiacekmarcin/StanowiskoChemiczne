#ifndef TESTDATA_H
#define TESTDATA_H
#include <QString>

class TestData
{
public:
    TestData() {}
    TestData(const QString & nazwa, short doz, const QString & c, const double & obj, const QString & zaplon1,
             const QString zaplon2 );

    unsigned short getDozownik() const;
    void setDozownik(unsigned short value);

    QString getCiecz() const;
    void setCiecz(const QString &value);

    double getObjetosc() const;
    void setObjetosc(double value);

    QString getZaplon() const;
    void setZaplon(const QString &value);

    QString getZaplonExt() const;
    void setZaplonExt(const QString &value);

    QString getNazwa() const;
    void setNazwa(const QString &value);

    unsigned short getNrCykliInit() const;
    void setNrCykliInit(unsigned short newNrCykliInit);

private:
    unsigned short dozownik;
    QString ciecz;
    double objetosc;
    QString zaplon;
    QString zaplon_ext;
    QString nazwa;
    unsigned short nrCykliInit;

};

#endif // TESTDATA_H
