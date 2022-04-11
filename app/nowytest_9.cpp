#include "nowytest_9.h"
#include "ui_nowytest_9.h"
#include "createtestwizard.h"
#include "testpage.h"

NowyTest_9::NowyTest_9(QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_9)
{
    ui->setupUi(this);
    textMsg1 = "<html><head/><body><p>Badanie zakończone ";
    zaplonMsg = "<span style=\" font-weight:600; color:#8ae234;\">zapłonem</span>";
    brakZaplMsg = "<span style=\" font-weight:600; color:#cc0000;\">brakiem zapłonu</span>";

    textMsg2 = "<span style=\" color:#000000;\">mieszaniny par </span><span style=\" font-weight:600; color:#000000;\">";
    textMsg3 = "</span>";
    textMsg4 = "<span style=\" color:#000000;\">z pomiarem stężenia </span><span style=\" font-weight:600; color:#000000;\">";
    textMsg5 = "</span>";
    textMsg6 = "<span style=\" color:#000000;\"> poprzez </span><span style=\" font-weight:600; color:#000000;\">";
    textMsg7 = "<span><span style=\" color:#000000;\"></span></p></body></html>";
}

NowyTest_9::~NowyTest_9()
{
    delete ui;
}

void NowyTest_9::initializePage()
{
    QString ciecz = field(TestPage::ciecz).toString();
    QString zaplon = field(TestPage::zaplon).toString();
    bool czybylzaplon = field(TestPage::bylWybuch).toBool();
    QString stezenie("0.0");

    QString resultat = textMsg1 + (czybylzaplon ? zaplonMsg : brakZaplMsg);
    resultat += textMsg2 + ciecz + textMsg3;
    resultat += czybylzaplon ? (textMsg4 + stezenie + textMsg5) : "";
    resultat += textMsg6 + zaplon + textMsg7;

    ui->label->setText(resultat);
    setFinished(true);
    updateOutput(o_wentylator, true);
}




