#ifndef TESTTABSWIDGET_H
#define TESTTABSWIDGET_H

#include <QWidget>
#include "projectitem.h"
#include "testdata.h"

namespace Ui {
class TestTabsWidget;
}

class TestTabsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestTabsWidget(QWidget *parent = 0);
    TestTabsWidget(const ProjectItem & pr, const TestData & d, QWidget * parent);
    ~TestTabsWidget();
    void setActive();

private:
    Ui::TestTabsWidget *ui;
    ProjectItem projekt;
    TestData test;
};

#endif // TESTTABSWIDGET_H
