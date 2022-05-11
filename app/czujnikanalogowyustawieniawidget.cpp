#include "czujnikanalogowyustawieniawidget.h"
#include "ui_czujnikanalogowyustawieniawidget.h"

CzujnikAnalogowyUstawieniaWidget::CzujnikAnalogowyUstawieniaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CzujnikAnalogowyUstawieniaWidget)
{
    ui->setupUi(this);
    connect(ui->name, &QLineEdit::editingFinished,
            this, &CzujnikAnalogowyUstawieniaWidget::textChange);
    connect(ui->unit, &QLineEdit::editingFinished,
            this, &CzujnikAnalogowyUstawieniaWidget::textChange);
    connect(ui->ratio, &QLineEdit::editingFinished,
            this, &CzujnikAnalogowyUstawieniaWidget::textChange);

}

CzujnikAnalogowyUstawieniaWidget::~CzujnikAnalogowyUstawieniaWidget()
{
    delete ui;
}

bool CzujnikAnalogowyUstawieniaWidget::valid()
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

void CzujnikAnalogowyUstawieniaWidget::textChange()
{
    emit updateCzujnik();
}

void CzujnikAnalogowyUstawieniaWidget::setData(const UserPrivilige & user, const Ustawienia::CzujnikAnalogowy & czA)
{
    ui->name->setText(czA.name);
    ui->unit->setText(czA.unit);
    ui->ratio->setText(QString::number(czA.ratio));
    ui->baseUnit->setText(czA.baseUnit);
    ui->mnoznik->setText(QString::number(czA.convert));

    ui->name->setEnabled((user & U_STUDENT) == U_STUDENT);
    ui->unit->setEnabled((user & U_STUDENT) == U_STUDENT);
    ui->mnoznik->setEnabled((user & U_STUDENT) == U_STUDENT);

    ui->ratio->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->lratio->setEnabled((user & U_SERVISANT) == U_SERVISANT);
    ui->baseUnit->setReadOnly((user & U_SERVISANT) != U_SERVISANT);
}

Ustawienia::CzujnikAnalogowy CzujnikAnalogowyUstawieniaWidget::getUstawienia() const
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
