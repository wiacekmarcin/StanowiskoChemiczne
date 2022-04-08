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
    void init(const Ustawienia &ust, const QString &testName);

    void setField(TestPage::Value key, const QVariant & val);
    QVariant field(TestPage::Value key) const;
    void addPage(TestPage * page, TestPage::PageId id, short step);
    TestPage * currentPage() const;
    void changePage(TestPage::PageId id);
    void setFinished(bool success);

    bool z_drzwi_prawe();
    bool z_wentylacja_lewa();
    bool z_proznia();
    bool z_pom_stez_1();
    bool z_drzwi_lewe();
    bool z_wentylacja_prawa();
    bool z_wlot_powietrza();
    bool z_pom_stez_2();
    bool z_pilot();

    void updateOutput(uint16_t mask, bool on);
    void runZaplon(short id);
    void runCykleDozownik(uint8_t nr, uint32_t steps);
    void runDozownikMl(uint8_t nr, uint32_t mlx10);

public slots:
    void nextPage(TestPage::PageId id);

    void changeDigitalIn(uint16_t id, bool value);
    void changeAnalog(double val0, double val1, double val2, double val3, double val4, double val5, double val6,  double val7);

    void dozownikDone(bool succes);

signals:
    void changeTestName(QString);
    void finishedTest(bool success);
    void writeOutValues(uint16_t, bool on);
    void cykleDozownik(uint8_t nr, uint32_t steps);
    void dozownikMl(uint8_t nr, uint32_t mlx10);
    void zaplon(short Id);




    void setDigitalOut(int id, bool value);

protected:

    void initializePage();
    void showWarning(bool value);
private:
    QMap<TestPage::Value,  QVariant> values;
    QMap<TestPage::PageId, TestPageForm*> pages;
    //QMap<TestPage::PageId, TestPageForm*> pages;
    TestPage::PageId selectedId;

    bool finished;
    OtwarteZawory * dlgOtwarte;


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

    QMap<uint16_t, bool> zaworyMap;
};

#endif // CREATETESTWIZARD_H
