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
        zaplon,
        zaplonExtra,
        dozownikNr,
        czyPompaMebr,
        brakZaplonu,
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
    virtual bool isComplete() const { return true; }
    virtual PageId nextPageId() const;
    virtual void nextPage(PageId idPage);
    virtual void changeData() { }
    virtual void updateWejscia() {};

public slots:
   void readAll(uint16_t vals);


signals:
    void completeChanged();
    void changePage(TestPage::PageId id);
    void updateData(uint16_t vals);


protected:
    bool valid;
    bool b_drzwi_prawe, b_wentylacja_lewa, b_proznia, b_pom_stez_1, b_drzwi_lewe, b_wentylacja_prawa, b_wlot_powietrza, b_pom_stez_2, b_pilot;

    bool isZamknietyZawor() {
        qDebug("%d%d%d%d%d%d%d%d", b_drzwi_prawe, b_wentylacja_lewa, b_proznia, b_pom_stez_1, b_drzwi_lewe, b_wentylacja_prawa, b_wlot_powietrza, b_pom_stez_2);
        return b_drzwi_prawe || b_wentylacja_lewa || b_proznia || b_pom_stez_1 || b_drzwi_lewe || b_wentylacja_prawa || b_wlot_powietrza || b_pom_stez_2;
    }
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
