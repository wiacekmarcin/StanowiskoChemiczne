#ifndef CREATETESTWIZARD_H
#define CREATETESTWIZARD_H

#include <QObject>
#include <QStackedWidget>
#include <QMap>
#include <QList>
#include <QMutex>
#include <QVector>

#include "testpageform.h"
#include "testpage.h"
#include "testdata.h"

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

    void setTestData(TestData * dt);
    TestData * getTestData();
    void init(Ustawienia * ust, const UserPrivilige &user, const QString &testName);
    void initFromFile();

    void setField(TestPage::Value key, const QVariant & val);
    QVariant field(TestPage::Value key) const;
    void addPage(TestPage * page, TestPage::PageId id, short step);
    TestPage * currentPage() const;
    void changePage(TestPage::PageId id);
    void setFinished(bool success);


    bool zi_drzwi_prawe();
    bool zi_wentylacja_lewa();
    bool zi_proznia();
    bool zi_pom_stez_1();
    bool zi_drzwi_lewe();
    bool zi_wentylacja_prawa();
    bool zi_wlot_powietrza();
    bool zi_pom_stez_2();
    bool zi_pilot();

    void updateOutput(digitalOut mask, bool on);
    void runZaplon(ZaplonRodzaj id);
    void runCykleDozownik(uint8_t nr, uint32_t steps);
    void runDozownikMl(uint8_t nr, uint32_t mlx10);
    void runCheckPositionHome();
    void runResetSterownik(uint8_t nr);

    void setZ_criticalMask(uint16_t newZ_criticalMask);
    void setZ_warningMask(uint16_t newZ_criticalMask);

    float getCzujnik(analogIn czujnik);

    void setDebug(const QString & d) { emit debug(d); }
    void setError(const QString & e) { emit error(e); }

public slots:
    void nextPage(TestPage::PageId id);

    void changeDigitalIn(uint16_t id, bool value);
    void changeAnalog(double val0, double val1, double val2, double val3, double val4, double val5, double val6,  double val7);

    void dozownikDone(bool succes);
    void checkPositionHomeDone(bool ok, bool d1, bool d2, bool d3, bool d4, bool d5);

signals:
    void changeTestName(QString);
    void finishedTest(bool success);
    void writeOutValues(digitalOut, bool on);
    void cykleDozownik(uint8_t nr, uint32_t steps);
    void dozownikMl(uint8_t nr, uint32_t mlx10);
    void zaplon(ZaplonRodzaj Id);
    void checkPositionHome();
    void criticalZaworOpenSignal(uint16_t idz);
    void warningZaworOpenSignal(uint16_t idz);
    void resetDozownik();
    void resetSterownik(uint8_t nr);


    void setDigitalOut(digitalOut id, bool value);
    void readsInputs();

    void error(const QString & e);
    void debug(const QString & d);

private slots:
    void criticalZaworOpenSlot(uint16_t idz);
    void warningZaworOpenSlot(uint16_t idz);

protected:

    void initializePage();
    void runResetDozownik();

    void addToVector(QVector<float> &vect, float val);
private:
    QMap<TestPage::Value,  QVariant> values;
    QMap<TestPage::PageId, TestPageForm*> pages;
    TestPage::PageId selectedId;

    bool finished;
    OtwarteZawory * dlgOtwarte;


    unsigned short numberInitDozCycles;
    QMap<unsigned int, QString> m_namesZawory;

    Ustawienia * ustawienia;

    QString m_nazwaTestu;
    short m_nrDozownika;
    QString m_nazwacieczy;
    double m_objetosc;
    QString m_zaplon;
    QString m_zaplonExt; //tylko w przypadku zaplonu elektrycznego
    bool zaplonExt; //

    QMap<uint16_t, bool> zaworyMap;
    QMap<uint16_t, bool> criticalMap;
    bool showCrit;

    QMap<uint16_t, bool> warningMap;
    bool showWarn;

    QMap<analogIn, QVector<float>> m_czujniki;
    TestData * dt;
    bool registerPomiary;
    QMutex mutex;

    UserPrivilige user;
};

#endif // CREATETESTWIZARD_H
