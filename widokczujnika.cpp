#include "widokczujnika.h"
#include "ui_widokczujnika.h"

WidokCzujnika::WidokCzujnika(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidokCzujnika)
{
    ui->setupUi(this);
}

WidokCzujnika::~WidokCzujnika()
{
    delete ui;
}

void WidokCzujnika::setValue(const int & val)
{
    ui->value->setText(QString::number(val));
}
void WidokCzujnika::setValue(const long & val)
{
    ui->value->setText(QString::number(val));
}
void WidokCzujnika::setValue(const long long & val)
{
    ui->value->setText(QString::number(val));
}
void WidokCzujnika::setValue(const float & val)
{
    ui->value->setText(QString::number(val));
}
void WidokCzujnika::setValue(const double & val)
{
    ui->value->setText(QString::number(val));
}
void WidokCzujnika::setDescription(const QString & opis, const QString & jedn)
{
    ui->opis->setText(opis);
    ui->jednostka->setText(jedn);
}
