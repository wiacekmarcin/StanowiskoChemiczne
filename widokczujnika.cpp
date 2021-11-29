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
