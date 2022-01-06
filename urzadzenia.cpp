#include "urzadzenia.h"
#include "ui_urzadzenia.h"

Urzadzenia::Urzadzenia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Urzadzenia)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

}

Urzadzenia::~Urzadzenia()
{
    delete ui;
}

void Urzadzenia::setLabels(const Ustawienia &ust)
{
    ui->cz_label_1->setText(ust.getName(1));
    ui->cz_label_2->setText(ust.getName(2));
    ui->cz_label_3->setText(ust.getName(3));
    ui->cz_label_4->setText(ust.getName(4));
    ui->cz_label_5->setText(ust.getName(5));
    ui->cz_label_6->setText(ust.getName(6));
    ui->cz_label_7->setText(ust.getName(7));
    ui->cz_label_8->setText(ust.getName(8));


}

void Urzadzenia::on_analog_1_valueChanged(int value)
{
    emit analogValueChanged(1, value);
}

void Urzadzenia::on_analog_2_valueChanged(int value)
{
    emit analogValueChanged(2, value);
}

void Urzadzenia::on_analog_3_valueChanged(int value)
{
    emit analogValueChanged(3, value);
}

void Urzadzenia::on_analog_4_valueChanged(int value)
{
    emit analogValueChanged(4, value);
}

void Urzadzenia::on_analog_5_valueChanged(int value)
{
    emit analogValueChanged(5, value);
}

void Urzadzenia::on_analog_6_valueChanged(int value)
{
    emit analogValueChanged(6, value);
}

void Urzadzenia::on_analog_7_valueChanged(int value)
{
    emit analogValueChanged(7, value);
}

void Urzadzenia::on_analog_8_valueChanged(int value)
{
    emit analogValueChanged(8, value);
}

