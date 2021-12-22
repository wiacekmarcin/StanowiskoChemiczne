#ifndef TESTDATA_H
#define TESTDATA_H
#include <QString>
#include "createtestwizard.h"
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

    CreateTestWizard *getWizard();
    CreateTestWizard *createWizard(QWidget *parent);
    void setVisible(bool visible);


private:
    short dozownik;
    QString ciecz;
    double objetosc;
    QString zaplon;
    QString zaplon_ext;
    QString nazwa;

    CreateTestWizard * wizard;
};

#endif // TESTDATA_H
