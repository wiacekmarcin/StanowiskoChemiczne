#include "nowytest_2.h"
#include "ui_nowytest_2.h"
#include "formularzdialogbuttonbox.h"
#define MAX_STEP    6
#define ACT_STEP    2

NowyTest_2::NowyTest_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NowyTest_2)
{
    ui->setupUi(this);
    FormularzDialogButtonBox * dlg = new FormularzDialogButtonBox(ui->cbenergiaIskry, ACT_STEP, MAX_STEP, this);
    ui->formLayout->addWidget(dlg);
    dlg->setValid(false);
    connect(this, &NowyTest_2::valid, dlg, &FormularzDialogButtonBox::setValid);

    setWindowTitle(windowTitle() + QString( " Krok %1 z %2").arg(ACT_STEP).arg(MAX_STEP));
    ui->cbCiecz->setDisabled(true);

}

NowyTest_2::~NowyTest_2()
{
    delete ui;
}

void NowyTest_2::on_cbDozownik_currentTextChanged(const QString &arg1)
{
    ui->cbCiecz->setDisabled(arg1 == "--");

}
