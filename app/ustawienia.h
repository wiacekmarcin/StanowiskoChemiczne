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
    i_drzwi_prawe     = 0x001, //P2.0
    i_wentylacja_lewa = 0x002, //P2.1
    i_proznia         = 0x004, //P2.2
    i_pom_stez_1      = 0x008, //P2.3
    i_drzwi_lewe      = 0x010, //p2.4
    i_wentylacja_prawa= 0x020, //P2.5
    i_wlot_powietrza  = 0x040, //P2.6
    i_pom_stez_2      = 0x080, //P2.7
    i_pilot           = 0x100, //P1.4
} digitalIn;

typedef enum _oDig {
    o_hv_onoff        = 0x001, //P0.0 X1
    o_hv_bezpiecznik  = 0x002, //P0.1 X2
    o_hv_iskra        = 0x004, //P0.2 X3
    o_mech_iskra      = 0x008, //P0.3 X4
    o_grzalka         = 0x010, //P0.4 X5
    o_pompa_prozniowa = 0x020, //P0.5 X6
    o_pompa_powietrza = 0x040, //P0.6 X7
    o_wentylator      = 0x080, //P0.7 X8
    o_mieszadlo       = 0x100, //P1.0 X9
    o_unknown         = 0x200, //P1.1 X10 - nie podlaczone
    o_trigger         = 0x400, //P1.2

} digitalOut;

typedef enum _zaplon {
    z_iskra_elektryczna = 1,
    z_iskra_mechaniczna = 2,
    z_iskra_plomien     = 3
} ZaplonRodzaj;

#define PLOMIEN QString("Płomień")
#define ISKRA_MECH QString("Iskra mechaniczna")
#define ISKRA_ELE QString("Iskra elektryczna")


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
