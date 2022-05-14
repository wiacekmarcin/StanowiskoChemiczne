#include "sygnalanalogowyustawienia.h"
#include "ui_SygnalAnalogowyUstawienia.h"

#include <QDebug>


SygnalAnalogowyUstawienia::SygnalAnalogowyUstawienia(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SygnalAnalogowyUstawienia)
{
    ui->setupUi(this);
    connect(ui->name, &QLineEdit::editingFinished,
            this, &SygnalAnalogowyUstawienia::textChange);
    connect(ui->unit, &QLineEdit::editingFinished,
            this, &SygnalAnalogowyUstawienia::textChange);
    connect(ui->ratio, &DoubleLineEdit::editingFinished,
            this, &SygnalAnalogowyUstawienia::textChange);
    connect(ui->mnoznik, &DoubleLineEdit::editingFinished,
            this, &SygnalAnalogowyUstawienia::textChange);
    connect(ui->minval, &DoubleLineEdit::editingFinished,
            this, &SygnalAnalogowyUstawienia::textChange);
    connect(ui->percent, &DoubleLineEdit::editingFinished,
            this, &SygnalAnalogowyUstawienia::textChange);

    ui->ratio->setMin(-1000000000);
    ui->ratio->setMax( 1000000000);
    ui->ratio->setPrec(10);

    ui->mnoznik->setMin(-1000000000);
    ui->mnoznik->setMax( 1000000000);
    ui->mnoznik->setPrec(10);

    ui->minval->setMin(-1000000000);
    ui->minval->setMax( 1000000000);
    ui->minval->setPrec(10);

    ui->percent->setMin(-1000000000);
    ui->percent->setMax( 1000000000);
    ui->percent->setPrec(10);

}

SygnalAnalogowyUstawienia::~SygnalAnalogowyUstawienia()
{
    delete ui;
}

bool SygnalAnalogowyUstawienia::valid()
{

    if (ui->name->text().isEmpty())
        return false;

    if (ui->unit->text().isEmpty())
        return false;
    QString r = ui->ratio->text();

    if (r.isEmpty())
        return false;
    bool ok;
    rr = r.toDouble(&ok);
    //
    //        name().toStdString().c_str(), unit().toStdString().c_str(),ui->ratio->text().toStdString().c_str(),
    //       rr,ok);
    return ok && rr > 0;
}

void SygnalAnalogowyUstawienia::textChange()
{
    emit updateCzujnik();
}

void SygnalAnalogowyUstawienia::setData(const UserPrivilige & user, const Ustawienia::CzujnikAnalogowy & czA)
{
    ui->name->setText(czA.name);
    ui->unit->setText(czA.unit);
    ui->ratio->setText(QString::number(czA.ratio));
    ui->baseUnit->setText(czA.baseUnit);
    ui->mnoznik->setText(QString::number(czA.convert));
    ui->minval->setText(QString::number(czA.minVal));
    ui->percent->setText(QString::number(czA.percentStab));


    ui->name->setEnabled((user & U_ADMIN) == U_ADMIN);
    ui->lname->setEnabled((user & U_ADMIN) == U_ADMIN);

    ui->unit->setEnabled((user & U_ADMIN) == U_ADMIN);
    ui->lunit->setEnabled((user & U_ADMIN) == U_ADMIN);

    ui->mnoznik->setEnabled((user & U_ADMIN) == U_ADMIN);
    ui->lmniznik->setEnabled((user & U_ADMIN) == U_ADMIN);

    ui->ratio->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->lratio->setEnabled((user & U_SERVISANT) == U_SERVISANT);

    ui->minval->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->lminval->setEnabled((user & U_SERVISANT) == U_SERVISANT);

    ui->percent->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->lprocent->setEnabled((user & U_SERVISANT) == U_SERVISANT);

    ui->baseUnit->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->lBaseUnit->setEnabled((user & U_SERVISANT) == U_SERVISANT);



}

Ustawienia::CzujnikAnalogowy SygnalAnalogowyUstawienia::getUstawienia() const
{
    Ustawienia::CzujnikAnalogowy c;
    c.name = ui->name->text();
    c.unit = ui->unit->text();
    bool ok;
    c.ratio = ui->ratio->text().toDouble(&ok);
    c.baseUnit = ui->baseUnit->text();
    c.convert = ui->mnoznik->text().toDouble(&ok);
    c.minVal = ui->minval->text().toDouble(&ok);
    c.percentStab = ui->percent->text().toDouble(&ok);
    return c;
}
