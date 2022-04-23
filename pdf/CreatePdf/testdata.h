#ifndef TESTDATA_H
#define TESTDATA_H

#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QList>
typedef struct proba {
    bool success; //czy udana proba ekran 6/7
    QString zrodloZaplonu; //ekran 6
    bool powtarzanyZaplon; //ekran 7
    bool powtarzaneDozowanie; //ekran 7
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
} ProbaType;

typedef struct data {
    QDateTime dataTestu; //project
    QStringList uczestnicy; //project
    QString nazwaCieczy; //ekran 1
    float tempKomoraPoczatek; //(temperatura odczytana w momencie przejścia z kroku 1 do 2, Czujnik 6)
    float wilgotnosc; //wilgotność podana na ekranie 1
    QList<ProbaType> proby;
    QList<float> iloscCieczy; //ekran 1 + 7
    float tempKomoraKoniec; //(odczytana w momencie zatwierdzenia ustabilizowania się odczytów z czujników stężenia, tych po zapłonie w kroku 8)
    float cisnienieKoniec; //(odczytana w momencie kiedy użytkownik potwierdzi, że odczyty z czujników stężenia się ustabilizowały)
    float voc1; //ekran 8
    float voc2; //ekran 8
    float o2;   //ekran 8
    float co2;  //ekran 8
    float cz8;  //ekran 8

} SDataType;



class TestData
{
public:
    TestData();
    QString getBody() const ;

    /** Funkcje ustawiające parametry **/
    void setDateTime(const QDateTime &dt) { d.dataTestu = dt; }

protected:
/** Funkcje od formularza **/

    static QString getTitle() { return QString("<div align=\"center\">%1%2</div>").arg(getTitle1(), getTitle2()); }
    static QString getTemat() { return QString("<div><p><b>%1</b><br>%2</p></div>").arg(temat, tematValue); }
    QString getData() const { return QString("<p>%1%2</p>").arg(TestData::dataWykonania, d.dataTestu.toString("dd.MM.yyyy hh:mm")); }
    QString getUczestnicy() const ;
    QString getNazwaCieczy() const ;
    QString getTempKomoraStart() const ;
    QString getWilgotnosc() const;

    QString getProba(const ProbaType & proba, const QList<float> &dozowanie, unsigned int nrProba) const;
    QString getProbaWynik(bool wynik) const;
    QString getZrodloZaplonu(const QString & zaplon) const;
    QString getIloscCieczy(const QList<float> &dozowanie, unsigned int nrProba) const;
    QString getWarunkiPoczatkowe(const ProbaType & proba) const;
    QString getWarunkiPrzedZaplonem(const ProbaType &proba) const;
    QString getOdczytyStezen(const ProbaType &proba) const;
    QString getWarunkiPoUdanejProbie() const;

    QString getImageUrl() const;
private:
    static QString getTitle1() { return QString("<h1>%1</h1>").arg(TestData::title1); }
    static QString getTitle2() { return QString("<h1>%1</h1>").arg(TestData::title2); }


    static constexpr char title1[]          = "Szkoła Główna Służby Pożarniczej w Warszawie";
    static constexpr char title2[]          = "Zakład Badania Przyczyn Pożarów";
    static constexpr char temat[]           = "Temat ćwiczenia:";
    static constexpr char tematValue[]      = "Badanie dolnej granicy wybuchowości par cieczy palnych";
    static constexpr char dataWykonania[]   = "Data wykonania testu:";
    static constexpr char skladzespolu[]    = "Skład zespołu przeprowadzającego test:";
    static constexpr char nazwacieczy[]     = "Nazwa badanej cieczy:";
    static constexpr char tempKomoraStartTest[]="Temperatura wewnątrz komory w momencie rozpoczęcia testu:";
    static constexpr char wilgotnosc[]      = "Wilgotność w pomieszczeniu:";
    static constexpr char proba[]           = "Próba:";
    static constexpr char probaWynik[]      = "Wynik:";
    static constexpr char probaPozytywna[]  = "pozytywny";
    static constexpr char probaNegatywna[]  = "negatywny";
    static constexpr char zrodloZaplonu[]   = "Źródło zapłonu:";
    static constexpr char iloscCieczy[]     = "Ilość dozowanej cieczy:";
    static constexpr char warunkiPoczatkowe[]= "Warunki początkowe:";
    static constexpr char warunkiPrzedZaplonem[]="Warunki przed zapłonem";
    static constexpr char tempParownika[]   = "Temperatura parownika cieczy:";
    static constexpr char tempKomory[]      = "Temperatura wewnątrz komory:";
    static constexpr char cisnKomory[]      = "Ciśnienie wewnątrz komory:";
    static constexpr char koncetracjaPar[]  = "Koncentracja par cieczy wyliczona metodą ciśnień cząstkowych:";
    static constexpr char zlaKoncetracjaPar[]="!!! UWAGA: w tym momencie wyliczona wartość koncentracji par cieczy, może być obarczona błędem, wynikającym ze zmiany temp. wew. komory.";
    static constexpr char odczytyStezen[]   = "Odczyty z przetworników stężenia:";
    static constexpr char warunkiPoUdanejProba[]="Warunki po udanej próbie zapłonu mieszaniny par cieczy z powietrzem:";

    SDataType d;
};

#endif // TESTDATA_H
