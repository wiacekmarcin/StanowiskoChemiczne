#ifndef PDFCREATOR_H
#define PDFCREATOR_H

#include "ustawienia.h"
#include "testdata.h"

#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QList>
#include <QPair>
#include <QObject>


typedef struct visWykres {
    analogIn id;
    double ratio;
    bool show;
    float minVal;
    float maxVal;
    float minTime;
    float maxTime;
    QString opis;
    QString opis2;
    QString jedn;
} visibleWykresType;

class PdfCreator
{

public:
    PdfCreator(const TestData & td, const Ustawienia & ust);
    QString getBody() const ;

    void addImage(const QString & file, const QString &descr);
    void clearImage();
    void setComment(const QString &comm);

    void setWykresVisible(analogIn wykresId, bool show, float minV, float maxV,
                          double ratio, unsigned int pages, const QString &opis,
                          const QString & opis2, const QString &unit);
protected:

/** Funkcje od formularza **/
    QString getTitle() const;
    QString getTemat() const;
    QString getLogo() const;
    QString getData() const;
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

    QString getImages() const;
    QString getImageWykresPage(const visibleWykresType & var) const;

    QString getOsX(unsigned int i, unsigned int max, const float &minv, const float &maxv) const;
    QString getOsY(unsigned int i, const float &imax, const float &minv, const float &maxv) const;

    QString getImageWykres(const visibleWykresType & var) const;

private:
    QString getTitle1() const;
    QString getTitle2() const;

    QString getPicture(int num, const QPair<QString, QString> &filename) const;
    QString getComment() const;


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
    static constexpr char zdjeciaPrzeprBadania[] = "Zdjęcia z przeprowadzonego badania";

    const TestData & td;
    const Ustawienia & ust;

    QString comment;
    QList<QPair<QString, QString>> fileList;

    QVector<visibleWykresType> visibleWykres;
};


#endif // PDFCREATOR_H
