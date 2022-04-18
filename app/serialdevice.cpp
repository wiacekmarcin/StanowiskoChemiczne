#include "serialdevice.h"
#include <QTime>
#include <QThread>
#include <QSerialPortInfo>
#include "rs232.h"
#include <QDebug>

SerialWorker::SerialWorker(SerialDevice * device):
    QThread(nullptr),
    sd(device)
{
    actTask = IDLE;
    runWorker = true;
}

SerialWorker::~SerialWorker()
{
    mutex.lock();
    runWorker = false;
    newTask.wakeOne();
    mutex.unlock();
    QThread::currentThread()->wait();
}

void SerialWorker::command(Task curr)
{
    const QMutexLocker locker(&mutex);

    actTask = curr;
    newTask.wakeOne();
}

void SerialWorker::setStop()
{
    mutex.lock();
    runWorker = false;
    mutex.unlock();
}

void SerialWorker::run()
{
    mutex.lock();
    short zadanie = actTask;
    mutex.unlock();
    short nrTrying = 0;
    bool quit = false;
    do {
        switch(zadanie) {
        case IDLE:
            if (!sd->m_connected) {
                zadanie = CONNECT;
                QThread::currentThread()->sleep(10);
            } else if (!sd->m_configured) {
                zadanie = CONFIGURE;
                QThread::currentThread()->sleep(5);
            } else {
                qInfo() << "Going to sleep";
                mutex.lock();
                newTask.wait(&mutex);
                zadanie = actTask;
                mutex.unlock();
            }
            break;
        case CONNECT:
            sd->connectToSerialJob();
            if (!sd->m_connected) {
                zadanie = IDLE;
            } else if (!sd->m_configured) {
                if (nrTrying++ > 5) {
                    nrTrying = 0;
                    zadanie = IDLE;
                    sd->closeDevice();
                    sd->m_connected = false;
                } else {
                    zadanie = CONFIGURE;
                }
            } else if (sd->m_connected && sd->m_configured) {
                nrTrying = 0;
                zadanie = IDLE;
            }
            break;

        case CONFIGURE:
            sd->m_configured = sd->configureDeviceJob();
            zadanie = IDLE;
            break;

        case SET_PARAMS:
            sd->setParamsJob();
            zadanie = IDLE;
            break;

        case SET_HOME:
            sd->setHomeJob();
            zadanie = IDLE;
            break;

        case SET_CYCLE:
            sd->setCykleJob();
            zadanie = IDLE;
            break;

        case SET_POSITION:
            sd->setPosJob();
            zadanie = IDLE;
            break;

        case SET_STEPS:
            sd->setStepsJob();
            zadanie = IDLE;
            break;

        case SET_ECHO2:
            sd->setEchoJob();
            zadanie = IDLE;
            break;
        default:
            zadanie = IDLE;
            break;
        }
        mutex.lock();
        quit = !runWorker;
        mutex.unlock();
    } while (!quit);
}
/**************************************************************************/
#define setReverse(N) m_reverse##N = m_ustawienia.getReverse_##N()

SerialDevice::SerialDevice(Ustawienia & u, QObject *parent)
    : QObject(parent),
      m_portName(""), m_portNr(-1),
      m_connected(false), m_configured(false),
      m_ustawienia(u), m_worker(this)
{
    setReverse(1);
    setReverse(2);
    setReverse(3);
    setReverse(4);
    setReverse(5);

    m_maxImp = m_ustawienia.getMaxImp();
    m_timeImp = m_ustawienia.getImpTime();

    homePositionMask = 0;
    m_worker.command(SerialWorker::IDLE);
}

SerialDevice::~SerialDevice()
{
    m_worker.setStop();
}

void SerialDevice::setThread(QThread *trh)
{
    m_worker.moveToThread(trh);
    m_worker.start();
}

void SerialDevice::setStop()
{
    m_worker.setStop();
}

void SerialDevice::setMaxImp(uint32_t imp)
{
    m_maxImp = imp;
    m_ustawienia.setMaxImp(imp);
    m_worker.command(SerialWorker::SET_PARAMS);
}

