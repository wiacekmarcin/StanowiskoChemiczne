#include "czujnikanalogowyustawieniaokno.h"
#include "ui_czujnikanalogowyustawieniaokno.h"
#include "czujnikianalogoweokno.h"
#include <QPushButton>

CzujnikAnalogowyUstawieniaOkno::CzujnikAnalogowyUstawieniaOkno(const Ustawienia &sett, QWidget *parent) :
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

    for (int i=0; i < sett.maxCzujek; ++i) {
        auto r = sett.getCzujka(i+1);
        ustawienia[i]->setData(r.name, r.unit, r.ratio);
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

void CzujnikAnalogowyUstawieniaOkno::getData(CzujnikAnalogowyUstawieniaOkno *dlg)
{
    for (int i = 0; i < 6; i++) {
        dlg->setData(i, ustawienia[i]->name(),
                ustawienia[i]->unit(),
                ustawienia[i]->ratio());
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
