#include "czujnikanalogowyustawieniaokno.h"
#include "ui_czujnikanalogowyustawieniaokno.h"

#include <QPushButton>

CzujnikAnalogowyUstawieniaOkno::CzujnikAnalogowyUstawieniaOkno(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CzujnikAnalogowyUstawieniaOkno)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    ustawienia[0] = ui->widget_1;
    ustawienia[1] = ui->widget_2;
    ustawienia[2] = ui->widget_3;
    ustawienia[3] = ui->widget_4;
    ustawienia[4] = ui->widget_5;
    ustawienia[5] = ui->widget_6;

    for (int i = 0; i < 6; i++) {
        connect(ustawienia[i], &CzujnikAnalogowyUstawieniaWidget::updateCzujnik,
                this, &CzujnikAnalogowyUstawieniaOkno::updateCzujnik);
    }
}

CzujnikAnalogowyUstawieniaOkno::~CzujnikAnalogowyUstawieniaOkno()
{
    delete ui;
}

void CzujnikAnalogowyUstawieniaOkno::setData(int i, const QString &name, const QString &unit, const double &ratio)
{
    if (i >= 0 && i < 6) {
        ustawienia[i]->setData(name, unit, ratio);
    }
}

void CzujnikAnalogowyUstawieniaOkno::updateCzujnik()
{
    bool valid = true;
    for (int i = 0; i < 6; i++) {
        if (!ustawienia[i]->valid()) {
            valid = false;
            break;
        }
    }
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(valid);
}
