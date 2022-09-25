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
    actTask = QList<Task>();
    actTask.push_back(CONNECT);
    runWorker = true;
}

SerialWorker::~SerialWorker()
{
    mutex.lock();
    runWorker = false;
    actTask.clear();
    newTask.wakeOne();
    mutex.unlock();
}

void SerialWorker::command(Task curr)
{
    const QMutexLocker locker(&mutex);

    actTask.push_back(curr);
    newTask.wakeOne();
}

void  SerialWorker::commandNoRepeat(Task task)
{
    const QMutexLocker locker(&mutex);
    if (actTask.size() == 0) {
        actTask.push_back(task);
        newTask.wakeOne();
        return;
    }

    if (task != actTask.back()) {
        actTask.push_back(task);
        newTask.wakeOne();
        return;
    }
}

void SerialWorker::setStop()
{
    mutex.lock();
    runWorker = false;
    actTask.clear();
    
    newTask.wakeOne();
    mutex.unlock();
    wait();
}

void SerialWorker::run()
{
    
    short nrTrying = 0;
    bool quit = false;
    short zadanie;
    do {
        mutex.lock();
        if (actTask.size() == 0) {
            newTask.wait(&mutex);
        }
        quit = !runWorker;
        if (actTask.size() > 0) {
            zadanie = actTask.front();
            actTask.pop_front();
        }
        mutex.unlock();
        if (quit)
            zadanie = IDLE;

        switch(zadanie) {
        case CONNECT:
            sd->connectToSerialJob();
            if (!sd->getConnected()) {
                QThread::currentThread()->sleep(1);
                command(CONNECT);
            } else if (!sd->m_configured) {
                if (nrTrying++ > 5) {
                    nrTrying = 0;
                    zadanie = IDLE;
                    sd->closeDevice();
                    sd->setConnected(false);
                    command(CONNECT);
                } else {
                    command(CONFIGURE);
                }
            } else if (sd->getConnected() && sd->m_configured) {
                nrTrying = 0;
            }
            break;

        case CONFIGURE:
            sd->m_configured = sd->configureDeviceJob();
            break;

        case SET_PARAMS:
            sd->setParamsJob();
            break;

        case SET_HOME:
            sd->setHomeJob();
            break;

        case SET_CYCLE:
            sd->setCykleJob();
            break;

        case SET_POSITION:
            sd->setPosJob();
            break;

        case SET_STEPS:
            sd->setStepsJob();
            break;

        case SET_ECHO2:
            sd->setCheckHomeJob();
            break;

        case SET_ECHO:
            sd->setEchoJob();
            break;
        
        case CLOSE_DEVICE:
            sd->setCloseDeviceJob();
            break;
        default:
            break;
        }
    } while (!quit);
    
   qDebug() << "return SerialWorker::run()";
   this->quit();
}
/**************************************************************************/
#define setReverse(N) m_reverse##N = m_ustawienia.getReverse_##N()

SerialDevice::SerialDevice(Ustawienia & u, QObject *parent)
    : QObject(parent),
      m_portName(""), m_portNr(-1),
      m_connected(false), m_configured(false),
      m_ustawienia(u), m_worker(this),
     checkTimer(this)
{
    setReverse(1);
    setReverse(2);
    setReverse(3);
    setReverse(4);
    setReverse(5);

    m_maxImp = m_ustawienia.getMaxImp();
    m_timeImp = m_ustawienia.getImpTime();

    homePositionMask = 0;
    connect(&checkTimer, &QTimer::timeout, this, &SerialDevice::updateDevice);
    checkTimer.setInterval(1000);
    checkTimer.start();
}

