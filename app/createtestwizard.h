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
class CreateTestWizard : public QStackedWidget
{
    Q_OBJECT

public:
    explicit CreateTestWizard(QWidget *parent = 0);
    void setTestData(const TestData & dt);
    ~CreateTestWizard();

    void setField(const QString & key, const QVariant & val);
    QVariant field(const QString & key) const;
    void addPage(TestPage * page, int id);
    TestPage * currentPage() const;
    bool checkZawory() const;
    bool getZamknietaKomora() const;

    Ustawienia *getUst() const;

    //void setUst(Ustawienia *newUst);

public slots:
    void changeDigitalIn(int id, bool value);
    void changeAnalog(int id, double value);
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
    void zaplon(const QString & zaplon, const QString & zaplonExt);
    void triggerCamera(bool on);
    void pomiarCisnienia(int idCzujnik, unsigned long time_ms);

    void komora(bool);

    void setDigitalOut(int id, bool value);


protected slots:
    void nextPage(int id);
    void checkValidPage();
protected:
    void init();
    void initializePage();
    void showWarning(bool value);
private:
    QMap<QString,  QVariant> values;
    QMap<int, TestPageForm*> pages;
    short selectedId;
    bool finished;
    QMap<int, bool> zawory;
    QMap<int, double> stezenia;
    OtwarteZawory * dlgOtwarte;
    bool zamknietaKomoraA;
    bool zamknietaKomoraB;

    Ustawienia * ust;
};

#endif // CREATETESTWIZARD_H
