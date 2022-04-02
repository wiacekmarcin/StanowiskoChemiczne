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
#include <QTreeWidgetItem>
#include <QDebug>

#include "urzadzenia.h"
#include "glowneokno.h"

CreateTestWizard::CreateTestWizard(QWidget *parent) :
    QStackedWidget(parent),
    dlgOtwarte(nullptr)
{
    selectedId = TestPage::PAGE_U;
    zamknietaKomoraLewa = false;
    zamknietaKomoraPrawa = false;
    setObjectName(QString::fromUtf8("TestWizard"));
}

void CreateTestWizard::setUstawienia(const Ustawienia & ust)
{
    numberInitDozCycles = ust.getNrInitializeCycles();
    for (short id = 0; id < Ustawienia::maxCzujekCyfrIn; ++id)
        m_namesZawory[id] = ust.wejscie(0x1 << id);
}



CreateTestWizard::~CreateTestWizard()
{

}

void CreateTestWizard::init(Urzadzenia * u, const Ustawienia & ust,
                            const QString & testName)
{
    NowyTest_1 * page_1 = new NowyTest_1(testName, this);
    addPage(page_1, TestPage::PAGE_1, 1, ust, u);

    NowyTest_2 * page_2 = new NowyTest_2(u, ust.getNrInitializeCycles(), this);
    connect(u, &Urzadzenia::digitalAllRead, page_2, &TestPage::readAll);
    addPage(page_2, TestPage::PAGE_2, 2, ust, u);

    NowyTest_3 * page_3 = new NowyTest_3(u, ust.getCisnienieProzni(), this);
    connect(this, &CreateTestWizard::openZawor, page_3, &NowyTest_3::openZawor);
    connect(this, &CreateTestWizard::cisnienieVal, page_3, &NowyTest_3::cisnienieKomory);
    addPage(page_3, TestPage::PAGE_3, 3, ust, u);


    addPage(new NowyTest_4(this), TestPage::PAGE_4, 4, ust, u);
    addPage(new NowyTest_5(this), TestPage::PAGE_5, 5, ust, u);
    addPage(new NowyTest_6(this), TestPage::PAGE_6, 6, ust, u);

    NowyTest_7 * page_7 = new NowyTest_7(u, this);
    connect(u, &Urzadzenia::digitalAllRead, page_7, &TestPage::readAll);
    addPage(page_7, TestPage::PAGE_7, 7, ust, u);


    addPage(new NowyTest_8(this), TestPage::PAGE_8, 8, ust, u);
    selectedId = TestPage::PAGE_1;
    finished = false;

    initializePage();
    connect(u, &Urzadzenia::digitalRead, this, &CreateTestWizard::changeDigitalIn);

    nextPage(TestPage::PAGE_1);
}

void CreateTestWizard::initializePage()
{
    //qDebug("CreateTestWizard::initializePage()");
    if (pages.contains(selectedId))
        pages[selectedId]->initializePage();
}

void CreateTestWizard::setField(TestPage::Value key, const QVariant &val)
{
    if (key == TestPage::nazwaTest)
        emit changeTestName(val.toString());
    values[key] = val;
}

QVariant CreateTestWizard::field(TestPage::Value key) const
{
    if (values.contains(key))
        return values[key];
    return QVariant();
}

void CreateTestWizard::addPage(TestPage *page, TestPage::PageId id, short step, const Ustawienia & ust, Urzadzenia *urz)
{
    TestPageForm *t = new TestPageForm(this);
    t->setStep(step);
    page->setId(id);


    t->addWidget(page);
    t->setCreateTestWizard(this);
    t->setLabels(ust);

    page->setParent(t->widgetFrame());
    page->setWizard(this);
    page->setForm(t);

    pages[id] = t;
    addWidget(t);

    connect(t, &TestPageForm::writeDigital, urz, &Urzadzenia::digitalWrite);
    connect(page, &TestPage::completeChanged, this, &CreateTestWizard::checkValidPage);
    connect(page, &TestPage::updateData, this, &CreateTestWizard::updatePageData);
}


TestPage *CreateTestWizard::currentPage() const
{
    return static_cast<TestPageForm*>(currentWidget())->widget();
}

void CreateTestWizard::setFinished(bool success)
{
    finished = success;
    emit finishedTest(success);

}

