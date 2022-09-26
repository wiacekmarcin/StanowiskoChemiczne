#include "urzadzenia.h"
#include "common.h"
#include <math.h>
#include "version.h"

#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>

Urzadzenia::Urzadzenia(Ustawienia & ustawiania_, QObject *parent)
    : QObject{parent},
      m_NI_Cards(),
      m_serialDev(ustawiania_, this),
      m_ustawienia(ustawiania_)

{
    m_logFile = new QFile;
    m_logFile->setFileName(QString("log_%1.txt").arg(QDateTime::currentDateTime().toString().replace(' ', '_').replace(':', '_').replace('.', '_')));
    m_logFile->open(QIODevice::Append | QIODevice::Text);

    prevInputs = 0x00;

    connect(&m_NI_Cards, &NICards::digitalReadValueChanged,    this,   &Urzadzenia::ni_digitalReadAllValueChanged, Qt::DirectConnection);
    connect(&m_NI_Cards, &NICards::analogValueChanged,         this,   &Urzadzenia::ni_analogValueChanged,      Qt::DirectConnection);

    connect(&m_NI_Cards, &NICards::error,                      this,   &Urzadzenia::ni_error,              Qt::QueuedConnection);
    connect(&m_NI_Cards, &NICards::debug,                      this,   &Urzadzenia::ni_debug,              Qt::QueuedConnection);
    connect(&m_NI_Cards, &NICards::usb6210,                    this,   &Urzadzenia::usb6210,               Qt::QueuedConnection);
    connect(&m_NI_Cards, &NICards::usb6501,                    this,   &Urzadzenia::ni_usb6501,            Qt::QueuedConnection);
    connect(&m_NI_Cards, &NICards::usb6501,                    this,   &Urzadzenia::usb6501,               Qt::QueuedConnection);

    connect(&m_serialDev, &SerialDevice::dozownikConfigured,    this, &Urzadzenia::ds_dozownikConfigured, Qt::QueuedConnection);
    connect(&m_serialDev, &SerialDevice::setCykleDone,          this, &Urzadzenia::setCykleDone,          Qt::QueuedConnection);
    connect(&m_serialDev, &SerialDevice::setStepsDone,          this, &Urzadzenia::setStepsDone,          Qt::QueuedConnection);
    connect(&m_serialDev, &SerialDevice::checkPositionHomeDone, this, &Urzadzenia::checkPositionHomeDone, Qt::QueuedConnection);

    connect(&m_serialDev, &SerialDevice::debug,                 this, &Urzadzenia::ds_debug,              Qt::QueuedConnection);
    connect(&m_serialDev, &SerialDevice::error,                 this, &Urzadzenia::ds_error,              Qt::QueuedConnection);

    connect(this, &Urzadzenia::log, this, &Urzadzenia::logSlot, Qt::QueuedConnection);

    logSlot(QString("%1.%2 - %3 %4").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(QString(__DATE__), QString(__TIME__)));}

Urzadzenia::~Urzadzenia()
{
    m_logFile->close();
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
    static short index = 0;
    double v0 = val0 / m_ustawienia.getRatio((analogIn)0);
    double v1 = val1 / m_ustawienia.getRatio((analogIn)1);
    double v2 = val2 / m_ustawienia.getRatio((analogIn)2);
    double v3 = val3 / m_ustawienia.getRatio((analogIn)3);
    double v4 = val4 / m_ustawienia.getRatio((analogIn)4);
    double v5 = val5 / m_ustawienia.getRatio((analogIn)5);
    double v6 = val6 / m_ustawienia.getRatio((analogIn)6);
    double v7 = val0 / m_ustawienia.getRatio((analogIn)7);
    emit analogValueChanged(v0, v1, v2, v3, v4, v5, v6, v7);
    if (++index == 10) {
        urz_debug(QString("2. Odczytano %1,%2,%3,%4,%5,%6,%7,%8").arg(v0).arg(v1).arg(v2).arg(v3).arg(v4).arg(v5).arg(v6).arg(v7));
        index = 0;
    }
}

