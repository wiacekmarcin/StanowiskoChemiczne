#ifndef URZADZENIA_H
#define URZADZENIA_H

#include <QObject>
#include <QString>

#include "nicards.h"
#include "ustawienia.h"
#include "serialdevice.h"

class Urzadzenia : public QObject
{
    Q_OBJECT
public:
    explicit Urzadzenia(Ustawienia & ustawiania, QObject *parent = nullptr);


public slots:
    void setCykle(uint8_t nrDoz, uint32_t nrCyckli);

    void setMl(uint8_t nrDoz, uint32_t ml);
    void digitalWriteAll(uint16_t vals);
    void digitalWrite(uint16_t mask, bool on);
    void zaplon(short idiskra);
    void checkPositionHome();

signals:
    void analogValueChanged(double val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8);
    void digitalValueChanged(uint16_t value);

    void usb6210(bool open, bool conf);
    void usb6501(bool open, bool conf);
    void dozownik(bool open, bool conf);
    //void digitalRead(uint16_t vals, bool open);
    //void digitalAllRead(uint16_t vals);

    void digitalWriteDevice(uint16_t vals);

    void connDozownik(bool);
    void resetDone(bool);
    void setParamsDone(bool);
    void setPositionHomeDone(bool);
    void setPositionDone(bool);
    void setCykleDone(bool);
    void setStepsDone(bool);
    void checkPositionHomeDone(bool,bool,bool,bool,bool,bool);

public slots:
    void ni_analogValueChanged(double val0, double val1, double val2, double val3, double val4, double val5, double val6);
    void digitalWriteDebug(uint16_t vals);
    void readInputs();

private slots:

    void ni_error(const QString &s);
    void ni_debug(const QString &d);
    void ni_usb6501(bool open, bool conf);

    void ds_error(const QString &s);
    void ds_debug(const QString &d);
    void ds_errorSerial(const QString &s);
    void ds_dozownikConfigured(bool open, bool conf);

protected:
    void runIskraElektryczna();
    void runIskraMechaniczna();
    void runPlomien();
    void setSteps(uint8_t nrDoz, uint32_t impuls);

private slots:
    void runIskraElektryczna1();
    void runIskraElektryczna2();
    void runIskraMechaniczna1();
    void runPlomien1();

private:
#if !SYMULATOR
     NICards nicards;
#endif
     SerialDevice serial;
     Ustawienia & ustawienia;

     bool digitalConn;
     bool dozownikConn;

     uint16_t m_inputs;

     QTimer plTimer;
     QTimer imTimer;
     QTimer elTimer;
};

#endif // URZADZENIA_H

