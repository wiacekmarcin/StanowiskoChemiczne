#include "nicards.h"
#include "NIDAQmx.h"

#include "ustawienia.h"

NICards::NICards(QObject *parent)
    : QThread{parent},
      anConf(false),
      digConf(false),
      readDigString("USB6501/port2,USB6501/port1/Line4"),
      writeDigString("USB6501/port0,USB6501/port1/Line0:2"),
      readAnalString("USB6210/ai0, USB6210/ai1, USB6210/ai2, USB6210/ai3, USB6210/ai4, USB6210/ai5, USB6210/ai6")
{

}

NICards::~NICards()
{
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}

void NICards::run()
{
    unsigned short loopNr = 0;
    find();
    resetDevice(true, true);
    while (!m_quit) {
        if (loopNr == 10)
            loopNr = 0;

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
            writeDigital();
        }
        //currentThread::::msleep(100);

        ++loopNr;
    }
}

//find cards
void NICards::find() {
    int32		errCode;
    char buf[128];
    if(DAQmxFailed(errCode=DAQmxGetSysDevNames(buf, 128))) {
        DAQmxGetExtendedErrorInfo(buf, 128);
        emit error(QString::fromUtf8("Błąd podczas pobierania listy kart NI [%1]").arg(buf));
        return;
    }

    QString allNames(buf);
    QStringList names = allNames.split(",");
    char bufProduct[128];
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

        if (anConf && analog.isConnected() && QString(bufProduct) == QString("USB-6210") &&
                                                deviceid == 14643 && serialid == 33770223) {
            emit debug(QString("Znalazłem kartę analogową : %1").arg(name));
            analogDevice = name;
            analogConfString = QString(readAnalString).replace("USB6210", name);
            analogConfigure();
        }

        if (digConf && digital.isConnected() && QString(bufProduct) == QString("USB-6501") &&
                                                deviceid == 14646 && serialid == 33665651) {
            emit debug(QString("Znalazłem kartę cyfrową : %1").arg(name));
            digitalDevice = name;
            digitalConfReadString = QString(readDigString).replace("USB6501", name);
            digitalConfWriteString = QString(writeDigString).replace("USB6501", name);
            digitalConfigure();
        }
    }
}

void NICards::analogConfigure()
{
    
    anConf = analog.configure(analogConfString);
    emit debug(QString("Konfiguracja karty analogowej zakonczyła się : %1").arg(anConf ? "sukcesem" : "porażką"));
    emit usb6210(anConf && analog.isConnected());
}

void NICards::digitalConfigure()
{
    digConf = digital.configure(digitalConfReadString, digitalConfWriteString);
    emit debug(QString("Konfiguracja karty cyfrowej zakonczyła się : %1").arg(anConf ? "sukcesem" : "porażką"));
    emit usb6501(digConf && digital.isConnected());
    maskOutput = ~hv_bezpieczenstwa; //Stan niski to zalaczenie - na starcie załaczym bezpiecznik na iskrze elektrycznej
}

void NICards::resetDevice(bool analog, bool digital)
{
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
}

void NICards::readAnalog()
{
    float val0, val1, val2, val3, val4, val5, val6;
    if (!analog.readValue(val0, val1, val2, val3, val4, val5, val6)) {
        emit debug("Nie mogę odczytać analoga");
        emit usb6210(false);
        return;
    }
    emit analogValueChanged(val0, val1, val2, val3, val4, val5, val6);
}

void NICards::writeDigital()
{
    if (!digital.writeValue(maskOutput)) {
        emit usb6501(false);
    }
}

void NICards::readDigital()
{
    uint16_t val;
    if (!digital.readValue(val)) {
        emit usb6501(false);
        return;
    }

    emit digitalRead(~val & 0x1ff);
}