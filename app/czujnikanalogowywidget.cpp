#include "czujnikanalogowywidget.h"
#include "ui_czujnikanalogowywidget.h"
#include <QDebug>
#include <cmath>
CzujnikAnalogowyWidget::CzujnikAnalogowyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CzujnikAnalogowyWidget)
{
    ui->setupUi(this);
    for ( unsigned short i = 0 ; i < cntAvgDisp; ++i)
        values[i] = 0;
    for (int i = 0; i < 10; ++i)
        prev[i] = i;
    prevAvg = 0;
    actId = 0;
    idx = 0;
    prec = 0;
    connect(&timer, &QTimer::timeout, this, &CzujnikAnalogowyWidget::timeout);
    timer.setInterval(100*cntAvgDisp);
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
    if (idx >= cntAvgDisp )
        idx = 0;
 }

void CzujnikAnalogowyWidget::timeout()
{
    double suma = 0;
    mutex.lock();
    QList<double> l;
    for (unsigned short i = 0; i < cntAvgDisp; ++i) {
        suma += values[i];
        l << values[i];
    }
    mutex.unlock();

    qInfo() << "LIST: " << name << " " << l;

    double avg = suma / cntAvgDisp;
    ui->value->setText(QString::number(avg, 'f', prec ));

    prev[actId++] = avg - prevAvg;
    if (actId > 10)
        actId = 0;

    prevAvg = avg;
    double diffSuma = 0.0;

    for (int i=0; i < 10; ++i)
        diffSuma += prev[i];
    
    if (avg == 0) {

    }
    bool nostab = (100*abs(diffSuma)/avg) > 2;

    if (stab == nostab)
        return;
    ui->value->setStyleSheet(nostab ? "background-color:red" : "background-color:green");
    stab = nostab;
}

void CzujnikAnalogowyWidget::setPrec(unsigned short newPrec)
{
    prec = newPrec;
}
