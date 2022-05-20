#ifndef TESTDATA_H
#define TESTDATA_H

#include "ustawienia.h"

#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QList>
#include <QPair>
#include <QObject>
#include <QDataStream>


typedef struct _stezenia {
    float voc1;
    float voc2;
    float o2;
    float co2;
    float a8;
} StezeniaType;

typedef struct _restParam {
    float tempPar;
    float tempKom;
    float cisnKom;
} OtherParamType;     

typedef struct _analValue : public StezeniaType, public OtherParamType {
    
    
} AnalValType;


typedef struct proba {

    bool success; //czy udana proba ekran 6/7
    QString zrodloZaplonu; //ekran 6
    bool powtarzanyZaplon; //ekran 7
    bool powtarzaneDozowanie; //ekran 7
    float cisnKomoryPoczatek;
    float tempParownikaDozowanie; //ekran dozowania 4?
    float tempKomoryDozowanie; //ekran dozowania 4?
    float cisnKomoryDozowanie; //ekran dozowania 4?(odczytana w momencie rozpoczęcia dozowania – w
                                // przypadku kiedy nie robimy podciśnienia. Kiedy robimy podciśnienie, to tą wartość odczytujemy na koniec
                                // kroku 3, kiedy zatwierdzamy, że pompa dobrze ustaliła podciśnienie)
    float tempKomoryZaplon;     //(odczytana w momencie zatwierdzenia ustabilizowania się odczytów z czujników stężenia)
    float cisnKomoryZaplon;     //(odczytana w momencie kiedy użytkownik potwierdzi,
                                //że cała ciecz odparowała – w przypadku kiedy nie robimy podciśnienia. Kiedy robiliśmy podciśnienie, to tą
                                //wartość odczytujemy po otwarciu zawory dodawania powietrza, kiedy użytkownik potwierdzi, że ciśnienie się
                                //ustabilizowało)
    float koncentracjaPar;      //wyliczona metodą ciśnień cząstkowych
    bool zlaKoncetracja;
    float voc1;                 //(odczytane na końcu kroku 5)
    float voc2;
    float o2;
    float co2;
    float cz8;

    float iloscCalkowitaCieczy; //suma do tej poruy dozowanej cieczy
    float calkowitaKoncetracjaPar;
    float P3;
    float P2;
    float P1;
    bool podcisnienie;
} ProbaType;


class TestData : public QObject
{
    Q_OBJECT
signals:
    void debug(const QString & d);
    void error(const QString & e);

public:
    TestData();

    typedef enum _faza {
        FT_P1,
        FT_P2,
        FT_P3,
        FT_poczatek, //ekran 2
        FT_dozowanie, //ekran 4
        FT_przedZaplon, //ekran 5
        FT_zaplon,    //ekran 6
        FT_koniec,   //ekran 8
    } FazaTestu;

    /** Funkcje ustawiające parametry **/
    void setDateTime(const QDateTime &dt) { dataTestu = dt; }
    void setMembers(const QStringList & memb) { uczestnicy = memb; }
    void setLiquidName(const QString & name) { nazwaCieczy = name; }
    void setHumanity(const float & hum) { wilgotnosc = hum; }
    void setTemperaturaKomoryWarunkiPoczatkowe(const float & temp) { setTemperaturaKomory(FT_poczatek, temp); }
    void setTemperaturaKomoryWarunkiKoncowe(const float & temp) { setTemperaturaKomory(FT_koniec, temp); }
    void setTemperaturaKomoryDozowanie(const float & temp) { setTemperaturaKomory(FT_dozowanie, temp); }
    void setTemperaturaKomoryPrzedZaplonem(const float & temp) { setTemperaturaKomory(FT_przedZaplon, temp); }
    void setTemperaturaKomoryZaplon(const float & temp) { setTemperaturaKomory(FT_zaplon, temp); }


    void setCisnienieKomoryWarunkiPoczatkowe(const float & cisn) { setCisnienieKomory(FT_poczatek, cisn); }
    void setCisnienieKomoryWarunkiKoncowe(const float & cisn) { setCisnienieKomory(FT_koniec, cisn); }
    void setCisnienieKomoryDozowanie(const float & cisn) { setCisnienieKomory(FT_dozowanie, cisn); }
    void setCisnienieKomoryPrzedZaplonem(const float & cisn) { setCisnienieKomory(FT_przedZaplon, cisn); }
    void setCisnienieKomoryZaplon(const float & cisn) { setCisnienieKomory(FT_zaplon, cisn); }
    void setCisnienieParCieczy(const float & cisn);

