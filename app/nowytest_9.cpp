#include "nowytest_9.h"
#include "ui_nowytest_9.h"
#include "createtestwizard.h"
#include "testpage.h"

NowyTest_9::NowyTest_9(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_9)
{
    ui->setupUi(this);
}

NowyTest_9::~NowyTest_9()
{
    delete ui;
}

void NowyTest_9::initializePage()
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
    setFinished(true);
}




