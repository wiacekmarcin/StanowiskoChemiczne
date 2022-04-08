#include "serialdevice.h"
#include <QTime>
#include <QThread>
#include "rs232.h"


#define setReverse(N) reverse##N = ustawienia.getReverse_##N()

SerialDevice::SerialDevice(Ustawienia & u, QObject *parent)
    : m_connected(false), m_configured(false), my_thread(this, parent), ustawienia(u)
{
    
    setReverse(1);
    setReverse(2);
    setReverse(3);
    setReverse(4);
    setReverse(5);

    maxImp = ustawienia.getMaxImp();
    timeImp = ustawienia.getImpTime();
    qDebug("%s:%d",__FILE__,__LINE__);
    this->moveToThread(&my_thread);
    my_thread.start();
    qDebug("%s:%d",__FILE__,__LINE__);
    
    my_thread.command(SerialThread::IDLE);
}

SerialDevice::~SerialDevice()
{

}

void SerialDevice::setMaxImp(uint32_t imp)
{
    maxImp = imp;
    ustawienia.setMaxImp(imp);
    my_thread.command(SerialThread::SET_PARAMS);

}

void SerialDevice::setSettings(bool r1, bool r2, bool r3, bool r4, bool r5, uint32_t mImp, uint16_t impT)
{
    reverse1 = r1;
    reverse2 = r2;
    reverse3 = r3;
    reverse4 = r4;
    reverse5 = r5;

    maxImp = mImp;
    timeImp = impT;
    ustawienia.setMaxImp(maxImp);
    ustawienia.setImpTime(timeImp);
    ustawienia.setReverse_1(r1);
    ustawienia.setReverse_2(r2);
    ustawienia.setReverse_3(r3);
    ustawienia.setReverse_4(r4);
    ustawienia.setReverse_5(r5);
    my_thread.command(SerialThread::SET_PARAMS);
}

void SerialDevice::setImpTime(uint16_t impT)
{
    timeImp = impT;
    ustawienia.setImpTime(timeImp);
    my_thread.command(SerialThread::SET_PARAMS);
}

void SerialDevice::setPositionHome(uint8_t nrDoz)
{
    dozownikNr = nrDoz;
    my_thread.command(SerialThread::SET_HOME);
}

void SerialDevice::setPosition(uint8_t nrDoz, uint32_t pos)
{
    dozownikNr = nrDoz;
    val1 = pos;
    my_thread.command(SerialThread::SET_PARAMS);
}

void SerialDevice::setReset()
{
    my_thread.command(SerialThread::RESET);
}

void SerialDevice::setCykle(uint8_t nrDoz, uint32_t nrCyckli)
{
    dozownikNr = nrDoz;
    val1 = nrCyckli;
    my_thread.command(SerialThread::SET_CYCLE);
}

void SerialDevice::setImpulse(uint8_t nrDoz, uint64_t steps)
{
    dozownikNr = nrDoz;
    val2 = steps;
}

#define SET_GET_REVERSE_DEFINTION(N) \
bool SerialDevice::getReverse##N() const \
{ \
    return reverse##N; \
} \
void SerialDevice::setReverse##N(bool newReverse##N) \
{ \
    reverse##N = newReverse##N; \
    ustawienia.setReverse_##N(newReverse##N); \
    my_thread.command(SerialThread::SET_PARAMS);\
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
    qDebug("%s:%d write = %s", __FILE__, __LINE__, currentRequest.toHex().constData());
    readError = false;
    if (currentRequest.size() == 0)
        return msg;

    RS232_SendBuf(portNr, (unsigned char*)currentRequest.constData(), currentRequest.size());
    qDebug("%s:%d write done", __FILE__, __LINE__);
    unsigned char recvBuffor[20];
    QThread::msleep(currentWaitWriteTimeout + currentReadWaitTimeout);
    int rc = RS232_PollComport(portNr, recvBuffor, 20);
    qDebug("%s:%d read done %d", __FILE__, __LINE__, rc);
    QByteArray responseData((const char*)recvBuffor, rc);
    qDebug("%s:%d parse %d [%s]", __FILE__, __LINE__, responseData.size(), responseData.toHex().constData());
    return parseMessage(responseData);
}



bool SerialDevice::configureDeviceJob()
{


    //qDebug"%s:%d", __FILE__, __LINE__);
    emit debug(QString("Konfiguracja %1%2%3%4%5 %6 %7").arg(reverse1).arg(reverse2).arg(reverse3).arg(reverse4).arg(reverse5).
               arg(maxImp).arg(timeImp));

   qDebug("%s:%d", __FILE__, __LINE__);
    auto s = write(SerialMessage::welcomeMsg(), 100, 100).getParseReply();

    if (s != SerialMessage::WELCOME_REPLY)
        return false;

    //qDebug("%s:%d", __FILE__, __LINE__);
    //s = write(SerialMessage::setReset(), 100, 3000).getParseReply();
    //if (s != SerialMessage::RESET_REPLY)
    //    return false;
    QThread::msleep(2000);
    qDebug("%s:%d", __FILE__, __LINE__);
    //s = write(SerialMessage::setSettingsMsg(reverse1, reverse2, reverse3, reverse4, reverse5, maxImp, timeImp),
    //          100, 100).getParseReply();
    //qDebug("%s:%d", __FILE__, __LINE__);
    //if (s != SerialMessage::SETPARAMS_REPLY)
    //    return false;
    qDebug("%s:%d", __FILE__, __LINE__);

    return true;
}

