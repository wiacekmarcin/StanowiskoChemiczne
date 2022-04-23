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
#include <QPrinter>
#include <QTextDocument>
#include <QTextCursor>

#include "urzadzenia.h"
#include "glowneokno.h"

#include <QMessageBox>

CreateTestWizard::CreateTestWizard(QWidget *parent) :
    QStackedWidget(parent),
    dlgOtwarte(nullptr)
{
    selectedId = TestPage::PAGE_U;
    setObjectName(QString::fromUtf8("TestWizard"));
    zaworyMap[i_drzwi_prawe] = false;
    zaworyMap[i_wentylacja_lewa] = false;
    zaworyMap[i_proznia] = false;
    zaworyMap[i_pom_stez_1] = false;
    zaworyMap[i_drzwi_lewe] = false;
    zaworyMap[i_wentylacja_prawa] = false;
    zaworyMap[i_wlot_powietrza] = false;
    zaworyMap[i_pom_stez_2] = false;
    zaworyMap[i_pilot] = false;
    showCrit = false;
    showWarn = false;
    registerPomiary = false;
    connect(this, &CreateTestWizard::criticalZaworOpenSignal, this, &CreateTestWizard::criticalZaworOpenSlot, Qt::QueuedConnection);
    connect(this, &CreateTestWizard::warningZaworOpenSignal, this, &CreateTestWizard::warningZaworOpenSlot, Qt::QueuedConnection);
}


CreateTestWizard::~CreateTestWizard()
{

}

void CreateTestWizard::setTestData(TestData &dt)
{
    this->dt = dt;
}

TestData &CreateTestWizard::testData()
{
    return this->dt;
}

void CreateTestWizard::init(const Ustawienia & ust,
                            const QString & testName)
{
    numberInitDozCycles = ust.getNrInitializeCycles();

    NowyTest_1 * page_1 = new NowyTest_1(testName, this);
     addPage(page_1, TestPage::PAGE_1, 1);


    NowyTest_2 * page_2 = new NowyTest_2(ust.getNrInitializeCycles(), this);
    addPage(page_2, TestPage::PAGE_2, 2);

    NowyTest_3 * page_3 = new NowyTest_3(this);
    addPage(page_3, TestPage::PAGE_3, 3);

    NowyTest_4 * page_4 = new NowyTest_4(this);
    addPage(page_4, TestPage::PAGE_4, 4);

    NowyTest_5 * page_5 = new NowyTest_5(this);
    addPage(page_5 , TestPage::PAGE_5, 5);

    NowyTest_6 * page_6 = new NowyTest_6(this);
    addPage(page_6, TestPage::PAGE_6, 6);

    NowyTest_7 * page_7 = new NowyTest_7(this);
    addPage(page_7, TestPage::PAGE_7, 7);

    addPage(new NowyTest_8(this), TestPage::PAGE_8, 8);

    addPage(new NowyTest_9(this), TestPage::PAGE_9, 9);

    selectedId = TestPage::PAGE_1;
    finished = false;

    initializePage();
    nextPage(TestPage::PAGE_1);
}

void CreateTestWizard::initializePage()
{
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

    if (id == TestPage::PAGE_9)
        t->visibleAbortBtn(false);
}


TestPage *CreateTestWizard::currentPage() const
{
    return static_cast<TestPageForm*>(currentWidget())->widget();
}

void CreateTestWizard::setFinished(bool success)
{
    finished = success;

    updateOutput(o_hv_onoff, false);
    updateOutput(o_hv_bezpiecznik, true);
    updateOutput(o_hv_iskra, false);
    updateOutput(o_mech_iskra, false);
    updateOutput(o_grzalka, false);
    updateOutput(o_pompa_prozniowa, false);
    if (!success)
        updateOutput(o_wentylator, false);
    updateOutput(o_mieszadlo, false);
    updateOutput(o_trigger, false);

    emit finishedTest(success);
    disconnect(this, nullptr, nullptr, nullptr);

    if (success) {
        QPrinter printer(QPrinter::PrinterResolution);
        QTextDocument * textDocument = new QTextDocument;

        QTextCursor cursor(textDocument);

        textDocument->setHtml(testData().getBody());
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName("test.pdf");
        printer.setPageSize(QPageSize(QPageSize::A4));
        printer.setFullPage(true);
        textDocument->print(&printer);
    }
}

