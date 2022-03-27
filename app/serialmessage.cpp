#include "serialmessage.h"
#include "crc8.h"


#include <QDebug>

SerialMessage::SerialMessage()
{
    //revByte = 0x0f;
    //maxImp = 55000;
    //impTime = 200;
}

SerialMessage::~SerialMessage()
{

}

QByteArray SerialMessage::setSettingsMsg(bool reverse1, bool reverse2, bool reverse3, bool reverse4, bool reverse5, uint32_t maxImp, uint16_t imptime)
{
    uint8_t revByte = 0;
    revByte |= reverse1 ? 0x1<<0 : 0;
    revByte |= reverse2 ? 0x1<<1 : 0;
    revByte |= reverse3 ? 0x1<<2 : 0;
    revByte |= reverse4 ? 0x1<<3 : 0;
    revByte |= reverse5 ? 0x1<<4 : 0;
    return setSettingsMsg(revByte, maxImp, imptime);
}

QByteArray SerialMessage::setSettingsMsg(uint8_t revByte, uint32_t maxImp, uint16_t impTime)
{
    uint8_t tab[10];
    tab[0] = revByte & 0xff;
    tab[1] = (maxImp >> 24) & 0xff;
    tab[2] = (maxImp >> 16) & 0xff;
    tab[3] = (maxImp >> 8) & 0xff;
    tab[4] = maxImp & 0xff;
    tab[5] = (impTime >> 8) & 0xff;
    tab[6] = impTime & 0xff;
    return prepareMessage(SET_PARAM_REQ, tab, 7);
}

QByteArray SerialMessage::setPosition(short dozownikNr, uint32_t x)
{
    uint8_t tab[5];
    tab[0] = dozownikNr & 0xff;
    tab[1] = (x >> 24) & 0xff;
    tab[2] = (x >> 16) & 0xff;
    tab[3] = (x >> 8) & 0xff;
    tab[4] = x & 0xff;
    return prepareMessage(POSITION_REQ, tab, 5);
}

QByteArray SerialMessage::setReset()
{
    return QByteArray();
}

QByteArray SerialMessage::echoMsg()
{
    return prepareMessage(ECHO_REQ, NULL, 0);
}

QByteArray SerialMessage::welcomeMsg()
{
    return prepareMessage(WELCOME_REQ, NULL, 0);
}

QByteArray SerialMessage::setPositionHome(short dozownikNr)
{
    uint8_t tab[1];
    tab[0] = dozownikNr & 0xff;
    return prepareMessage(MOVEHOME_REQ, tab, 1);
}

bool SerialMessage::checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len,  QByteArray & data)
{
    errT = "";
    errB = false;
    if (arr.length() == 0) {
        errT = "Pusta komenda";
        errB = true;
        return false;
    }

    qDebug("cmd ascii = %s", arr.toStdString().c_str());
    qDebug("cmd hex = %s", arr.toHex().toStdString().c_str());
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
        errT = QString("Nie poprawna dlugosc rozkazu %1 != %2").arg(i-1).arg(len);
        errB = true;
        return false;
    }
    if (i >= arr.size()) {
        errT = QString("Za dlugi rozkaz %1 > 15").arg(i);
        errB = true;
        return false;
    }
    unsigned short msgcrc = arr.at(i) & 0xff;

    if (crc.getCRC() != msgcrc) {
        errT = QString("Nie zgodne crc");
        errB = true;
        //emit debug(QString("crc = x%1 val=x%2").arg(crc.getCRC(), 16).arg(msgcrc,16));
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
    qDebug("%s:%d", __FILE__, __LINE__);
    uint8_t cmd;
    uint8_t len;
    QByteArray data;
    errB = false;
    parseReply = INVALID_REPLY;
    if (!checkHead(arr, cmd, len, data)) {
        qDebug("%s:%d INVALID HEADER", __FILE__, __LINE__);
        qDebug("%s", errT.toStdString().c_str());
        return false;
    }
    qDebug("%s:%d", __FILE__, __LINE__);

    switch (cmd) {
        case WELCOME_REP:
        {
            qDebug("%s %d Recv Welcome Msg", __FILE__,__LINE__);
            if (len != 15) {
                errT = QString("Nie poprawna dlugosc wiadomosci powitalnej %d").arg(len);
                errB = true;
                return false;
            }

            uint8_t wzorzec[15] = {'K','O','N','T','R','O','L','E','R','P','O','S','R','E', 'D'};
            for (int i = 0; i < 15; ++i) {
                if (wzorzec[i] != data[i]) {
                    errT = QString("Nie zgodny wzorzec");
                    errB = true;
                    return false;
                }
            }
            qDebug("%s:%d", __FILE__, __LINE__);
            parseReply = WELCOME_REPLY;
            return true;
        }
        case ECHO_REP:
            parseReply = ECHO_REPLY;
            return true;

    case MOVEHOME_REP:
        {
            qDebug("%s %d MoveHome Msg", __FILE__,__LINE__);
            //set home position
            //0xB0 CRC8 - req
            //0xC1 s/K/E CRC8 - s=start,K=stop
            if (len == 4) {
                uint32_t word = (((unsigned int)data[0])<<24) + (((unsigned int)data[1])<<16) + (((unsigned int)data[2])<<8) + (unsigned int)data[3];
                qDebug("%s:%d %d", __FILE__, __LINE__, word); 
                steps = word;
                parseReply = MOVEHOME_REPLY;
                return true;
            }
            errT = QString("Nie poprawna dlugosc odpowiedzi movehome_rep %1").arg(len);
            errB = true;
            return false;
        }

        case POSITION_REP:
        {
            qDebug("%s %d Recv position Msg", __FILE__,__LINE__);
              if (len == 4) {
                uint32_t word = (((unsigned int)data[0])<<24) + (((unsigned int)data[1])<<16) + (((unsigned int)data[2])<<8) + (unsigned int)data[3];
                qDebug("%s:%d %d", __FILE__, __LINE__, word);
                steps = word;
                parseReply = POSITION_REPLY;
                return true;
            }
            errT = QString("Nie poprawna dlugosc odpowiedzi position_rep %1").arg(len);
            errB = true;
            return false;
        }

        case SET_PARAM_REP:
        {
            qDebug("%s %d Set params Msg", __FILE__,__LINE__);
            parseReply = SETPARAMS_REPLY;
            return true;
        }

        default:
            errT = QString("Nieznana komenda");
            errB = true;
            return false;
    }

    return false;
}

void SerialMessage::setInvalidReply()
{
    parseReply = INVALID_REPLY;
}

void SerialMessage::setInProgressReply()
{
    parseReply = INPROGRESS_REPLY;
}

void SerialMessage::setTimeoutReply(bool write)
{
    parseReply = write ? TIMEOUT_WRITE_REPLY : TIMEOUT_READ_REPLY;
}

bool SerialMessage::isInvalidReply()
{
    return parseReply == INVALID_REPLY;
}

bool SerialMessage::isInProgressReply()
{
    return parseReply == INPROGRESS_REPLY;
}

bool SerialMessage::isTimeoutWriteReply()
{
    return parseReply == TIMEOUT_WRITE_REPLY;
}

bool SerialMessage::isTimeoutReadReply()
{
    return parseReply == TIMEOUT_READ_REPLY;
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
    ret.append(arr);
    //emit debug(QString("Wysylam:")+QString(ret.toHex(' ').data()));
    return ret;
}

short SerialMessage::getParseReply() const
{
    return parseReply;
}

uint32_t SerialMessage::getSteps() const
{
    return steps;
}

