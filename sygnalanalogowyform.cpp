#include "sygnalanalogowyform.h"
#include "ui_sygnalanalogowyform.h"

SygnalAnalogowyForm::SygnalAnalogowyForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SygnalAnalogowyForm)
{
    ui->setupUi(this);
}

SygnalAnalogowyForm::~SygnalAnalogowyForm()
{
    delete ui;
}

QString SygnalAnalogowyForm::name() const
{
    return ui->name->text();
}

QString SygnalAnalogowyForm::unit() const
{
    return ui->unit->text();
}

QString SygnalAnalogowyForm::ratio() const
{
    return ui->ratio->text();
}
