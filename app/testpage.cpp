#include "testpage.h"
#include "createtestwizard.h"
#include "testpageform.h"

#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

TestPage::TestPage(QWidget *parent) :
    QWidget(parent), wiz(nullptr), form(nullptr)

{
    prevVals = 0;
    
}

TestPage::~TestPage()
{

}

void TestPage::setField(Value key, const QVariant &val)
{
    
    wiz->setField(key, val);
}

QVariant TestPage::field(Value key) const
{
    
    return wiz->field(key);
}

void TestPage::setWizard(CreateTestWizard *wizard)
{
    
    wiz = wizard;
}

CreateTestWizard *TestPage::wizard() const
{
    
    return wiz;
}

TestData &TestPage::testData()
{
    return wiz->testData();
}

float TestPage::getCzujnik(analogIn czujnik)
{
    return wiz->getCzujnik(czujnik);
}

TestPage::PageId TestPage::getId() const
{
    return id;
}

void TestPage::setId(TestPage::PageId value)
{
    id = value;
}

QString TestPage::title() const
{
    return m_title;
}

void TestPage::setTitle(const QString &t)
{
    if (form)
        form->setTitle(t);
    m_title = t;
}

QString TestPage::subTitle() const
{
    return m_subTitle;
}

void TestPage::setSubTitle(const QString &t)
{
    if (form)
        form->setSubTitle(t);
    m_subTitle = t;
}

TestPageForm *TestPage::getForm() const
{
    return form;
}

void TestPage::setForm(TestPageForm *value)
{
    form = value;
    setSubTitle(subTitle());
    setTitle(title());
}

void TestPage::initializePage()
{
    //
}

TestPage::PageId TestPage::nextPageId() const
{
    return (PageId) ( (int)id+1);
}

void TestPage::nextPage(TestPage::PageId idPage)
{
    wiz->nextPage(idPage);
}

void TestPage::updateOutput(digitalOut mask, bool on)
{
    wiz->updateOutput(mask, on);
}

void TestPage::cykleDozownik(uint8_t nr, uint32_t steps)
{
    wiz->runCykleDozownik(nr, steps);
}

void TestPage::dozownikMl(uint8_t nr, uint32_t mlx10)
{
    wiz->runDozownikMl(nr, mlx10);
}

void TestPage::checkPositionHome()
{
    wiz->runCheckPositionHome();
}

void TestPage::runZaplon(ZaplonRodzaj idZaplon)
{
    wiz->runZaplon(idZaplon);
}



#define FUN_ZAWOR(Z) bool TestPage::z##Z() { return wiz->z##Z(); }
FUN_ZAWOR(i_drzwi_prawe)
FUN_ZAWOR(i_drzwi_lewe)
FUN_ZAWOR(i_wentylacja_prawa)
FUN_ZAWOR(i_wentylacja_lewa)
FUN_ZAWOR(i_pom_stez_1)
FUN_ZAWOR(i_pom_stez_2)
FUN_ZAWOR(i_wlot_powietrza)
FUN_ZAWOR(i_proznia)
FUN_ZAWOR(i_pilot)

void TestPage::setFinished(bool success)
{
    updateOutput(o_pompa_prozniowa, false);
    updateOutput(o_pompa_powietrza, false);
    updateOutput(o_wentylator, false);
    updateOutput(o_mieszadlo, false);
    updateOutput(o_trigger, false);
    wiz->setFinished(success);
}

#define B_ZAWOR(Z) bool b##Z = z##Z()
#define BOOLS_ZAWORY B_ZAWOR(i_wentylacja_lewa);\
                     B_ZAWOR(i_wentylacja_prawa);\
                     B_ZAWOR(i_pom_stez_1);\
                     B_ZAWOR(i_pom_stez_2);\
                     B_ZAWOR(i_wlot_powietrza);\
                     B_ZAWOR(i_proznia);\
                     B_ZAWOR(i_drzwi_prawe);\
                     B_ZAWOR(i_drzwi_lewe);

