#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include <QString>
#include <QSettings>
#include <QMap>
typedef enum _aInput {
    a_voc1              = 0,
    a_voc2              = 1,
    a_o2                = 2,
    a_co2               = 3,
    a_cisn_komora       = 4,
    a_temp_komory       = 5,
    a_temp_parownik     = 6,
    a_8                 = 7
} analogIn;

typedef enum _iDig {
    i_drzwi_lewe      = 0x001, //P2.0
    i_wentylacja_lewa = 0x002, //P2.1
    i_proznia         = 0x004, //P2.2
    i_pom_stez_1      = 0x008, //P2.3
    i_drzwi_prawe     = 0x010, //p2.4
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
    o_trigger         = 0x400, //P1.2 X11

} digitalOut;

typedef enum _zaplon {
    z_iskra_elektryczna = 1,
    z_iskra_mechaniczna = 2,
    z_iskra_plomien     = 3
} ZaplonRodzaj;

typedef enum _kindUser {
    U_NONE  = 0,
    U_STUDENT = 0x1,
    U_ADMIN = 0x3,
    U_SERVISANT = 0x83,
} UserPrivilige;

#define PLOMIEN QString("Płomień")
#define ISKRA_MECH QString("Iskra mechaniczna")
#define ISKRA_ELE QString("Iskra elektryczna")


#define SETGET_REVERSE(N) bool getReverse_##N() const; void setReverse_##N(bool newReverse);
#define REVERSE(N) bool reverse_##N;

struct UEkran1 {
    float maxAceton;
    float maxEtanol;
    //float maxOlej_napedowy;
    float maxIzopropanol;
    float maxBenzyna;
    float maxToluen;
};

struct UEkran3 {
    float minCisnieniePomProz;
    float downLevelHistPomProz;
    float upLevelHistPomProz;
    unsigned int firsTimeWaitPomProz;
    unsigned int secondTimeWaitPomProz;
    unsigned long allTimeRunPomProz;
    unsigned short numberHistPomProz;
    unsigned int minTimeBetweenRunPomProz;
};

struct UEkran4 {
    unsigned int minTimeAfterDozowanie;
    unsigned int minTimeAfterPowietrze;
};

struct UPomiarStezen {
    unsigned int minTimePompaMebramowa;
    unsigned int minTimeAfterPompaOff;
};

struct UEkran6 {
    float minDeltaCisnZaplon;
    unsigned int minTimeZaplonPlomien;
    unsigned int minTimeZaplonIskra;
};

class Ustawienia
{
public:
    Ustawienia();

    typedef struct _czujnikAnalogowy {
        QString name;
        QString unit;
        double ratio;
        QString baseUnit;
        double convert;
        double minVal; //minimalna wartosc powyzej ktorej sprawdzana jest stabilnosc
        double percentStab; //procent ponizej ktorego stabilna jest wartosc
    } CzujnikAnalogowy;

    void setCzujka(analogIn id, const CzujnikAnalogowy &czAnal);
    CzujnikAnalogowy getCzujnikAnalogowyUstawienia(analogIn id) const;
    QString getCzAnalName(analogIn id) const;
    double getRatio(analogIn id) const;

    static constexpr int maxCzujekAnal = 8;
    static constexpr int maxCzujekCyfrIn = 9;
    static constexpr int maxCzujekCyfrOut = 11;

    void setWejscie(digitalIn id, const QString & name);
    QString wejscie(digitalIn id) const;

    void setWyjscie(digitalOut id, const QString & name);
    QString wyjscie(digitalOut id) const;

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

    float getMaxAceton() const;
    void setMaxAceton(float newMaxAceton);

    float getMaxEtanol() const;
    void setMaxEtanol(float newMaxEtanol);

    float getMaxIzopropanol() const;
    void setMaxIzopropanol(float newMaxIzopropanol);

    float getMaxBenzyna() const;
    void setMaxBenzyna(float newMaxBenzyna);

    float getMaxToluen() const;
    void setMaxToluen(float newMaxToluen);

    const UEkran1 &getEkran1() const;
    void setEkran1(const UEkran1 &newEkran1);

    float getMinCisnieniePomProz() const;
    void setMinCisnieniePomProz(float newMinCisnieniePomProz);

    float getDownLevelHistPomProz() const;
    void setDownLevelHistPomProz(float newDownLevelHistPomProz);

    const UEkran3 &getEkran3() const;
    void setEkran3(const UEkran3 &newEkran3);

    float getUpLevelHistPomProz() const;
    void setUpLevelHistPomProz(float newUpLevelHistPomProz);

    unsigned int getFirsTimeWaitPomProz() const;
    void setFirsTimeWaitPomProz(unsigned int newFirsTimeWaitPomProz);

    unsigned int getSecondTimeWaitPomProz() const;
    void setSecondTimeWaitPomProz(unsigned int newSecondTimeWaitPomProz);

    unsigned long getAllTimeRunPomProz() const;
    void setAllTimeRunPomProz(unsigned long newAllTimeRunPomProz);

    unsigned short getNumberHistPomProz() const;
    void setNumberHistPomProz(unsigned short newNumberHistPomProz);

    unsigned int getMinTimeBetweenRunPomProz() const;
    void setMinTimeBetweenRunPomProz(unsigned int newMinTimeBetweenRunPomProz);

    unsigned int getMinTimeAfterDozowanie() const;
    void setMinTimeAfterDozowanie(unsigned int newMinTimeAfterDozowanie);

    const UEkran4 &getEkran4() const;
    void setEkran4(const UEkran4 &newEkran4);

    unsigned int getMinTimeAfterPowietrze() const;
    void setMinTimeAfterPowietrze(unsigned int newMinTimeAfterPowietrze);

    const UPomiarStezen &getPstezen() const;
    void setPstezen(const UPomiarStezen &newPstezen);

    unsigned int getMinTimePompaMebramowa() const;
    void setMinTimePompaMebramowa(unsigned int newMinTimePompaMebramowa);

    unsigned int getMinTimeAfterPompaOff() const;
    void setMinTimeAfterPompaOff(unsigned int newMinTimeAfterPompaOff);

    const UEkran6 &getEkran6() const;
    void setEkran6(const UEkran6 &newEkran6);

    float getMinDeltaCisnZaplon() const;
    void setMinDeltaCisnZaplon(float newMinDeltaCisnZaplon);

    unsigned int getMinTimeZaplonPlomien() const;
    void setMinTimeZaplonPlomien(unsigned int newMinTimeZaplonPlomien);

    unsigned int getMinTimeZaplonIskra() const;
    void setMinTimeZaplonIskra(unsigned int newMinTimeZaplonIskra);

protected:
    void initialSetting();
    void setCzujka(analogIn id, const QString & name, const QString & baseUnit, const QString & unit,
                   const double & ratio, const double & convert, const double & minVal, const double & percent);

private:

    CzujnikAnalogowy czujki[maxCzujekAnal];
    QMap<digitalIn, QString> wejscia;
    QMap<digitalOut, QString> wyjscia;
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


    UEkran1 ekran1;
    UEkran3 ekran3;
    UEkran4 ekran4;
    UPomiarStezen pstezen;

    UEkran6 ekran6;




};

#endif // USTAWIENIA_H
