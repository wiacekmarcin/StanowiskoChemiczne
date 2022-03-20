#include "serialmessage.h"
#include "crc8.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTextStream>
#include <QCoreApplication>

#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif
void qSleep(int ms)
{

#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

#include <ustawienia.h>

void SerialMessage::writeMessage(const QByteArray &writeData)
{
    //qDebug("%s %d",__FILE__,__LINE__);
    const qint64 bytesWritten = m_serialPort.write(writeData);
    m_serialPort.flush();

    if (bytesWritten == -1) {
        connSerial = false;
        connectToSerial();
        return;
    } else if (bytesWritten != writeData.size()) {
        emit debug(QObject::tr("Czesciowy zapis do urzadzenia"));
        return;
    }

    emit debug(QString("Wyslalem %1 bytes %2").arg(bytesWritten).arg(writeData.toHex().toStdString().c_str()));
}


SerialMessage::SerialMessage(QObject *parent) :
    QObject(parent)
{
    portName = "N/A";
    connSerial = false;
    lenEcho=0;
    reverse=false;
    maxImp=0;
    revByte = 0;

    connect(&m_serialPort, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(&m_serialPort, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(serialError(QSerialPort::SerialPortError)));

    //connect(&m_timer, &QTimer::timeout, this, &SerialPortReader::handleTimeout);

    commands.clear();
    cmd.clear();


}

SerialMessage::~SerialMessage()
{
    closeDevice();
}

void SerialMessage::handleReadyRead()
{
    QByteArray readData = m_serialPort.readAll();
    //qDebug("%s %d Recv data %d", __FILE__,__LINE__, readData.size());
    while (readData.size() > 0 ) {
        unsigned char c = readData.at(0);
        readData.remove(0,1);
        cmd.append(c);

        if (cmd.size() == 1) {
            lenCmd = c & 0x0f;
        } else if (cmd.size() - 2 == lenCmd) {
            qDebug("cmd = %s", cmd.toHex().toStdString().c_str());
            parseCommand(cmd);
            cmd.clear();
            lenCmd = 0;
        }
    }
}

void SerialMessage::connectToSerial()
{
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    QString description;
    QString manufacturer;
    QString serialNumber;
    //const QString serialNumberKontroler = "D76ED16151514C4B39202020FF012E1C";
    closeDevice();

    for (const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        qDebug("DESC %s", description.toStdString().c_str());
        qDebug("MENU %s", manufacturer.toStdString().c_str());
        qDebug("SERIAL %s", serialNumber.toStdString().c_str());


        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
            auto vendorId = serialPortInfo.vendorIdentifier();
            auto productId = serialPortInfo.productIdentifier();
            qDebug("VID %d", vendorId);
            qDebug("PID %d", productId);
            if (vendorId == 6991 && productId == 37382 /* && serialNumber == serialNumberKontroler */) {
                bool openSuccess = openDevice(serialPortInfo);
                qDebug("%s %d", __FILE__, __LINE__);
                emit successOpenDevice(openSuccess);
                return;
            }
        }
    }
    //qDebug("%s %d", __FILE__, __LINE__);
    connSerial = false;
    emit successOpenDevice(false);
}


void SerialMessage::serialError(const QSerialPort::SerialPortError &error)
{
    connSerial = false;
    switch (error) {
    case QSerialPort::NoError:
    default:
        break;

    case QSerialPort::DeviceNotFoundError:
        emit errorSerial("Nie znaleziono urządzenia");
        break;

    case QSerialPort::PermissionError:
        emit errorSerial("Brak uprawnień do urządzenia");
        break;

    case QSerialPort::OpenError:
        emit errorSerial("Błąd podczas otwierania urządzenia");
        break;

    case QSerialPort::ParityError:
        emit errorSerial("Błąd parzystości");
        break;

    case QSerialPort::FramingError:
    case QSerialPort::BreakConditionError:
        emit errorSerial("Błąd danych");
        break;

    case QSerialPort::WriteError:
        emit errorSerial("Błąd zapisu");
        break;

    case QSerialPort::ReadError:
        emit errorSerial("Błąd odczytu");
        break;

    case QSerialPort::ResourceError:
        emit errorSerial("Błąd zasobu");
        break;

    case QSerialPort::UnsupportedOperationError:
        emit errorSerial("Niedozwolona operacja");
        break;

    case QSerialPort::UnknownError:
        emit errorSerial("Nieznany błąd");
        break;

    case QSerialPort::TimeoutError:
        emit errorSerial("Błąd timeout");
        break;

    case QSerialPort::NotOpenError:
        emit errorSerial("Inny błąd");
        break;

    }
}



void SerialMessage::echo()
{
    //qDebug("%s %d echo %d", __FILE__, __LINE__, connSerial);
    if (!connSerial)
    {
        //qDebug("dozownik = false");
        emit dozownik(false);
        connectToSerial();
        lenEcho = 0;
        return;
    }

    //emit debug(QString("Echo [%1] [%2]").arg(echoMsg().toHex().toStdString().c_str()).arg(lenEcho));
    if (++lenEcho == 5) {
        //qDebug("dozownik = false");
        connSerial = false;
        closeDevice();
        emit dozownik(false);
        return;
    }

    writeMessage(echoMsg());
}

void SerialMessage::setPositionHome(short DozownikNr)
{
    emit debug(QString("Ustawiam pozycje startowa nrDozownik = %1 [%2]").arg(DozownikNr).arg(homePositionMsg(DozownikNr).toHex().toStdString().c_str()));
    writeMessage(homePositionMsg(DozownikNr));
}

void SerialMessage::setSettings(bool reverse_, uint32_t maxImp_)
{
    qDebug("%s %d Set settings (%d)", __FILE__, __LINE__, connSerial);
    reverse = reverse_;
    maxImp = maxImp_;
    if (connSerial) {
        emit debug(QString("Ustawiam parametry reverse = %1 maxImp = %2 [%3]").arg(reverse).arg(maxImp).arg(settingsMsg(reverse, maxImp).toHex().toStdString().c_str()));
        writeMessage(settingsMsg(reverse, maxImp));
    }
}

void SerialMessage::setSettings5(bool reverse1, bool reverse2, bool reverse3, bool reverse4, bool reverse5, uint32_t maxImp_)
{
    qDebug("%s %d Set settings (%d)", __FILE__, __LINE__, connSerial);
    revByte = 0;
    revByte |= reverse1 ? 0x1<<0 : 0;
    revByte |= reverse2 ? 0x1<<1 : 0;
    revByte |= reverse3 ? 0x1<<2 : 0;
    revByte |= reverse4 ? 0x1<<3 : 0;
    revByte |= reverse5 ? 0x1<<4 : 0;
    maxImp = maxImp_;
    if (connSerial) {
        emit debug(QString("Ustawiam parametry reverse = %1,%2,%3,%4,%5 maxImp = %6 [%7]").arg(reverse1).arg(reverse2)
                   .arg(reverse3).arg(reverse4).arg(reverse5).arg(maxImp).arg(settingsMsg5(revByte, maxImp).toHex().toStdString().c_str()));
        writeMessage(settingsMsg5(revByte, maxImp));
    }
}

void SerialMessage::setPosition(short DozownikNr, uint32_t x)
{
    emit debug(QString("Ustawiam pozycje dozownikNr = %1, x = %2 [%3]").arg(DozownikNr).arg(x).arg(positionMsg(DozownikNr, x).toHex().toStdString().c_str()));
    writeMessage(positionMsg(DozownikNr, x));
}

void SerialMessage::setReset()
{
    emit debug(QString("Reset device"));
    writeMessage(resetMsg());
}

void SerialMessage::setParameters()
{
    emit debug(QString("Ustawiam parametry reverse = %1 maxImp = %2 [%3]").arg(revByte, 16).arg(maxImp).arg(settingsMsg(reverse, maxImp).toHex().toStdString().c_str()));
    writeMessage(settingsMsg5(revByte, maxImp));
}

void SerialMessage::closeDevice()
{
    if (m_serialPort.isOpen()) {
        m_serialPort.flush();
        m_serialPort.clear();
        m_serialPort.clearError();
        m_serialPort.close();
    }
    connSerial = false;
}

void SerialMessage::response(const QByteArray &s)
{
    qDebug("%s %d", __FILE__,__LINE__);
    QByteArray resp = s;
    while(resp.size() > 0) {
        parseCommand(resp);
    }
}

bool SerialMessage::openDevice(const QSerialPortInfo &port)
{
    //qDebug("openDevice");



    m_serialPort.setPort(port);
    portName = port.portName();


    //m_serialPort.close();
    if (!m_serialPort.open(QIODevice::ReadWrite)) {
        emit errorSerial(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error  %2")).arg(portName, m_serialPort.errorString()));
        qDebug("Nie mozna otworzyc urzadzenia %s, error  %s", portName.toStdString().c_str(), m_serialPort.errorString().toStdString().c_str());
        return false;
    }
    m_serialPort.flush();
    m_serialPort.clear();
    m_serialPort.clearError();
    connSerial = true;

    m_serialPort.setBaudRate(QSerialPort::Baud9600);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort.setParity(QSerialPort::OddParity);
    m_serialPort.setStopBits(QSerialPort::TwoStop);
    m_serialPort.setReadBufferSize(20);
    m_serialPort.clear();
    delay(1);

    //int bread;
    qDebug("%s %d",__FILE__, __LINE__);
    //while (m_serialPort.bytesAvailable() <= 0)
    //    writeMessage(QByteArray(1, 0));
    //qDebug("%s %d",__FILE__, __LINE__);
    //m_serialPort.readAll();
    //QByteArray test;
    //test.append("H");
    //test.append("e");
    //test.append("l");
    //test.append("l");
    //test.append("o");
    //writeMessage(test);
    writeMessage(welcomeMsg());

    return true;
}

QByteArray SerialMessage::welcomeMsg()
{
    return prepareMessage(WELCOME_REQ, NULL, 0);
}

QByteArray SerialMessage::homePositionMsg(short dozownikNr)
{
    uint8_t tab[1];
    tab[0] = dozownikNr & 0xff;
    return prepareMessage(MOVEHOME_REQ, tab, 1);
}

QByteArray SerialMessage::positionMsg(short dozownikNr, uint32_t x)
{
    uint8_t tab[5];
    tab[0] = dozownikNr & 0xff;
    tab[1] = (x >> 24) & 0xff;
    tab[2] = (x >> 16) & 0xff;
    tab[3] = (x >> 8) & 0xff;
    tab[4] = x & 0xff;
    return prepareMessage(POSITION_REQ, tab, 5);
}

QByteArray SerialMessage::echoMsg()
{
    return prepareMessage(ECHO_REQ, NULL, 0);
}

QByteArray SerialMessage::settingsMsg(bool reverse, uint32_t maxImp)
{
    uint8_t tab[10];
    tab[0] = reverse ? 0x01 : 0x00;
    tab[1] = (maxImp >> 24) & 0xff;
    tab[2] = (maxImp >> 16) & 0xff;
    tab[3] = (maxImp >> 8) & 0xff;
    tab[4] = maxImp & 0xff;
    return prepareMessage(SET_PARAM_REQ, tab, 5);
}

QByteArray SerialMessage::settingsMsg5(uint8_t reverse, uint32_t maxImp)
{
    uint8_t tab[10];
    tab[0] = reverse;
    tab[1] = (maxImp >> 24) & 0xff;
    tab[2] = (maxImp >> 16) & 0xff;
    tab[3] = (maxImp >> 8) & 0xff;
    tab[4] = maxImp & 0xff;
    return prepareMessage(SET_PARAM_REQ, tab, 5);
}

QByteArray SerialMessage::resetMsg()
{
    return QByteArray();
}

uint32_t SerialMessage::getMaxImp() const
{
    return maxImp;
}

void SerialMessage::setMaxImp(uint32_t newMaxImp)
{
    maxImp = newMaxImp;
}

bool SerialMessage::getReverse() const
{
    return reverse;
}

void SerialMessage::setReverse(bool newReverse)
{
    reverse = newReverse;
}

bool SerialMessage::getReverse1() const
{
    return revByte & (0x1 << 0);
}

void SerialMessage::setReverse1(bool newReverse)
{
    if (newReverse)
        revByte |= 0x1 << 0;
    else
        revByte &= ~(0x1 << 0);
}

bool SerialMessage::getReverse2() const
{
    return revByte & (0x1 << 1);
}

void SerialMessage::setReverse2(bool newReverse)
{
    if (newReverse)
        revByte |= 0x1 << 1;
    else
        revByte &= ~(0x1 << 1);
}

bool SerialMessage::getReverse3() const
{
    return revByte & (0x1 << 2);
}

void SerialMessage::setReverse3(bool newReverse)
{
    if (newReverse)
        revByte |= 0x1 << 2;
    else
        revByte &= ~(0x1 << 2);
}

bool SerialMessage::getReverse4() const
{
    return revByte & (0x1 << 3);
}

void SerialMessage::setReverse4(bool newReverse)
{
    if (newReverse)
        revByte |= 0x1 << 3;
    else
        revByte &= ~(0x1 << 3);
}

bool SerialMessage::getReverse5() const
{
    return revByte & (0x1 << 4);
}

void SerialMessage::setReverse5(bool newReverse)
{
    if (newReverse)
        revByte |= 0x1 << 4;
    else
        revByte &= ~(0x1 << 4);
}

bool SerialMessage::checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len,  QByteArray & data)
{
    if (arr.length() == 0) {
        emit debug("Pusta komenda");
        return false;
    }

    //qDebug("cmd ascii = %s", arr.toStdString().c_str());
    //qDebug("cmd hex = %s", arr.toHex().toStdString().c_str());
    cmd = (arr[0] >> 4) & 0x0f;
    len = arr[0] & 0x0f;
    //emit debug(QString("cmd = %1 len = %2").arg(cmd).arg(len));
    CRC8 crc;
    data = QByteArray(len, Qt::Uninitialized);
    //crc.add(arr.at(0));
    int i = 0;
    for (i = 0; i < arr.length() - 1 && i <= len; i++) {
        crc.add(arr.at(i));
        if (i) {
            data[i-1] = arr.at(i);
        }
    }

    if (i-1 != len) {
        emit debug(QString("Nie poprawna dlugosc rozkazu"));
        return false;
    }
    if (i >= arr.size()) {
        emit debug(QString("Za dlugi rozkaz"));
        return false;
    }
    unsigned short msgcrc = arr.at(i) & 0xff;

    if (crc.getCRC() != msgcrc) {
        emit debug(QString("crc = x%1 val=x%2").arg(crc.getCRC(), 16).arg(msgcrc,16));
        //qDebug("arr = %s", arr.toHex().toStdString().c_str());
        //qDebug("cmd = %d", cmd);
        //qDebug("len = %d", len);
        return false;
    }
    //emit debug("Naglowek OK");
    return true;
}

bool SerialMessage::parseCommand(const QByteArray &arr)
{
    uint8_t cmd;
    uint8_t len;
    QByteArray data;

    if (!checkHead(arr, cmd, len, data)) {
        emit debug(QString("CheckHead faild %1").arg(arr.toHex(' ').toStdString().c_str()));
        return false;
    }

    switch (cmd) {
        case WELCOME_REP:
        {
            qDebug("%s %d Recv Welcome Msg", __FILE__,__LINE__);
            if (len != 15)
                return false;

            uint8_t wzorzec[15] = {'K','O','N','T','R','O','L','E','R','P','O','S','R','E', 'D'};
            for (int i = 0; i < 15; ++i) {
                if (wzorzec[i] != data[i]) {
                    return false;
                }
            }
            connSerial = true;
            setSettings(reverse, maxImp);
            return true;
        }
        case ECHO_REP:
            //qDebug("%s %d Recv Echo Msg. Emit Dozownik true", __FILE__,__LINE__);
            lenEcho = 0;
            emit dozownik(true);
            return true;

    case MOVEHOME_REP:
        {
            qDebug("%s %d MoveHome Msg", __FILE__,__LINE__);
            //set home position
            //0xB0 CRC8 - req
            //0xC1 s/K/E CRC8 - s=start,K=stop
            if (len == 1) {
                switch(data[0]) {
                    case 's':
                        emit debug("Rozpoczynam pozycjonowanie do bazy");
                        return true;
                    case 'K':
                        emit debug("Zakonczylem pozycjonowanie do bazy");
                        emit donePositionHome(true);
                        return true;
                    case 'E':
                        emit debug("Blad podczas pozycjonowania do bazy");
                        emit donePositionHome(false);
                        return true;
                    default:
                        return false;
                }
            }
            return false;
        }

        case POSITION_REP:
        {
            qDebug("%s %d Recv position Msg", __FILE__,__LINE__);
        //0xC1 s/K CRC8 - s=start, K=stop
            if (len == 1) {
                switch(data[0]) {
                    case 's':
                        emit debug("Rozpoczynam ustawianie pozycji");
                        return true;
                    case 'K':
                        emit debug("Zakonczylem ustawianie pozycji");
                        emit donePosition();
                        return true;
                    default:
                        return false;
                }
            }
            return false;
        }

        case SET_PARAM_REP:
        {
            qDebug("%s %d Set params Msg", __FILE__,__LINE__);
            emit setParamsDone();
            return true;
        }

        default:
            emit debug("Nieznana komenda");
            return false;
    }

    return false;
}

QByteArray SerialMessage::prepareMessage(uint8_t cmd, uint8_t tab[], uint8_t len)
{
    QByteArray arr(len+2,0);
    arr[0] = (cmd << 4) | (len & 0xff);
    CRC8 c;
    c.add(arr[0]);
    for (int i = 0; i < len; i++) {
        arr[i+1] = tab[i];
        c.add(tab[i]);
    }
    arr[len+1] = c.getCRC();

    QByteArray ret;
    //ret.append('U');
    ret.append(arr);
    //ret.append(0xaa);
    //emit debug(QString("Wysylam:")+QString(ret.toHex(' ').data()));
    return ret;
}

