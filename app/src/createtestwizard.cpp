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

#include "otwartezawory.h"

#include <QAbstractButton>
#include <QVariant>
#include <QDebug>

CreateTestWizard::CreateTestWizard(QWidget *parent) :
    QStackedWidget(parent),
    dlgOtwarte(nullptr)
{
    selectedId = -1;
    init();
    zamknietaKomoraA = false;
    zamknietaKomoraB = false;
}

void CreateTestWizard::setTestData(const TestData &dt)
{
    setField(QString("nazwa"), QVariant::fromValue(dt.getNazwa()));
    setField(QString("dozownik"), QVariant::fromValue(dt.getDozownik()));
    setField(QString("ciecz"), QVariant::fromValue(dt.getCiecz()));
    setField(QString("objetosc"), QVariant::fromValue(dt.getObjetosc()));
    setField(QString("zaplon"), QVariant::fromValue(dt.getZaplon()));
    setField(QString("zaplonExt"), QVariant::fromValue(dt.getZaplonExt()));
    selectedId = 1;
    initializePage();
}

CreateTestWizard::~CreateTestWizard()
{

}

void CreateTestWizard::init()
{
    setObjectName(QString::fromUtf8("TestWizard"));

    for (int i=0; i<9; ++i) {
        zawory[i+1] = false;
        stezenia[i+1] = 0.0;
    }
    stezenia[10] = 0.0;

    addPage(new NowyTest_1(this), 1);
    addPage(new NowyTest_2(this), 2);
    addPage(new NowyTest_3(this), 3);
    addPage(new NowyTest_4(this), 4);
    addPage(new NowyTest_5(this), 5);
    addPage(new NowyTest_6(this), 6);
    addPage(new NowyTest_7(this), 7);
    addPage(new NowyTest_8(this), 8);
    selectedId = 1;
    finished = false;


    initializePage();
}

void CreateTestWizard::initializePage()
{
    qDebug("CreateTestWizard::initializePage()");
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
    TestPageForm *t = new TestPageForm(this);
    t->setId(id);
    page->setId(id);

    t->addWidget(page);
    t->setCreateTestWizard(this);

    page->setParent(t->widgetFrame());
    page->setWizard(this);
    page->setForm(t);

    pages[id] = t;
    addWidget(t);


    connect(t, &TestPageForm::clickButton, this, &CreateTestWizard::nextPage);
    connect(page, &TestPage::completeChanged, this, &CreateTestWizard::checkValidPage);
    connect(this, &CreateTestWizard::komora, page, &TestPage::komora);
    page->komora(getZamknietaKomora());
}


TestPage *CreateTestWizard::currentPage() const
{
    return static_cast<TestPageForm*>(currentWidget())->widget();
}

bool CreateTestWizard::checkZawory() const
{
    if (selectedId == 1 || selectedId == 2 || selectedId == 7 || selectedId == 8)
        return false;

    return !zawory[wentyl_1] || !zawory[wentyl_2] || !zawory[proznia] || !zawory[pom_stez_1] ||
            !zawory[pom_stez_2] || !zawory[powietrze];
}

void CreateTestWizard::changeDigitalIn(int id, bool value)
{
    qDebug("CreateTestWizard::changeDigitalIn id = %d, val = %d", id, value);
    if (id == wentyl_1 || id == wentyl_2 || id == proznia || id == pom_stez_1
            || id == pom_stez_2 || id == powietrze) {
        zawory[id] = value;
        showWarning(checkZawory());
        if (dlgOtwarte != nullptr)
            dlgOtwarte->set(id, value);
    }

    if (id == kont_komora_A) {
        zamknietaKomoraA = value;
        emit komora(getZamknietaKomora());
    }

    if (id == kont_komora_B) {
        zamknietaKomoraB = value;
        emit komora(getZamknietaKomora());
    }
}

void CreateTestWizard::changeAnalog(int id, double value)
{
    if (id == wentyl_1 || id == wentyl_2 || id == proznia || id == pom_stez_1
            || id == pom_stez_2 || id == powietrze)
        zawory[id] = value;
}

void CreateTestWizard::clickedZawory()
{
    dlgOtwarte = new OtwarteZawory(this);
    dlgOtwarte->set(wentyl_1, zawory[wentyl_1]);
    dlgOtwarte->set(wentyl_2, zawory[wentyl_2]);
    dlgOtwarte->set(proznia,  zawory[proznia]);
    dlgOtwarte->set(pom_stez_1,  zawory[pom_stez_1]);
    dlgOtwarte->set(pom_stez_2,  zawory[pom_stez_2]);
    dlgOtwarte->set(powietrze,  zawory[powietrze]);
    dlgOtwarte->exec();
    if (dlgOtwarte != nullptr)
        delete dlgOtwarte;
    dlgOtwarte = nullptr;
}

void CreateTestWizard::zaworProzni(bool open)
{
    //emit setDigitalOut(int id, open);

}

void CreateTestWizard::pompaProzniowa(bool start)
{

}

void CreateTestWizard::mieszadlo(bool start)
{

}

void CreateTestWizard::zaworPowietrza(bool open)
{

}

void CreateTestWizard::pomiary(bool start)
{

}

void CreateTestWizard::pompaMembramowa(bool start)
{

}

void CreateTestWizard::pomiarSingle(int idCzujka)
{

}

void CreateTestWizard::pomiarStezen()
{

}

void CreateTestWizard::wentylator(bool start)
{

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

void CreateTestWizard::showWarning(bool value)
{
    if (pages.contains(selectedId))
        pages[selectedId]->showZaworWarning(value);
}

bool CreateTestWizard::getZamknietaKomora() const
{
    return zamknietaKomoraA && zamknietaKomoraB;
}


