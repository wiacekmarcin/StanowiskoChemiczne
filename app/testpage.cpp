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
    if (!b_wentylacja_lewa || !b_wentylacja_prawa || !b_pom_stez_1 || !b_pom_stez_2 || !b_wlot_powietrza || !b_proznia ) {
        QMessageBox msgBox;
        QString s("Otwarte zawory : [");
        if (!b_drzwi_prawe)
            s+="drzwi prawe,";
        if (!b_drzwi_lewe)
            s+="drzwi lewe,";
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
        s += "] .";
        msgBox.setText(QString("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n").arg(s));
        msgBox.setInformativeText("Wszystkie zawory powinny być zamknięte. \n Czy chcesz kontynuować");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
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
   if(!b_drzwi_prawe || !b_wentylacja_lewa || b_proznia || !b_pom_stez_1 || !b_drzwi_lewe
         || !b_wentylacja_prawa || !b_wlot_powietrza || !b_pom_stez_2) {
       QMessageBox msgBox;
       QString s("Otwarte zawory : [ ");
       if (!b_drzwi_prawe)
           s+="drzwi prawe,";
       if (!b_drzwi_lewe)
           s+="drzwi lewe,";
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
       s += QString(" ] . Zawor proznia jest %1").arg(!b_proznia ? "otwarty" : "zamkniety");
       msgBox.setText(QString("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n").arg(s));
            msgBox.setInformativeText("Zawór proznia powinien być otwarty, reszta zamknieta. \n Czy chcesz kontynuować");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::No);
            int ret = msgBox.exec();
            if (ret == QMessageBox::No) {
                setFinished(false);
                return false;
            } else if (ret == QMessageBox::Cancel)
                return true;
       return false;
    }
   return true;
}
bool TestPage::sprawdzOtwarteZaworPowietrza()
{
    BOOLS_ZAWORY
    if(!b_drzwi_prawe || !b_wentylacja_lewa || !b_proznia || !b_pom_stez_1 || !b_drzwi_lewe
          || !b_wentylacja_prawa || b_wlot_powietrza || !b_pom_stez_2) {
        QMessageBox msgBox;
        QString s("Otwarte zawory : [ ");
        if (!b_drzwi_prawe)
            s+="drzwi prawe,";
        if (!b_drzwi_lewe)
            s+="drzwi lewe,";
        if (!b_wentylacja_lewa)
            s+="wentylacja lewa,";
        if (!b_wentylacja_prawa)
            s+="wentylacja prawa,";
        if (!proznia)
            s+="proznia,";
        if (!b_pom_stez_1)
            s+="pomiar stezenia 1,";
        if (!b_pom_stez_2)
            s+="pomiar stezenia 2,";
        s.remove(s.size()-1,1);
        s += QString(" ] . Zawor wlot powietrza jest %1").arg(!b_wlot_powietrza ? "otwarty" : "zamkniety");
        msgBox.setText(QString("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n").arg(s));
             msgBox.setInformativeText("Zawór proznia powinien być otwarty, reszta zamknieta. \n Czy chcesz kontynuować");

        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if (ret == QMessageBox::No) {
            setFinished(false);
            return false;
        }
        return false;
    }
    return true;
}

bool TestPage::sprawdzOtwarteZaworStezenia()
{
    BOOLS_ZAWORY
    if (!b_drzwi_prawe || !b_wentylacja_lewa || !b_proznia || b_pom_stez_1 || !b_drzwi_lewe
             || !b_wentylacja_prawa || !b_wlot_powietrza || b_pom_stez_2) {
        QMessageBox msgBox;
        QString s("Otwarte zawory : [ ");
        if (!b_drzwi_prawe)
            s+="drzwi prawe,";
        if (!b_drzwi_lewe)
            s+="drzwi lewe,";
        if (!b_wentylacja_lewa)
            s+="wentylacja lewa,";
        if (!b_wentylacja_prawa)
            s+="wentylacja prawa,";
        if (!b_proznia)
            s+="proznia,";
        if (!b_wlot_powietrza)
            s+="wlot powietza,";
         s.remove(s.size()-1,1);
        s += QString(" ] . Zawór 'Pomiar steżenie 1' jest %1 . Zawor 'Pomiar stezenia 2' jest %2)").
                arg(!b_pom_stez_1 ? "otwarty" : "zamkniety", !b_pom_stez_2 ? "otwarty" : "zamkniety");
        msgBox.setText(QString("Wykryto nie prawidłowe ustawienie zaworów. \n%1\n").arg(s));
        msgBox.setInformativeText("Zawory od pomiaru stężenia powinny być otwarte, reszta zaworów powinna być zamknieta. \n Czy chcesz kontynuować");

        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if (ret == QMessageBox::No) {
            setFinished(false);
            return false;
        }
        return false;
    }
    return true;
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