bool TestPage::sprawdzZawory(QPushButton *pbOk_1, QLabel *arrow_1, QFrame *frame_2)
{
    BOOLS_ZAWORY
    QString message;
    bool show = false;
    QMessageBox msgBox;
    
    if (!bi_wentylacja_lewa || !bi_wentylacja_prawa || !bi_pom_stez_1 || !bi_pom_stez_2 || !bi_wlot_powietrza || !bi_proznia ) {

        QString s("Otwarte zawory : [");
        if (!bi_wentylacja_lewa)
            s+="wentylacja lewa,";
        if (!bi_wentylacja_prawa)
            s+="wentylacja prawa,";
        if (!bi_proznia)
            s+="proznia,";
        if (!bi_wlot_powietrza)
            s+="wlot powietza,";
        if (!bi_pom_stez_1)
            s+="pomiar stezenia 1,";
        if (!bi_pom_stez_2)
            s+="pomiar stezenia 2,";
        s.remove(s.size()-1,1);
        s += " ]. \n";
        message = s;
        show = true;
    }

    if (!bi_drzwi_lewe || !bi_drzwi_prawe ) {

        QString s("Otwarte drzwi komory : [");
        if (!bi_drzwi_prawe)
            s+="prawe,";
        if (!bi_drzwi_lewe)
            s+="lewe,";
        s.remove(s.size()-1,1);
        s += " ]. \n";
        message += s;
        show = true;
    }
    if (show) {
        msgBox.setText(QString("Wykryto nie prawidłowe ustawienie: \n%1").arg(message));
        msgBox.setInformativeText("Wszystkie zawory i drzwi powinny być zamknięte. \n Czy chcesz kontynuować");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if (ret == QMessageBox::No) {
            setFinished(false);
        }
        return false;
    } else {
        pbOk_1->setEnabled(false);
        arrow_1->setVisible(false);
        frame_2->setVisible(true);
        return true;
    }
}

bool TestPage::sprawdzOtwarteZaworProzni()
{
    BOOLS_ZAWORY
    bool show = false;
    QMessageBox msgBox;
    QString msg;
    if(!bi_wentylacja_lewa || !bi_pom_stez_1 || !bi_wentylacja_prawa || !bi_wlot_powietrza || !bi_pom_stez_2) {
        show = true;
        QString s("Otwarte zawory : [ ");
        if (!bi_wentylacja_lewa)
            s+="wentylacja lewa,";
        if (!bi_wentylacja_prawa)
            s+="wentylacja prawa,";
        if (!bi_wlot_powietrza)
            s+="wlot powietza,";
        if (!bi_pom_stez_1)
            s+="pomiar stezenia 1,";
        if (!bi_pom_stez_2)
            s+="pomiar stezenia 2,";
        s.remove(s.size()-1,1);
        msg = s + QString(" ].\n");
    }
    if (!bi_drzwi_prawe || !bi_drzwi_lewe) {
        show = true;
        QString s("Otwarte drzwi komory : [ ");
            if (!bi_drzwi_prawe)
                s+="prawe,";
            if (!bi_drzwi_lewe)
                s+="lewe,";
            s.remove(s.size()-1,1);
            msg += s + QString(" ].\n");
    }

    if (bi_proznia) {
        show = true;
        msg += QString("Zawor proznia jest zamknięty.\n");
    }

    if (!show)
        return true;

    msgBox.setText(QString("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n").arg(msg));
    msgBox.setInformativeText("Zawór próznia powinien być otwarty, reszta zaworów powinna być zamknieta, dzwi komory powinny być zamknięte. \n Czy chcesz kontynuować");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    if (ret == QMessageBox::No) {
        setFinished(false);
        return false;
    }
    return false;
}

