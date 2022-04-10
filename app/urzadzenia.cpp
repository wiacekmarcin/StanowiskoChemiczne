#include "urzadzenia.h"
#include "common.h"

Urzadzenia::Urzadzenia(Ustawienia & ustawiania_, QObject *parent)
    : QObject{parent},
#if !SYMULATOR
      nicards(this),
#endif
      serial(ustawiania_, this),
      ustawienia(ustawiania_)

{
#if !SYMULATOR
    connect(&nicards, &NICards::digitalRead,    this,       &Urzadzenia::ni_digitalRead);
    connect(&nicards, &NICards::error,          this,       &Urzadzenia::ni_error);
    connect(&nicards, &NICards::debug,          this,       &Urzadzenia::ni_debug);
    connect(&nicards, &NICards::usb6210,        this,       &Urzadzenia::usb6210);
    connect(&nicards, &NICards::usb6501,        this,       &Urzadzenia::ni_usb6501);
    connect(&nicards, &NICards::usb6501,        this,       &Urzadzenia::usb6501);
    connect(&nicards, &NICards::analogValueChanged, this,   &Urzadzenia::ni_analogValueChanged);
#endif
    connect(&serial, &SerialDevice::dozownikConfigured, this, &Urzadzenia::ds_dozownikConfigured);
    connect(&serial, &SerialDevice::setCykleDone, this,     &Urzadzenia::setCykleDone);
    connect(&serial, &SerialDevice::setStepsDone, this,     &Urzadzenia::setStepsDone);
    connect(&serial, &SerialDevice::checkPositionHomeDone, this,     &Urzadzenia::checkPositionHomeDone);

}

void Urzadzenia::setCykle(uint8_t nrDoz, uint32_t nrCyckli)
{
    //qDebug"%s:%d setCykle [%d %d]", __FILE__, __LINE__, nrDoz, nrCyckli);
    serial.setCykle(nrDoz,nrCyckli);
}

void Urzadzenia::setMl(uint8_t nrDoz, uint32_t mlx10)
{
    uint32_t steps = round(mlx10 * ustawienia.getStepsOnMl() / 10.0);
    //qDebug("%s:%d %f ml => %d steps", __FILE__, __LINE__, mlx10/10.0, steps);
    setSteps(nrDoz, steps);
}

void Urzadzenia::digitalWriteAll(uint16_t vals)
{
#if !SYMULATOR
    nicards.digitalWriteDebug(vals);
    emit digitalWriteDevice(nicards.getDigitalWrite());
#else
    (void)vals;
#endif
}

void Urzadzenia::digitalWrite(uint16_t mask, bool on)
{
#if !SYMULATOR
    nicards.digitalWrite(mask, on);
    emit digitalWriteDevice(nicards.getDigitalWrite());
#else
    (void)mask;
    (void)on;
#endif
}

void Urzadzenia::zaplon(short idiskra)
{
    switch(idiskra) {
    case ISKRA_ELEKTRYCZNA:
        runIskraElektryczna();
        break;
    case ISKRA_MECHANICZNA:
        runIskraMechaniczna();
        break;
    case ISKRA_PLOMIEN:
        runPlomien();
        break;
    default:
        break;
    }

    //qDebug"%s:%d run_iskra %d", __FILE__, __LINE__, idiskra);
}

void Urzadzenia::checkPositionHome()
{
    serial.checkPositionHome();
}

void Urzadzenia::ni_error(const QString &s)
{
    //qDebug"ni_error %s", s.toStdString().c_str());
}

void Urzadzenia::ni_debug(const QString &d)
{
    //qDebug"NI %s", d.toStdString().c_str());
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
    //qDebug("%s:%d n_usb6501 %d:%d",__FILE__,__LINE__,open,conf);
    digitalConn = open && conf;
}

void Urzadzenia::ni_digitalRead(uint16_t vals)
{

    if (m_inputs == vals)
        return;

    if (m_inputs != vals) {
        emit digitalAllRead(vals);
    }

    uint16_t changeMask = vals ^ m_inputs;
    uint16_t mask = 0x1;

    for (short i = 0; i < Ustawienia::maxCzujekCyfrIn; ++i) {
        if (mask & changeMask) {
            //qDebug"%s:%d %04x %d", __FILE__,__LINE__,mask, vals & mask ? 1 : 0);
            emit digitalRead(mask, vals & mask);
        }
        mask <<= 1;
    }
    m_inputs = vals;
}

void Urzadzenia::digitalWriteDebug(uint16_t vals)
{
#if !SYMULATOR
    nicards.digitalWriteDebug(vals);
    emit digitalWriteDevice(nicards.getDigitalWrite());
#else
    (void)vals;
#endif
}

void Urzadzenia::readInputs()
{
     emit digitalAllRead(m_inputs);

    uint16_t mask = 0x1;
    for (short i = 0; i < Ustawienia::maxCzujekCyfrIn; ++i) {
        emit digitalRead(mask, m_inputs & mask);
        mask <<= 1;
    }
}

void Urzadzenia::ds_error(const QString &s)
{
    //qDebug"ds_error %s", s.toStdString().c_str());
}

void Urzadzenia::ds_debug(const QString &d)
{
    //qDebug"DS %s", d.toStdString().c_str());
}

void Urzadzenia::ds_errorSerial(const QString & s)
{
    //qDebug"ds_error_serial %s", s.toStdString().c_str());
}

void Urzadzenia::ds_dozownikConfigured(bool open, bool conf)
{
    digitalConn = open && conf;
    emit dozownik(open, conf);
}

void Urzadzenia::runIskraElektryczna()
{

    digitalWrite(hv_onoff, true);
    digitalWrite(hv_bezpieczenstwa, false);
    digitalWrite(hw_iskra, false);

    QTimer::singleShot(2000, this, &Urzadzenia::runIskraElektryczna1);
}

void Urzadzenia::runIskraElektryczna1()
{
    digitalWrite(trigger, true);
    digitalWrite(hv_onoff, false);
    digitalWrite(hv_bezpieczenstwa, false);
    digitalWrite(hw_iskra, true);

    QTimer::singleShot(1000, this, &Urzadzenia::runIskraElektryczna2);
}

void Urzadzenia::runIskraElektryczna2()
{
    digitalWrite(trigger, false);
    digitalWrite(hv_onoff, false);
    digitalWrite(hv_bezpieczenstwa, true);
    digitalWrite(hw_iskra, false);
}

void Urzadzenia::runIskraMechaniczna()
{
    digitalWrite(trigger, true);
    digitalWrite(mech_iskra, true);
    QTimer::singleShot(2000, this, &Urzadzenia::runIskraMechaniczna1);
}

void Urzadzenia::runIskraMechaniczna1()
{
    digitalWrite(trigger, false);
    digitalWrite(mech_iskra, false);
}

void Urzadzenia::runPlomien()
{
    digitalWrite(trigger, true);
    digitalWrite(plomien, true);
    QTimer::singleShot(6000, this, &Urzadzenia::runPlomien1);

}

void Urzadzenia::setSteps(uint8_t nrDoz, uint32_t impuls)
{
    serial.setSteps(nrDoz,impuls);
}

void Urzadzenia::runPlomien1()
{
    digitalWrite(trigger, false);
    digitalWrite(plomien, false);
}





