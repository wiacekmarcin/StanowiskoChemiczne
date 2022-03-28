#ifndef CREATETESTWIZARD_H
#define CREATETESTWIZARD_H

#include <QObject>
#include <QStackedWidget>
#include <QMap>
#include <QList>
#include "testpageform.h"
#include "testdata.h"

class TestPage;
class OtwarteZawory;
class Urzadzenia;
class GlowneOkno;

class CreateTestWizard : public QStackedWidget
{
    Q_OBJECT

public:
    explicit CreateTestWizard(QWidget *parent = 0);
    ~CreateTestWizard();
    void setTestData(const TestData & dt);
    void setUstawienia(const Ustawienia &ust);
    void init(Urzadzenia *u, const Ustawienia &ust);

    void setField(const QString & key, const QVariant & val);
    QVariant field(const QString & key) const;
    void addPage(TestPage * page, int id);
    TestPage * currentPage() const;
    bool checkZawory() const;
    bool getZamknietaKomora() const;


public slots:
    void changeDigitalIn(int id, bool value);
    void changeAnalog(double val0, double val1, double val2, double val3, double val4, double val5, double val6,  double val7);

    void clickedZawory();

    void zaworProzni(bool open);
    void pompaProzniowa(bool start);
    void mieszadlo(bool start);
    void zaworPowietrza(bool open);
    void pomiary(bool start);
    void pompaMembramowa(bool start);
    void pomiarSingle(int idCzujka);
    void pomiarStezen();
    void wentylator(bool start);


signals:
    void zamknietaKomora(bool);

    void zaplon(const QString & zaplon, const QString & zaplonExt);
    void triggerCamera(bool on);
    void pomiarCisnienia(int idCzujnik, unsigned long time_ms);



    void setDigitalOut(int id, bool value);


protected slots:
    void nextPage(int id);
    void checkValidPage();
protected:

    void initializePage();
    void showWarning(bool value);
private:
    QMap<QString,  QVariant> values;
    QMap<int, TestPageForm*> pages;
    short selectedId;
    bool finished;
    QMap<unsigned int, bool> zawory;
    QMap<int, double> stezenia;
    OtwarteZawory * dlgOtwarte;
    bool zamknietaKomoraLewa;
    bool zamknietaKomoraPrawa;

    double temperaturaKomory;
    double cisnienieKomory;

    unsigned short numberInitDozCycles;
    QMap<unsigned int, QString> m_namesZawory;

    //Ustawienia & ustawienia;
};

#endif // CREATETESTWIZARD_H