SerialDevice::~SerialDevice()
{

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

void SerialDevice::updateDevice()
{
    if (getConnected())
        m_worker.commandNoRepeat(SerialWorker::SET_ECHO);
}

void SerialDevice::setMaxImp(uint32_t imp)
{
    m_maxImp = imp;
    m_ustawienia.setMaxImp(imp);
    if (getConnected())
        m_worker.command(SerialWorker::SET_PARAMS);
    else
        emit setParamsDone(false);
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
    qDebug() << "r1=" << r1 << " r2=" << r2 << " r3=" << r3 << " r4=" << r4 << " r5=" << r5 << " mImp=" << mImp << "mtime" << impT << "\n";
    m_ustawienia.setMaxImp(m_maxImp);
    m_ustawienia.setImpTime(m_timeImp);
    m_ustawienia.setReverse_1(r1);
    m_ustawienia.setReverse_2(r2);
    m_ustawienia.setReverse_3(r3);
    m_ustawienia.setReverse_4(r4);
    m_ustawienia.setReverse_5(r5);
    if (getConnected())
        m_worker.command(SerialWorker::SET_PARAMS);
    else
        emit setParamsDone(false);
}

void SerialDevice::setImpTime(uint16_t impT)
{
    m_timeImp = impT;
    m_ustawienia.setImpTime(m_timeImp);
    if (getConnected())
        m_worker.command(SerialWorker::SET_PARAMS);
    else
        emit setParamsDone(false);
}

void SerialDevice::setPositionHome(uint8_t nrDoz)
{
    dozownikNr = nrDoz;
    if (getConnected())
        m_worker.command(SerialWorker::SET_HOME);
    else
        emit setPositionHomeDone(false);
}

void SerialDevice::setSteps(uint8_t nrDoz, uint64_t pos)
{
    dozownikNr = nrDoz;
    val2 = pos;
    if (getConnected())
        m_worker.command(SerialWorker::SET_STEPS);
    else
        emit setStepsDone(false);
}

void SerialDevice::setReset()
{
    m_worker.command(SerialWorker::RESET);
}

void SerialDevice::setCykle(uint8_t nrDoz, uint32_t nrCyckli)
{
    dozownikNr = nrDoz;
    val1 = nrCyckli;
    if (getConnected())
        m_worker.command(SerialWorker::SET_CYCLE);
    else
        emit setCykleDone(false);
}

void SerialDevice::setPosition(uint8_t nrDoz, uint32_t steps)
{
    dozownikNr = nrDoz;
    val1 = steps;
    if (getConnected())
        m_worker.command(SerialWorker::SET_POSITION);
    else
        emit setPositionDone(false);

}

void SerialDevice::checkPositionHome()
{
    if (getConnected())
        m_worker.command(SerialWorker::SET_ECHO2);
    else
        emit checkPositionHomeDone(false, false, false, false, false, false);
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
        msg.setTimeoutReply(false);
        return msg;
    }
    QByteArray responseData((const char*)recvBuffor, rc);
    emit debug(QString("read [%1]").arg(responseData.toHex().constData()));
    return parseMessage(responseData);
}

#define REPLYTIMEOUT(s) do { if ((s) == SerialMessage::TIMEOUT_READ_REPLY || (s) == SerialMessage::TIMEOUT_WRITE_REPLY) {\
                closeDevice(); \
                openDevice(); \
                return false; \
                } } while(false)