void CreateTestWizard::changeDigitalIn(uint16_t id, bool value)
{
    
    zaworyMap[id] = value;
    if (selectedId == TestPage::PAGE_6 && id == i_pilot) {
            currentPage()->updateWejscia();
    }
    if (showWarn || showCrit)
        return;

    
    if (criticalMap[id] && !value && !showCrit) {
        showCrit = true;
        emit criticalZaworOpenSignal(id);
    }

    if (warningMap[id] && value && !showWarn) {
        showWarn = true;
        emit warningZaworOpenSignal(id);
    }
}

void CreateTestWizard::changeAnalog(double val0, double val1, double val2, double val3, double val4, double val5, double val6,  double val7)
{
    mutex.lock();
    double vals[8] = { val0, val1, val2, val3, val4, val5, val6, val7};
    if (registerPomiary){
        pomiary.append(QVector<float>({ static_cast<float>(val0),
                                        static_cast<float>(val1),
                                        static_cast<float>(val2),
                                        static_cast<float>(val3),
                                        static_cast<float>(val4),
                                        static_cast<float>(val5),
                                        static_cast<float>(val6),
                                        static_cast<float>(val7)}));
    }
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

    m_czujniki[a_vol1] = val0;
    m_czujniki[a_vol2] = val1;
    m_czujniki[a_o2] = val2;
    m_czujniki[a_co2] = val3;
    m_czujniki[a_cisn_komora] = val4;
    m_czujniki[a_temp_komory] = val5;
    m_czujniki[a_temp_parownik] = val6;
    m_czujniki[a_8] = val7;

    mutex.unlock();
    if (selectedId == TestPage::PAGE_3)
        currentPage()->setCisnKomory(vals[4]);

    if (selectedId == TestPage::PAGE_5 || selectedId == TestPage::PAGE_8) {
        //currentPage()->setStezenie(val0, val1, val2, val3);
    }
    /*
    if (rejestracja) {
        QList.append(struct vals);
    }
    */
}

void CreateTestWizard::dozownikDone(bool success)
{
    if (selectedId == TestPage::PAGE_2 || selectedId == TestPage::PAGE_4) {
        currentPage()->dozownikDone(success);
    }
}

void CreateTestWizard::checkPositionHomeDone(bool ok, bool d1, bool d2, bool d3, bool d4, bool d5)
{
    if (selectedId == TestPage::PAGE_2) {
        currentPage()->checkPositionHomeDone(ok, d1, d2, d3, d4, d5);
    }
}

void CreateTestWizard::criticalZaworOpenSlot(uint16_t idz)
{
    QString s("Wykryto otwarty zawor :  ");
    switch(idz) {
    case i_drzwi_prawe:       s+= QString("drzwi prawe komory"); break;
    case i_wentylacja_lewa:   s+= QString("zawór wentylacji lewy"); break;
    case i_proznia:           s+= QString("zawór próżni"); break;
    case i_pom_stez_1:        s+= QString("zawór od pomiaru stężenia 1"); break;
    case i_drzwi_lewe:        s+= QString("drzwi lewe komory"); break;
    case i_wentylacja_prawa:  s+= QString("zawór od wentylacji prawy"); break;
    case i_wlot_powietrza:    s+= QString("zawór od wlot powietrza"); break;
    case i_pom_stez_2:        s+= QString("zawór od pomiaru stężenia 1"); break;
    default:                s+= QString(""); break;

    }

    QMessageBox::critical(this, s, "Wykryto owarty zawór ( lub drzwi w komorze ), jego otwarcie spowodowało zakończenie testu");
    showCrit = false;
    setFinished(false);
    //delay(2);
    //emit readsInputs();
}

