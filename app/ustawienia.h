#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include <QString>
#include <QSettings>
#include <QMap>
typedef enum _aInput {
    a_vol1              = 0,
    a_vol2              = 1,
    a_o2                = 2,
    a_co2               = 3,
    a_cisn_komora       = 4,
    a_temp_komory       = 5,
    a_temp_parownik     = 6,
    a_8                 = 7
} analogIn;

typedef enum _iDig {
    drzwi_prawe     = 0x001, //P2.0
    wentylacja_lewa = 0x002, //P2.1
    proznia         = 0x004, //P2.2
    pom_stez_1      = 0x008, //P2.3
    drzwi_lewe      = 0x010, //p2.4
    wentylacja_prawa= 0x020, //P2.5
    wlot_powietrza  = 0x040, //P2.6
    pom_stez_2      = 0x080, //P2.7
    pilot           = 0x100, //P1.4
} digitalIn;

typedef enum _oDig {
    hv_onoff        = 0x001, //P0.0 X1
    hv_bezpieczenstwa = 0x002, //P0.1 X2
    hw_iskra        = 0x004, //P0.2 X3
    mech_iskra      = 0x008, //P0.3 X4
    plomien         = 0x010, //P0.4 X5
    pompa_prozniowa = 0x020, //P0.5 X6
    pompa_powietrza = 0x040, //P0.6 X7
    wentylator      = 0x080, //P0.7 X8
    mieszadlo       = 0x100, //P1.0 X9
    unknown         = 0x200, //P1.1 X10 - nie podlaczone
    trigger         = 0x400, //P1.2

} digitalOut;


void delay(unsigned int time);
void delayMs(unsigned int ms);

#define SETGET_REVERSE(N) bool getReverse_##N() const; void setReverse_##N(bool newReverse);
#define REVERSE(N) bool reverse_##N;


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

    SETGET_REVERSE(1)
    SETGET_REVERSE(2)
    SETGET_REVERSE(3)
    SETGET_REVERSE(4)
    SETGET_REVERSE(5)

    int getMaxImp() const;
    void setMaxImp(int newMaxImp);

    int getImpTime() const;
    void setImpTime(int newImpTime);

    double getStepsOnMl() const;
    void setStepsOnMl(double newStepsOnMl);

    unsigned short getNrInitializeCycles() const;
    void setNrInitializeCycles(unsigned short newNrInitializeCycles);

    double getCisnienieProzni() const;
    void setCisnienieProzni(double newCisnienieProzni);

protected:
    void initialSetting();

private:

    CzujnikAnalogowy czujki[maxCzujekAnal];
    QMap<unsigned int, QString> wejscia;
    QMap<unsigned int, QString> wyjscia;
    QSettings settings;
    REVERSE(1)
    REVERSE(2)
    REVERSE(3)
    REVERSE(4)
    REVERSE(5)
    long maxImp;
    int impTime;
    double stepsOnMl;
    unsigned short nrInitializeCycles;
    double cisnienieProzni;
};

#endif // USTAWIENIA_H
