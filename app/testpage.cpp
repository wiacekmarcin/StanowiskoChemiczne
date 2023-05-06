#include "testpage.h"
#include "createtestwizard.h"
#include "testpageform.h"
#include "common.h"
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
    wiz->setDebug(QString("Saving key=%1 = %2").arg(key).arg(val.toString()));
    wiz->setField(key, val);
}

QVariant TestPage::field(Value key) const
{
    //qDebug() << __FILE__ << __LINE__ << "wiz" << (unsigned long)wiz << "key=" << (int)key ; 
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

TestData * TestPage::getTestData()
{
    return wiz->getTestData();
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

}

TestPage::PageId TestPage::nextPageId() const
{
    return (PageId) ( (int)id+1);
}

void TestPage::nextPage(TestPage::PageId idPage)
{
    wiz->setDebug(QString("Initialize page = %1").arg(idPage));
    wiz->nextPage(idPage);
}

void TestPage::updateOutput(digitalOut mask, bool on)
{
    wiz->setDebug(QString("Set Output = %1 => %2").arg(mask).arg(on));
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
    updateOutput(o_wentylator, success);
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
#if SYMULATOR
    return true;
#endif    
    BOOLS_ZAWORY
    QString message;
    bool show = false;
    QMessageBox msgBox;
    
    if (!bi_wentylacja_lewa || !bi_wentylacja_prawa || !bi_pom_stez_1 || !bi_pom_stez_2 || !bi_wlot_powietrza || !bi_proznia ) {

        QString s(tr("Otwarte zawory : ["));
        if (!bi_wentylacja_lewa)
            s+=tr("wentylacja lewa,");
        if (!bi_wentylacja_prawa)
            s+=tr("wentylacja prawa,");
        if (!bi_proznia)
            s+=tr("próżnia,");
        if (!bi_wlot_powietrza)
            s+=tr("wlot powietrza,");
        if (!bi_pom_stez_1)
            s+=tr("pomiar stężenia 1,");
        if (!bi_pom_stez_2)
            s+=tr("pomiar stężenia 2,");
        s.remove(s.size()-1,1);
        s += tr(" ]. \n");
        message = s;
        show = true;
    }

    if (!bi_drzwi_lewe || !bi_drzwi_prawe ) {

        QString s(tr("Otwarte drzwi komory : ["));
        if (!bi_drzwi_prawe)
            s+=tr("prawe,");
        if (!bi_drzwi_lewe)
            s+=tr("lewe,");
        s.remove(s.size()-1,1);
        s += tr(" ]. \n");
        message += s;
        show = true;
    }
    if (show) {
        msgBox.setText(QString(tr("Wykryto nie prawidłowe ustawienie:\n%1")).arg(message));
        msgBox.setInformativeText(tr("Wszystkie zawory i drzwi powinny być zamknięte.\nCzy chcesz kontynuować"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setButtonText(QMessageBox::Yes, tr("Tak"));
        msgBox.setButtonText(QMessageBox::No, tr("Nie"));
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
#if SYMULATOR
    return true;
#endif 
    BOOLS_ZAWORY
    bool show = false;
    QMessageBox msgBox;
    QString msg;
    if(!bi_wentylacja_lewa || !bi_pom_stez_1 || !bi_wentylacja_prawa || !bi_wlot_powietrza || !bi_pom_stez_2) {
        show = true;
        QString s(tr("Otwarte zawory : [ "));
        if (!bi_wentylacja_lewa)
            s+=tr("wentylacja lewa,");
        if (!bi_wentylacja_prawa)
            s+=tr("wentylacja prawa,");
        if (!bi_wlot_powietrza)
            s+=tr("wlot powietrza,");
        if (!bi_pom_stez_1)
            s+=tr("pomiar stężenia 1,");
        if (!bi_pom_stez_2)
            s+=tr("pomiar stężenia 2,");
        s.remove(s.size()-1,1);
        msg = s + QString(tr(" ].\n"));
    }
    if (!bi_drzwi_prawe || !bi_drzwi_lewe) {
        show = true;
        QString s(tr("Otwarte drzwi komory : [ "));
            if (!bi_drzwi_prawe)
                s+=tr("prawe,");
            if (!bi_drzwi_lewe)
                s+=tr("lewe,");
            s.remove(s.size()-1,1);
            msg += s + QString(tr(" ].\n"));
    }

    if (bi_proznia) {
        show = true;
        msg += QString(tr("Zawor próżnia jest zamknięty.\n"));
    }

    if (!show)
        return true;

    msgBox.setText(QString(tr("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n")).arg(msg));
    msgBox.setInformativeText(tr("Zawór próznia powinien być otwarty, reszta zaworów powinna być zamknieta, dzwi komory powinny być zamknięte.\nCzy chcesz kontynuować"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, tr("Tak"));
    msgBox.setButtonText(QMessageBox::No, tr("Nie"));
    int ret = msgBox.exec();
    if (ret == QMessageBox::No) {
        setFinished(false);
        return false;
    }
    return false;
}

bool TestPage::sprawdzOtwarteZaworPowietrza()
{
#if SYMULATOR
    return true;
#endif 
    BOOLS_ZAWORY
    bool show = false;
    QMessageBox msgBox;
    QString msg;

    if(!bi_wentylacja_lewa || !bi_pom_stez_1 || !bi_wentylacja_prawa || !bi_proznia || !bi_pom_stez_2) {
        show = true;
        QString s(tr("Otwarte zawory : [ "));
        if (!bi_wentylacja_lewa)
            s+=tr("wentylacja lewa,");
        if (!bi_wentylacja_prawa)
            s+=tr("wentylacja prawa,");
        if (!bi_proznia)
            s+=tr("próżnia,");
        if (!bi_pom_stez_1)
            s+=tr("pomiar stężenia 1,");
        if (!bi_pom_stez_2)
            s+=tr("pomiar stężenia 2,");
        s.remove(s.size()-1,1);
        msg = s + QString(tr(" ].\n"));
    }
    if (!bi_drzwi_prawe || !bi_drzwi_lewe) {
        show = true;
        QString s(tr("Otwarte drzwi komory : [ "));
        if (!bi_drzwi_prawe)
            s+=tr("prawe,");
        if (!bi_drzwi_lewe)
            s+=tr("lewe,");
        s.remove(s.size()-1,1);
        msg += s + QString(tr(" ].\n"));
    }

    if (bi_wlot_powietrza) {
        show = true;
        msg += QString(tr("Zawór wlot powietrza jest zamknięty.\n"));
    }
    if (!show)
        return true;
    msgBox.setText(QString(tr("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n")).arg(msg));
    msgBox.setInformativeText(tr("Zawór wlot powietrza powinien być otwarty, reszta zaworów powinna być zamknięta. Drzwi od komory powinny być zamknięte.\nCzy chcesz kontynuować"));

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, tr("Tak"));
    msgBox.setButtonText(QMessageBox::No, tr("Nie"));
    int ret = msgBox.exec();
    if (ret == QMessageBox::No) {
        setFinished(false);
        return false;
    }
    return false;
}

bool TestPage::sprawdzOtwarteZaworStezenia()
{
#if SYMULATOR
    return true;
#endif 
    BOOLS_ZAWORY
    bool show = false;
    QMessageBox msgBox;
    QString msg;

    if(!bi_wentylacja_lewa || !bi_wentylacja_prawa || !bi_proznia || !bi_wlot_powietrza) {
        show = true;
        QString s(tr("Otwarte zawory : [ "));
        if (!bi_wentylacja_lewa)
            s+=tr("wentylacja lewa,");
        if (!bi_wentylacja_prawa)
            s+=tr("wentylacja prawa,");
        if (!bi_proznia)
            s+=tr("próżnia,");
        if (!bi_wlot_powietrza)
            s+=tr("wlot powietrza,");
        s.remove(s.size()-1,1);
        msg = s + QString(tr(" ].\n"));
    }
    if (!bi_drzwi_prawe || !bi_drzwi_lewe) {
        show = true;
        QString s(tr("Otwarte drzwi komory : [ "));
        if (!bi_drzwi_prawe)
            s+=tr("prawe,");
        if (!bi_drzwi_lewe)
            s+=tr("lewe,");
        s.remove(s.size()-1,1);
        msg += s + QString(tr(" ].\n"));
    }

    if (bi_pom_stez_1 || bi_pom_stez_2) {
        show = true;
        if (bi_pom_stez_1 && bi_pom_stez_2)
            msg += QString(tr("Oba zawory do pomiaru stężen są zamknięte.\n"));
        else if (bi_pom_stez_1)
            msg += QString(tr("Lewy zawór do pomiaru stężen jest zamknięty.\n"));
        else if (bi_pom_stez_2)
            msg += QString(tr("Prawy zawór do pomiaru stężen jest zamknięty.\n"));
    }
    if (!show)
        return true;
    msgBox.setText(QString(tr("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n")).arg(msg));
    msgBox.setInformativeText(tr("Zawory do pomiarów stężen powinny być otwarte, reszta zaworów powinna być zamknięta. Drzwi od komory powinny być zamknięte. \n Czy chcesz kontynuować?"));

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, tr("Tak"));
    msgBox.setButtonText(QMessageBox::No, tr("Nie"));
    int ret = msgBox.exec();
    if (ret == QMessageBox::No) {
        setFinished(false);
        return false;
    }
    return false;
}

bool TestPage::sprawdzOtwarteZawor2Calowe()
{
#if SYMULATOR
    return true;
#endif 
    BOOLS_ZAWORY
    bool show = false;
    QMessageBox msgBox;
    QString msg;

    if(!bi_pom_stez_1 || !bi_pom_stez_2 || !bi_proznia || !bi_wlot_powietrza) {
        show = true;
        QString s(tr("Otwarte zawory : [ "));
        if (!bi_pom_stez_1)
            s+=tr("pomiar stężenia 1,");
        if (!bi_pom_stez_2)
            s+=tr("pomiar stężenia 2,");
        if (!bi_proznia)
            s+=tr("próżnia,");
        if (!bi_wlot_powietrza)
            s+=tr("wlot powietrza,");
        s.remove(s.size()-1,1);
        msg = s + QString(tr(" ].\n"));
    }
    if (!bi_drzwi_prawe || !bi_drzwi_lewe) {
        show = true;
        QString s(tr("Otwarte drzwi komory : [ "));
        if (!bi_drzwi_prawe)
            s+=tr("prawe,");
        if (!bi_drzwi_lewe)
            s+=tr("lewe,");
        s.remove(s.size()-1,1);
        msg += s + QString(tr(" ].\n"));
    }

    if (bi_wentylacja_lewa || bi_wentylacja_prawa) {
        show = true;
        if (bi_wentylacja_lewa && bi_wentylacja_prawa)
            msg += QString(tr("Oba zawory do przedmuchu są zamknięte.\n"));
        else if (bi_wentylacja_lewa)
            msg += QString(tr("Lewy zawór do przedmuchu jest zamknięty.\n"));
        else if (bi_wentylacja_prawa)
            msg += QString(tr("Prawy zawór do przedmuchu jest zamknięty.\n"));
    }
    if (!show)
        return true;
    msgBox.setText(QString(tr("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n")).arg(msg));
    msgBox.setInformativeText(tr("Zawory do przedmuchu powietrza powinny być otwarte, reszta zaworów powinna być zamknieta. Drzwi od komory powinny być zamknięte.\nCzy chcesz kontynuować"));

    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, tr("Tak"));
    msgBox.setButtonText(QMessageBox::No, tr("Nie"));
    int ret = msgBox.exec();
    if (ret == QMessageBox::No) {
        return false;
    } else
        return true;
    return false;
}


#define CHECK_ZAWOR(Z, T)     do { if (((mask & Z) == Z) && !b##Z) {\
                s+=T; openZ = true; } }while(false)