void SerialDevice::setSettings(bool r1, bool r2, bool r3, bool r4, bool r5, uint32_t mImp, uint16_t impT)
{
    m_reverse1 = r1;
    m_reverse2 = r2;
    m_reverse3 = r3;
    m_reverse4 = r4;
    m_reverse5 = r5;

    m_maxImp = mImp;
    m_timeImp = impT;
    m_ustawienia.setMaxImp(m_maxImp);
    m_ustawienia.setImpTime(m_timeImp);
    m_ustawienia.setReverse_1(r1);
    m_ustawienia.setReverse_2(r2);
    m_ustawienia.setReverse_3(r3);
    m_ustawienia.setReverse_4(r4);
    m_ustawienia.setReverse_5(r5);
    m_worker.command(SerialWorker::SET_PARAMS);
}

void SerialDevice::setImpTime(uint16_t impT)
{
    m_timeImp = impT;
    m_ustawienia.setImpTime(m_timeImp);
    m_worker.command(SerialWorker::SET_PARAMS);
}

void SerialDevice::setPositionHome(uint8_t nrDoz)
{
    dozownikNr = nrDoz;
    m_worker.command(SerialWorker::SET_HOME);
}

void SerialDevice::setSteps(uint8_t nrDoz, uint64_t pos)
{
    dozownikNr = nrDoz;
    val2 = pos;
    m_worker.command(SerialWorker::SET_STEPS);
}

void SerialDevice::setReset()
{
    m_worker.command(SerialWorker::RESET);
}

void SerialDevice::setCykle(uint8_t nrDoz, uint32_t nrCyckli)
{
    dozownikNr = nrDoz;
    val1 = nrCyckli;
    m_worker.command(SerialWorker::SET_CYCLE);
}

void SerialDevice::setPosition(uint8_t nrDoz, uint32_t steps)
{
    dozownikNr = nrDoz;
    val1 = steps;
    m_worker.command(SerialWorker::SET_POSITION);
}

void SerialDevice::checkPositionHome()
{
    m_worker.command(SerialWorker::SET_ECHO2);
}


#define SET_GET_REVERSE_DEFINTION(N) \
bool SerialDevice::getReverse##N() const \
{ \
    return m_reverse##N; \
} \
void SerialDevice::setReverse##N(bool newReverse##N) \
{ \
    m_reverse##N = newReverse##N; \
    m_ustawienia.setReverse_##N(newReverse##N); \
    m_worker.command(SerialWorker::SET_PARAMS);\
}

SET_GET_REVERSE_DEFINTION(1)
SET_GET_REVERSE_DEFINTION(2)
SET_GET_REVERSE_DEFINTION(3)
SET_GET_REVERSE_DEFINTION(4)
SET_GET_REVERSE_DEFINTION(5)



SerialMessage SerialDevice::write(const QByteArray &currentRequest, int currentWaitWriteTimeout, int currentReadWaitTimeout)
{
    SerialMessage msg;
    msg.setInvalidReply();
    
    if (currentRequest.size() == 0)
        return msg;
    emit debug(QString("write [%1]").arg(currentRequest.toHex().constData()));
    RS232_SendBuf(m_portNr, (unsigned char*)currentRequest.constData(), currentRequest.size());
    QThread::currentThread()->msleep(currentWaitWriteTimeout);

    unsigned char recvBuffor[20];
    int readTimeout = currentReadWaitTimeout;
    int rc = 0;
    do {

        rc = RS232_PollComport(m_portNr, recvBuffor, 20);
        if (rc == 0) {
            readTimeout -= 50;
            QThread::currentThread()->msleep(50);
        }
    } while(rc == 0 && readTimeout > 0);
    if (readTimeout <= 0) {
        emit error(QString("Timeout"));
    }
    QByteArray responseData((const char*)recvBuffor, rc);
    emit debug(QString("read [%1]").arg(responseData.toHex().constData()));
    return parseMessage(responseData);
}

bool SerialDevice::configureDeviceJob()
{
    homePositionMask = 0;
    emit debug(QString("Konfiguracja %1%2%3%4%5 %6 %7").arg(m_reverse1).arg(m_reverse2).arg(m_reverse3).arg(m_reverse4).arg(m_reverse5).
               arg(m_maxImp).arg(m_timeImp));
;
    auto s = write(SerialMessage::welcomeMsg(), 100, 100).getParseReply();

    if (s != SerialMessage::WELCOME_REPLY)
        return false;

    s = write(SerialMessage::setReset(), 100, 3000).getParseReply();
    if (s != SerialMessage::RESET_REPLY)
        return false;

    QThread::currentThread()->sleep(2);

    s = write(SerialMessage::setSettingsMsg(m_reverse1, m_reverse2, m_reverse3, m_reverse4, m_reverse5, m_maxImp, m_timeImp),
              100, 100).getParseReply();

    if (s != SerialMessage::SETPARAMS_REPLY)
        return false;

    emit dozownikConfigured(true, true);
    return true;
}

