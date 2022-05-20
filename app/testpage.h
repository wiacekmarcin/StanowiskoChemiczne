#ifndef TESTPAGE_H
#define TESTPAGE_H

#include "ustawienia.h"
#include "testdata.h"

#include <QString>
#include <QObject>
#include <QWidget>
#include <QDebug>

class CreateTestWizard;
class TestPageForm;
class QFrame;
class QPushButton;
class QLabel;



class TestPage : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle CONSTANT)
    Q_PROPERTY(QString subTitle READ subTitle WRITE setSubTitle CONSTANT)


public:
    explicit TestPage(QWidget *parent = 0);
    ~TestPage();

    typedef enum _pageId {
        PAGE_U = 0,
        PAGE_1 = 1,
        PAGE_2,
        PAGE_3,
        PAGE_4,
        PAGE_5,
        PAGE_6,
        PAGE_7,
        PAGE_8,
        PAGE_9
    } PageId;

    typedef enum _value {
        nazwaTest = 0,
        ciecz,
        objetosc,
        calaObjetosc,
        zaplon,
        dozownikNr,
        czyPompaProzn,
        brakZaplonu,
        rodzajZaplonu,
        wybranyPlomien,
        bylWybuch,
        powtarzanyTest
    } Value;

    void setField(Value key, const QVariant & val);
    QVariant field(Value key) const;
    void setWizard(CreateTestWizard * wiz);
    CreateTestWizard *wizard() const;
    TestData * getTestData();
    float getCzujnik(analogIn czujnik);

    QString title() const;
    void setTitle(const QString & t);

    QString subTitle() const;
    void setSubTitle(const QString & t);

    PageId getId() const;
    void setId(PageId value);

    TestPageForm *getForm() const;
    void setForm(TestPageForm *value);
    virtual void initializePage();

    virtual PageId nextPageId() const;
    virtual void nextPage(PageId idPage);
    virtual void updateWejscia() {};
    virtual void dozownikDone(bool) {};
    virtual void setCisnKomory(const double & ) {};
    virtual void checkPositionHomeDone(bool /*ok*/, bool /*d1*/, bool /*d2*/, bool /*d3*/, bool /*d4*/, bool /*d5*/) {};

    void updateOutput(digitalOut mask, bool on);
    void cykleDozownik(uint8_t nr, uint32_t steps);
    void dozownikMl(uint8_t nr, uint32_t mlx10);
    void runZaplon(ZaplonRodzaj idZaplon);
    void checkPositionHome();
    void setZ_warningMask(uint16_t newZ_warningMask);
    void setZ_criticalMask(uint16_t newZ_criticalMask);
    void setFinished(bool success);

signals:
    void changePage(TestPage::PageId id);



protected:

    bool zi_drzwi_prawe();
    bool zi_wentylacja_lewa();
    bool zi_proznia();
    bool zi_pom_stez_1();
    bool zi_drzwi_lewe();
    bool zi_wentylacja_prawa();
    bool zi_wlot_powietrza();
    bool zi_pom_stez_2();
    bool zi_pilot();



    bool sprawdzZawory(QPushButton *pbOk_1, QLabel * arrow_1, QFrame * frame_2);

    bool sprawdzOtwarteZaworProzni();
    bool sprawdzOtwarteZaworPowietrza();
    bool sprawdzOtwarteZaworStezenia();
    bool sprawdzOtwarteZawor2Calowe();

    bool sprawdzOtwarteZawory(uint16_t mask);
private:
    CreateTestWizard * wiz;
    TestPageForm * form;
    PageId id;
    QString m_title;
    QString m_subTitle;
    bool restricted;
    QMap<uint16_t, bool> restrictedMap;
    uint16_t prevVals;




};

#endif // TESTPAGE_H
