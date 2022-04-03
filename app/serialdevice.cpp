#include "serialdevice.h"
#include <QTime>
#include <QSerialPortInfo>


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

    this->moveToThread(&my_thread);
    m_serial.moveToThread(&my_thread);
    my_thread.start();
    
    //my_thread.command(SerialThread::IDLE);
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
    m_serial.write(currentRequest);
    if (m_serial.waitForBytesWritten(currentWaitWriteTimeout)) {
        // read response
        qDebug("%s:%d write done wait for read", __FILE__, __LINE__);
        if (m_serial.waitForReadyRead(currentReadWaitTimeout)) {
            QByteArray responseData = m_serial.readAll();
            qDebug("%s:%d read done %d [%s]", __FILE__, __LINE__, responseData.size(), responseData.toHex().constData());
            while (m_serial.waitForReadyRead(10))
                responseData += m_serial.readAll();
            qDebug("%s:%d read all done %d -> parse", __FILE__, __LINE__, responseData.size());
            return parseMessage(responseData);
        } else {
            qDebug("%s:%d %s", __FILE__, __LINE__, "Timeout przy odczycie");
            //emit timeout(tr("Wait read response timeout %1")
            //             .arg(QTime::currentTime().toString()));
            msg.setTimeoutReply(false);
            readError = true;
        }
    } else {
        qDebug("%s:%d %s", __FILE__, __LINE__, "Timeout przy zapisie");
        //emit timeout(tr("Wait write request timeout %1")
        //             .arg(QTime::currentTime().toString()));
        msg.setTimeoutReply(true);
        readError = true;
    }
    qDebug("%s:%d Nieudany zapis/odczyt", __FILE__, __LINE__);
    return msg;
}



bool SerialDevice::configureDeviceJob()
{


    qDebug("%s:%d", __FILE__, __LINE__);
    emit debug(QString("Konfiguracja %1%2%3%4%5 %6 %7").arg(reverse1).arg(reverse2).arg(reverse3).arg(reverse4).arg(reverse5).
               arg(maxImp).arg(timeImp));

    qDebug("%s:%d", __FILE__, __LINE__);
    auto s = write(SerialMessage::welcomeMsg(), 100, 100).getParseReply();

    if (s != SerialMessage::WELCOME_REPLY)
        return false;

    qDebug("%s:%d", __FILE__, __LINE__);
    s = write(SerialMessage::setReset(), 100, 3000).getParseReply();
    if (s != SerialMessage::RESET_REPLY)
        return false;

    qDebug("%s:%d", __FILE__, __LINE__);
    s = write(SerialMessage::setSettingsMsg(reverse1, reverse2, reverse3, reverse4, reverse5, maxImp, timeImp),
              100, 100).getParseReply();
    qDebug("%s:%d", __FILE__, __LINE__);
    if (s != SerialMessage::SETPARAMS_REPLY)
        return false;
    qDebug("%s:%d", __FILE__, __LINE__);

    return true;
}

void SerialDevice::setParamsJob()
{
    qDebug("%s:%d", __FILE__, __LINE__);
    emit debug(QString("Konfiguracja %1%2%3%4%5 %6 %7").arg(reverse1).arg(reverse2).arg(reverse3).arg(reverse4).arg(reverse5).
               arg(maxImp).arg(timeImp));

    qDebug("%s:%d", __FILE__, __LINE__);
    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit setParamsDone(false);
        return;
    }

    qDebug("%s:%d", __FILE__, __LINE__);
    s = write(SerialMessage::setReset(), 100, 3000).getParseReply();
    if (s != SerialMessage::RESET_REPLY) {
        m_configured = false;
        emit setParamsDone(false);
        return;
    }

    qDebug("%s:%d", __FILE__, __LINE__);
    s = write(SerialMessage::setSettingsMsg(reverse1, reverse2, reverse3, reverse4, reverse5, maxImp, timeImp),
              100, 100).getParseReply();

    if (s != SerialMessage::SETPARAMS_REPLY) {
        m_configured = false;
        emit setParamsDone(false);
        return;
    }
    qDebug("%s:%d", __FILE__, __LINE__);
    emit setParamsDone(true);
}

