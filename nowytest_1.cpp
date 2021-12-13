#include "nowytest_1.h"
#include "ui_nowytest_1.h"
#include "formularzdialogbuttonbox.h"
NowyTest_1::NowyTest_1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NowyTest_1)
{
    ui->setupUi(this);
    FormularzDialogButtonBox * dlg = new FormularzDialogButtonBox(1, 6, this);
    ui->formLayout->addWidget(dlg);
    dlg->setValid(false);
    connect(this, &NowyTest_1::valid, dlg, &FormularzDialogButtonBox::setValid);


}


NowyTest_1::~NowyTest_1()
{
    delete ui;
}


void NowyTest_1::on_lineEdit_textChanged(const QString &arg1)
{
    emit valid(!arg1.isEmpty());
}
