#include "nicards.h"
#include <QThread>
#include <QDebug>
#include <QElapsedTimer>
#if  !SYMULATOR
#include "NIDAQmx.h"
#endif

#include "ustawienia.h"


NICards::NICards(QObject *parent)
    : QObject(parent), QRunnable(),
      m_anConf(false), m_digConf(false),
      readAnalString("USB6210/ai0, USB6210/ai1, USB6210/ai2, USB6210/ai3, USB6210/ai4, USB6210/ai5, USB6210/ai6"),
      writeDigString("USB6501/port0,USB6501/port1/Line0:2"),
      readDigString("USB6501/port2,USB6501/port1/Line4")
{
    m_quit = false;
    maskOutput = o_hv_bezpiecznik;
    prevInputs = 0;
}

NICards::~NICards()
{
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();
}

void NICards::setThreads(QThread *thr1)
{
    this->moveToThread(thr1);
}

void NICards::setStop()
{
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();
}

void NICards::digitalWrite(digitalOut _out, bool val)
{
    unsigned int out = (unsigned int)_out;
    emit debug(QString("Zapisz %1 => %2").arg(out,16).arg(val));
    unsigned short i = 0;
    unsigned long mask = 0x1;
    m_mutex.lock();

    while ( i++ < Ustawienia::maxCzujekCyfrOut) {
        if ((out & mask) != 0) {

            if (val) {
                maskOutput |= mask;
            } else {
                maskOutput &= ~mask;
            }
        }
        mask <<= 1;
    }
    m_mutex.unlock();
}

void NICards::analogConfigure()
{
    m_anConf = analog.configure(analogConfString);
    emit debug(QString("Konfiguracja karty analogowej zakonczyła się : %1").arg(m_anConf ? "sukcesem" : "porażką"));
    emit usb6210(analog.isConnected(), m_anConf);
}

void NICards::digitalConfigure()
{
    m_digConf = digital.configure(digitalConfReadString, digitalConfWriteString);
    emit debug(QString("Konfiguracja karty cyfrowej zakonczyła się : %1").arg(m_anConf ? "sukcesem" : "porażką"));
    emit usb6501(digital.isConnected(), m_digConf);
    maskOutput = o_hv_bezpiecznik; //Stan niski to zalaczenie - na starcie załaczym bezpiecznik na iskrze elektrycznej
}

void NICards::readAnalog()
{
    float val0, val1, val2, val3, val4, val5, val6;
    if (!analog.readValue(val0, val1, val2, val3, val4, val5, val6)) {
        emit error(QString("Odczyt danych z karty analogowej nie powiódł się"));
        emit usb6210(true, false);
        return;
    }
    emit debug(QString("Odczytano %1,%2,%3,%4,%5,%6,%7").arg(val0).arg(val1).arg(val2).arg(val3).arg(val4).arg(val5).arg(val6));
    emit analogValueChanged(val0, val1, val2, val3, val4, val5, val6);
}

void NICards::writeDigital()
{
    emit debug(QString("Zapis do karty %1").arg(maskOutput, 0, 16));
    if (!digital.writeValue(maskOutput)) {
        emit error(QString("Zapis danych do karty cyfrowej nie powiódł się"));
        emit usb6501(true, false);
    }
}

void NICards::readDigital()
{
    uint16_t val;
    if (!digital.readValue(val)) {
        emit error(QString("Odczyt danych z karty cyfrowej nie powiódł się"));
        emit usb6501(true, false);
        return;
    }
    if (prevInputs != val) {
        emit debug(QString("Odczytano %1").arg(val, 0, 2));
        prevInputs = val;
        emit digitalReadValueChanged(val);
    }
}

