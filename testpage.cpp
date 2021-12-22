#include "testpage.h"
#include "createtestwizard.h"
#include "testpageform.h"

TestPage::TestPage(QWidget *parent) :
    QWidget(parent), wiz(nullptr), form(nullptr)
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
    qDebug("wiz=%p",wiz);
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

void TestPage::setTitle(const QString &t)
{
    if (form)
        form->setTitle(t);
}

void TestPage::setSubTitle(const QString &t)
{
    if (form)
        form->setSubTitle(t);
}

TestPageForm *TestPage::getForm() const
{
    return form;
}

void TestPage::setForm(TestPageForm *value)
{
    form = value;
}