bool CreateTestWizard::checkZawory() const
{
    if (selectedId == 1 || selectedId == 2 || selectedId == 7 || selectedId == 8)
        return false;

    return !zawory[wentylacja_lewa] || !zawory[wentylacja_prawa] || !zawory[proznia] || !zawory[pom_stez_1] ||
            !zawory[pom_stez_2] || !zawory[wlot_powietrza];
}

void CreateTestWizard::changeDigitalIn(int id, bool value)
{
    //qDebug("CreateTestWizard::changeDigitalIn id = %d, val = %d", id, value);
    if (id == wentylacja_lewa || id == wentylacja_prawa || id == proznia || id == pom_stez_1
            || id == pom_stez_2 || id == wlot_powietrza || id == drzwi_lewe || id == drzwi_prawe
            ) {
        zawory[id] = value;
        emit openZawor(id, value);
        showWarning(checkZawory());
        if (dlgOtwarte != nullptr)
            dlgOtwarte->set(id, value);
    }

    if (id == drzwi_lewe) {
        zamknietaKomoraLewa = value;
        emit zamknietaKomora(getZamknietaKomora());
    }

    if (id == drzwi_prawe) {
        zamknietaKomoraPrawa = value;
        emit zamknietaKomora(getZamknietaKomora());
    }
}

void CreateTestWizard::changeAnalog(double val0, double val1, double val2, double val3, double val4, double val5, double val6,  double val7)
{
    double vals[8] = { val0, val1, val2, val3, val4, val5, val6, val7};
    /*
    a_vol1              = 0,
    a_vol2              = 1,
    a_o2                = 2,
    a_co2               = 3,
    a_cisn_komora       = 4,
    a_temp_komory       = 5,
    a_temp_parownik     = 6,
    a_8                 = 7
 */
    cisnienieKomory = vals[a_cisn_komora];
    emit cisnienieVal(cisnienieKomory);
    temperaturaKomory = vals[a_temp_komory];
}

void CreateTestWizard::changeDigitalOut(int16_t vals)
{
    for (QMap<TestPage::PageId, TestPageForm*>::iterator it = pages.begin(); it != pages.end(); ++it)
    {
        it.value()->setOnOff(vals);
    }
}

void CreateTestWizard::clickedZawory()
{
    dlgOtwarte = new OtwarteZawory(m_namesZawory, this);
    dlgOtwarte->set(wentylacja_lewa, zawory[wentylacja_lewa]);
    dlgOtwarte->set(wentylacja_prawa, zawory[wentylacja_prawa]);
    dlgOtwarte->set(proznia,  zawory[proznia]);
    dlgOtwarte->set(pom_stez_1,  zawory[pom_stez_1]);
    dlgOtwarte->set(pom_stez_2,  zawory[pom_stez_2]);
    dlgOtwarte->set(wlot_powietrza,  zawory[wlot_powietrza]);
    dlgOtwarte->set(drzwi_lewe,  zawory[drzwi_lewe]);
    dlgOtwarte->set(drzwi_prawe,  zawory[drzwi_prawe]);
    //dlgOtwarte->setSettings
    dlgOtwarte->exec();
    if (dlgOtwarte != nullptr)
        delete dlgOtwarte;
    dlgOtwarte = nullptr;
}

void CreateTestWizard::updatePageData()
{
    qDebug("%s:%d --- ", __FILE__, __LINE__);
    auto currPage = currentPage();
    if (currPage)
        currPage->changeData();
}

void CreateTestWizard::nextPage(TestPage::PageId id)
{
    //qDebug("nextPage %d", id);
    if (finished)
        return;

    if (pages.contains(id)) {
        selectedId = id;
        setCurrentWidget(pages[selectedId]);
    }
    initializePage();
    if (id == TestPage::PAGE_7) {
        //emit zaplon(field("zaplon").toString(), field("zaplonExt").toString());
        //emit triggerCamera(true);
        //emit pomiarCisnienia(1 /*cisnienie*/, 5*60&1000);
    }
    if (id == TestPage::PAGE_8)
        finished = true;
}

void CreateTestWizard::checkValidPage()
{
    if (pages.contains(selectedId))
        pages[selectedId]->isComplete();
}

void CreateTestWizard::showWarning(bool value)
{

}

bool CreateTestWizard::getZamknietaKomora() const
{
    return zamknietaKomoraLewa && zamknietaKomoraPrawa;
}