//find cards
bool NICards::find() {
#if SYMULATOR
    emit usb6210(false, false);
    emit usb6501(false, false);
    return false;

#else

    emit usb6210(false, false);
    emit usb6501(false, false);

    int32		errCode;
    char buf[128];
    if(DAQmxFailed(errCode=DAQmxGetSysDevNames(buf, 128))) {
        DAQmxGetExtendedErrorInfo(buf, 128);
        emit error(QString::fromUtf8("Błąd podczas pobierania listy kart NI [%1]").arg(buf));
        return false;
    }

    QString allNames(buf);
    QStringList names = allNames.split(",");
    char bufProduct[128];
    bool ret1 = false, ret2 = false;
    for (auto & name : names) {
        if (DAQmxFailed(DAQmxGetDevProductType(name.toStdString().c_str(), bufProduct, 128))) {
            emit error(QString::fromUtf8("Nie mogłem odczytać productType dla %1").arg(name));
            continue;
        }
        int32 deviceid;
        if (DAQmxFailed(DAQmxGetDevProductCategory(name.toStdString().c_str(), &deviceid))) {
            emit error(QString::fromUtf8("Nie mogłem odczytać productCategory dla %1").arg(name));
            continue;
        }
        uInt32 serialid;
        if (DAQmxFailed(DAQmxGetDevSerialNum(name.toStdString().c_str(), &serialid))) {
            emit error(QString::fromUtf8("Nie mogłem odczytać serialNum dla %1").arg(name));
            continue;
        }
        emit debug(QString("Device %1 ProductType=%2 DeviceID=%3 SerialID=%4").arg(name).arg(bufProduct).
                   arg(deviceid).arg(serialid));

        if (!m_anConf && !analog.isConnected() && QString(bufProduct) == QString("USB-6210") &&
                                                deviceid == 14643 && serialid == 33770223) {
            emit debug(QString("Znalazłem kartę analogową : %1").arg(name));
            analogDevice = name;
            analogConfString = QString(readAnalString).replace("USB6210", name);
            analogConfigure();
            ret1 = true;
        }

        if (!m_digConf && !digital.isConnected() && QString(bufProduct) == QString("USB-6501") &&
                                                deviceid == 14646 && serialid == 33665651) {
            emit debug(QString("Znalazłem kartę cyfrową : %1").arg(name));
            digitalDevice = name;
            digitalConfReadString = QString(readDigString).replace("USB6501", name);
            digitalConfWriteString = QString(writeDigString).replace("USB6501", name);
            digitalConfigure();
            ret2 = true;
        }
    }
    if (!ret1) {
        emit error(QString("Nie znaleziono kart analogowych"));
    }

    if (!ret2) {
        emit error(QString("Nie znaleziono kart cyfrowych"));
    }
    return ret1 && ret2;
#endif
}

void NICards::resetDevice(bool analog, bool digital)
{
#if !SYMULATOR
    int32 errCode;
    if (analog) {
        emit debug(QString("Reset i testy karty analogowej"));
        if(DAQmxFailed(errCode=DAQmxResetDevice(analogDevice.toStdString().c_str()))) {
            emit error(QString("Błąd podczas resetu karty analogowej %d").arg(errCode));
            m_anConf = false;
        } else if (DAQmxFailed(errCode=DAQmxSelfTestDevice(analogDevice.toStdString().c_str()))) {
            emit error(QString("Błąd podczas testu karty analogowej %d").arg(errCode));
            m_anConf = false;
        }
    }
    if (digital) {
        emit debug(QString("Reset i testy karty cyfrowej"));
        if(DAQmxFailed(errCode=DAQmxResetDevice(digitalDevice.toStdString().c_str()))) {
            emit error(QString("Błąd podczas resetu karty cyfrowej %1").arg(errCode));
            m_digConf = false;
        } else if (DAQmxFailed(errCode=DAQmxSelfTestDevice(digitalDevice.toStdString().c_str()))) {
            emit error(QString("Błąd podczas testu karty cyfrowej %1").arg(errCode));
            m_digConf = false;
        }
    }
#else
    (void)analog;
    (void)digital;
#endif
}

void NICards::run()
{
    unsigned short loopNr1 = 10, loopNr2 = 10;
    bool fDev = find();
    bool runWorker;
    QElapsedTimer stoper;
    m_mutex.lock();
    runWorker = !m_quit;
    m_mutex.unlock();

    while (runWorker) {
        stoper.start();
        m_mutex.lock();
        runWorker = !m_quit;
        m_mutex.unlock();

        qInfo() << "Nicards:thread: " << QThread::currentThread()->objectName();
        if (!fDev) {
            fDev = find();
            QThread::currentThread()->sleep(10);
            continue;
        }

        if (!m_anConf) {
            m_anConf = analog.configure(analogConfString);
            if (!m_anConf && !analog.isConnected()) {
                if (--loopNr1 == 0) {
                    resetDevice(true, false);
                    loopNr1 = 10;
                }
                QThread::currentThread()->sleep(10);
            }
            if (m_anConf)
                loopNr1 = 10;
            continue;
        }

        if (!m_digConf) {
            m_digConf = m_digConf = digital.configure(readDigString, writeDigString);
            if (!m_digConf && !digital.isConnected()) {
                if (--loopNr2 == 0) {
                    resetDevice(false, true);
                    loopNr2 = 10;
                }
                QThread::currentThread()->sleep(10);
            }
            if (m_digConf)
                loopNr2 = 10;
            continue;
        }
        if (!analog.isConnected() || !digital.isConnected()) {
            QThread::currentThread()->sleep(10);
        }

        if (m_anConf && analog.isConnected())  {
            readAnalog();
        }

        if (m_digConf && digital.isConnected())  {
            readDigital();
        }

        if (m_digConf && digital.isConnected())  {
            m_mutex.lock();
            writeDigital();
            m_mutex.unlock();
        }
        int elapsed = stoper.elapsed();
        if (elapsed > 100)
            QThread::currentThread()->msleep(100);
        else
            QThread::currentThread()->msleep(100-elapsed);
    }
}