void SerialDevice::setParamsJob()
{
    //qDebug"%s:%d", __FILE__, __LINE__);
    emit debug(QString("Konfiguracja %1%2%3%4%5 %6 %7").arg(reverse1).arg(reverse2).arg(reverse3).arg(reverse4).arg(reverse5).
               arg(maxImp).arg(timeImp));

    //qDebug"%s:%d", __FILE__, __LINE__);
    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit setParamsDone(false);
        return;
    }

    //qDebug"%s:%d", __FILE__, __LINE__);
    s = write(SerialMessage::setReset(), 100, 3000).getParseReply();
    if (s != SerialMessage::RESET_REPLY) {
        m_configured = false;
        emit setParamsDone(false);
        return;
    }

    //qDebug"%s:%d", __FILE__, __LINE__);
    s = write(SerialMessage::setSettingsMsg(reverse1, reverse2, reverse3, reverse4, reverse5, maxImp, timeImp),
              100, 100).getParseReply();

    if (s != SerialMessage::SETPARAMS_REPLY) {
        m_configured = false;
        emit setParamsDone(false);
        return;
    }
    //qDebug"%s:%d", __FILE__, __LINE__);
    emit setParamsDone(true);
}

void SerialDevice::setResetJob()
{
    //qDebug"%s:%d", __FILE__, __LINE__);
    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit resetDone(false);
        return;
    }

    //qDebug"%s:%d", __FILE__, __LINE__);
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
    //qDebug"%s:%d", __FILE__, __LINE__);
    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit setCykleDone(false);
        return;
    }

    //qDebug"%s:%d val1=%d", __FILE__, __LINE__, val1);
    for (uint32_t n = 0; n < val1; ++n) {
        s = write(SerialMessage::setPosition(dozownikNr, maxImp), 100, 60000).getParseReply();
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
    //qDebug"%s:%d", __FILE__, __LINE__);
    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit setStepsDone(false);
        return;
    }

    uint64_t stepsAll = val2;
    uint32_t steps;
    while (stepsAll > 0) {
        steps = stepsAll > maxImp ? maxImp : stepsAll - maxImp;
        stepsAll -= steps;

        s = write(SerialMessage::setPosition(dozownikNr, steps), 100, 60000).getParseReply();
        if (s != SerialMessage::ECHO_REPLY) {
            emit setCykleDone(false);
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

void SerialDevice::setHomeJob()
{
    //qDebug"%s:%d", __FILE__, __LINE__);
    emit debug(QString("Ustaw pozycje startowa"));

    //qDebug"%s:%d", __FILE__, __LINE__);
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
    //qDebug"%s:%d", __FILE__, __LINE__);
    emit debug(QString("Ustaw pozycje %1").arg(val1));

    //qDebug"%s:%d", __FILE__, __LINE__);
    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit setPositionDone(false);
        return;
    }

    s = write(SerialMessage::setPosition(dozownikNr, val1), 100, 60000).getParseReply();
    if (s != SerialMessage::POSITION_REP) {
        emit setPositionDone(false);
        return;
    }

    emit setPositionDone(true);
}

bool SerialDevice::openDevice()
{
    qDebug("%s:%d open Devicedd", __FILE__, __LINE__);
    char mode[]={'8','O','1',0};
    if (RS232_OpenComport(portNr, 115200, mode, 0)) {
        m_connected = false;
        return false;
    }
    qDebug("%s:%d", __FILE__, __LINE__);
    RS232_flushRXTX(portNr);

    const unsigned char startBuf[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int rs = RS232_SendBuf(portNr, (unsigned char*)startBuf, 16);
    qDebug("%s:%d write %d", __FILE__,__LINE__,rs);
    if (rs <= 0) {
        RS232_CloseComport(portNr);
        return false;
    }
    QThread::msleep(200);
    unsigned char recvBuf[100];
    int recv = RS232_PollComport(portNr, recvBuf, 100);;
    
    qDebug("%s:%d recv=%d", __FILE__, __LINE__, recv);
    return true;
}

SerialMessage SerialDevice::parseMessage(const QByteArray &reply)
{
    SerialMessage msg;
    msg.parseCommand(reply);
    //qDebug"%s:%d parse = %d", __FILE__,__LINE__, msg.parseCommand(reply));
    return msg;
}

void SerialDevice::closeDevice()
{
    RS232_CloseComport(portNr);
    m_connected = false;
}

void SerialDevice::connectToSerialJob()
{
    QString description;
    QString manufacturer;
    QString serialNumber;
    static short tryConfigure = 0;
    //const QString serialNumberKontroler = "D76ED16151514C4B39202020FF012E1C";

    emit dozownikConfigured(false, false);
    char bufPortName[12] = {0};
    portNr = -1;
    qDebug("%s:%d",__FILE__,__LINE__);
    GetComPortUsb(bufPortName,"1B4F","9206");
    qDebug("%s:%d [%s]",__FILE__,__LINE__, bufPortName);

    portNr = RS232_GetPortnr(bufPortName);
    portNr = RS232_GetPortnr("COM5");
    qDebug("%s:%d portNr=%d",__FILE__,__LINE__, portNr);
    if (portNr == -1)
        return;
    m_connected = openDevice();
    if(!m_connected)
        return;

    m_configured = configureDeviceJob();
    qDebug("%s:%d conn = %d conf = %d", __FILE__, __LINE__, m_connected, m_configured);
    if (!m_configured) {
        closeDevice();
        m_connected = false;
        return;
    }
    //qDebug"%s:%d  conn = %d, conf = %d", __FILE__, __LINE__, m_connected, m_configured);
    emit debug(QString("Konfiguracja urządzenia %1").arg(m_configured ? "Sukces" : "Porażka"));
    emit dozownikConfigured(m_connected, m_configured);


}


SerialThread::SerialThread(SerialDevice * device, QObject *parent) :
    QThread(parent), sd(device)
{
    //qDebug"%s:%d", __FILE__, __LINE__);
    //sd = device;
    nrZadania = IDLE;
    m_quit = false;
}

SerialThread::~SerialThread()
{
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}

void SerialThread::command(Task curr)
{
    const QMutexLocker locker(&m_mutex);

    nrZadania = curr;
    if (! isRunning())
         start();
    else
        m_cond.wakeOne();
}

void SerialThread::run()
{
    qDebug("%s:%d run", __FILE__,__LINE__);
    m_mutex.lock();
    short zadanie = nrZadania;
    m_mutex.unlock();
    short nrTrying = 0;
    while (!m_quit) {
        qDebug("%s:%d zadanie %d", __FILE__,__LINE__, zadanie);
        switch(zadanie) {
        case IDLE:
            qDebug("conn=%d conf=%d read=%d",sd->m_connected,sd->m_configured, sd->readError);
            if (sd->readError) {
                if (nrTrying++ > 5) {
                    nrTrying = 0;
                    zadanie = CONNECT;
                    sd->closeDevice();
                    sd->readError = false;
                    sd->m_connected = false;
                } else {
                    sd->readError = false;
                    zadanie = CONFIGURE;
                }
            }
            if (!sd->m_connected) {
                zadanie = CONNECT;
                sleep(1);
            } else if (!sd->m_configured) {
                zadanie = CONFIGURE;
                msleep(300);
            } else {
                m_mutex.lock();
                m_cond.wait(&m_mutex);
                zadanie = nrZadania;
                m_mutex.unlock();
            }
            break;
        case CONNECT:
            qDebug("%s:%d zadanie CONNECT ", __FILE__,__LINE__);
            sd->connectToSerialJob();
            if (!sd->m_connected) {
                zadanie = CONNECT;
                sleep(1);
            } else if (!sd->m_configured) {
                if (nrTrying++ > 5) {
                    nrTrying = 0;
                    zadanie = CONNECT;
                    sd->closeDevice();
                    sd->readError = false;
                    sd->m_connected = false;
                }
                zadanie = CONFIGURE;
                sleep(1);
            } else
                zadanie = IDLE;
            //qDebug"%s:%d %d %d", __FILE__, __LINE__, sd->m_connected, sd->m_configured);
            break;

        case CONFIGURE:
            //qDebug"%s:%d zadanie CONFIGURE ", __FILE__,__LINE__);
            sd->m_configured = sd->configureDeviceJob();
            zadanie = IDLE;
            break;

        case SET_PARAMS:
            //qDebug"%s:%d zadanie SET_PARAMS ", __FILE__,__LINE__);
            sd->setParamsJob();
            zadanie = IDLE;
            break;

        case SET_HOME:
            //qDebug"%s:%d zadanie SET_PARAMS ", __FILE__,__LINE__);
            sd->setHomeJob();
            zadanie = IDLE;
            break;

        case SET_CYCLE:
            //qDebug"%s:%d zadanie SET_CYCLE ", __FILE__,__LINE__);
            sd->setCykleJob();
            zadanie = IDLE;
            break;
        default:
            zadanie = IDLE;
        }
    }
}