void SerialDevice::setParamsJob()
{
    emit debug(QString("Konfiguracja %1%2%3%4%5 %6 %7").arg(m_reverse1).arg(m_reverse2).arg(m_reverse3).arg(m_reverse4).arg(m_reverse5).
               arg(m_maxImp).arg(m_timeImp));

    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit setParamsDone(false);
        return;
    }

    s = write(SerialMessage::setReset(), 100, 3000).getParseReply();
    if (s != SerialMessage::RESET_REPLY) {
        m_configured = false;
        emit setParamsDone(false);
        return;
    }

    QThread::currentThread()->sleep(2);

    s = write(SerialMessage::setSettingsMsg(m_reverse1, m_reverse2, m_reverse3, m_reverse4, m_reverse5, m_maxImp, m_timeImp),
              100, 100).getParseReply();

    if (s != SerialMessage::SETPARAMS_REPLY) {
        m_configured = false;
        emit setParamsDone(false);
        return;
    }

    emit setParamsDone(true);
}

void SerialDevice::setResetJob()
{
    emit debug(QString("Resetowanie sterownika silników"));
    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit resetDone(false);
        return;
    }

    s = write(SerialMessage::setReset(), 100, 3000).getParseReply();
    if (s != SerialMessage::RESET_REPLY) {
        m_configured = false;
        emit resetDone(false);
        return;
    }

    emit resetDone(true);
}

void SerialDevice::setCykleJob()
{
    emit debug(QString("Wstepne dozowania dozownika %1").arg(dozownikNr + 1));
    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit setCykleDone(false);
        return;
    }

    s = write(SerialMessage::setSettingsMsg(m_reverse1, m_reverse2, m_reverse3, m_reverse4, m_reverse5, m_maxImp, m_timeImp),
              100, 100).getParseReply();
    if (s != SerialMessage::SETPARAMS_REPLY) {
        emit setCykleDone(false);
        return;
    }

    s = write(SerialMessage::setPositionHome(dozownikNr), 100, 60000).getParseReply();
    if (s != SerialMessage::MOVEHOME_REPLY) {
        emit setCykleDone(false);
        return;
    }

    for (uint32_t n = 0; n < val1; ++n) {
        s = write(SerialMessage::setPosition(dozownikNr, m_maxImp), 100, 60000).getParseReply();
        if (s != SerialMessage::POSITION_REPLY) {
            emit setCykleDone(false);
            return;
        }

        s = write(SerialMessage::setPositionHome(dozownikNr), 100, 60000).getParseReply();
        if (s != SerialMessage::MOVEHOME_REPLY) {
            emit setCykleDone(false);
            return;
        }
    }

    emit setCykleDone(true);
}

void SerialDevice::setStepsJob()
{
    emit debug(QString("Ustawiam %1 kroków").arg(val2));
    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit setStepsDone(false);
        return;
    }

    s = write(SerialMessage::setSettingsMsg(m_reverse1, m_reverse2, m_reverse3, m_reverse4, m_reverse5, m_maxImp, m_timeImp),
              100, 100).getParseReply();
    if (s != SerialMessage::SETPARAMS_REPLY) {
        emit setCykleDone(false);
        return;
    }

    int64_t stepsAll = val2;
    uint32_t steps;
    while (stepsAll > 0) {
        if (stepsAll > m_maxImp && stepsAll <= 2* m_maxImp )
            steps = stepsAll / 2;
        else if (stepsAll <= m_maxImp)
            steps = stepsAll;
        else
            steps = m_maxImp;
        stepsAll -= steps;
        s = write(SerialMessage::setPosition(dozownikNr, steps), 100, 60000).getParseReply();
        if (s != SerialMessage::POSITION_REPLY) {
            emit setStepsDone(false);
            return;
        }

        s = write(SerialMessage::setPositionHome(dozownikNr), 100, 60000).getParseReply();
        if (s != SerialMessage::MOVEHOME_REPLY) {
            emit setStepsDone(false);
            return;
        }
    }
    emit setStepsDone(true);
}

