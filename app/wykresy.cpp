#include "wykresy.h"
#include "ui_wykresy.h"

#include "digitaloutwidget.h"
#include <QMutexLocker>
#include "ustawienia.h"

Wykresy::Wykresy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Wykresy)
{
    ui->setupUi(this);
    this->setModal(false);
    widgets[0] = ui->widget_1;
    widgets[1] = ui->widget_2;
    widgets[2] = ui->widget_3;
    widgets[3] = ui->widget_4;
    widgets[4] = ui->widget_5;
    widgets[5] = ui->widget_6;
    widgets[6] = ui->widget_7;
    widgets[7] = ui->widget_8;

    //voc1 voc2 o2 co2 cisnkomor tempkomory temppar a8
    mapIdAnal[0] = a_voc1;
    mapIdAnal[1] = a_voc2;
    mapIdAnal[2] = a_o2;
    mapIdAnal[3] = a_co2;
    mapIdAnal[4] = a_cisn_komora;
    mapIdAnal[5] = a_temp_komory;
    mapIdAnal[6] = a_temp_parownik;
    mapIdAnal[7] = a_8;

    for (int i=0; i < Ustawienia::maxCzujekAnal; i++) {
        dane[i].clear();
        convRatio[i] = 1.0;
    }
    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, &Wykresy::updateTime);
}

Wykresy::~Wykresy()
{
    delete ui;
}

void Wykresy::setWykresVisible(unsigned int id, bool vis)
{
    showW[id] = vis;
    bool visible = false;
    widgets[id]->setVisible(vis);
    for (int i=0; i < Ustawienia::maxCzujekAnal; i++) {
        if (showW[i]) {
            visible = true;
            break;
        }
    }
    this->setVisible(visible);
}

void Wykresy::updateValue(double val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8)
{
    QMutexLocker locker(&mutex);
    dane[0].push_back(val1);
    dane[1].push_back(val2);
    dane[2].push_back(val3);
    dane[3].push_back(val4);
    dane[4].push_back(val5);
    dane[5].push_back(val6);
    dane[6].push_back(val7);
    dane[7].push_back(val8);
}

void Wykresy::updateTime()
{
    QVector<float> temp[Ustawienia::maxCzujekAnal];

    mutex.lock();
    for (int i = 0; i < Ustawienia::maxCzujekAnal; i++) {
        temp[i].clear();
        dane[i].swap(temp[i]);
    }
    mutex.unlock();
    for (int i = 0; i < Ustawienia::maxCzujekAnal; i++) {
        double suma = 0.0;
        foreach(auto f, temp[i]) {
            suma += f;
        }
        if (temp[i].size() > 0)
            suma = suma / temp[i].size();
        widgets[i]->addValue(suma * convRatio[i]);
        widgets[i]->updateVals();
    }
}

void Wykresy::setUstawienia(const Ustawienia &ust)
{
    for (int i = 0; i < Ustawienia::maxCzujekAnal; i++) {
        Ustawienia::CzujnikAnalogowy an = ust.getCzujnikAnalogowyUstawienia((analogIn)mapIdAnal[i]);
        convRatio[i] = an.convert;
        widgets[i]->setUnit(an.unit);
    }
}
