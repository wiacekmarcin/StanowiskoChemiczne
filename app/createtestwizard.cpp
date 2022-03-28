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

#include "urzadzenia.h"
#include "glowneokno.h"

CreateTestWizard::CreateTestWizard(QWidget *parent) :
    QStackedWidget(parent),
    dlgOtwarte(nullptr)
{
    selectedId = -1;
    zamknietaKomoraLewa = false;
    zamknietaKomoraPrawa = false;
    setObjectName(QString::fromUtf8("TestWizard"));
    addPage(new NowyTest_1(this), 1);
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

void CreateTestWizard::setUstawienia(const Ustawienia & ust)
{
    numberInitDozCycles = ust.getNrInitializeCycles();
    for (short id = 0; id < Ustawienia::maxCzujekCyfrIn; ++id)
        m_namesZawory[id] = ust.wejscie(0x1 << id);
}



CreateTestWizard::~CreateTestWizard()
{

}

void CreateTestWizard::init(Urzadzenia * u, const Ustawienia & ust)
{

    NowyTest_2 * page_2 = new NowyTest_2(u, ust.getNrInitializeCycles(), this);
    connect(this, &CreateTestWizard::zamknietaKomora, page_2, &NowyTest_2::zamknietaKomora);
    addPage(page_2, 2);

    addPage(new NowyTest_3(this), 3);
    addPage(new NowyTest_4(this), 4);
    addPage(new NowyTest_5(this), 5);
    addPage(new NowyTest_6(this), 6);
    addPage(new NowyTest_7(this), 7);
    addPage(new NowyTest_8(this), 8);
    selectedId = 1;
    finished = false;

    initializePage();
    connect(u, &Urzadzenia::digitalRead, this, &CreateTestWizard::changeDigitalIn);
}

void CreateTestWizard::initializePage()
{
    //qDebug("CreateTestWizard::initializePage()");
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
}


TestPage *CreateTestWizard::currentPage() const
{
    return static_cast<TestPageForm*>(currentWidget())->widget();
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
    temperaturaKomory = vals[a_temp_komory];
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
    //qDebug("nextPage %d", id);
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
    return zamknietaKomoraLewa && zamknietaKomoraPrawa;
}


