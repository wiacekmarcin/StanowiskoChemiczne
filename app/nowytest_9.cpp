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
    TestData * test = getTestData();


    QString ciecz = field(TestPage::ciecz).toString();
    QString zaplon = field(TestPage::zaplon).toString();
    bool czybylzaplon = field(TestPage::bylWybuch).toBool();
    QString stezenie("0.0");

    QString resultat = textMsg1 + (czybylzaplon ? zaplonMsg : brakZaplMsg);
    resultat += textMsg2 + ciecz + textMsg3;
    resultat += czybylzaplon ? (textMsg4 + stezenie + textMsg5) : "";
    resultat += textMsg6 + zaplon + textMsg7;

    //ui->label->setText(resultat);
    setFinished(true);
    updateOutput(o_wentylator, true);
}




