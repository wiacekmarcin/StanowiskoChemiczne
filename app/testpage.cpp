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
    //qDebug"%s:%d",__FILE__,__LINE__);
}

TestPage::~TestPage()
{

}

void TestPage::setField(Value key, const QVariant &val)
{
    //qDebug"%s:%d",__FILE__,__LINE__);
    wiz->setField(key, val);
}

QVariant TestPage::field(Value key) const
{
    //qDebug"%s:%d",__FILE__,__LINE__);
    return wiz->field(key);
}

void TestPage::setWizard(CreateTestWizard *wizard)
{
    //qDebug"%s:%d",__FILE__,__LINE__);
    wiz = wizard;
}

CreateTestWizard *TestPage::wizard() const
{
    //qDebug"%s:%d",__FILE__,__LINE__);
    return wiz;
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
    //qDebug"%s:%d %p",__FILE__,__LINE__, form);
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
    //qDebug"%s:%d",__FILE__,__LINE__);
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
    //qDebug"%s:%d",__FILE__,__LINE__);
    form = value;
    setSubTitle(subTitle());
    setTitle(title());
}

void TestPage::initializePage()
{
    ////qDebug"TestPage::initializePage()");
}

TestPage::PageId TestPage::nextPageId() const
{
    return (PageId) ( (int)id+1);
}

void TestPage::nextPage(TestPage::PageId idPage)
{
    //qDebug"%s:%d",__FILE__,__LINE__);
    wiz->nextPage(idPage);
}

void TestPage::updateOutput(uint16_t mask, bool on)
{
    //qDebug"%s:%d %04x %d",__FILE__,__LINE__, mask, on);
    wiz->updateOutput(mask, on);
}

void TestPage::cykleDozownik(uint8_t nr, uint32_t steps)
{
    //qDebug"%s:%d",__FILE__,__LINE__);
    wiz->runCykleDozownik(nr, steps);
}

void TestPage::dozownikMl(uint8_t nr, uint32_t mlx10)
{
    //qDebug"%s:%d",__FILE__,__LINE__);
    wiz->runDozownikMl(nr, mlx10);
}

void TestPage::checkPositionHome()
{
    wiz->checkPositionHome();
}

void TestPage::runZaplon(short idZaplon)
{
    //qDebug"%s:%d %d, %p",__FILE__,__LINE__, idZaplon, wiz);

    wiz->runZaplon(idZaplon);
}



#define FUN_ZAWOR(Z) bool TestPage::z_##Z() { return wiz->z_##Z(); }
FUN_ZAWOR(drzwi_prawe)
FUN_ZAWOR(drzwi_lewe)
FUN_ZAWOR(wentylacja_prawa)
FUN_ZAWOR(wentylacja_lewa)
FUN_ZAWOR(pom_stez_1)
FUN_ZAWOR(pom_stez_2)
FUN_ZAWOR(wlot_powietrza)
FUN_ZAWOR(proznia)
FUN_ZAWOR(pilot)

void TestPage::setFinished(bool success)
{
    wiz->setFinished(success);
}

#define B_ZAWOR(Z) bool b_##Z = z_##Z()
#define BOOLS_ZAWORY B_ZAWOR(wentylacja_lewa);\
                     B_ZAWOR(wentylacja_prawa);\
                     B_ZAWOR(pom_stez_1);\
                     B_ZAWOR(pom_stez_2);\
                     B_ZAWOR(wlot_powietrza);\
                     B_ZAWOR(proznia);\
                     B_ZAWOR(drzwi_prawe);\
                     B_ZAWOR(drzwi_lewe);

