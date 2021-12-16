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

    FormularzDialogButtonBox * buttons = new FormularzDialogButtonBox(ui->nameTest, ACT_STEP, MAX_STEP, this);
    ui->formLayout->addWidget(buttons);
    buttons->setValid(false);
    connect(this, &NowyTest_1::valid, buttons, &FormularzDialogButtonBox::setValid);

    setWindowTitle(windowTitle() + QString( " Krok %1 z %2").arg(ACT_STEP).arg(MAX_STEP));

    connect(buttons, &FormularzDialogButtonBox::reset, this, &NowyTest_1::form_reset);
}


NowyTest_1::~NowyTest_1()
{
    delete ui;
}


void NowyTest_1::on_nameTest_textChanged(const QString &arg1)
{
    emit valid(!arg1.isEmpty());
}

void NowyTest_1::form_reset()
{
    ui->nameTest->setText("");
}
