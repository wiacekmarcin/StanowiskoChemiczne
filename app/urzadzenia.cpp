#include "urzadzenia.h"
#include "common.h"
#include <math.h>
#include "version.h"

#include <QDateTime>
#include <QTextStream>

Urzadzenia::Urzadzenia(Ustawienia & ustawiania_, QObject *parent)
    : QObject{parent},
#if !SYMULATOR
      nicards(this),
#endif
      m_serialDev(ustawiania_, this),
      m_ustawienia(ustawiania_)

{
    m_logFile =new QFile;
    m_logFile->setFileName(QString("%s.txt").arg(QDateTime::currentDateTime().toString()));
    m_logFile->open(QIODevice::Append | QIODevice::Text);

    log(QString("%1.%2 - %3 %4").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(__DATE__).arg(__TIME__));

#if !SYMULATOR
    connect(&nicards, &NICards::digitalReadValueChanged,    this,   &Urzadzenia::ni_digitalRead,        Qt::DirectConnection);
    connect(&nicards, &NICards::analogValueChanged,         this,   &Urzadzenia::ni_analogValueChanged, Qt::DirectConnection);

    connect(&nicards, &NICards::error,                      this,   &Urzadzenia::ni_error,              Qt::QueuedConnection);
    connect(&nicards, &NICards::debug,                      this,   &Urzadzenia::ni_debug,              Qt::QueuedConnection);
    connect(&nicards, &NICards::usb6210,                    this,   &Urzadzenia::usb6210,               Qt::QueuedConnection);
    connect(&nicards, &NICards::usb6501,                    this,   &Urzadzenia::ni_usb6501,            Qt::QueuedConnection);
    connect(&nicards, &NICards::usb6501,                    this,   &Urzadzenia::usb6501,               Qt::QueuedConnection);

#endif
    connect(&m_serialDev, &SerialDevice::dozownikConfigured,    this, &Urzadzenia::ds_dozownikConfigured, Qt::QueuedConnection);
    connect(&m_serialDev, &SerialDevice::setCykleDone,          this, &Urzadzenia::setCykleDone,          Qt::QueuedConnection);
    connect(&m_serialDev, &SerialDevice::setStepsDone,          this, &Urzadzenia::setStepsDone,          Qt::QueuedConnection);
    connect(&m_serialDev, &SerialDevice::checkPositionHomeDone, this, &Urzadzenia::checkPositionHomeDone, Qt::QueuedConnection);

    connect(&m_serialDev, &SerialDevice::debug,                 this, &Urzadzenia::ds_debug,              Qt::QueuedConnection);
    connect(&m_serialDev, &SerialDevice::error,                 this, &Urzadzenia::ds_error,              Qt::QueuedConnection);

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
}

void Urzadzenia::ni_usb6501(bool open, bool conf)
{
    m_digCardsOk = open && conf;
    if (m_digCardsOk) {
        //TODO save cache output value
        //TODO pobierz info o wejsciach
    } else {
        //TODO clear inputs
        //TODO clear outputs
    }
}

void Urzadzenia::setCykle(uint8_t nrDoz, uint32_t nrCyckli)
{
    m_serialDev.setCykle(nrDoz,nrCyckli);
}

void Urzadzenia::setMl(uint8_t nrDoz, uint32_t mlx10)
{
    uint32_t steps = round(mlx10 * m_ustawienia.getStepsOnMl() / 10.0);
    m_serialDev.setSteps(nrDoz, steps);
}

void Urzadzenia::checkPositionHome()
{
    m_serialDev.checkPositionHome();
}

void Urzadzenia::resetDozownik()
{
    m_serialDev.setReset();
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

void Urzadzenia::zaplon(ZaplonRodzaj idiskra)
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
}



void Urzadzenia::ni_error(const QString &s)
{
    log(QString("%1.ERROR.NICARDS - %2").arg(QDateTime::currentDateTime().toString()).arg(s));
}

void Urzadzenia::ni_debug(const QString &d)
{
    log(QString("%1.DEBUG.NICARDS - %2").arg(QDateTime::currentDateTime().toString()).arg(d));
}

void Urzadzenia::ds_error(const QString &s)
{
    log(QString("%1.ERROR.DOZOWNIK - %2").arg(QDateTime::currentDateTime().toString()).arg(s));
}

void Urzadzenia::ds_debug(const QString &d)
{
    log(QString("%1.DEBUG.DOZOWNIK - %2").arg(QDateTime::currentDateTime().toString()).arg(d));
}

void Urzadzenia::log(const QString &msg)
{
    QTextStream out(m_logFile);
    out.setCodec("UTF-8");
    if (m_logFile != 0) {
        out << msg << "\n";
    }
}

void Urzadzenia::readInputs()
{
     emit digitalReadAllValueChanged(m_inputs);

    uint16_t mask = 0x1;
    for (short i = 0; i < Ustawienia::maxCzujekCyfrIn; ++i) {
        emit digitalReadValueChanged((digitalIn)mask, m_inputs & mask);
        mask <<= 1;
    }
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

void Urzadzenia::runPlomien1()
{
    digitalWrite(o_trigger, false);
    digitalWrite(o_grzalka, false);
}





