#ifndef URZADZENIA_H
#define URZADZENIA_H

#include <QWidget>
#include <QDialog>

#include "ustawienia.h"
#include "serialmessage.h"
namespace Ui {
class Urzadzenia;
}

class Urzadzenia : public QDialog
{
    Q_OBJECT

public:
    explicit Urzadzenia(QWidget *parent = 0);
    ~Urzadzenia();

    void setLabels(const Ustawienia & ust);
signals:
    void analogValueChanged(int id, int mV);
    void digitalValueChanged(int id, bool high);
    void dozownik(bool ok);



private slots:


    void on_analog_1_valueChanged(int value);
    void on_analog_2_valueChanged(int value);
    void on_analog_3_valueChanged(int value);
    void on_analog_4_valueChanged(int value);
    void on_analog_5_valueChanged(int value);
    void on_analog_6_valueChanged(int value);
    void on_analog_7_valueChanged(int value);
    void on_analog_8_valueChanged(int value);

    void changeDigital_1(bool val);
    void changeDigital_2(bool val);
    void changeDigital_3(bool val);
    void changeDigital_4(bool val);
    void changeDigital_5(bool val);
    void changeDigital_6(bool val);
    void changeDigital_7(bool val);
    void changeDigital_8(bool val);
    void changeDigital_9(bool val);

    void dozownikTimeout();

    /* serial */
    void successOpenDevice(bool);
    void deviceName(QString);
    void controllerOK();
    void echoOK();

    void errorSerial(QString);
    void debug(QString);

signals:
    /* serial */
    void connectToSerial();
    void echo();

private:
    Ui::Urzadzenia *ui;
    SerialMessage smg;
    bool connect2Serial;
    short cntEcho;
    QTimer dozownikLoop;
};

#endif // URZADZENIA_H
