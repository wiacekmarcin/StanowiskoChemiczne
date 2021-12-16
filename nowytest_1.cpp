#include "nowytest_1.h"
#include "ui_nowytest_1.h"
#include "formularzdialogbuttonbox.h"

#define MAX_STEP    6
#define ACT_STEP    1

NowyTest_1::NowyTest_1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NowyTest_1)
{
    ui->setupUi(this);
    FormularzDialogButtonBox * dlg = new FormularzDialogButtonBox(ui->nameTest, ACT_STEP, MAX_STEP, this);
    ui->formLayout->addWidget(dlg);
    dlg->setValid(false);
    connect(this, &NowyTest_1::valid, dlg, &FormularzDialogButtonBox::setValid);

    setWindowTitle(windowTitle() + QString( " Krok %1 z %2").arg(ACT_STEP).arg(MAX_STEP));
}


NowyTest_1::~NowyTest_1()
{
    delete ui;
}


void NowyTest_1::on_nameTest_textChanged(const QString &arg1)
{
    emit valid(!arg1.isEmpty());
}
