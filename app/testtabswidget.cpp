#include "testtabswidget.h"
#include "ui_testtabswidget.h"

TestTabsWidget::TestTabsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestTabsWidget)
{
    ui->setupUi(this);
}

TestTabsWidget::TestTabsWidget(const ProjectItem &pr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestTabsWidget),
    projekt(pr)
{
    ui->setupUi(this);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);

    ui->stackedWidget->setCurrentIndex(0);
    testDane.setDateTime(pr.getCreateDt());
    testDane.setMembers(pr.getMembers().split("\n"));
    ui->stackedWidget->setTestData(testDane);
}

TestTabsWidget::~TestTabsWidget()
{
    delete ui;
}

void TestTabsWidget::setActive()
{
    ui->tabWidget->setCurrentIndex(0);
}

CreateTestWizard * TestTabsWidget::createTestWizard() const
{
    return ui->stackedWidget;
}

void TestTabsWidget::finishedTest(bool success)
{
    if (success) {
        ui->tabWidget->setTabEnabled(1, true);
        ui->tabWidget->setTabEnabled(2, true);
        ui->tabWidget->setTabEnabled(3, true);
    }
}
