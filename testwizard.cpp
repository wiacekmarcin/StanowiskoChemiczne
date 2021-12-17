#include "testwizard.h"
#include "nowytest_1.h"
#include "nowytest_2.h"
#include "nowytest_3.h"
#include "nowytest_4.h"
TestWizard::TestWizard(QWidget *parent) :
    QWizard(parent)
{

    setObjectName(QString::fromUtf8("TestWizard"));
    resize(551, 564);

    QMetaObject::connectSlotsByName(this);
    setWindowTitle("Test");


    addPage(new NowyTest_1(this));
    addPage(new NowyTest_2(this));
    addPage(new NowyTest_3(this));
    addPage(new NowyTest_4(this));

    setButtonText(QWizard::BackButton, QString("Wstecz"));
    setButtonText(QWizard::NextButton, QString("Dalej"));
    setButtonText(QWizard::CancelButton, QString("Porzuć"));
    setButtonText(QWizard::CommitButton, QString("Dodaj"));


}

TestWizard::~TestWizard()
{

}
