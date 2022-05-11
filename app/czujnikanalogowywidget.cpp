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
    runTimer = true;
    timer.start();

    QFont fontOpis = ui->opis->font();
    fontOpis.setPointSize(12);
    ui->opis->setFont(fontOpis);

    QFont fontJedn = ui->jednostka->font();
    fontJedn.setPointSize(18);
    fontJedn.setBold(true);
    ui->jednostka->setFont(fontJedn);

    QFont fontVal = ui->value->font();
    fontVal.setPointSize(32);
    fontVal.setBold(true);
    ui->value->setFont(fontVal);
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
    minVal = czA.minVal;
    percent = czA.percentStab;
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
    if (!runTimer)
        return;
    double suma = 0;
    mutex.lock();
    QList<double> l;
    for (unsigned short i = 0; i < cntAvgDisp; ++i) {
        suma += values[i];
        l << values[i];
    }
    mutex.unlock();

    //qInfo() << "LIST: " << name << " " << l;

    double avg = suma / cntAvgDisp;
    ui->value->setText(QString::number(avg, 'f', prec ));

    prev[actId++] = avg - prevAvg;
    if (actId > 10)
        actId = 0;

    prevAvg = avg;
    double diffSuma = 0.0;

    for (int i=0; i < 10; ++i)
        diffSuma += prev[i];
    bool actstab;
    if (avg < minVal) {
        actstab = true;
    } else {
        actstab = (100*abs(diffSuma)/avg) > percent;
        //qDebug() << "NIESTAB = " << diffSuma << " " << avg << " " << (100*abs(diffSuma)/avg);
    }

    if (stab == actstab)
        return;
    ui->value->setStyleSheet(actstab ? "background-color:red" : "background-color:green");
    stab = actstab;
}

void CzujnikAnalogowyWidget::setPrec(unsigned short newPrec)
{
    prec = newPrec;
}

void CzujnikAnalogowyWidget::stopTimer()
{
    runTimer = false;
}
