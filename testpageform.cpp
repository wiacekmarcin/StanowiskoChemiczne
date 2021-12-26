#include "testpageform.h"
#include "ui_testpageform.h"
#include "testpage.h"
TestPageForm::TestPageForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestPageForm)
{
    ui->setupUi(this);
    ui->subtitle->setVisible(false);
    ui->title->setVisible(false);

    connect(ui->pbNext, &QPushButton::clicked, this, &TestPageForm::click);
}

void TestPageForm::addWidget(TestPage *page_)
{
    ui->testPageLayout->addWidget(page_);
    page = page_;
}

void TestPageForm::setTitle(const QString &title)
{
    ui->title->setText(QString("%1 - Krok %2").arg(title).arg(id));
    ui->title->setVisible(!title.isEmpty());
}

void TestPageForm::setSubTitle(const QString &title)
{
    ui->subtitle->setText(title);
    ui->subtitle->setVisible(!title.isEmpty());
}

void TestPageForm::setButtonName(const QString &name)
{
    ui->pbNext->setText(name);
    ui->pbNext->setVisible(!name.isEmpty());
}

void TestPageForm::isComplete()
{
    disableButton(!widget()->isComplete());
}

void TestPageForm::initializePage()
{
    widget()->initializePage();
}

QFrame *TestPageForm::widgetFrame()
{
    return ui->frTestPage;
}

void TestPageForm::click()
{
    emit clickButton(widget()->nextPage());
}

TestPageForm::~TestPageForm()
{
    delete ui;
}

int TestPageForm::getId() const
{
    return id;
}

void TestPageForm::setId(int value)
{
    id = value;
}

void TestPageForm::disableButton(bool disable)
{
    ui->pbNext->setEnabled(!disable);
}
