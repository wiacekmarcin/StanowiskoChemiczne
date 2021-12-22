#include "testpageform.h"
#include "ui_testpageform.h"
TestPageForm::TestPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestPageForm)
{
    ui->setupUi(this);
}

void TestPageForm::addWidget(QWidget *page)
{
    ui->verticalLayout->insertWidget(2, page);
}

void TestPageForm::setTitle(const QString &title)
{
    ui->title->setText(title);
}

void TestPageForm::setSubTitle(const QString &title)
{
    ui->title->setText(title);
}

TestPageForm::~TestPageForm()
{
    delete ui;
}
