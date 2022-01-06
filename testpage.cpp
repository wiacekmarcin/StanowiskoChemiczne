#include "testpage.h"
#include "createtestwizard.h"
#include "testpageform.h"

TestPage::TestPage(QWidget *parent) :
    QWidget(parent), wiz(nullptr), form(nullptr),
    valid(true)
{

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
    qDebug("TestPage::initializePage()");
}

int TestPage::nextPage() const
{
    return id+1;
}



