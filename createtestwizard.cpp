#include "createtestwizard.h"

#include "testpage.h"
#include "testpageform.h"

#include "nowytest_1.h"
#include "nowytest_2.h"
#include "nowytest_3.h"
#include "nowytest_4.h"
#include "nowytest_5.h"
#include "nowytest_6.h"
#include "nowytest_7.h"

#include <QAbstractButton>
#include <QVariant>
#include <QDebug>
CreateTestWizard::CreateTestWizard(QWidget *parent) :
    QStackedWidget(parent)
{
    selectedId = -1;
    init();

}

void CreateTestWizard::setTestData(const TestData &dt)
{
    setField(QString("nazwa"), QVariant::fromValue(dt.getNazwa()));
    setField(QString("dozownik"), QVariant::fromValue(dt.getDozownik()));
    setField(QString("ciecz"), QVariant::fromValue(dt.getCiecz()));
    setField(QString("objetosc"), QVariant::fromValue(dt.getObjetosc()));
    setField(QString("zaplon"), QVariant::fromValue(dt.getZaplon()));
    setField(QString("zaplonExt"), QVariant::fromValue(dt.getZaplonExt()));
    initializePage();
}

CreateTestWizard::~CreateTestWizard()
{

}

void CreateTestWizard::init()
{
    setObjectName(QString::fromUtf8("TestWizard"));


    addPage(new NowyTest_1(this), 1);
    addPage(new NowyTest_2(this), 2);
    addPage(new NowyTest_3(this), 3);
    addPage(new NowyTest_4(this), 4);
    addPage(new NowyTest_5(this), 5);
    addPage(new NowyTest_6(this), 6);
    addPage(new NowyTest_7(this), 7);

    nextPage(1);
    initializePage();
}

void CreateTestWizard::initializePage()
{
    if (selectedId < pages.size())
        pages[selectedId]->initializePage();
}

void CreateTestWizard::setField(const QString &key, const QVariant &val)
{
    values[key] = val;
}

QVariant CreateTestWizard::field(const QString &key) const
{
    if (values.contains(key))
        return values[key];
    return QVariant();
}

void CreateTestWizard::addPage(TestPage *page, int id)
{
    TestPageForm *t = new TestPageForm(this);
    t->addWidget(page);
    page->setParent(t->widgetFrame());
    page->setId(id);
    page->setWizard(this);
    page->setForm(t);
    t->setId(id);
    pages[id] = t;
    addWidget(t);


    connect(t, &TestPageForm::clickButton, this, &CreateTestWizard::nextPage);
    connect(page, &TestPage::completeChanged, this, &CreateTestWizard::checkValidPage);
}

TestPage *CreateTestWizard::currentPage() const
{
    return static_cast<TestPageForm*>(currentWidget())->widget();
}

void CreateTestWizard::nextPage(int id)
{
    qDebug("nextPage %d", id);
    if (pages.contains(id)) {
        selectedId = id;
        setCurrentWidget(pages[selectedId]);
    }
    initializePage();
}

void CreateTestWizard::checkValidPage()
{
    if (selectedId < pages.contains(selectedId))
        pages[selectedId]->isComplete();
}
