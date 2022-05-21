#ifndef WYKRESY_H
#define WYKRESY_H

#include <QDialog>
#include "ustawienia.h"
#include <QMutex>
#include <QTimer>

namespace Ui {
class Wykresy;
}
class DigitalOutWidget;

class Wykresy : public QDialog
{
    Q_OBJECT

public:
    explicit Wykresy(QWidget *parent = nullptr);
    ~Wykresy();
    void setWykresVisible(unsigned int id, bool vis);
    void setUstawienia(const Ustawienia & ust);
public slots:
    void updateValue(double val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8);

private slots:
    void updateTime();

private:
    Ui::Wykresy *ui;
    bool showW[Ustawienia::maxCzujekAnal];
    double convRatio[Ustawienia::maxCzujekAnal];
    DigitalOutWidget* widgets[Ustawienia::maxCzujekAnal];
    short mapIdAnal[Ustawienia::maxCzujekAnal];
    QVector<float> dane[Ustawienia::maxCzujekAnal];
    QMutex mutex;
    Ustawienia ust;
    QTimer timer;
};

#endif // WYKRESY_H
