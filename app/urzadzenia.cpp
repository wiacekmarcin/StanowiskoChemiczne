#include "urzadzenia.h"
#include "common.h"
#include <math.h>
Urzadzenia::Urzadzenia(Ustawienia & ustawiania_, QObject *parent)
    : QObject{parent},
#if !SYMULATOR
      nicards(this),
#endif
      m_serialDev(ustawiania_, this),
      m_ustawienia(ustawiania_)

{
#if !SYMULATOR
    connect(&nicards, &NICards::digitalReadValueChanged,    this,       &Urzadzenia::ni_digitalRead);
    connect(&nicards, &NICards::error,          this,       &Urzadzenia::ni_error);
    connect(&nicards, &NICards::debug,          this,       &Urzadzenia::ni_debug);
    connect(&nicards, &NICards::usb6210,        this,       &Urzadzenia::usb6210);
    connect(&nicards, &NICards::usb6501,        this,       &Urzadzenia::ni_usb6501);
    connect(&nicards, &NICards::usb6501,        this,       &Urzadzenia::usb6501);
    connect(&nicards, &NICards::analogValueChanged, this,   &Urzadzenia::ni_analogValueChanged);
#endif
    connect(&m_serialDev, &SerialDevice::dozownikConfigured, this, &Urzadzenia::ds_dozownikConfigured);
    connect(&m_serialDev, &SerialDevice::setCykleDone, this,     &Urzadzenia::setCykleDone);
    connect(&m_serialDev, &SerialDevice::setStepsDone, this,     &Urzadzenia::setStepsDone);
    connect(&m_serialDev, &SerialDevice::checkPositionHomeDone, this,     &Urzadzenia::checkPositionHomeDone);

}

void Urzadzenia::setThreads(QThread *niReads, QThread *dozownik)
{
    m_NI_Cards.setThreads(niReads);
    m_serialDev.setThread(dozownik);
}

void Urzadzenia::setStop()
{
    m_NI_Cards.setStop();
    m_serialDev.setStop();
}

void Urzadzenia::setCykle(uint8_t nrDoz, uint32_t nrCyckli)
{
    //qDebug"%s:%d setCykle [%d %d]", __FILE__, __LINE__, nrDoz, nrCyckli);
    m_serialDev.setCykle(nrDoz,nrCyckli);
}

void Urzadzenia::setMl(uint8_t nrDoz, uint32_t mlx10)
{
    uint32_t steps = round(mlx10 * m_ustawienia.getStepsOnMl() / 10.0);
    //qDebug("%s:%d %f ml => %d steps", __FILE__, __LINE__, mlx10/10.0, steps);
    setSteps(nrDoz, steps);
}

void Urzadzenia::digitalWriteAll(uint16_t vals)
{
#if !SYMULATOR
    nicards.digitalWriteAll(vals);
    emit digitalWriteDevice(nicards.getDigitalWrite());
#else
    (void)vals;
#endif
}

void Urzadzenia::digitalWrite(digitalOut mask, bool on)
{
#if !SYMULATOR
    nicards.digitalWrite(mask, on);
    emit digitalWriteDevice(nicards.getDigitalWrite());
#else
    (void)mask;
    (void)on;
#endif
}

void Urzadzenia::zaplon(rodzajZaplonu idiskra)
{
    switch(idiskra) {
    case z_iskra_elektryczna:
        runIskraElektryczna();
        break;
    case z_iskra_mechaniczna:
        runIskraMechaniczna();
        break;
    case z_iskra_plomien:
        runPlomien();
        break;
    default:
        break;
    }

    //qDebug"%s:%d run_iskra %d", __FILE__, __LINE__, idiskra);
}

void Urzadzenia::checkPositionHome()
{
    m_serialDev.checkPositionHome();
}

