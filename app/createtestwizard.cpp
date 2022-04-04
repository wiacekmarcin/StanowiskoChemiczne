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
#include "nowytest_9.h"

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
    setObjectName(QString::fromUtf8("TestWizard"));
    zaworyMap[drzwi_prawe] = false;
    zaworyMap[wentylacja_lewa] = false;
    zaworyMap[proznia] = false;
    zaworyMap[pom_stez_1] = false;
    zaworyMap[drzwi_lewe] = false;
    zaworyMap[wentylacja_prawa] = false;
    zaworyMap[wlot_powietrza] = false;
    zaworyMap[pom_stez_2] = false;
    zaworyMap[pilot] = false;
}


CreateTestWizard::~CreateTestWizard()
{

}

void CreateTestWizard::init(const Ustawienia & ust,
                            const QString & testName)
{
    numberInitDozCycles = ust.getNrInitializeCycles();
    //for (short id = 0; id < Ustawienia::maxCzujekCyfrIn; ++id)
    //    m_namesZawory[id] = ust.wejscie(0x1 << id);
    //qDebug("%s:%d",__FILE__,__LINE__);
    NowyTest_1 * page_1 = new NowyTest_1(testName, this);
    //qDebug("%s:%d",__FILE__,__LINE__);
     addPage(page_1, TestPage::PAGE_1, 1);
    //qDebug("%s:%d",__FILE__,__LINE__);

    NowyTest_2 * page_2 = new NowyTest_2(ust.getNrInitializeCycles(), this);
    //qDebug("%s:%d",__FILE__,__LINE__);
    addPage(page_2, TestPage::PAGE_2, 2);
    //qDebug("%s:%d",__FILE__,__LINE__);

    NowyTest_3 * page_3 = new NowyTest_3(this);
    //qDebug("%s:%d",__FILE__,__LINE__);
    addPage(page_3, TestPage::PAGE_3, 3);
    //qDebug("%s:%d",__FILE__,__LINE__);

    NowyTest_4 * page_4 = new NowyTest_4(this);
    //qDebug("%s:%d",__FILE__,__LINE__);
    addPage(page_4, TestPage::PAGE_4, 4);
    //qDebug("%s:%d",__FILE__,__LINE__);

    NowyTest_5 * page_5 = new NowyTest_5(this);
    //qDebug("%s:%d",__FILE__,__LINE__);
    addPage(page_5 , TestPage::PAGE_5, 5);
    //qDebug("%s:%d",__FILE__,__LINE__);

    NowyTest_6 * page_6 = new NowyTest_6(this);
    //qDebug("%s:%d",__FILE__,__LINE__);
    addPage(page_6, TestPage::PAGE_6, 6);
    //qDebug("%s:%d",__FILE__,__LINE__);

    NowyTest_7 * page_7 = new NowyTest_7(this);
    //qDebug("%s:%d",__FILE__,__LINE__);
    addPage(page_7, TestPage::PAGE_7, 7);
    //qDebug("%s:%d",__FILE__,__LINE__);

    addPage(new NowyTest_8(this), TestPage::PAGE_8, 8);

    addPage(new NowyTest_9(this), TestPage::PAGE_9, 9);
    //qDebug("%s:%d",__FILE__,__LINE__);
    selectedId = TestPage::PAGE_1;
    finished = false;

    initializePage();
    nextPage(TestPage::PAGE_1);
}

void CreateTestWizard::initializePage()
{
    ////qDebug("CreateTestWizard::initializePage()");
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

void CreateTestWizard::addPage(TestPage *page, TestPage::PageId id, short step)
{
    TestPageForm *t = new TestPageForm(this);
    t->setStep(step);
    page->setId(id);

    t->addWidget(page);
    t->setCreateTestWizard(this);

    page->setParent(t->widgetFrame());
    page->setWizard(this);
    page->setForm(t);

    pages[id] = t;
    addWidget(t);


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

void CreateTestWizard::changeDigitalIn(uint16_t id, bool value)
{
    //qDebug("CreateTestWizard::changeDigitalIn id = %d, val = %d", id, value);
    zaworyMap[id] = value;
    if (selectedId == TestPage::PAGE_6 && id == pilot) {
            currentPage()->updateWejscia();
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
    if (selectedId == TestPage::PAGE_3)
        currentPage()->setCisnKomory(vals[4]);
}

void CreateTestWizard::dozownikDone(bool success)
{
    if (selectedId == TestPage::PAGE_2 || selectedId == TestPage::PAGE_4) {
        currentPage()->dozownikDone(success);
    }

}

void CreateTestWizard::nextPage(TestPage::PageId id)
{
    ////qDebug("nextPage %d", id);
    if (finished)
        return;

    if (pages.contains(id)) {
        selectedId = id;
        setCurrentWidget(pages[selectedId]);
    }
    initializePage();
    if (id == TestPage::PAGE_8)
        finished = true;
}

void CreateTestWizard::showWarning(bool value)
{

}


#define ZAWOR_DEFINE(Z) bool CreateTestWizard::z_##Z() { return zaworyMap[Z]; }
ZAWOR_DEFINE(drzwi_prawe)
ZAWOR_DEFINE(drzwi_lewe)
ZAWOR_DEFINE(wentylacja_lewa)
ZAWOR_DEFINE(wentylacja_prawa)
ZAWOR_DEFINE(proznia)
ZAWOR_DEFINE(wlot_powietrza)
ZAWOR_DEFINE(pom_stez_1)
ZAWOR_DEFINE(pom_stez_2)
ZAWOR_DEFINE(pilot)

void CreateTestWizard::updateOutput(uint16_t mask, bool on)
{
    emit writeOutValues(mask, on);
}

void CreateTestWizard::runZaplon(short id)
{
    //qDebug("%s:%d", __FILE__, __LINE__);
    emit zaplon(id);
}

void CreateTestWizard::runCykleDozownik(uint8_t nr, uint32_t steps)
{
    emit cykleDozownik(nr, steps);
}

