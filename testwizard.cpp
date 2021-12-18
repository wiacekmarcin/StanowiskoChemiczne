#include "testwizard.h"
#include "nowytest_1.h"
#include "nowytest_2.h"
#include "nowytest_3.h"
#include "nowytest_4.h"
#include "nowytest_5.h"
#include "nowytest_6.h"
#include "nowytest_7.h"

#include <QAbstractButton>

TestWizard::TestWizard(QWidget *parent) :
    QWizard(parent)
{

    setObjectName(QString::fromUtf8("TestWizard"));
    resize(551, 564);

    QMetaObject::connectSlotsByName(this);
    setWindowTitle("Test");
    setOption(QWizard::IndependentPages);

    addPage(new NowyTest_1(this));
    addPage(new NowyTest_2(this));
    addPage(new NowyTest_3(this));
    addPage(new NowyTest_4(this));
    addPage(new NowyTest_5(this));
    addPage(new NowyTest_6(this));
    addPage(new NowyTest_7(this));

    setButtonText(QWizard::BackButton, QString("Wstecz"));
    setButtonText(QWizard::NextButton, QString("Dalej"));
    setButtonText(QWizard::CancelButton, QString("Porzuć"));
    setButtonText(QWizard::CommitButton, QString("Dodaj"));

    connect(this, &QWizard::currentIdChanged, this, &TestWizard::changePage);
}

TestWizard::~TestWizard()
{

}

void TestWizard::changePage(int id)
{
    qDebug("%d", id);
    if (id == 2 || id == 5) {
        initializePage(id);
    }
    if (id >= 3) {
        QWizard::button(QWizard::BackButton)->setVisible(false);
        QWizard::button(QWizard::BackButton)->setEnabled(false);
        QWizard::button(QWizard::CancelButton)->setVisible(false);
        QWizard::button(QWizard::CancelButton)->setEnabled(false);
    }
}