bool TestPage::sprawdzOtwarteZaworPowietrza()
{
    BOOLS_ZAWORY
    bool show = false;
    QMessageBox msgBox;
    QString msg;

    if(!bi_wentylacja_lewa || !bi_pom_stez_1 || !bi_wentylacja_prawa || !bi_proznia || !bi_pom_stez_2) {
        show = true;
        QString s("Otwarte zawory : [ ");
        if (!bi_wentylacja_lewa)
            s+="wentylacja lewa,";
        if (!bi_wentylacja_prawa)
            s+="wentylacja prawa,";
        if (!bi_proznia)
            s+="proznia,";
        if (!bi_pom_stez_1)
            s+="pomiar stezenia 1,";
        if (!bi_pom_stez_2)
            s+="pomiar stezenia 2,";
        s.remove(s.size()-1,1);
        msg = s + QString(" ].\n");
    }
    if (!bi_drzwi_prawe || !bi_drzwi_lewe) {
        show = true;
        QString s("Otwarte drzwi komory : [ ");
        if (!bi_drzwi_prawe)
            s+="prawe,";
        if (!bi_drzwi_lewe)
            s+="lewe,";
        s.remove(s.size()-1,1);
        msg += s + QString(" ].\n");
    }

    if (bi_wlot_powietrza) {
        show = true;
        msg += QString("Zawor wlot powietrza jest zamknięty.\n");
    }
    if (!show)
        return true;
    msgBox.setText(QString("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n").arg(msg));
    msgBox.setInformativeText("Zawór wlot powietrza powinien być otwarty, reszta zaworów powinna być zamknieta. Drzwi od komory powinny być zamknięte. \n Czy chcesz kontynuować");

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    if (ret == QMessageBox::No) {
        setFinished(false);
        return false;
    }
    return false;
}

bool TestPage::sprawdzOtwarteZaworStezenia()
{
    BOOLS_ZAWORY
    bool show = false;
    QMessageBox msgBox;
    QString msg;

    if(!bi_wentylacja_lewa || !bi_wentylacja_prawa || !bi_proznia || !bi_wlot_powietrza) {
        show = true;
        QString s("Otwarte zawory : [ ");
        if (!bi_wentylacja_lewa)
            s+="wentylacja lewa,";
        if (!bi_wentylacja_prawa)
            s+="wentylacja prawa,";
        if (!bi_proznia)
            s+="proznia,";
        if (!bi_wlot_powietrza)
            s+="wlot_powietrze,";
        s.remove(s.size()-1,1);
        msg = s + QString(" ].\n");
    }
    if (!bi_drzwi_prawe || !bi_drzwi_lewe) {
        show = true;
        QString s("Otwarte drzwi komory : [ ");
        if (!bi_drzwi_prawe)
            s+="prawe,";
        if (!bi_drzwi_lewe)
            s+="lewe,";
        s.remove(s.size()-1,1);
        msg += s + QString(" ].\n");
    }

    if (bi_pom_stez_1 || bi_pom_stez_2) {
        show = true;
        if (bi_pom_stez_1 && bi_pom_stez_2)
            msg += QString("Oba zawory do pomiaru stężen są zamknięte.\n");
        else if (bi_pom_stez_1)
            msg += QString("Lewy zawor do pomiaru stężen jest zamknięty.\n");
        else if (bi_pom_stez_2)
            msg += QString("Prawy zawor do pomiaru stężen jest zamknięty.\n");
    }
    if (!show)
        return true;
    msgBox.setText(QString("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n").arg(msg));
    msgBox.setInformativeText("Zawoy do pomiarów stężen powinny być otwarte, reszta zaworów powinna być zamknieta. Drzwi od komory powinny być zamknięte. \n Czy chcesz kontynuować");

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    if (ret == QMessageBox::No) {
        setFinished(false);
        return false;
    }
    return false;
}

