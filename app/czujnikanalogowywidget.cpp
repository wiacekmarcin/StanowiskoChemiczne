#include "czujnikanalogowywidget.h"
#include "ui_czujnikanalogowywidget.h"

CzujnikAnalogowyWidget::CzujnikAnalogowyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CzujnikAnalogowyWidget)
{
    ui->setupUi(this);
    for ( unsigned short i = 0 ; i < maxPoz; ++i)
        values[i] = 0;
    idx = 0;
    prec = 0;
    connect(&timer, &QTimer::timeout, this, &CzujnikAnalogowyWidget::timeout);
    timer.setInterval(100*maxPoz);
    timer.start();
}

CzujnikAnalogowyWidget::~CzujnikAnalogowyWidget()
{
    timer.stop();
    delete ui;
}

void CzujnikAnalogowyWidget::setParam(const QString & _name, const double & _ratio, const QString & _unit)
{
    name = _name;
    ratio = _ratio;
    unit = _unit;
    ui->opis->setText(name);
    ui->jednostka->setText(unit);
    ui->value->setText("-");
}

void CzujnikAnalogowyWidget::setValue(const double &val)
{
    mutex.lock();
    values[idx++] = val;
    mutex.unlock();
    if (idx >= maxPoz )
        idx = 0;

}

void CzujnikAnalogowyWidget::timeout()
{
    double suma = 0;
    mutex.lock();
    for (unsigned short i = 0; i < maxPoz; ++i) {
        suma += values[i];
    }
    mutex.unlock();
    double avg = suma / maxPoz;
    ui->value->setText(QString::number(avg, 'f', prec ));
}

void CzujnikAnalogowyWidget::setPrec(unsigned short newPrec)
{
    prec = newPrec;
}
