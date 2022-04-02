#ifndef TESTTABSWIDGET_H
#define TESTTABSWIDGET_H

#include <QWidget>
#include "projectitem.h"

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
    TestTabsWidget(const ProjectItem & pr, const Ustawienia & ust, QWidget * parent);
    ~TestTabsWidget();
    void setActive();
    CreateTestWizard * createTestWizard() const;
private:
    Ui::TestTabsWidget *ui;
    ProjectItem projekt;
};

#endif // TESTTABSWIDGET_H