void CreateTestWizard::warningZaworOpenSlot(uint16_t idz)
{
    QString s("Wykryto zamkniety zawor :  ");
    switch(idz) {
    case i_drzwi_prawe:       s+= QString("drzwi prawe komory"); break;
    case i_wentylacja_lewa:   s+= QString("zawór wentylacji lewy"); break;
    case i_proznia:           s+= QString("zawór próżni"); break;
    case i_pom_stez_1:        s+= QString("zawór od pomiaru stężenia 1"); break;
    case i_drzwi_lewe:        s+= QString("drzwi lewe komory"); break;
    case i_wentylacja_prawa:  s+= QString("zawór od wentylacji prawy"); break;
    case i_wlot_powietrza:    s+= QString("zawór od wlot powietrza"); break;
    case i_pom_stez_2:        s+= QString("zawór od pomiaru stężenia 1"); break;
    default:                s+= QString(""); break;

    }

    QMessageBox::warning(this, s, "Wykryto zamknięty zawór ( lub drzwi w komorze ).");
    showWarn = false;
    //delay(2);
    //emit readsInputs();
}

void CreateTestWizard::nextPage(TestPage::PageId id)
{
    //
    if (finished)
        return;

    if (pages.contains(id)) {
        selectedId = id;
        setCurrentWidget(pages[selectedId]);
    }
    initializePage();
    if (id == TestPage::PAGE_3) {
        registerPomiary = true;
    }
    if (id == TestPage::PAGE_9) {
        registerPomiary = false;
        testData().setListValues(pomiary);
    }
    //    finished = true;
}


#define SETCRITICALZAWOR(M) criticalMap[M] = (newZ_criticalMask & M) == M
void CreateTestWizard::setZ_criticalMask(uint16_t newZ_criticalMask)
{

    SETCRITICALZAWOR(0x1);
    SETCRITICALZAWOR(0x2);
    SETCRITICALZAWOR(0x4);
    SETCRITICALZAWOR(0x8);
    SETCRITICALZAWOR(0x10);
    SETCRITICALZAWOR(0x20);
    SETCRITICALZAWOR(0x40);
    SETCRITICALZAWOR(0x80);
    SETCRITICALZAWOR(0x100);
}

#define SETWARNINGZAWOR(M) warningMap[M] = (newZ_warningMask & M) == M
void CreateTestWizard::setZ_warningMask(uint16_t newZ_warningMask)
{
    SETWARNINGZAWOR(0x1);
    SETWARNINGZAWOR(0x2);
    SETWARNINGZAWOR(0x4);
    SETWARNINGZAWOR(0x8);
    SETWARNINGZAWOR(0x10);
    SETWARNINGZAWOR(0x20);
    SETWARNINGZAWOR(0x40);
    SETWARNINGZAWOR(0x80);
    SETWARNINGZAWOR(0x100);
}

#define ZAWOR_DEFINE(Z) bool CreateTestWizard::z##Z() { return zaworyMap[Z]; }
ZAWOR_DEFINE(i_drzwi_prawe)
ZAWOR_DEFINE(i_drzwi_lewe)
ZAWOR_DEFINE(i_wentylacja_lewa)
ZAWOR_DEFINE(i_wentylacja_prawa)
ZAWOR_DEFINE(i_proznia)
ZAWOR_DEFINE(i_wlot_powietrza)
ZAWOR_DEFINE(i_pom_stez_1)
ZAWOR_DEFINE(i_pom_stez_2)
ZAWOR_DEFINE(i_pilot)

void CreateTestWizard::updateOutput(digitalOut mask, bool on)
{
    emit writeOutValues(mask, on);
}

void CreateTestWizard::runZaplon(ZaplonRodzaj id)
{
emit zaplon(id);
}

void CreateTestWizard::runCykleDozownik(uint8_t nr, uint32_t steps)
{
    emit cykleDozownik(nr, steps);
}

void CreateTestWizard::runDozownikMl(uint8_t nr, uint32_t mlx10)
{
    emit dozownikMl(nr, mlx10);
}

void CreateTestWizard::runCheckPositionHome()
{
    emit checkPositionHome();
}

void CreateTestWizard::runResetDozownik() {
    emit resetDozownik();
}

float CreateTestWizard::getCzujnik(analogIn czujnik)
{
    mutex.lock();
    return m_czujniki[czujnik];
    mutex.unlock();
}