void SerialDevice::setResetJob()
{
    qDebug("%s:%d", __FILE__, __LINE__);
    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit resetDone(false);
        return;
    }

    qDebug("%s:%d", __FILE__, __LINE__);
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
    qDebug("%s:%d", __FILE__, __LINE__);
    auto s = write(SerialMessage::echoMsg(), 100, 100).getParseReply();
    if (s != SerialMessage::ECHO_REPLY) {
        m_configured = false;
        emit setCykleDone(false);
        return;
    }

    qDebug("%s:%d val1=%d", __FILE__, __LINE__, val1);
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
    qDebug("%s:%d", __FILE__, __LINE__);
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
    qDebug("%s:%d", __FILE__, __LINE__);
    emit debug(QString("Ustaw pozycje startowa"));

    qDebug("%s:%d", __FILE__, __LINE__);
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
    qDebug("%s:%d", __FILE__, __LINE__);
    emit debug(QString("Ustaw pozycje %1").arg(val1));

    qDebug("%s:%d", __FILE__, __LINE__);
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

bool SerialDevice::openDevice(const QSerialPortInfo &port)
{
    qDebug("%s:%d open Devicedd", __FILE__, __LINE__);
    m_serial.setPort(port);
    m_portName = port.portName();

    if (!m_serial.open(QIODevice::ReadWrite)) {
        serialError(m_serial.error());
        emit error(QString("Nie można otworzyć urządzenia %1, error  %2").arg(m_portName, m_serial.errorString()));
        return false;
    }
    qDebug("%s:%d", __FILE__, __LINE__);
    m_serial.flush();
    m_serial.clear();
    m_serial.clearError();


    m_serial.setBaudRate(QSerialPort::Baud9600);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
    m_serial.setParity(QSerialPort::OddParity);
    m_serial.setStopBits(QSerialPort::TwoStop);
    m_serial.setReadBufferSize(20);
    m_serial.clear();
    m_serial.flush();

    while(false) {
        QByteArray zerowaMsg(1, (char)0);
        m_serial.write(zerowaMsg);
        if (m_serial.waitForBytesWritten(100)) {
            // read response
            qDebug("%s:%d write done wait for read", __FILE__, __LINE__);
            if (m_serial.waitForReadyRead(100)) {
                QByteArray responseData = m_serial.readAll();
                qDebug("%s:%d read done %d [%s]", __FILE__, __LINE__, responseData.size(), responseData.toHex().constData());
                while (m_serial.waitForReadyRead(10))
                    responseData += m_serial.readAll();
                break;
                qDebug("%s:%d read all done %d -> parse", __FILE__, __LINE__, responseData.size());
            } else {
                qDebug("%s:%d %s", __FILE__, __LINE__, "Timeout przy odczycie");
            }
        } else {
            qDebug("%s:%d %s", __FILE__, __LINE__, "Timeout przy zapisie");
        }
    }
    qDebug("%s:%d", __FILE__, __LINE__);
    return true;
}

SerialMessage SerialDevice::parseMessage(const QByteArray &reply)
{
    SerialMessage msg;
    qDebug("%s:%d parse = %d", __FILE__,__LINE__, msg.parseCommand(reply));
    return msg;
}

void SerialDevice::closeDevice()
{
    if (m_serial.isOpen()) {
        m_serial.flush();
        m_serial.clear();
        m_serial.clearError();
        m_serial.close();
    }
    m_connected = false;
}

