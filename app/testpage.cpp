#include "testpage.h"
#include "createtestwizard.h"
#include "testpageform.h"

#include <QMessageBox>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

TestPage::TestPage(QWidget *parent) :
    QWidget(parent), valid(true), wiz(nullptr), form(nullptr)

{
    prevVals = 0;
    b_drzwi_prawe = b_wentylacja_lewa = b_proznia = b_pom_stez_1 = b_drzwi_lewe = b_wentylacja_prawa = b_wlot_powietrza = b_pom_stez_2 = b_pilot = false;
    restricted = false;
    restrictedMap[drzwi_prawe] = true;
    restrictedMap[drzwi_lewe] = true;
    restrictedMap[wentylacja_prawa] = true;
    restrictedMap[wentylacja_lewa] = true;
    restrictedMap[proznia] = true;
    restrictedMap[wlot_powietrza] = true;
    restrictedMap[pom_stez_1] = true;
    restrictedMap[pom_stez_2] = true;
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
    //qDebug("TestPage::initializePage()");
}

TestPage::PageId TestPage::nextPageId() const
{
    return (PageId) ( (int)id+1);
}

void TestPage::nextPage(TestPage::PageId idPage)
{
    wiz->nextPage(idPage);
}

#define CHECK_BOOL(X) b_##X = vals & X
void TestPage::readAll(uint16_t vals)
{
    //qDebug("%s:%d", __FILE__, __LINE__);
    CHECK_BOOL(drzwi_prawe);
    CHECK_BOOL(drzwi_lewe);
    CHECK_BOOL(wentylacja_lewa);
    CHECK_BOOL(wentylacja_prawa);
    CHECK_BOOL(pom_stez_1);
    CHECK_BOOL(pom_stez_2);
    CHECK_BOOL(wlot_powietrza);
    CHECK_BOOL(proznia);
    CHECK_BOOL(pilot);
    qDebug("%s:%d %x %d", __FILE__,__LINE__,vals,b_pilot);

    if (vals != prevVals) {
        updateWejscia();
        emit updateData(vals);
    }
    prevVals = vals;
    if (restricted) {
        for (QMap<uint16_t, bool>::const_iterator it = restrictedMap.begin(); it != restrictedMap.end(); ++it) {

        }
    }
}

void TestPage::setFinished(bool success)
{
    wiz->setFinished(success);
}

bool TestPage::sprawdzZawory(QPushButton *pbOk_1, QLabel *arrow_1, QFrame *frame_2)
{
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
    }
   return true;
}
bool TestPage::sprawdzOtwarteZaworPowietrza()
{
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
    while (!b_drzwi_prawe || !b_wentylacja_lewa || !b_proznia || b_pom_stez_1 || !b_drzwi_lewe
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
