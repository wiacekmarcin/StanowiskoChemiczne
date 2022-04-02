#ifndef CREATETESTWIZARD_H
#define CREATETESTWIZARD_H

#include <QObject>
#include <QStackedWidget>
#include <QMap>
#include <QList>
#include "testpageform.h"
#include "testpage.h"

class TestPage;
class OtwarteZawory;
class Urzadzenia;
class GlowneOkno;
class QTreeWidgetItem;
class CreateTestWizard : public QStackedWidget
{
    Q_OBJECT

public:
    explicit CreateTestWizard(QWidget *parent = 0);
    ~CreateTestWizard();
    void setUstawienia(const Ustawienia &ust);
    void init(Urzadzenia *u, const Ustawienia &ust, const QString &testName);

    void setField(TestPage::Value key, const QVariant & val);
    QVariant field(TestPage::Value key) const;
    void addPage(TestPage * page, TestPage::PageId id, short step);
    TestPage * currentPage() const;
    void changePage(TestPage::PageId id);
    void setFinished(bool success);

    bool checkZawory() const;
    bool getZamknietaKomora() const;


public slots:
    void nextPage(TestPage::PageId id);

    void changeDigitalIn(int id, bool value);
    void changeAnalog(double val0, double val1, double val2, double val3, double val4, double val5, double val6,  double val7);

    void clickedZawory();
    void updatePageData();


signals:
    void changeTestName(QString);
    void finishedTest(bool success);

    void zamknietaKomora(bool);
    void openZawor(unsigned int id, bool val);
    void cisnienieVal(double val);



    void zaplon(const QString & zaplon, const QString & zaplonExt);
    void triggerCamera(bool on);
    void pomiarCisnienia(int idCzujnik, unsigned long time_ms);



    void setDigitalOut(int id, bool value);


protected slots:

    void checkValidPage();
protected:

    void initializePage();
    void showWarning(bool value);
private:
    QMap<TestPage::Value,  QVariant> values;
    QMap<TestPage::PageId, TestPageForm*> pages;
    TestPage::PageId selectedId;

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

    QString m_nazwaTestu;
    short m_nrDozownika;
    QString m_nazwacieczy;
    double m_objetosc;
    QString m_zaplon;
    QString m_zaplonExt; //tylko w przypadku zaplonu elektrycznego
    bool zaplonExt; //
};

#endif // CREATETESTWIZARD_H