bool SerialDevice::configureDeviceJob()
{
    homePositionMask = 0;
    emit debug(QString("Konfiguracja %1%2%3%4%5 %6 %7").arg(m_reverse1).arg(m_reverse2).arg(m_reverse3).arg(m_reverse4).arg(m_reverse5).
               arg(m_maxImp).arg(m_timeImp));
;
    auto s = write(SerialMessage::welcomeMsg(), 100, 100).getParseReply();
    REPLYTIMEOUT(s);

    if (s != SerialMessage::WELCOME_REPLY)
        return false;

    s = write(SerialMessage::setReset(), 100, 3000).getParseReply();
    REPLYTIMEOUT(s);
    if (s != SerialMessage::RESET_REPLY)
        return false;

    QThread::currentThread()->sleep(2);

    s = write(SerialMessage::setSettingsMsg(m_reverse1, m_reverse2, m_reverse3, m_reverse4, m_reverse5, m_maxImp, m_timeImp),
              100, 100).getParseReply();
    REPLYTIMEOUT(s);
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
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
        return;
    }

    s = write(SerialMessage::setReset(), 100, 3000).getParseReply();

    if (s != SerialMessage::RESET_REPLY) {
        m_configured = false;
        emit setParamsDone(false);
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
        return;
    }

    QThread::currentThread()->sleep(2);

    s = write(SerialMessage::setSettingsMsg(m_reverse1, m_reverse2, m_reverse3, m_reverse4, m_reverse5, m_maxImp, m_timeImp),
              100, 100).getParseReply();
    
    if (s != SerialMessage::SETPARAMS_REPLY) {
        m_configured = false;
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
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
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
        return;
    }

    s = write(SerialMessage::setReset(), 100, 3000).getParseReply();

    if (s != SerialMessage::RESET_REPLY) {
        m_configured = false;
        emit resetDone(false);
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
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
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
        return;
    }

    s = write(SerialMessage::setSettingsMsg(m_reverse1, m_reverse2, m_reverse3, m_reverse4, m_reverse5, m_maxImp, m_timeImp),
              100, 100).getParseReply();

    if (s != SerialMessage::SETPARAMS_REPLY) {
        emit setCykleDone(false);
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
        return;
    }

    s = write(SerialMessage::setPositionHome(dozownikNr), 100, 60000).getParseReply();

    if (s != SerialMessage::MOVEHOME_REPLY) {
        emit setCykleDone(false);
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
        return;
    }

    for (uint32_t n = 0; n < val1; ++n) {
        s = write(SerialMessage::setPosition(dozownikNr, m_maxImp), 100, 60000).getParseReply();
        if (s != SerialMessage::POSITION_REPLY) {
            emit setCykleDone(false);
            closeDevice();
            m_worker.command(SerialWorker::CONNECT);
            return;
        }

        s = write(SerialMessage::setPositionHome(dozownikNr), 100, 60000).getParseReply();

        if (s != SerialMessage::MOVEHOME_REPLY) {
            emit setCykleDone(false);
            closeDevice();
            m_worker.command(SerialWorker::CONNECT);
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
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
        return;
    }

    s = write(SerialMessage::setSettingsMsg(m_reverse1, m_reverse2, m_reverse3, m_reverse4, m_reverse5, m_maxImp, m_timeImp),
              100, 100).getParseReply();
    if (s != SerialMessage::SETPARAMS_REPLY) {
        emit setStepsDone(false);
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
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
            closeDevice();
            m_worker.command(SerialWorker::CONNECT);
            return;
        }

        s = write(SerialMessage::setPositionHome(dozownikNr), 100, 60000).getParseReply();
        if (s != SerialMessage::MOVEHOME_REPLY) {
            emit setStepsDone(false);
            closeDevice();
            m_worker.command(SerialWorker::CONNECT);
            return;
        }
    }
    emit setStepsDone(true);
}

void SerialDevice::setCheckHomeJob()
{
    emit debug(QString("Sprawdzam pozycje startowe"));

    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();

    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit checkPositionHomeDone(false, false, false, false, false, false);
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
        return;
    }

    auto s2 = write(SerialMessage::echoMsg2(), 100, 200);
    if (s2.getParseReply() != SerialMessage::ECHO_REPLY2) {
        emit checkPositionHomeDone(false, false, false, false, false, false);
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
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

void SerialDevice::setEchoJob()
{
    emit debug(QString("Sprawdzam kontroler"));

    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();

    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
        return;
    }
}

void SerialDevice::setCloseDeviceJob()
{
    emit debug(QString("Zamykam urzadzenie"));
    closeDevice();
}

void SerialDevice::setHomeJob()
{

    emit debug(QString("Ustaw pozycje startowa dla dozownika %1").arg(dozownikNr + 1));

    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit setPositionHomeDone(false);
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
        return;
    }

    s = write(SerialMessage::setPositionHome(dozownikNr), 100, 60000).getParseReply();
    if (s != SerialMessage::MOVEHOME_REPLY) {
        emit setPositionHomeDone(false);
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
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
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
        return;
    }

    s = write(SerialMessage::setPosition(dozownikNr, val1), 100, 60000).getParseReply();
    if (s != SerialMessage::POSITION_REPLY) {
        emit setPositionDone(false);
        closeDevice();
        m_worker.command(SerialWorker::CONNECT);
        return;
    }

    emit setPositionDone(true);
}

bool SerialDevice::openDevice()
{
    emit debug(QString("Otwieram urządzenia %1"));
    char mode[]={'8','O','1',0};
    if (RS232_OpenComport(m_portNr, 115200, mode, 0)) {
        setConnected(false);
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
    setConnected(false);
    m_configured = false;
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
    setConnected(openDevice());
}

void SerialDevice::setConnected(bool conn)
{
    mutex.lock();
    m_connected = conn;
    mutex.unlock();
}

bool SerialDevice::getConnected()
{
    bool ret;
    mutex.lock();
    ret = m_connected;
    mutex.unlock();
    return ret;
}

