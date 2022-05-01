#include "czujnikanalogowywidget.h"
#include "ui_czujnikanalogowywidget.h"

CzujnikAnalogowyWidget::CzujnikAnalogowyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CzujnikAnalogowyWidget)
{
    ui->setupUi(this);
    ui->green->setVisible(false);
    for ( unsigned short i = 0 ; i < maxPoz; ++i)
        values[i] = 0;
    prev2 = prev3 = 0;
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

void CzujnikAnalogowyWidget::setParam(const Ustawienia::CzujnikAnalogowy & czA)
{
    name = czA.name;
    convert = czA.convert;
    unit = czA.unit;
    ui->opis->setText(name);
    ui->jednostka->setText(unit);
    ui->value->setText("-");
}

void CzujnikAnalogowyWidget::setValue(const double &val)
{
    mutex.lock();
    values[idx++] = convert * val;
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

    bool stab = ((avg-prev2)*(avg-prev2)/(avg*avg) < 0.01) && ((avg-prev3)*(avg-prev3)/(avg*avg) < 0.01);
    prev3 = prev2;
    prev2 = avg;
    ui->red->setVisible(!stab);
    ui->green->setVisible(stab);
}

void CzujnikAnalogowyWidget::setPrec(unsigned short newPrec)
{
    prec = newPrec;
}
