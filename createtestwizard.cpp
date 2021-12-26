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
#include "nowytest_8.h"

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
    addPage(new NowyTest_8(this), 8);
    finished = false;
    initializePage();
}

void CreateTestWizard::initializePage()
{
    if (pages.contains(selectedId))
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
    qDebug("id = %d",  id);
    TestPageForm *t = new TestPageForm(this);
    t->setId(id);
    page->setId(id);
    t->addWidget(page);
    page->setParent(t->widgetFrame());
    page->setWizard(this);
    page->setForm(t);

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
    if (finished)
        return;

    if (pages.contains(id)) {
        selectedId = id;
        setCurrentWidget(pages[selectedId]);
    }
    initializePage();
    if (id == 7) {
        emit zaplon(field("zaplon").toString(), field("zaplonExt").toString());
        emit triggerCamera(true);
        emit pomiarCisnienia(1 /*cisnienie*/, 5*60&1000);
    }
    if (id == 8)
        finished = true;
}

void CreateTestWizard::checkValidPage()
{
    if (pages.contains(selectedId))
        pages[selectedId]->isComplete();
}