void Urzadzenia::resetDozownik()
{
    m_serialDev.setReset();
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
    double v0 = val0 * m_ustawienia.getRatio(0);
    double v1 = val1 * m_ustawienia.getRatio(1);
    double v2 = val2 * m_ustawienia.getRatio(2);
    double v3 = val3 * m_ustawienia.getRatio(3);
    double v4 = val4 * m_ustawienia.getRatio(4);
    double v5 = val5 * m_ustawienia.getRatio(5);
    double v6 = val6 * m_ustawienia.getRatio(6);
    double v7 = val0 * m_ustawienia.getRatio(7);
    emit analogValueChanged(v0, v1, v2, v3, v4, v5, v6, v7);
    //TODO zapis do plikow
}

void Urzadzenia::ni_usb6501(bool open, bool conf)
{
    //qDebug("%s:%d n_usb6501 %d:%d",__FILE__,__LINE__,open,conf);
    m_digCardsOk = open && conf;
}

void Urzadzenia::ni_digitalRead(uint16_t vals)
{

    if (m_inputs == vals)
        return;

    if (m_inputs != vals) {
        emit digitalReadAllValueChanged(vals);
    }

    uint16_t changeMask = vals ^ m_inputs;
    uint16_t mask = 0x1;

    for (short i = 0; i < Ustawienia::maxCzujekCyfrIn; ++i) {
        if (mask & changeMask) {
            //qDebug"%s:%d %04x %d", __FILE__,__LINE__,mask, vals & mask ? 1 : 0);
            emit digitalReadValueChanged(mask, vals & mask);
        }
        mask <<= 1;
    }
    m_inputs = vals;
}

void Urzadzenia::digitalWriteDebug(uint16_t vals)
{
#if !SYMULATOR
    if (m_digCardsOk) {
        nicards.digitalWriteAll(vals);
        emit digitalWriteDevice(nicards.getDigitalWrite());
    }
#else
    (void)vals;
#endif
}

void Urzadzenia::readInputs()
{
     emit digitalReadAllValueChanged(m_inputs);

    uint16_t mask = 0x1;
    for (short i = 0; i < Ustawienia::maxCzujekCyfrIn; ++i) {
        emit digitalReadValueChanged(mask, m_inputs & mask);
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
    m_dozownikOk = open && conf;
    emit dozownik(open, conf);
}

void Urzadzenia::runIskraElektryczna()
{

    digitalWrite(o_hv_onoff, true);
    digitalWrite(o_hv_bezpiecznik, false);
    digitalWrite(o_hv_iskra, false);

    QTimer::singleShot(2000, this, &Urzadzenia::runIskraElektryczna1);
}

void Urzadzenia::runIskraElektryczna1()
{
    digitalWrite(o_trigger, true);
    digitalWrite(o_hv_onoff, false);
    digitalWrite(o_hv_bezpiecznik, false);
    digitalWrite(o_hv_iskra, true);

    QTimer::singleShot(1000, this, &Urzadzenia::runIskraElektryczna2);
}

void Urzadzenia::runIskraElektryczna2()
{
    digitalWrite(o_trigger, false);
    digitalWrite(o_hv_onoff, false);
    digitalWrite(o_hv_bezpiecznik, true);
    digitalWrite(o_hv_iskra, false);
}

void Urzadzenia::runIskraMechaniczna()
{
    digitalWrite(o_trigger, true);
    digitalWrite(o_mech_iskra, true);
    QTimer::singleShot(2000, this, &Urzadzenia::runIskraMechaniczna1);
}

void Urzadzenia::runIskraMechaniczna1()
{
    digitalWrite(o_trigger, false);
    digitalWrite(o_mech_iskra, false);
}

void Urzadzenia::runPlomien()
{
    digitalWrite(o_trigger, true);
    digitalWrite(o_grzalka, true);
    QTimer::singleShot(6000, this, &Urzadzenia::runPlomien1);

}

void Urzadzenia::setSteps(uint8_t nrDoz, uint32_t impuls)
{
    m_serialDev.setSteps(nrDoz,impuls);
}

void Urzadzenia::runPlomien1()
{
    digitalWrite(o_trigger, false);
    digitalWrite(o_grzalka, false);
}





