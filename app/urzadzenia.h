#ifndef URZADZENIA_H
#define URZADZENIA_H

#include <QWidget>
#include <QDialog>
#include <QTimer>

#include "ustawienia.h"
#include "serialmessage.h"

#include "niusb6501.h"
#include "niusb6210.h"

namespace Ui {
class Urzadzenia;
}

class QToolButton;
class DigitalOutWidget;

class Urzadzenia : public QDialog
{
    Q_OBJECT

    typedef enum _aInput {
        vol1,
        vol2,
        o2,
        co2,
        wika_a10,
        temp_komory
    } analogIn;

    typedef enum _iDig {
        drzwi_lewe      = 0x01,
        wentylacja_lewa = 0x02,
        proznia         = 0x04,
        probka_in       = 0x08,
        drzwi_prawe     = 0x10,
        wentylacja_prawa= 0x20,
        wlot_powietrza  = 0x40,
        probka_out      = 0x80
    } digitalIn;

    typedef enum _oDig {
        high_voltage    = 0x001,
        bezpiecznik     = 0x002,
        hw_iskra        = 0x004,
        mech_iskra      = 0x008,
        plomien         = 0x010,
        pompa_proznia   = 0x020,
        pompa_powietrza = 0x040,
        wentylator      = 0x080,
        mieszadlo       = 0x100,
        trigger         = 0x200,
        pilot           = 0x400,
    } digitalOut;


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
    //void changeDigital_8(bool val);
    //void changeDigital_9(bool val);

    void dozownikTimeout();

    /* serial */
    void successOpenDevice(bool);
    void deviceName(QString);
    void controllerOK();
    void echoOK();

    void errorSerial(QString);
    void debug(QString);


    void on_ml_valueChanged(int arg1);
    void on_steps_valueChanged(int arg1);
    void on_pbUstaw_clicked();
    void on_rbDirectionLeft_clicked(bool checked);
    void on_parameters_clicked();
    void on_pbReturn_clicked();

    void timeoutDI100ms();
    void timeoutAI100ms();
    void on_tb_out_1_clicked();
    void on_tb_out_2_clicked();
    void on_tb_out_3_clicked();
    void on_tb_out_4_clicked();
    void on_tb_out_5_clicked();
    void on_tb_out_6_clicked();
    void on_tb_out_7_clicked();
    void on_tb_out_8_clicked();
    void on_tb_out_9_clicked();
    void on_tb_out_a_clicked();


    void on_sbDozownik_valueChanged(int arg1);

    void on_tb_out_b_clicked();

signals:
    /* serial */
    void connectToSerial();
    void echo();
    void setPositionHome(short dozownikNr);
    void setSettings(bool reverse, uint32_t maxImp);
    void setPosition(short dozownikNr, uint32_t x);
protected:
    short getDozownikNr() { return dozownikNr; };
    void on_tb_out_clicked(QToolButton * tb,  DigitalOutWidget * dow, uint16_t mask);
private:
    Ui::Urzadzenia *ui;
    SerialMessage smg;
    bool connect2Serial;
    short cntEcho;
    QTimer dozownikLoop;
    bool onlyOne;

    NIDAQMxUSB6501 dio;
    NIDAQMxUSB6210 ai;
    uint16_t vals; //wyjscia
    QTimer timerDI100;
    QTimer timerAI100;
    short dozownikNr;
};

#endif // URZADZENIA_H
