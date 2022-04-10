#include "czujnikanalogowywidget.h"
#include "ui_czujnikanalogowywidget.h"

CzujnikAnalogowyWidget::CzujnikAnalogowyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CzujnikAnalogowyWidget)
{
    ui->setupUi(this);
}

CzujnikAnalogowyWidget::~CzujnikAnalogowyWidget()
{
    delete ui;
}

void CzujnikAnalogowyWidget::setParam(const QString & _name, const double & _ratio, const QString & _unit)
{
    name = _name;
    ratio = _ratio;
    unit = _unit;
    ui->opis->setText(name);
    ui->jednostka->setText(unit);
    ui->value->setText("0.0");
}

void CzujnikAnalogowyWidget::setValue(const double &val)
{
    //valConv = val;
    ui->value->setText(QString::number(val, 'G',5));
}
