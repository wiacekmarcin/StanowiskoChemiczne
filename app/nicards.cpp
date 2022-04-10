#include "nicards.h"


#if  !SYMULATOR
#include "NIDAQmx.h"
#endif

#include "ustawienia.h"


NICards::NICards(QObject *parent)
    : QThread{parent},
      anConf(false),
      digConf(false),
      readAnalString("USB6210/ai0, USB6210/ai1, USB6210/ai2, USB6210/ai3, USB6210/ai4, USB6210/ai5, USB6210/ai6"),
      writeDigString("USB6501/port0,USB6501/port1/Line0:2"),
      readDigString("USB6501/port2,USB6501/port1/Line4")
{
    m_quit = false;
    start();
    maskOutput = hv_bezpieczenstwa;
    prevInputs = 0;
}

NICards::~NICards()
{
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();
    wait();
}

void NICards::digitalWrite(uint16_t out, bool val)
{
    //qDebug("%s:%d", __FILE__,__LINE__);
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

void NICards::getDigitalInput()
{
    uint16_t val;
    mutex.lock();
    val = prevInputs;
    mutex.unlock();
    emit digitalReadChanged(val);
}

void NICards::run()
{
    unsigned short loopNr = 0;
    bool fDev = find();
    //resetDevice(true, true);
    //qDebug("%s:%d run = %p", __FILE__, __LINE__, QThread::currentThreadId());
    while (!m_quit) {

        if (loopNr == 10) {
            loopNr = 0;
            if (!fDev)
                fDev = find();
        }
        if (!fDev)
            continue;

        if (loopNr == 0) {
            if (!anConf)
                anConf = analog.configure(analogConfString);

            if (!digConf)
                digConf = digital.configure(readDigString, writeDigString);

            if (anConf && !analog.isConnected()) {
                resetDevice(true, false);
                anConf = false;
            }

            if (digConf && !digital.isConnected()) {
                resetDevice(false, true);
                digConf = false;
            }
        }

        if (anConf && analog.isConnected())  {
            readAnalog();
        }

        if (digConf && digital.isConnected())  {
            readDigital();
        }

        if (digConf && digital.isConnected())  {
            m_mutex.lock();
            writeDigital();
            m_mutex.unlock();
        }
        currentThread()->msleep(100);
        ++loopNr;
    }
}

//find cards
bool NICards::find() {
#if SYMULATOR
    return false;
    emit usb6210(false, false);
    emit usb6501(false, false);
#endif

    emit usb6210(false, false);
    emit usb6501(false, false);
    //qDebug("%s:%d find", __FILE__, __LINE__);
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
    bool ret = false;
    for (auto & name : names) {
        if (DAQmxFailed(DAQmxGetDevProductType(name.toStdString().c_str(), bufProduct, 128))) {
            continue;
        }
        int32 deviceid;
        if (DAQmxFailed(DAQmxGetDevProductCategory(name.toStdString().c_str(), &deviceid))) {
            continue;
        }
        uInt32 serialid;
        if (DAQmxFailed(DAQmxGetDevSerialNum(name.toStdString().c_str(), &serialid))) {
            continue;
        }
        emit debug(QString("Device %1 ProductType=%2 DeviceID=%3 SerialID=%4").arg(name).arg(bufProduct).
                   arg(deviceid).arg(serialid));

        if (!anConf && !analog.isConnected() && QString(bufProduct) == QString("USB-6210") &&
                                                deviceid == 14643 && serialid == 33770223) {
            //qDebug("Znalazlem!!!!");
            emit debug(QString("Znalazłem kartę analogową : %1").arg(name));
            analogDevice = name;
            analogConfString = QString(readAnalString).replace("USB6210", name);
            analogConfigure();
            ret = true;
        }

        if (!digConf && !digital.isConnected() && QString(bufProduct) == QString("USB-6501") &&
                                                deviceid == 14646 && serialid == 33665651) {
            emit debug(QString("Znalazłem kartę cyfrową : %1").arg(name));
            digitalDevice = name;
            digitalConfReadString = QString(readDigString).replace("USB6501", name);
            digitalConfWriteString = QString(writeDigString).replace("USB6501", name);
            digitalConfigure();
            ret = true;
        }
    }
    return ret;
}

void NICards::analogConfigure()
{
    //qDebug("Konfiguracja ANAL");
    anConf = analog.configure(analogConfString);
    emit debug(QString("Konfiguracja karty analogowej zakonczyła się : %1").arg(anConf ? "sukcesem" : "porażką"));
    emit usb6210(analog.isConnected(), anConf);
}

void NICards::digitalConfigure()
{
    //qDebug("Konfiguracja DIG");
    digConf = digital.configure(digitalConfReadString, digitalConfWriteString);
    emit debug(QString("Konfiguracja karty cyfrowej zakonczyła się : %1").arg(anConf ? "sukcesem" : "porażką"));
    emit usb6501(digital.isConnected(), digConf);
    //qDebug("%s:%d conn/conf %d %d | output = %04x", __FILE__, __LINE__, digital.isConnected(), digConf, ~hv_bezpieczenstwa) ;
    maskOutput = hv_bezpieczenstwa; //Stan niski to zalaczenie - na starcie załaczym bezpiecznik na iskrze elektrycznej
}

void NICards::resetDevice(bool analog, bool digital)
{
#if !SYMULATOR
    int32 errCode;
    if (analog) {
        emit debug(QString("Reset i testy karty analogowej"));
        if(DAQmxFailed(errCode=DAQmxResetDevice(analogDevice.toStdString().c_str()))) {
            emit debug(QString("Błąd podczas resetu karty analogowej %d").arg(errCode));
            anConf = false;
        } else if (DAQmxFailed(errCode=DAQmxSelfTestDevice(analogDevice.toStdString().c_str()))) {
            emit debug(QString("Błąd podczas testu karty analogowej %d").arg(errCode));
            anConf = false;
        }
    }
    if (digital) {
        emit debug(QString("Reset i testy karty cyfrowej"));
        if(DAQmxFailed(errCode=DAQmxResetDevice(digitalDevice.toStdString().c_str()))) {
            emit debug(QString("Błąd podczas resetu karty cyfrowej %1").arg(errCode));
            digConf = false;
        } else if (DAQmxFailed(errCode=DAQmxSelfTestDevice(digitalDevice.toStdString().c_str()))) {
            emit debug(QString("Błąd podczas testu karty cyfrowej %1").arg(errCode));
            digConf = false;
        }
    }
#else
    (void)analog;
    (void)digital;
#endif
}

void NICards::readAnalog()
{
    //qDebug("READ ANAL");
    float val0, val1, val2, val3, val4, val5, val6;
    if (!analog.readValue(val0, val1, val2, val3, val4, val5, val6)) {
        emit debug("Nie mogę odczytać analoga");
        emit usb6210(true, false);
        return;
    }
    emit analogValueChanged(val0, val1, val2, val3, val4, val5, val6);
}

void NICards::writeDigital()
{
    if (!digital.writeValue(maskOutput)) {
        emit usb6501(true, false);
    }
}

void NICards::readDigital()
{
    //qDebug("%s:%d:%p readDigital",__FILE__, __LINE__, QThread::currentThreadId());

    uint16_t val;
    if (!digital.readValue(val)) {
        emit usb6501(true, false);
        return;
    }
    if (prevInputs != val) {
        mutex.lock();
        //qDebug("%s:%d %04x", __FILE__,__LINE__, val);
        prevInputs = val;
        mutex.unlock();
        emit digitalReadChanged(val);
    }
}