bool TestPage::sprawdzOtwarteZawor2Calowe()
{
    BOOLS_ZAWORY
    bool show = false;
    QMessageBox msgBox;
    QString msg;

    if(!bi_pom_stez_1 || !bi_pom_stez_2 || !bi_proznia || !bi_wlot_powietrza) {
        show = true;
        QString s("Otwarte zawory : [ ");
        if (!bi_pom_stez_1)
            s+="pomiar stężenia 1,";
        if (!bi_pom_stez_2)
            s+="pomiar stezenia 2,";
        if (!bi_proznia)
            s+="proznia,";
        if (!bi_wlot_powietrza)
            s+="wlot_powietrze,";
        s.remove(s.size()-1,1);
        msg = s + QString(" ].\n");
    }
    if (!bi_drzwi_prawe || !bi_drzwi_lewe) {
        show = true;
        QString s("Otwarte drzwi komory : [ ");
        if (!bi_drzwi_prawe)
            s+="prawe,";
        if (!bi_drzwi_lewe)
            s+="lewe,";
        s.remove(s.size()-1,1);
        msg += s + QString(" ].\n");
    }

    if (bi_wentylacja_lewa || bi_wentylacja_prawa) {
        show = true;
        if (bi_wentylacja_lewa && bi_wentylacja_prawa)
            msg += QString("Oba zawory do przedmuchu są zamknięte.\n");
        else if (bi_wentylacja_lewa)
            msg += QString("Lewy zawor do przedmuchu jest zamknięty.\n");
        else if (bi_wentylacja_prawa)
            msg += QString("Prawy zawor do przedmuchu jest zamknięty.\n");
    }
    if (!show)
        return true;
    msgBox.setText(QString("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n").arg(msg));
    msgBox.setInformativeText("Zawoy do przedmuchu powietrza powinny być otwarte, reszta zaworów powinna być zamknieta. Drzwi od komory powinny być zamknięte. \n Czy chcesz kontynuować");

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    if (ret == QMessageBox::No) {
        setFinished(false);
        return false;
    }
    return false;
}


#define CHECK_ZAWOR(Z, T)     do { if (((mask & Z) == Z) && !b##Z) {\
                s+=T; openZ = true; } }while(false)

bool TestPage::sprawdzOtwarteZawory(uint16_t mask)
{
    BOOLS_ZAWORY;
    bool op = false;
    QString msg;
    {
        bool openZ = false;
        QString s = QString("Wykryto otwarte zawory : [ ");

        CHECK_ZAWOR(i_wentylacja_lewa, "wentylacja lewa,");
        CHECK_ZAWOR(i_wentylacja_prawa, "wentylacja prawa,");

        CHECK_ZAWOR(i_pom_stez_1, "pomiar stezenia 1,");
        CHECK_ZAWOR(i_pom_stez_2, "pomiar stezenia 2,");

        CHECK_ZAWOR(i_proznia, "próznia,");
        CHECK_ZAWOR(i_wlot_powietrza, "wlot powietrza,");

        if (openZ) {
            op = true;
            s.remove(s.size()-1,1);
            msg = s + QString(" ].\n");
        }
    }
    {
        bool openZ = false;
        QString s = QString("Wykryto otwarte drzwi : [ ");

        CHECK_ZAWOR(i_drzwi_prawe, "prawe,");
        CHECK_ZAWOR(i_drzwi_lewe, "lewe,");

        if (openZ) {
            op = true;
            s.remove(s.size()-1,1);
            msg += s + QString(" ].\n");
        }
    }


    if (!op)
        return true;
    msg += QString("\nZamknij aby kontynuować");
    QMessageBox::warning(this, "Otwarty zawór lub drzwi komory", msg);
    return false;
}

void TestPage::setZ_criticalMask(uint16_t newZ_criticalMask)
{
    wiz->setZ_criticalMask(newZ_criticalMask);
}

void TestPage::setZ_warningMask(uint16_t newZ_warningMask)
{
    wiz->setZ_warningMask(newZ_warningMask);
}
