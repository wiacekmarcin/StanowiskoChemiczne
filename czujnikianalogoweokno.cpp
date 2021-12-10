#include "czujnikianalogoweokno.h"
#include "ui_czujnikianalogoweokno.h"
#include "czujnikanalogowywidget.h"
CzujnikiAnalogoweOkno::CzujnikiAnalogoweOkno(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CzujnikiAnalogoweOkno)
{
    ui->setupUi(this);
    czujniki[0] = ui->widget_1;
    czujniki[1] = ui->widget_2;
    czujniki[2] = ui->widget_3;
    czujniki[3] = ui->widget_4;
    czujniki[4] = ui->widget_5;
    czujniki[5] = ui->widget_6;


}

CzujnikiAnalogoweOkno::~CzujnikiAnalogoweOkno()
{
    delete ui;
}

void CzujnikiAnalogoweOkno::updateValue(int id, const double & val)
{
    if (id <= 0 || id > 6)
        return;
    czujniki[id]->setValue(val);
    emit valueChange(id, czujniki[id]->getConvValue());
}
