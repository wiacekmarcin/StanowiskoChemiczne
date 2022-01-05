#include "testtabswidget.h"
#include "ui_testtabswidget.h"

TestTabsWidget::TestTabsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestTabsWidget)
{
    ui->setupUi(this);
}

TestTabsWidget::TestTabsWidget(const ProjectItem &pr, const TestData &t, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestTabsWidget),
    projekt(pr), test(t)
{
    ui->setupUi(this);

    ui->stackedWidget->setTestData(t);

    ui->tabFoto->setEnabled(false);
    ui->tabRaport->setEnabled(false);
    ui->tabWideo->setEnabled(false);
    ui->stackedWidget->setCurrentIndex(0);
}

TestTabsWidget::~TestTabsWidget()
{
    delete ui;
}

void TestTabsWidget::setActive()
{
    ui->tabWidget->setCurrentIndex(0);
}
