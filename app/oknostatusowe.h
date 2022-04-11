#ifndef OKNOSTATUSOWE_H
#define OKNOSTATUSOWE_H

#include <QWidget>
#include <QTimer>
#include <QFrame>
#include <QMap>

#include "ustawienia.h"

class StanCzujnika;
namespace Ui {
class OknoStatusowe;
}

class OknoStatusowe : public QFrame
{
    Q_OBJECT

public:
    explicit OknoStatusowe(QWidget *parent = 0);
    ~OknoStatusowe();
    void setLabels(const Ustawienia & set);
public slots:
    void setDigitalValue(int id, bool val);
    void setDozownik(bool open, bool conf);
    void setUSB6210(bool open, bool conf);
    void setUSB6501(bool open, bool conf);

private slots:
    void timeout();
private:
    Ui::OknoStatusowe *ui;
    QTimer timer;

    bool bOpenUsb6210;
    bool bOpenUsb6501;
    bool bOpenDozownik;

    bool bConfUsb6210;
    bool bConfUsb6501;
    bool bConfDozownik;

    QMap<unsigned int, bool> bMapZawor;
    QMap<unsigned int, StanCzujnika*> sMapZawor;
    const unsigned int mapDigitalOrder[10];
    const short maxZawors;
};

#endif // OKNOSTATUSOWE_H
