#ifndef TESTTABSWIDGET_H
#define TESTTABSWIDGET_H

#include <QWidget>
#include "projectitem.h"
#include "testdata.h"
#include "ustawienia.h"
namespace Ui {
class TestTabsWidget;
}

class CreateTestWizard;

class TestTabsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestTabsWidget(QWidget *parent = 0);
    TestTabsWidget(const ProjectItem & pr, QWidget * parent);
    ~TestTabsWidget();
    void setActive();
    CreateTestWizard * createTestWizard() const;

public slots:
    void finishedTest(bool success);
private:
    Ui::TestTabsWidget *ui;
    ProjectItem projekt;
    TestData testDane;
};

#endif // TESTTABSWIDGET_H