    void setCisnienieP1(const float & cisn) { setCisnienieKomory(FT_P1, cisn); }
    void setCisnienieP2(const float & cisn) { setCisnienieKomory(FT_P2, cisn); }
    void setCisnienieP3(const float & cisn) { setCisnienieKomory(FT_P3, cisn); }

    void setStezeniaPrzedZaplonem(const float & voc1, const float & voc2, const float & o2, const float & co2, const float & cz8)
    {
        setStezenia(FT_przedZaplon, voc1, voc2, o2, co2, cz8);
    } 
    void setStezeniaPoZaplonie(const float & voc1, const float & voc2, const float & o2, const float & co2, const float & cz8)
    {
        setStezenia(FT_koniec, voc1, voc2, o2, co2, cz8);
    }   
    void setPodcisnienie(bool val);

    void setLiquidVolue(const float & vol) { iloscCieczy << vol; }
    
    void setUdanaProba(bool success, bool powtarzaneDozowanie, bool powtarzanyZaplon);
    void setZrodloZaplonu(const QString & zrZaplon) { proby.last().zrodloZaplonu = zrZaplon; }
    void setTemperaturaParownika(const float & temp) { proby.last().tempParownikaDozowanie = temp; }

    const QString &getNazwaTestu() const;
    void setNazwaTestu(const QString &newNazwaTestu);

    void addValues(float voc1, float voc2, float o2, float co2, float a8, float tempPar, float tempKom, float cisnKom);

    void setStartTest(const QTime &newStartTest);
    void setStopTest(const QTime &newStopTest);

    friend QDataStream & operator<<(QDataStream & ds, const TestData & item);
    friend QDataStream & operator>>(QDataStream & ds, TestData & item);

    const QDateTime &getDataTestu() const;

    const QStringList &getUczestnicy() const;

    const QString &getNazwaCieczy() const;

    float getTempKomoraPoczatek() const;

    float getWilgotnosc() const;

    const QList<ProbaType> &getProby() const;

    const QList<float> &getIloscCieczy() const;

    float getTempKomoraKoniec() const;

    float getCisnienieKoniec() const;

    float getVoc1() const;

    float getVoc2() const;

    float getO2() const;

    float getCo2() const;

    float getCz8() const;

    const QList<AnalValType> &getValues() const;

    void start();

    void cisnieniaCzastkoweOblicz(ProbaType &proba);

    float getCisnieniePoczatkowe() const;
    void setCisnieniePoczatkowe(float newCisnieniePoczatkowe);

protected:
    void setTemperaturaKomory(FazaTestu ft, const float & temp);
    void setCisnienieKomory(FazaTestu ft, const float & cisn);
    void setStezenia(FazaTestu ft, const float & voc1, const float & voc2, const float & o2, const float & co2, const float & cz8);

    void initProba(ProbaType & p);
private:

    QDateTime dataTestu; //project
    QStringList uczestnicy; //project
    QString nazwaCieczy; //ekran 1
    float tempKomoraPoczatek; //(temperatura odczytana w momencie przejścia z kroku 1 do 2, Czujnik 6)
    float wilgotnosc; //wilgotność podana na ekranie 1
    QList<ProbaType> proby;
    QList<float> iloscCieczy; //ekran 1 + 7
    float tempKomoraKoniec; //(odczytana w momencie zatwierdzenia ustabilizowania się odczytów z czujników stężenia,
                            //tych po zapłonie w kroku 8)
    float cisnienieKoniec; //(odczytana w momencie kiedy użytkownik potwierdzi, że odczyty z czujników stężenia się ustabilizowały)
    float voc1; //ekran 8
    float voc2; //ekran 8
    float o2;   //ekran 8
    float co2;  //ekran 8
    float cz8;  //ekran 8

    QString nazwaTestu;
    QList<AnalValType> values;
    QTime startTest;
    QTime stopTest;


};

QDataStream & operator<<(QDataStream & ds, const TestData & item);
QDataStream & operator>>(QDataStream & ds, TestData & item);

QDataStream & operator<<(QDataStream & ds, const ProbaType & item);
QDataStream & operator>>(QDataStream & ds, ProbaType & item);

QDataStream & operator<<(QDataStream & ds, const AnalValType & item);
QDataStream & operator>>(QDataStream & ds, AnalValType & item);



#endif // TESTDATA_H
