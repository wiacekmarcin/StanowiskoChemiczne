#include "nowytest_6.h"
#include "ui_nowytest_6.h"
#include "createtestwizard.h"
#include "testpage.h"

NowyTest_6::NowyTest_6(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_6)
{
    ui->setupUi(this);

    valid = false;
    emit completeChanged();
}

NowyTest_6::~NowyTest_6()
{
    delete ui;
}

void NowyTest_6::initializePage()
{
    QString zaplon = field("zaplon").toString();
    QString zaplonExt = field("zaplonExt").toString();
    if (zaplonExt != QString("--"))
        zaplon = zaplon + QString (" ( %1 )").arg(zaplonExt);
    ui->lStep2->setText(ui->lStep2->text().replace("[ZAPLON]", zaplon));
    valid = true;
    emit completeChanged();
}

bool NowyTest_6::isComplete() const
{
    if (wizard()->currentPage() != this)
        return true;

    if (!TestPage::isComplete())
        return false;

    return valid;
}