bool TestPage::sprawdzOtwarteZawory(uint16_t mask)
{
#if SYMULATOR
    return true;
#endif 
    BOOLS_ZAWORY;
    bool op = false;
    QString msg;
    {
        bool openZ = false;
        QString s = QString(tr("Wykryto otwarte zawory : [ "));

        CHECK_ZAWOR(i_wentylacja_lewa, tr("wentylacja lewa,"));
        CHECK_ZAWOR(i_wentylacja_prawa, tr("wentylacja prawa,"));

        CHECK_ZAWOR(i_pom_stez_1, tr("pomiar stężenia 1,"));
        CHECK_ZAWOR(i_pom_stez_2, tr("pomiar stężenia 2,"));

        CHECK_ZAWOR(i_proznia, tr("próznia,"));
        CHECK_ZAWOR(i_wlot_powietrza, tr("wlot powietrza,"));

        if (openZ) {
            op = true;
            s.remove(s.size()-1,1);
            msg = s + QString(tr(" ].\n"));
        }
    }
    {
        bool openZ = false;
        QString s = QString(tr("Wykryto otwarte drzwi : [ "));

        CHECK_ZAWOR(i_drzwi_prawe, tr("prawe,"));
        CHECK_ZAWOR(i_drzwi_lewe, tr("lewe,"));

        if (openZ) {
            op = true;
            s.remove(s.size()-1,1);
            msg += s + QString(tr(" ].\n"));
        }
    }


    if (!op)
        return true;
    msg += QString(tr("\nZamknij drzwi, aby kontynuować"));
    QMessageBox::warning(this, tr("Otwarty zawór lub drzwi komory"), msg);
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
