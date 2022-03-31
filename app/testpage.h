#ifndef TESTPAGE_H
#define TESTPAGE_H

#include <QString>
#include <QObject>
#include <QWidget>
#include <QDebug>

class CreateTestWizard;
class TestPageForm;

class TestPage : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString subTitle READ subTitle WRITE setSubTitle)
    Q_PROPERTY(QString buttonName READ buttonName WRITE setButtonName)


public:
    explicit TestPage(QWidget *parent = 0);
    ~TestPage();

    enum {
        PAGE_1 = 1,
        PAGE_2,
        PAGE_3,
        PAGE_4,
        PAGE_5,
        PAGE_6,
        PAGE_7,
        PAGE_8
    };

    void setField(const QString & key, const QVariant & val);
    QVariant field(const QString & key) const;
    void setWizard(CreateTestWizard * wiz);
    CreateTestWizard *wizard() const;

    QString title() const;
    void setTitle(const QString & t);

    QString subTitle() const;
    void setSubTitle(const QString & t);

    QString buttonName() const;
    void setButtonName(const QString & t);

    int getId() const;
    void setId(int value);

    TestPageForm *getForm() const;
    void setForm(TestPageForm *value);
    virtual void initializePage();
    virtual bool isComplete() const { return true; }
    virtual int nextPageId() const;
    virtual void nextPage(short idPage);
    virtual void changeData() { }
    virtual void updateWejscia() {};

public slots:
   void readAll(uint16_t vals);


signals:
    void completeChanged();
    void changePage(int id);
    void updateData(uint16_t vals);


protected:
    bool valid;
    bool b_drzwi_prawe, b_wentylacja_lewa, b_proznia, b_pom_stez_1, b_drzwi_lewe, b_wentylacja_prawa, b_wlot_powietrza, b_pom_stez_2, b_pilot;

    bool isZamknietyZawor() {
        qDebug("%d%d%d%d%d%d%d%d", b_drzwi_prawe, b_wentylacja_lewa, b_proznia, b_pom_stez_1, b_drzwi_lewe, b_wentylacja_prawa, b_wlot_powietrza, b_pom_stez_2);
        return b_drzwi_prawe && b_wentylacja_lewa && b_proznia && b_pom_stez_1 && b_drzwi_lewe && b_wentylacja_prawa && b_wlot_powietrza && b_pom_stez_2;
    }

private:
    CreateTestWizard * wiz;
    TestPageForm * form;
    int id;
    QString m_title;
    QString m_subTitle;
    QString m_buttonName;

    uint16_t prevVals;

};

#endif // TESTPAGE_H
