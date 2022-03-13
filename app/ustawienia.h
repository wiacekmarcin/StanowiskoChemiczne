#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include <QString>
#include <QSettings>

typedef enum _aInput {
    a_vol1 = 0,
    a_vol2,
    a_o2,
    a_co2,
    a_temp_parownik,
    a_temp_komory,
    a_cisn_komora,
    a_8
} analogIn;

typedef enum _iDig {
    drzwi_lewe      = 0x001, //P2.0
    wentylacja_lewa = 0x002, //P2.1
    proznia         = 0x004, //P2.2
    pom_stez_1      = 0x008, //P2.3
    drzwi_prawe     = 0x010, //p2.4
    wentylacja_prawa= 0x020, //P2.5
    wlot_powietrza  = 0x040, //P2.6
    pom_stez_2      = 0x080, //P2.7
    pilot           = 0x100, //
} digitalIn;

typedef enum _oDig {
    hv_onoff        = 0x001, //P0.0
    hv_zaplon       = 0x002, //P0.1
    hw_iskra        = 0x004, //P0.2
    mech_iskra      = 0x008, //PO.3
    plomien         = 0x010, //PO.4
    pompa_prozniowa = 0x020, //PO.5
    pompa_powietrza = 0x040, //PO.6
    wentylator      = 0x080, //PO.7
    mieszadlo       = 0x100, //P1.0
    trigger         = 0x200, //P1.1
} digitalOut;


void delay(unsigned int time);

class Ustawienia
{
public:
    Ustawienia();

    typedef struct _czujnikAnalogowy {
        QString name;
        QString unit;
        double ratio;
    } CzujnikAnalogowy;

    void setCzujka(short id, const QString &name, const QString &unit, const double &ratio);
    CzujnikAnalogowy getCzujka(short id) const;
    QString getName(short id) const;
    QString getUnit(short id) const;
    double getRatio(short id) const;

    static constexpr int maxCzujekAnal = 8;
    static constexpr int maxCzujekCyfrIn = 9;
    static constexpr int maxCzujekCyfrOut = 11;

    void setWejscie(int id, const QString & name);
    QString wejscie(int id) const;

    void setWyjscie(int id, const QString & name);
    QString wyjscie(int id) const;

    bool getReverseMotors() const;
    void setReverseMotors(bool newReverseMotors);

    int getMaxImp() const;
    void setMaxImp(int newMaxImp);

private:

    CzujnikAnalogowy czujki[maxCzujekAnal];
    QString wejscia[maxCzujekCyfrIn];
    QString wyjscia[maxCzujekCyfrOut];
    QSettings settings;
    bool reverseMotors;
    long maxImp;
};

#endif // USTAWIENIA_H
