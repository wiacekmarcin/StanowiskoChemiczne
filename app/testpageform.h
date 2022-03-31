#ifndef TESTPAGEBASE_H
#define TESTPAGEBASE_H

#include <QWidget>
//#include "urzadzenia.h"
#include "ustawienia.h"

class QFrame;
class CreateTestWizard;
namespace Ui {
class TestPageForm;
}
class TestPage;

class TestPageForm : public QWidget
{
    Q_OBJECT
public:

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

    explicit TestPageForm(QWidget *parent = nullptr);

    void addWidget(TestPage * page);
    TestPage*  widget() const { return page; }
    void setTitle(const QString & title);
    void setSubTitle(const QString & title);
    void setButtonName(const QString & name);
    void showVisibleButton(bool value);
    void isComplete();
    void initializePage();
    QFrame * widgetFrame();

    void setCreateTestWizard(CreateTestWizard * wiz);

protected slots:
    void click();
signals:
    void clickButton(int id);
public:
    ~TestPageForm();
    int getId() const;
    void setId(int value);
    void disableButton(bool disable);

private:
    Ui::TestPageForm *ui;
    int id;
    TestPage * page;
    CreateTestWizard * wizard;

};


#endif // TESTPAGEBASE_H
