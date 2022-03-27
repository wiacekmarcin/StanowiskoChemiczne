#include "urzadzenia.h"

Urzadzenia::Urzadzenia(Ustawienia & ustawiania_, QObject *parent)
    : QObject{parent},
      nicards(this),
      serial(ustawiania_, this),
      ustawienia(ustawiania_)

{
    connect(&nicards, &NICards::digitalRead,    this,       &Urzadzenia::ni_digitalRead);
    connect(&nicards, &NICards::error,          this,       &Urzadzenia::ni_error);
    connect(&nicards, &NICards::debug,          this,       &Urzadzenia::ni_debug);
    connect(&nicards, &NICards::usb6210,        this,       &Urzadzenia::usb6210);
    connect(&nicards, &NICards::usb6501,        this,       &Urzadzenia::ni_usb6501);
    connect(&nicards, &NICards::analogValueChanged, this,   &Urzadzenia::ni_analogValueChanged);

    connect(&serial, &SerialDevice::dozownikConfigured, this, &Urzadzenia::ds_dozownikConfigured);


    for (short i = 0; i < Ustawienia::maxCzujekCyfrIn; ++i) {
        m_inputsMap[0x1 << i] = false;
    }


}

void Urzadzenia::ni_error(const QString &s)
{
    qDebug("ni_error %s", s.toStdString().c_str());
}

void Urzadzenia::ni_debug(const QString &d)
{
    qDebug("NI %s", d.toStdString().c_str());
}

void Urzadzenia::ni_analogValueChanged(double val0, double val1, double val2, double val3, double val4, double val5, double val6)
{
    double v0 = val0 * ustawienia.getRatio(0);
    double v1 = val1 * ustawienia.getRatio(1);
    double v2 = val2 * ustawienia.getRatio(2);
    double v3 = val3 * ustawienia.getRatio(3);
    double v4 = val4 * ustawienia.getRatio(4);
    double v5 = val5 * ustawienia.getRatio(5);
    double v6 = val6 * ustawienia.getRatio(6);
    double v7 = val0 * ustawienia.getRatio(7);
    emit analogValueChanged(v0, v1, v2, v3, v4, v5, v6, v7);
    //TODO zapis do plikow
}

void Urzadzenia::ni_usb6501(bool open, bool conf)
{
    digitalConn = open && conf;
    emit usb6210(open, conf);
}

void Urzadzenia::ni_digitalRead(uint16_t vals)
{
    qDebug("%s:%d", __FILE__,__LINE__);
    uint32_t mask;
    bool bval;
    for (short i = 0; i < Ustawienia::maxCzujekCyfrIn; ++i) {
        mask = 0x1 << i;
        bval = ~vals & mask;
        if (bval != m_inputsMap[mask]) {
            m_inputsMap[mask] = bval;
            qDebug("%s:%d", __FILE__,__LINE__);
            emit digitalRead(mask, bval);
        }
    }
}

void Urzadzenia::ds_error(const QString &s)
{
    qDebug("ds_error %s", s.toStdString().c_str());
}

void Urzadzenia::ds_debug(const QString &d)
{
    qDebug("DS %s", d.toStdString().c_str());
}

void Urzadzenia::ds_errorSerial(const QString & s)
{
    qDebug("ds_error_serial %s", s.toStdString().c_str());
}

void Urzadzenia::ds_dozownikConfigured(bool open, bool conf)
{
    digitalConn = open && conf;
    emit dozownik(open, conf);
}

