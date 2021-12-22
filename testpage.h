#ifndef TESTPAGE_H
#define TESTPAGE_H

#include <QWidget>
#include <QDebug>

class CreateTestWizard;
class TestPageForm;

class TestPage : public QWidget
{
    Q_OBJECT

public:
    explicit TestPage(QWidget *parent = 0);
    ~TestPage();
    void setField(const QString & key, const QVariant & val);
    QVariant field(const QString & key) const;
    void setWizard(CreateTestWizard * wiz);
    CreateTestWizard *wizard() const;

    int getId() const;
    void setId(int value);
    void setTitle(const QString & t);
    void setSubTitle(const QString & t);
    TestPageForm *getForm() const;
    void setForm(TestPageForm *value);
    virtual void initialPage() { qDebug("ini"); }
signals:
    void completeChanged();
protected:
    virtual bool isComplete() const { return true; }

private:
    CreateTestWizard * wiz;
    int id;
    TestPageForm * form;
};

#endif // TESTPAGE_H