void SerialDevice::connectToSerialJob()
{
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    QString description;
    QString manufacturer;
    QString serialNumber;
    static short tryConfigure = 0;
    //const QString serialNumberKontroler = "D76ED16151514C4B39202020FF012E1C";

    emit dozownikConfigured(m_connected, m_configured);
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        emit debug(QString("DESC ") + description);
        emit debug(QString("MENU ") + manufacturer);
        emit debug(QString("SERIAL") + serialNumber);
        qDebug("%s:%d", __FILE__, __LINE__);

        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
            auto vendorId = serialPortInfo.vendorIdentifier();
            auto productId = serialPortInfo.productIdentifier();
            emit debug(QString("Znaleziono kandydata"));
            qDebug("%s:%d", __FILE__, __LINE__);
            if (vendorId == 6991 && productId == 37382 /* && serialNumber == serialNumberKontroler */) {
               m_connected = openDevice(serialPortInfo);
                emit dozownikConfigured(m_connected, m_configured);
                qDebug("%s:%d conn = %d", __FILE__, __LINE__, m_connected);
                emit debug(QString("Urzadzenie %1 zostalo otwarte").arg(m_portName));
                if (m_connected) {
                    m_configured = configureDeviceJob();
                    qDebug("%s:%d conn = %d conf = %d", __FILE__, __LINE__, m_connected, m_configured);
                    if (!m_configured) {
                        if (tryConfigure++ > 5) {
                            tryConfigure = 0;
                            closeDevice();
                            m_connected = false;
                        }
                    }
                    qDebug("%s:%d  conn = %d, conf = %d", __FILE__, __LINE__, m_connected, m_configured);
                    emit debug(QString("Konfiguracja urządzenia %1").arg(m_configured ? "Sukces" : "Porażka"));
                    emit dozownikConfigured(m_connected, m_configured);
                }
                return;
            }
        }
    }
    //qDebug("%s %d", __FILE__, __LINE__);
    m_connected = false;
}

void SerialDevice::serialError(const QSerialPort::SerialPortError &errorSer)
{
    switch (errorSer) {
    case QSerialPort::NoError:
    default:
        break;

    case QSerialPort::DeviceNotFoundError:
        emit error("Nie znaleziono urządzenia");
        break;

    case QSerialPort::PermissionError:
        emit error("Brak uprawnień do urządzenia");
        break;

    case QSerialPort::OpenError:
        emit error("Błąd podczas otwierania urządzenia");
        break;

    case QSerialPort::ParityError:
        emit error("Błąd parzystości");
        break;

    case QSerialPort::FramingError:
    case QSerialPort::BreakConditionError:
        emit error("Błąd danych");
        break;

    case QSerialPort::WriteError:
        emit error("Błąd zapisu");
        break;

    case QSerialPort::ReadError:
        emit error("Błąd odczytu");
        break;

    case QSerialPort::ResourceError:
        emit error("Błąd zasobu");
        break;

    case QSerialPort::UnsupportedOperationError:
        emit error("Niedozwolona operacja");
        break;

    case QSerialPort::UnknownError:
        emit error("Nieznany błąd");
        break;

    case QSerialPort::TimeoutError:
        emit error("Błąd timeout");
        break;

    case QSerialPort::NotOpenError:
        emit error("Inny błąd");
        break;

    }
}

SerialThread::SerialThread(SerialDevice * device, QObject *parent) :
    QThread(parent), sd(device)
{
    qDebug("%s:%d", __FILE__, __LINE__);
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
            qDebug("%s:%d %d %d", __FILE__, __LINE__, sd->m_connected, sd->m_configured);
            break;

        case CONFIGURE:
            qDebug("%s:%d zadanie CONFIGURE ", __FILE__,__LINE__);
            sd->m_configured = sd->configureDeviceJob();
            zadanie = IDLE;
            break;

        case SET_PARAMS:
            qDebug("%s:%d zadanie SET_PARAMS ", __FILE__,__LINE__);
            sd->setParamsJob();
            zadanie = IDLE;
            break;

        case SET_HOME:
            qDebug("%s:%d zadanie SET_PARAMS ", __FILE__,__LINE__);
            sd->setHomeJob();
            zadanie = IDLE;
            break;

        case SET_CYCLE:
            qDebug("%s:%d zadanie SET_CYCLE ", __FILE__,__LINE__);
            sd->setCykleJob();
            zadanie = IDLE;
            break;
        default:
            zadanie = IDLE;
        }
    }
}