void SerialDevice::setEchoJob()
{
    emit debug(QString("Sprawdzam pozycje startowe"));

    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit checkPositionHomeDone(false, false, false, false, false, false);
        return;
    }

    auto s2 = write(SerialMessage::echoMsg2(), 100, 200);

    if (s2.getParseReply() != SerialMessage::ECHO_REPLY2) {
        emit checkPositionHomeDone(false, false, false, false, false, false);
        return;
    }

    homePositionMask = ~s2.getHomePosition();

    emit checkPositionHomeDone(true, (homePositionMask & 0x1) == 0x1,
                                   (homePositionMask & 0x2) == 0x2,
                                   (homePositionMask & 0x4) == 0x4,
                                   (homePositionMask & 0x8) == 0x8,
                                   (homePositionMask & 0x10) == 0x10
                             );
}

void SerialDevice::setHomeJob()
{

    emit debug(QString("Ustaw pozycje startowa dla dozownika %1").arg(dozownikNr + 1));

    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit setPositionHomeDone(false);
        return;
    }

    s = write(SerialMessage::setPositionHome(dozownikNr), 100, 60000).getParseReply();
    if (s != SerialMessage::MOVEHOME_REPLY) {
        emit setPositionHomeDone(false);
        return;
    }

    emit setPositionHomeDone(true);
}

void SerialDevice::setPosJob()
{
    emit debug(QString("Ustaw pozycje %1").arg(val1));

    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit setPositionDone(false);
        return;
    }

    s = write(SerialMessage::setPosition(dozownikNr, val1), 100, 60000).getParseReply();
    if (s != SerialMessage::POSITION_REPLY) {
        emit setPositionDone(false);
        return;
    }

    emit setPositionDone(true);
}

bool SerialDevice::openDevice()
{
    emit debug(QString("Otwieram urządzenia %1"));
    char mode[]={'8','O','1',0};
    if (RS232_OpenComport(m_portNr, 115200, mode, 0)) {
        m_connected = false;
        return false;
    }

    RS232_flushRXTX(m_portNr);

    const unsigned char startBuf[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int rs = RS232_SendBuf(m_portNr, (unsigned char*)startBuf, 16);

    if (rs <= 0) {
        RS232_CloseComport(m_portNr);
        return false;
    }
    QThread::currentThread()->msleep(200);
    unsigned char recvBuf[100];
    int recv = RS232_PollComport(m_portNr, recvBuf, 100);
    (void)recv;

    return true;
}

SerialMessage SerialDevice::parseMessage(const QByteArray &reply)
{
    SerialMessage msg;
    msg.parseCommand(reply);
    return msg;
}

void SerialDevice::closeDevice()
{
    RS232_CloseComport(m_portNr);
    m_connected = false;
}

void SerialDevice::connectToSerialJob()
{
    QString description;
    QString manufacturer;
    QString serialNumber;

    QString systemLocation = "";
    bool findDevice = false;
    m_configured = false;
    emit debug(QString("Szukam urządzenia"));
    emit dozownikConfigured(false, false);

    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        emit debug(QString("Znaleziono : DESC ") + description + QString(" MENU ") + manufacturer + QString(" SERIAL") + serialNumber);

        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
            auto vendorId = serialPortInfo.vendorIdentifier();
            auto productId = serialPortInfo.productIdentifier();
            emit debug(QString("Znaleziono kandydata"));
            if (vendorId == 6991 && productId == 37382 /* && serialNumber == serialNumberKontroler */) {
                m_portName = serialPortInfo.portName();
                systemLocation = serialPortInfo.systemLocation();
                emit debug(QString("Znaleziono urządzenie"));
                findDevice = true;
            }
        }
    }
    if (!findDevice) {
        return;
    }

    emit dozownikConfigured(true, false);
    m_portNr = -1;
    //GetComPortUsb(bufPortName,"1B4F","9206");
    m_portNr = RS232_GetPortnr(m_portName.toStdString().c_str());

    if (m_portNr == -1)
        return;
    m_connected = openDevice();
}

