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
