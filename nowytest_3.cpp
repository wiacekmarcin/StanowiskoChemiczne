#include "nowytest_3.h"
#include "ui_nowytest_3.h"
#include "formularzdialogbuttonbox.h"

#define MAX_STEP    6
#define ACT_STEP    3

NowyTest_3::NowyTest_3(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NowyTest_3)
{
    ui->setupUi(this);
    FormularzDialogButtonBox * dlg = new FormularzDialogButtonBox(ui->pbStep4, ACT_STEP, MAX_STEP, this);
    ui->gridLayout->addWidget(dlg);
    dlg->setValid(false);
    connect(this, &NowyTest_3::valid, dlg, &FormularzDialogButtonBox::setValid);

    setWindowTitle(windowTitle() + QString( " Krok %1 z %2").arg(ACT_STEP).arg(MAX_STEP));
}

NowyTest_3::~NowyTest_3()
{
    delete ui;
}
