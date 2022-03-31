#include "testpage.h"
#include "createtestwizard.h"
#include "testpageform.h"

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

void TestPage::setField(const QString &key, const QVariant &val)
{
    if (wiz)
        wiz->setField(key, val);
}

QVariant TestPage::field(const QString &key) const
{
    if (wiz)
        return wiz->field(key);
    return QVariant();
}

void TestPage::setWizard(CreateTestWizard *wizard)
{
    wiz = wizard;
}

CreateTestWizard *TestPage::wizard() const
{
    return wiz;
}

int TestPage::getId() const
{
    return id;
}

void TestPage::setId(int value)
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

QString TestPage::buttonName() const
{
    return m_buttonName;
}

void TestPage::setButtonName(const QString &t)
{
    if (form)
        form->setButtonName(t);
    m_buttonName = t;
}

void TestPage::showButton(bool value)
{
    if (form)
        form->showVisibleButton(value);
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
    setButtonName(buttonName());
}

void TestPage::initializePage()
{
    //qDebug("TestPage::initializePage()");
}

int TestPage::nextPageId() const
{
    return id+1;
}

void TestPage::nextPage(short idPage)
{
    wiz->nextPage(idPage);
}

#define CHECK_BOOL(X) b_##X = vals & X
void TestPage::readAll(uint16_t vals)
{
    qDebug("%s:%d", __FILE__, __LINE__);
    CHECK_BOOL(drzwi_prawe);
    CHECK_BOOL(drzwi_lewe);
    CHECK_BOOL(wentylacja_lewa);
    CHECK_BOOL(wentylacja_prawa);
    CHECK_BOOL(pom_stez_1);
    CHECK_BOOL(pom_stez_2);
    CHECK_BOOL(wlot_powietrza);
    CHECK_BOOL(proznia);
    CHECK_BOOL(pilot);

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



