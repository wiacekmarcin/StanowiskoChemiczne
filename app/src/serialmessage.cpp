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

void SerialMessage::writeMessage(const QByteArray &writeData)
{

    const qint64 bytesWritten = m_serialPort.write(writeData);

    if (bytesWritten == -1) {
        emit debug(QObject::tr("Nie mozna zapisac do urzadzenia"));
        return;
    } else if (bytesWritten != writeData.size()) {
        emit debug(QObject::tr("Czesciowy zapis do urzadzenia"));
        return;
    }

    emit debug(QString("Wyslalem %1").arg(writeData.toHex().toStdString().c_str()));
    //m_timer.start(1000);
}




SerialMessage::SerialMessage(QObject *parent) :
    QObject(parent)
{
    portName = "N/A";
    connSerial = false;

    connect(&m_serialPort, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
    connect(&m_serialPort, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(serialError(QSerialPort::SerialPortError)));

    //connect(this, SIGNAL(setParams1()), this, SLOT(doneSettings1()));
    //connect(this, SIGNAL(setParams2()), this, SLOT(doneSettings2()));

    //connect(&m_timer, &QTimer::timeout, this, &SerialPortReader::handleTimeout);

    commands.clear();
    cmd.clear();


}

SerialMessage::~SerialMessage()
{
    if (m_serialPort.isOpen())
        m_serialPort.close();
}

void SerialMessage::handleReadyRead()
{
    QByteArray readData = m_serialPort.readAll();

    while (readData.size() > 0 ) {

        //unsigned char c = readData.front();
        unsigned char c = readData.at(0);
        readData.remove(0,1);
        cmd.append(c);

        if (cmd.size() == 1) {
            lenCmd = c & 0x0f;
        } else if (cmd.size() - 2 == lenCmd) {
            parseCommand(cmd);
            cmd.clear();
            lenCmd = 0;
        }
    }

    //if (!m_timer.isActive())
    //    m_timer.start(m_timeout);
    //emit response(m_readData);
    //m_readData.clear();
    }

void SerialMessage::serialError(const QSerialPort::SerialPortError &error)
{
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
        //qDebug("DESC %s", description.toStdString().c_str());
        //qDebug("MENU %s", manufacturer.toStdString().c_str());
        //qDebug("SERIAL %s", serialNumber.toStdString().c_str());


       //if (description == "SparkFun Pro Micro" &&
       //         manufacturer == "SparkFun") {
       //Pod windowsem jest uniwersalne urzadzenie USB firmy Microsoft
            if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
                auto vendorId = serialPortInfo.vendorIdentifier();
                auto productId = serialPortInfo.productIdentifier();
                qDebug("VID %d", vendorId);
                qDebug("PID %d", productId);
                if (vendorId == 6991 && productId == 37382 /* && serialNumber == serialNumberKontroler */) {
                    //if (sendMesgWelcome(serialPortInfo)) {
                    //    connSerial = true;
                    //    return;
                    //}
                    //TODO otworzmy urzadzenie
                    //a nastepnie wyslijmy wiadomosc powitalna welcomemsg i asynchronicznie oczekujmy odpowiedzi
                    emit debug("open device");
                    emit successOpenDevice(openDevice(serialPortInfo));
                    return;
                }
            }
        //}


    }
    emit successOpenDevice(false);
}

void SerialMessage::echo()
{
    emit debug(QString("Wysylam echo"));
    writeMessage(echoMsg());
}

void SerialMessage::setPositionHome()
{
    emit debug(QString(homePositionMsg().toHex().toStdString().c_str()));
    emit debug(QString("Ustawiam pozycje startowa"));
    writeMessage(homePositionMsg());
}

void SerialMessage::setSettings(bool reverse, uint32_t maxImp)
{
    emit debug(QString(settingsMsg(reverse, maxImp).toHex().toStdString().c_str()));
    writeMessage(settingsMsg(reverse, maxImp));
}

void SerialMessage::doneSettings()
{
    //setSettings();
    emit setParamsDone();
}

void SerialMessage::setPosition(uint32_t x)
{
    //emit debug(QString(positionMsg(x, y).toHex().toStdString().c_str()));
    emit debug(QString("Ustawiam pozycje %1").arg(x));
    writeMessage(positionMsg(x));
}

void SerialMessage::closeDevice()
{
    if (m_serialPort.isOpen())
        m_serialPort.close();
}

void SerialMessage::response(const QByteArray &s)
{
    QByteArray resp = s;
    while(resp.size() > 0) {
        parseCommand(resp);
    }
}

bool SerialMessage::openDevice(const QSerialPortInfo &port)
{
    qDebug("openDevice");
    m_serialPort.setPort(port);
    portName = port.portName();
    emit deviceName(portName);

    //m_serialPort.close();
    if (!m_serialPort.open(QIODevice::ReadWrite)) {
        emit errorSerial(QString(QObject::tr("Nie mozna otworzyc urzadzenia %1, error  %2")).arg(portName).arg(m_serialPort.errorString()));
        return false;
    }
    connSerial = true;

    m_serialPort.setBaudRate(QSerialPort::Baud115200);
    m_serialPort.setDataBits(QSerialPort::Data8);
    m_serialPort.setFlowControl(QSerialPort::NoFlowControl);
    m_serialPort.setParity(QSerialPort::NoParity);
    m_serialPort.setStopBits(QSerialPort::OneStop);

    //emit checkController();
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

bool SerialMessage::checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len,  QByteArray & data)
{
    if (arr.length() == 0) {
        emit debug("Pusta komenda");
        return false;
    }

    cmd = (arr[0] >> 4) & 0x0f;
    len = arr[0] & 0x0f;
    emit debug(QString("%1 %2").arg(cmd).arg(len));
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
        emit debug(QString("crc = %1x val=%2x").arg(crc.getCRC(), 16).arg(msgcrc,16));
        qDebug("arr = %s", arr.toHex().toStdString().c_str());
        qDebug("cmd = %d", cmd);
        qDebug("len = %d", len);
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
            if (len != 15) {
                qDebug("len != 15");
                return false;
            }
            uint8_t wzorzec[15] = {'K','O','N','T','R','O','L','E','R','P','O','S','R','E', 'D'};
            for (int i = 0; i < 15; ++i) {
                if (wzorzec[i] != data[i]) {
                    qDebug("wzorzec != data");
                    return false;
                }
            }

            qDebug("Kontroler OK");
            emit controllerOK();

            return true;
        }
        case ECHO_REP:
            emit echoOK();
            break;
        case MOVEHOME_REP:
        {
            //set home position
            //0xB0 CRC8 - req
            //0xC1 s/K/E CRC8 - s=start,K=stop
            if (len == 1) {
                switch(data[0]) {
                    case 's':
                        emit debug("Rozpoczynam pozycjonowanie do bazy");
                        emit startingHome();
                        return true;
                    case 'K':
                        emit debug("Zakonczylem pozycjonowanie do bazy");
                        emit doneHome();
                        return true;
                    case 'E':
                        emit debug("Blad podczas pozycjonowania do bazy");
                        emit errorHome();
                        return true;
                    default:
                        return false;
                }
            }
            return false;
        }

        case POSITION_REP:
        {
        //0xC1 s/K CRC8 - s=start, K=stop
            if (len == 1) {
                switch(data[0]) {
                    case 's':
                        emit debug("Rozpoczynam ustawianie pozycji");
                        emit startingPosition();
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
            emit setParams();
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

