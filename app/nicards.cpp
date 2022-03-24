#include "nicards.h"
#include "NIDAQmx.h"

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
    int32 errCode;
    //emit cards(false, false);
    find();
    while (!m_quit) {
        if (loopNr == 10)
            loopNr = 0;

        if (!anConf)
            anConf = analog.configure(analogConfString);

        if (!digConf)
            digConf = digital.configure(digitalConfString);

        if (anConf && !analog.isConnected()) {
            emit debug(QString("Reset i testy karty analogowej"));
            if(DAQmxFailed(errCode=DAQmxResetDevice(analogDevice.toStdString().c_str()))) {
                emit debug(QString("Błąd podczas resetu karty analogowej"));
                anConf = false;
                continue;
            }
            if(DAQmxFailed(errCode=DAQmxSelfTestDevice(analogDevice.toStdString().c_str()))) {
                emit debug(QString("Błąd podczas testu karty analogowej"));
                anConf = false;
                continue;
            }
        }

        if (digConf && !digital.isConnected()) {
            emit debug(QString("Reset i testy karty cyfrowej"));
            if(DAQmxFailed(errCode=DAQmxResetDevice(digitalDevice.toStdString().c_str()))) {
                emit debug(QString("Błąd podczas resetu karty cyfrowej"));
                digConf = false;
                continue;
            }
            if(DAQmxFailed(errCode=DAQmxSelfTestDevice(digitalDevice.toStdString().c_str()))) {
                emit debug(QString("Błąd podczas testu karty cyfrowej"));
                digConf = false;
                continue;
            }
        }
        ++loopNr;
    }
}

//find cards
void NICards::find() {
    int32		errCode;
    char buf[128];
#ifndef L_COMP
    if(DAQmxFailed(errCode=DAQmxGetSysDevNames(buf, 128))) {
        DAQmxGetExtendedErrorInfo(buf, 128);
        emit error(QString::fromUtf8("Błąd podczas pobierania listy kart NI [%1]").arg(buf));
        return;
    }

    QString allNames(buf);
    QStringList names = allNames.split(",");
    char bufProduct[128];
    unsigned deviceId;
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
            anConf = analog.configure(analogConfString);
            emit debug(QString("Konfiguracja karty analogowej zakonczyła się : %1").arg(anConf ? "sukcesem" : "porażką"));
            emit usb6210(anConf && analog.isConnected());
        }

        if (digConf && digital.isConnected() && QString(bufProduct) == QString("USB-6501") &&
            deviceid == 14646 && serialid == 33665651) {

        if (QString(buffer2) == QString("USB-6501") && deviceid == 14646 && serialid == 33665651) {
            qDebug("%d usbDio = %d", __LINE__,usbDio);
            if (!usbDio) {
                readDigString.replace("USB6501", name);
                writeDigString.replace("USB6501", name);
                usbDio = dio.configure(readDigString, writeDigString);
                qDebug("%s:%d configure %d", __FILE__,__LINE__,usbDio);
                emit usb6501(usbDio);
                setDigital(hv_onoff, false);
                setDigital(hv_bezpieczenstwa, true);
                setDigital(hw_iskra, false);
                setDigital(mech_iskra, false);
                setDigital(plomien, false);
                setDigital(pompa_prozniowa, false);
                setDigital(pompa_powietrza, false);
                setDigital(wentylator, false);
                setDigital(mieszadlo, false);
                setDigital(unknown, false);
                setDigital(trigger, false);
            }
        }
    }
}
#endif
}
