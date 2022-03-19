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
    void setDozownik(bool ok);
    void setUSB6210(bool ok);
    void setUSB6501(bool ok);

    void setDrzwiKomory(bool prawe, bool otwarte);
    void setZawor(int idZ, bool otwarte);
    void setPilot(bool przycisk);

private slots:
    void timeout();
private:
    Ui::OknoStatusowe *ui;
    QTimer timer;

    bool busb6210;
    bool busb6501;
    bool bdozownik;

    QMap<unsigned int, bool> bMapZawor;
    QMap<unsigned int, StanCzujnika*> sMapZawor;
    const unsigned int mapDigitalOrder[10];
    const short maxZawors;
};

#endif // OKNOSTATUSOWE_H
