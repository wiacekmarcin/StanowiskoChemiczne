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

QString CzujnikAnalogowyUstawieniaWidget::name() const
{
    return ui->name->text();
}

QString CzujnikAnalogowyUstawieniaWidget::unit() const
{
    return ui->unit->text();
}

double CzujnikAnalogowyUstawieniaWidget::ratio() const
{
    return rr;
}

bool CzujnikAnalogowyUstawieniaWidget::valid()
{
    if (name().isEmpty())
        return false;
    if (unit().isEmpty())
        return false;
    QString r = ui->ratio->text();
    if (r.isEmpty())
        return false;
    bool ok;
    rr = r.toDouble(&ok);
    return ok && rr > 0;
}

void CzujnikAnalogowyUstawieniaWidget::textChange()
{
    emit updateCzujnik();
}

void CzujnikAnalogowyUstawieniaWidget::setData(const QString &name, const QString &unit, const double &ratio)
{
    ui->name->setText(name);
    ui->unit->setText(unit);
    ui->ratio->setText(QString::number(ratio));
}
