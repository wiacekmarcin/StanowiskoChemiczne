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
    initialPage();
}

CreateTestWizard::~CreateTestWizard()
{

}

void CreateTestWizard::init()
{
    setObjectName(QString::fromUtf8("TestWizard"));
    //resize(551, 564);

    QMetaObject::connectSlotsByName(this);
    //setWindowTitle("Test");
    //setOption(QWizard::IndependentPages);

    addPage(new NowyTest_1(this));
    //addPage(new NowyTest_2(this));
    //addPage(new NowyTest_3(this));
    //addPage(new NowyTest_4(this));
    //addPage(new NowyTest_5(this));
    //addPage(new NowyTest_6(this));
    //addPage(new NowyTest_7(this));

    //setButtonText(QWizard::NextButton, QString("Dalej"));

    //connect(this, &QWizard::currentIdChanged, this, &CreateTestWizard::changePage);
    selectedId = 0;
    initialPage();
}

void CreateTestWizard::initialPage()
{
    pages[selectedId]->initialPage();
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

void CreateTestWizard::addPage(TestPage *page)
{
    TestPageForm *t = new TestPageForm(this);
    t->addWidget(page);
    page->setParent(t);
    page->setId(pages.size());
    pages.append(page);
    addWidget(t);
    page->setWizard(this);
}
