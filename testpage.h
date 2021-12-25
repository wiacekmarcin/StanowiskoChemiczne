#ifndef TESTPAGE_H
#define TESTPAGE_H

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
        PAGE_1 = 0,
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
    virtual int nextPage() const;
signals:
    void completeChanged();
    void changePage(int id);

    void dozownik(int dozownik, int cykle);
    void zaworProzni(bool open);
protected:
    bool valid;
private:
    CreateTestWizard * wiz;
    int id;
    TestPageForm * form;
    QString m_title;
    QString m_subTitle;
    QString m_buttonName;
};

#endif // TESTPAGE_H
