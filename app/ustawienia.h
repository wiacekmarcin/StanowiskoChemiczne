#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include <QString>
#include <QSettings>

typedef enum analoginIn {
    analog_1 = 1,
    analog_2,
    analog_3,
    analog_4,
    analog_5,
    analog_6,
    analog_7,
    analog_8,
} AnalogoweWejscia;

typedef enum digitalIn {
    kont_komora_A = 1,
    kont_komora_B,
    wentyl_1,
    wentyl_2,
    proznia,
    pom_stez_1,
    pom_stez_2,
    powietrze,
    pilot
} CyfroweWejscia;

typedef enum digitalOut {
    iskra_elektr_onoff = 1,
    iskra_elektr_hv,
    iskra_zaplon,
    iskra_mechaniczna,
    grzalka_onoff,
    pompa_prozniowa,
    pompa_mebramowa,
    wentylator,
    mieszadlo,
    kamera,
} CyfroweWyjscia;

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

    static constexpr int maxCzujek = 8;

    void setWejscie(int id, const QString & name);
    QString wejscie(int id) const;

    void setWyjscie(int id, const QString & name);
    QString wyjscie(int id) const;

private:

    CzujnikAnalogowy czujki[maxCzujek];
    QString wejscia[9];
    QString wyjscia[10];
    QSettings settings;
};

#endif // USTAWIENIA_H
