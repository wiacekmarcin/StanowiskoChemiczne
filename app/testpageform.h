#ifndef TESTPAGEBASE_H
#define TESTPAGEBASE_H

#include <QWidget>
//#include "urzadzenia.h"
#include "ustawienia.h"
#include "testpage.h"
//#include "createtestwizard.h"

class QFrame;
class CreateTestWizard;
namespace Ui {
class TestPageForm;
}


class TestPageForm : public QWidget
{
    Q_OBJECT
public:

    explicit TestPageForm(QWidget *parent = nullptr);

    void addWidget(TestPage * page);
    TestPage*  widget() const { return page; }
    void setTitle(const QString & title);
    void setSubTitle(const QString & title);
    void initializePage();
    QFrame * widgetFrame();
    void setCreateTestWizard(CreateTestWizard * wiz);
    CreateTestWizard * wiz() { return wizard; }

public:
    ~TestPageForm();
    void setStep(short newStep);
    void visibleAbortBtn(bool vs);

private slots:
    void on_pbAbort_clicked();
    void on_pbFinishTest_clicked();

private:
    Ui::TestPageForm *ui;
    TestPage * page;
    CreateTestWizard * wizard;
    short step;
};


#endif // TESTPAGEBASE_H
