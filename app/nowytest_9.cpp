#include "nowytest_9.h"
#include "ui_nowytest_9.h"
#include "createtestwizard.h"
#include "testpage.h"
#include "testdata.h"

#include <QFrame>
#include <QLabel>
#include <QGridLayout>

NowyTest_9::NowyTest_9(TestData * dt_, QWidget *parent) :
    TestPage(parent),
    ui(new Ui::NowyTest_9)
{
    ui->setupUi(this);
    dt = dt_;
    wasInit = false;
}

NowyTest_9::~NowyTest_9()
{
    delete ui;
}

#define ONETD(N, T) \
    QFrame * td_##N = new QFrame(ui->widget); \
    td_##N->setObjectName(QString("td_%1_%2").arg(N).arg(idProba));\
    td_##N->setFrameShape(QFrame::StyledPanel);\
    td_##N->setFrameShadow(QFrame::Raised);\
    QHBoxLayout * horizontalLayout_##N = new QHBoxLayout(td_##N);\
    horizontalLayout_##N ->setObjectName(QString("horizontalLayout1%1%2").arg(N).arg(idProba));\
    QLabel * label_##N = new QLabel(td_##N);\
    label_##N->setObjectName(QString("labeltd_1%1%2").arg(N).arg(idProba));\
    label_##N->setText(T);\
    horizontalLayout_##N ->addWidget(label_##N);\
    ui->gridLayout->addWidget(td_##N, idProba, N, 1, 1);

void NowyTest_9::initializePage()
{
    if (wasInit)
        return;
    wasInit = true;

    int idProba = 1;
    foreach ( auto proba, dt->getProby()) {
        ONETD(0, QString::number(idProba))
        ONETD(1, proba.zrodloZaplonu)
        ONETD(2, proba.success ? "Zapłon" : "Brak Zapłonu")
        ONETD(3, dt->getNazwaCieczy())
        ONETD(4, QString::number(proba.iloscCalkowitaCieczy))
        idProba+=1;
    }


    setFinished(true);
    updateOutput(o_wentylator, true);
}




