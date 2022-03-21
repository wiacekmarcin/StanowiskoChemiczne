#ifndef URZADZENIA_H
#define URZADZENIA_H

#include <QWidget>
#include <QDialog>
#include <QTimer>
#include <QMap>

#include "ustawienia.h"
#include "serialmessage.h"

#include "niusb6501.h"
#include "niusb6210.h"

namespace Ui {
class Urzadzenia;
}

class QToolButton;
class DigitalOutWidget;
class HighLowDigitalWidget;
class QSlider;

class Urzadzenia : public QDialog
{
    Q_OBJECT
public:



public:
    explicit Urzadzenia(QWidget *parent = 0);
    ~Urzadzenia();

    void setLabels(const Ustawienia & ust);

    void setUstawienia(const Ustawienia & ust);
    void setUstawienia(Ustawienia *ust_);

    SerialMessage* getSerial() { return &smg; }

    void setIskra();
    void setDigital(uint16_t mask, bool value);
signals:
    void analogValueChanged(double val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8);

    void digitalValueChanged(int id, bool high);

    void dozownik(bool ok);
    void usb6210(bool ok);
    void usb6501(bool ok);

    void drzwi_komory(bool prawe, bool otwarte);
    void zawor(int idz, bool otwarty);
    void pilot_btn(bool otwarty);

    /* serial */
    void connectToSerial();
    void echo();
    void setPositionHome(short dozownikNr);
    void setSettings(bool reverse, uint32_t maxImp);
    void setSettingsAll(bool reverse1, bool reverse2, bool reverse3, bool reverse4, bool reverse5, uint32_t maxImp);
    void setPosition(short dozownikNr, uint32_t x);

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
    void timerUsbDevice();
    void timeoutDI100ms();
    void timeoutAI100ms();

    /* serial */
    void successOpenDevice(bool);
    void echoOK(bool ok);

    void errorSerial(QString);
    void debug(QString);


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

    void on_pb_iskramechaniczna_clicked();

protected:
    short getDozownikNr() { return dozownikNr; };
    void checkUsbCard();
    void checkSerial();

    void on_tb_out_clicked(QToolButton * tb,  DigitalOutWidget * dow, uint16_t mask);
    void changeDigital(int maks, bool val);
    void changeAnalog(unsigned short aId, double val, bool device);
    void digitalChange(int id, bool val);
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
    QTimer timerCheckDevice;

    short dozownikNr;
    bool usbDio;
    bool usbAnal;
    QString readDigString;
    QString writeDigString;
    QString readAnalString;

    Ustawienia * ust;
    const unsigned int inMap[10];
    const unsigned int outMap[11];
    const unsigned int anMap[8];
    QMap<unsigned int, HighLowDigitalWidget*> inRevMap;
    QMap<unsigned short, QSlider*> anRevMap;
};

#endif // URZADZENIA_H
