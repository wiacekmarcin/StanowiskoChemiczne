#ifndef OKNOSTATUSOWE_H
#define OKNOSTATUSOWE_H

#include <QWidget>
#include <QTimer>
#include <QFrame>

#include "ustawienia.h"

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


    bool bZawor1;
    bool bZawor2;
    bool bZawor3;
    bool bZawor4;
    bool bZawor5;
    bool bZawor6;
    bool bZawor7;
    bool bZawor8;
    bool bpilot;
    bool busb6210;
    bool busb6501;
    bool bdozownik;
};

#endif // OKNOSTATUSOWE_H
