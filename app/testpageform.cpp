#include "testpageform.h"
#include "ui_testpageform.h"
#include "testpage.h"
#include "createtestwizard.h"
#include "otwartezawory.h"

TestPageForm::TestPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestPageForm)
{
    ui->setupUi(this);
    ui->subtitle->setVisible(false);
    ui->title->setVisible(false);
}

void TestPageForm::addWidget(TestPage *page_)
{
    ui->testPageLayout->addWidget(page_);
    page = page_;
}

void TestPageForm::setTitle(const QString &title)
{
    ui->title->setText(QString("%1 - Krok %2").arg(title).arg(step));
    ui->title->setVisible(!title.isEmpty());
}

void TestPageForm::setSubTitle(const QString &title)
{
    ui->subtitle->setText(title);
    ui->subtitle->setVisible(!title.isEmpty());
}

void TestPageForm::initializePage()
{
    widget()->initializePage();
}

QFrame *TestPageForm::widgetFrame()
{
    return ui->frTestPage;
}

void TestPageForm::setCreateTestWizard(CreateTestWizard *wiz)
{
    wizard = wiz;
}

TestPageForm::~TestPageForm()
{
    delete ui;
}

void TestPageForm::setStep(short newStep)
{
    step = newStep;
}

