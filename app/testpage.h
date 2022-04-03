#ifndef TESTPAGE_H
#define TESTPAGE_H

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

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString subTitle READ subTitle WRITE setSubTitle)


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
        PAGE_8
    } PageId;

    typedef enum _value {
        nazwaTest = 0,
        ciecz,
        objetosc,
        calaObjetosc,
        zaplon,
        zaplonExtra,
        dozownikNr,
        czyPompaMebr,
        brakZaplonu,
        rodzajZaplonu,
    } Value;

    void setField(Value key, const QVariant & val);
    QVariant field(Value key) const;
    void setWizard(CreateTestWizard * wiz);
    CreateTestWizard *wizard() const;

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

    void updateOutput(uint16_t mask, bool on);
    void cykleDozownik(uint8_t nr, uint32_t steps);
    void runZaplon(short idZaplon);

signals:
    void changePage(TestPage::PageId id);



protected:

    bool z_drzwi_prawe();
    bool z_wentylacja_lewa();
    bool z_proznia();
    bool z_pom_stez_1();
    bool z_drzwi_lewe();
    bool z_wentylacja_prawa();
    bool z_wlot_powietrza();
    bool z_pom_stez_2();
    bool z_pilot();

    void setFinished(bool success);

    bool sprawdzZawory(QPushButton *pbOk_1, QLabel * arrow_1, QFrame * frame_2);

    bool sprawdzOtwarteZaworProzni();
    bool sprawdzOtwarteZaworPowietrza();
    bool sprawdzOtwarteZaworStezenia();
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