void Urzadzenia::ni_digitalReadAllValueChanged(uint16_t vals)
{
    //qInfo() << __FILE__ << ":" << __LINE__ << "vals=" << vals;
    emit digitalReadAllValueChanged(vals);
    prevInputs = m_inputs;
    m_inputs = vals;

    uint16_t mask = 0x1;
    for (short i = 0; i < Ustawienia::maxCzujekCyfrIn; ++i) {
        //qInfo() << "mask=" << mask << " val=" << ((m_inputs & mask) == mask);
        emit digitalReadValueChanged((digitalIn)mask, (m_inputs & mask) == mask);
        mask <<= 1;
    }
}

void Urzadzenia::ni_usb6501(bool open, bool conf)
{
    m_digCardsOk = open && conf;
    if (m_digCardsOk) {
        readInputs();
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

void Urzadzenia::setDozownikHome(uint8_t dozownik)
{
    m_serialDev.setPositionHome(dozownik);
}

void Urzadzenia::setPosition(uint8_t dozownik, uint32_t pos)
{
    m_serialDev.setPosition(dozownik, pos);
}

void Urzadzenia::setSteps(uint8_t dozownik, uint64_t pos)
{
    m_serialDev.setSteps(dozownik, pos);
}

void Urzadzenia::digitalWriteAll(uint16_t vals)
{
#if !SYMULATOR
    m_NI_Cards.digitalWriteAll(vals);
    emit digitalWriteValueChanged(m_NI_Cards.getDigitalWrite());
#else
    (void)vals;
#endif
}

void Urzadzenia::digitalWrite(digitalOut mask, bool on)
{
#if !SYMULATOR
    m_NI_Cards.digitalWrite(mask, on);
    emit digitalWriteValueChanged(m_NI_Cards.getDigitalWrite());
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
    emit log(QString("%1.ERROR.NICARDS - %2").arg(QTime::currentTime().toString("HH:mm:ss:zzz"), s));
}

void Urzadzenia::ni_debug(const QString &d)
{
    emit log(QString("%1.DEBUG.NICARDS - %2").arg(QTime::currentTime().toString("HH:mm:ss:zzz"), d));
}

void Urzadzenia::ds_error(const QString &s)
{
    emit log(QString("%1.ERROR.DOZOWNIK - %2").arg(QTime::currentTime().toString("HH:mm:ss:zzz"), s));
}

void Urzadzenia::ds_debug(const QString &d)
{
    emit log(QString("%1.DEBUG.DOZOWNIK - %2").arg(QTime::currentTime().toString("HH:mm:ss:zzz"), d));
}

void Urzadzenia::urz_error(const QString &s)
{
    emit log(QString("%1.ERROR.URZADZENIE - %2").arg(QTime::currentTime().toString("HH:mm:ss:zzz"), s));
}

void Urzadzenia::urz_debug(const QString &d)
{
    emit log(QString("%1.DEBUG.URZADZENIE - %2").arg(QTime::currentTime().toString("HH:mm:ss:zzz"), d));
}

void Urzadzenia::gui_error(const QString &s)
{
    emit log(QString("%1.ERROR.GUI - %2").arg(QTime::currentTime().toString("HH:mm:ss:zzz"), s));
}

void Urzadzenia::gui_debug(const QString &d)
{
    emit log(QString("%1.DEBUG.GUI - %2").arg(QTime::currentTime().toString("HH:mm:ss:zzz"), d));
}

void Urzadzenia::test_error(const QString &s)
{
    emit log(QString("%1.ERROR.TEST - %2").arg(QTime::currentTime().toString("HH:mm:ss:zzz"), s));
}

void Urzadzenia::test_debug(const QString &d)
{
    emit log(QString("%1.DEBUG.TEST - %2").arg(QTime::currentTime().toString("HH:mm:ss:zzz"), d));
}


void Urzadzenia::logSlot(const QString &msg)
{
    mutex.lock();
    QTextStream out(m_logFile);
    out.setCodec("UTF-8");
    if (m_logFile != 0) {
        out << msg << "\n";
    }
    out.flush();
    mutex.unlock();
}

void Urzadzenia::readInputs()
{
    //qInfo() << __FILE__ << ":" << __LINE__ ;
    m_inputs = m_NI_Cards.getDigitalRead();
    emit digitalReadAllValueChanged(m_inputs);
    //qInfo() << "m_inputs=" << m_inputs;
    uint16_t mask = 0x1;
    for (short i = 0; i < Ustawienia::maxCzujekCyfrIn; ++i) {
        //qInfo() << "mask=" << mask << " val=" << ((m_inputs & mask) == mask);
        emit digitalReadValueChanged((digitalIn)mask, (m_inputs & mask) == mask);
        mask <<= 1;
    }
}

void Urzadzenia::ds_dozownikConfigured(bool open, bool conf)
{
    m_dozownikOk = open && conf;
    emit dozownik(open, conf);
}

void Urzadzenia::triggerKameryPrepare()
{
    digitalWrite(o_trigger, false);
}

void Urzadzenia::triggerKameryOn()
{
    digitalWrite(o_trigger, true);
}

void Urzadzenia::triggerKameryOff()
{
    digitalWrite(o_trigger, true);
}

void Urzadzenia::runIskraElektryczna()
{

    digitalWrite(o_hv_onoff, true);
    digitalWrite(o_hv_bezpiecznik, false);
    digitalWrite(o_hv_iskra, false);

    triggerKameryPrepare(); //podnosimy zbocze (przygotowujemy sie do "triggera" wyzwalanego zboczem opadajacym
    QTimer::singleShot(m_ustawienia.getDelayTimeIskraElektrycznaHV(), this, &Urzadzenia::runIskraElektryczna1);
}

void Urzadzenia::runIskraElektryczna1()
{
    triggerKameryOn();
    digitalWrite(o_hv_onoff, false);
    digitalWrite(o_hv_bezpiecznik, false);
    digitalWrite(o_hv_iskra, true);

    QTimer::singleShot(m_ustawienia.getDelayTimeIskraElektrycznaIskra(), this, &Urzadzenia::runIskraElektryczna2);
}

void Urzadzenia::runIskraElektryczna2()
{
    triggerKameryOff(); //zostawiamy zbocze wysokie
    digitalWrite(o_hv_onoff, false);
    digitalWrite(o_hv_bezpiecznik, true);
    digitalWrite(o_hv_iskra, false);
}

void Urzadzenia::runIskraMechaniczna()
{
    triggerKameryPrepare();
    QTimer::singleShot(100, this, &Urzadzenia::runIskraMechaniczna1);
}

void Urzadzenia::runIskraMechaniczna1()
{
    triggerKameryOn();
    digitalWrite(o_mech_iskra, true);
    QTimer::singleShot(m_ustawienia.getRunTimeIskraMechaniczna(), this, &Urzadzenia::runIskraMechaniczna2);
}

void Urzadzenia::runIskraMechaniczna2()
{
    triggerKameryOff();
    digitalWrite(o_mech_iskra, false);
}

void Urzadzenia::runPlomien()
{
    triggerKameryPrepare();
    QTimer::singleShot(100, this, &Urzadzenia::runPlomien1);
}


void Urzadzenia::runPlomien1()
{
    QTimer::singleShot(m_ustawienia.getDelayTimeTriggerPlomien(), this, &Urzadzenia::triggerKameryOn);
    digitalWrite(o_grzalka, true);
    QTimer::singleShot(m_ustawienia.getRunTimePlomien(), this, &Urzadzenia::runPlomien2);

}

void Urzadzenia::runPlomien2()
{
    triggerKameryOff();
    digitalWrite(o_grzalka, false);
}