bool TestPage::sprawdzZawory(QPushButton *pbOk_1, QLabel *arrow_1, QFrame *frame_2)
{
    BOOLS_ZAWORY
    QString message;
    bool show = false;
    QMessageBox msgBox;
    qDebug("%s:%d %d %d", __FILE__,__LINE__, b_drzwi_prawe, b_drzwi_lewe);
    if (!b_wentylacja_lewa || !b_wentylacja_prawa || !b_pom_stez_1 || !b_pom_stez_2 || !b_wlot_powietrza || !b_proznia ) {

        QString s("Otwarte zawory : [");
        if (!b_wentylacja_lewa)
            s+="wentylacja lewa,";
        if (!b_wentylacja_prawa)
            s+="wentylacja prawa,";
        if (!b_proznia)
            s+="proznia,";
        if (!b_wlot_powietrza)
            s+="wlot powietza,";
        if (!b_pom_stez_1)
            s+="pomiar stezenia 1,";
        if (!b_pom_stez_2)
            s+="pomiar stezenia 2,";
        s.remove(s.size()-1,1);
        s += " ]. \n";
        message = s;
        show = true;
    }

    if (!b_drzwi_lewe || !b_drzwi_prawe ) {

        QString s("Otwarte drzwi komory : [");
        if (!b_drzwi_prawe)
            s+="prawe,";
        if (!b_drzwi_lewe)
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
    if(!b_wentylacja_lewa || !b_pom_stez_1 || !b_wentylacja_prawa || !b_wlot_powietrza || !b_pom_stez_2) {
        show = true;
        QString s("Otwarte zawory : [ ");
        if (!b_wentylacja_lewa)
            s+="wentylacja lewa,";
        if (!b_wentylacja_prawa)
            s+="wentylacja prawa,";
        if (!b_wlot_powietrza)
            s+="wlot powietza,";
        if (!b_pom_stez_1)
            s+="pomiar stezenia 1,";
        if (!b_pom_stez_2)
            s+="pomiar stezenia 2,";
        s.remove(s.size()-1,1);
        msg = s + QString(" ].\n");
    }
    if (!b_drzwi_prawe || !b_drzwi_lewe) {
        show = true;
        QString s("Otwarte drzwi komory : [ ");
            if (!b_drzwi_prawe)
                s+="prawe,";
            if (!b_drzwi_lewe)
                s+="lewe,";
            s.remove(s.size()-1,1);
            msg += s + QString(" ].\n");
    }

    if (b_proznia) {
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

    if(!b_wentylacja_lewa || !b_pom_stez_1 || !b_wentylacja_prawa || !b_proznia || !b_pom_stez_2) {
        show = true;
        QString s("Otwarte zawory : [ ");
        if (!b_wentylacja_lewa)
            s+="wentylacja lewa,";
        if (!b_wentylacja_prawa)
            s+="wentylacja prawa,";
        if (!b_proznia)
            s+="proznia,";
        if (!b_pom_stez_1)
            s+="pomiar stezenia 1,";
        if (!b_pom_stez_2)
            s+="pomiar stezenia 2,";
        s.remove(s.size()-1,1);
        msg = s + QString(" ].\n");
    }
    if (!b_drzwi_prawe || !b_drzwi_lewe) {
        show = true;
        QString s("Otwarte drzwi komory : [ ");
        if (!b_drzwi_prawe)
            s+="prawe,";
        if (!b_drzwi_lewe)
            s+="lewe,";
        s.remove(s.size()-1,1);
        msg += s + QString(" ].\n");
    }

    if (b_wlot_powietrza) {
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

    if(!b_wentylacja_lewa || !b_wentylacja_prawa || !b_proznia || !b_wlot_powietrza) {
        show = true;
        QString s("Otwarte zawory : [ ");
        if (!b_wentylacja_lewa)
            s+="wentylacja lewa,";
        if (!b_wentylacja_prawa)
            s+="wentylacja prawa,";
        if (!b_proznia)
            s+="proznia,";
        if (!b_wlot_powietrza)
            s+="wlot_powietrze,";
        s.remove(s.size()-1,1);
        msg = s + QString(" ].\n");
    }
    if (!b_drzwi_prawe || !b_drzwi_lewe) {
        show = true;
        QString s("Otwarte drzwi komory : [ ");
        if (!b_drzwi_prawe)
            s+="prawe,";
        if (!b_drzwi_lewe)
            s+="lewe,";
        s.remove(s.size()-1,1);
        msg += s + QString(" ].\n");
    }

    if (b_pom_stez_1 || b_pom_stez_2) {
        show = true;
        if (b_pom_stez_1 && b_pom_stez_2)
            msg += QString("Oba zawory do pomiaru stężen są zamknięte.\n");
        else if (b_pom_stez_1)
            msg += QString("Lewy zawor do pomiaru stężen jest zamknięty.\n");
        else if (b_pom_stez_2)
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
    if(!b_drzwi_prawe || b_wentylacja_lewa || !b_proznia || !b_pom_stez_1 || !b_drzwi_lewe
            || b_wentylacja_prawa || !b_wlot_powietrza || !b_pom_stez_2) {
        //qDebug"--------------%d %d", b_wentylacja_lewa, wentylacja_prawa);
        QMessageBox msgBox;
        QString s("Otwarte zawory : [ ");
        if (!b_drzwi_prawe)
            s+="drzwi prawe,";
        if (!b_drzwi_lewe)
            s+="drzwi lewe,";
        if (!b_wlot_powietrza)
            s+="wlot powietza,";
        if (!b_pom_stez_1)
            s+="pomiar stezenia 1,";
        if (!b_pom_stez_2)
            s+="pomiar stezenia 2,";
        s.remove(s.size()-1,1);
        s += QString(" ] . Zawor wentylacji A jest %1, zawor wentylacji B jest %2").arg(!b_wentylacja_lewa ? "otwarty" : "zamkniety").
                arg(!b_wentylacja_prawa ? "otwarty" : "zamkniety");

        msgBox.setText(QString("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n").arg(s));
        msgBox.setInformativeText("Zawory Wentylacji powinny być otwarte a pozostałe zawory powinny być zamknięte.\n Czy chcesz kontynouwac?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if (ret == QMessageBox::No){
            setFinished(false);
        }
        return false;
    }
    return true;
}


#define CHECK_ZAWOR(Z, T)     do { if (((mask & Z) == Z) && !b_##Z) {\
                s+=T; openZ = true; } }while(false)

bool TestPage::sprawdzOtwarteZawory(uint16_t mask)
{
    BOOLS_ZAWORY;
    bool op = false;
    QString msg;
    {
        bool openZ = false;
        QString s = QString("Wykryto otwarte zawory : [ ");

        CHECK_ZAWOR(wentylacja_lewa, "wentylacja lewa,");
        CHECK_ZAWOR(wentylacja_prawa, "wentylacja prawa,");

        CHECK_ZAWOR(pom_stez_1, "pomiar stezenia 1,");
        CHECK_ZAWOR(pom_stez_2, "pomiar stezenia 2,");

        CHECK_ZAWOR(proznia, "próznia,");
        CHECK_ZAWOR(wlot_powietrza, "wlot powietrza,");

        if (openZ) {
            op = true;
            s.remove(s.size()-1,1);
            msg = s + QString(" ].\n");
        }
    }
    {
        bool openZ = false;
        QString s = QString("Wykryto otwarte drzwi : [ ");

        CHECK_ZAWOR(drzwi_prawe, "prawe,");
        CHECK_ZAWOR(drzwi_lewe, "lewe,");

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
