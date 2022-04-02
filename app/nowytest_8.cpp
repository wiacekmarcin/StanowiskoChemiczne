#include "nowytest_8.h"
#include "ui_nowytest_8.h"
#include "createtestwizard.h"
#include "testpage.h"

NowyTest_8::NowyTest_8(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_8)
{
    ui->setupUi(this);

    valid = false;
}

NowyTest_8::~NowyTest_8()
{
    delete ui;
}

void NowyTest_8::initializePage()
{
    QString ciecz = field(TestPage::ciecz).toString();
    QString zaplon = field(TestPage::zaplon).toString();
    QString zaplonExt = field(zaplonExtra).toString();
    if (zaplonExt != QString("--"))
        zaplon = zaplon + QString (" ( %1 )").arg(zaplonExt);

    QString resultat = ui->label->text();
    resultat = resultat.replace("[ZRODLOZAPLONU]", zaplon);
    resultat = resultat.replace("[CIECZ]", ciecz);
    bool brakzaplonu = field(brakZaplonu).toBool();
    resultat = resultat.replace("[ZAPLON]", brakzaplonu ? "" : QString::fromUtf8("zap\305\202onem"));
    resultat = resultat.replace("[BRAKZAPLONU]", brakzaplonu ? QString::fromUtf8("brakiem zap\305\202onu"):"");
    ui->label->setText(resultat);

}




