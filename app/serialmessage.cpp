#include "serialmessage.h"
#include "crc8.h"


#include <QDebug>

SerialMessage::SerialMessage()
{

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
    return prepareMessage(RESET_REQ, nullptr, 0);
}

QByteArray SerialMessage::echoMsg()
{
    uint8_t tab[1] = {0};
    return prepareMessage(ECHO_REQ, tab, 1);
}

QByteArray SerialMessage::echoMsg2()
{
    uint8_t tab[1] = {1};
    return prepareMessage(ECHO_REQ, tab, 1);
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
    m_errorText = "";
    m_errorBool = false;
    if (arr.length() == 0) {
        m_errorText = "Pusta komenda";
        m_errorBool = true;
        return false;
    }

    cmd = (arr[0] >> 4) & 0x0f;
    len = arr[0] & 0x0f;

    if (cmd == NOP_REP && len == 0xf)
        return true;
    CRC8 crc;
    data = QByteArray(len, Qt::Uninitialized);

    int i = 0;
    for (i = 0; i < arr.length() - 1 && i <= len; i++) {
        crc.add(arr.at(i));
        if (i) {
            data[i-1] = arr.at(i);
        }
    }

    if (i-1 != len) {
        m_errorText = QString("Nie poprawna dlugosc rozkazu %1 != %2").arg(i-1).arg(len);
        m_errorBool = true;
        return false;
    }
    if (i >= arr.size()) {
        m_errorText = QString("Za dlugi rozkaz %1 > 15").arg(i);
        m_errorBool = true;
        return false;
    }
    unsigned short msgcrc = arr.at(i) & 0xff;

    if (crc.getCRC() != msgcrc) {
        m_errorText = QString("Nie zgodne crc");
        m_errorBool = true;
        return false;
    }

    return true;
}

bool SerialMessage::parseCommand(const QByteArray &arr)
{
    uint8_t cmd;
    uint8_t len;
    QByteArray data;
    m_errorBool = false;
    m_parseReply = INVALID_REPLY;
    if (!checkHead(arr, cmd, len, data)) {
        return false;
    }

    switch (cmd) {
        case NOP_REP:
            return true;
        case WELCOME_REP:
        {
            if (len != 15) {
                m_errorText = QString("Nie poprawna dlugosc wiadomosci powitalnej %d").arg(len);
                m_errorBool = true;
                return false;
            }

            uint8_t wzorzec[15] = {'K','O','N','T','R','O','L','E','R','P','O','S','R','E', 'D'};
            for (int i = 0; i < 15; ++i) {
                if (wzorzec[i] != data[i]) {
                    m_errorText = QString("Nie zgodny wzorzec");
                    m_errorBool = true;
                    return false;
                }
            }
            m_parseReply = WELCOME_REPLY;
            return true;
        }
        case ECHO_REP:
        {
            if ((unsigned int)data[0] == 0 && len != 1) {
                m_errorText = QString("Nie poprawna dlugosc wiadomosci echo %1").arg(len);
                m_errorBool = true;
                return false;
            }
            if ((unsigned int)data[0] == 1 && len != 2) {
                m_errorText = QString("Nie poprawna dlugosc wiadomosci echo %1").arg(len);
                m_errorBool = true;
                return false;
            }
            if ((unsigned int)data[0] == 0) {
                m_parseReply = ECHO_REPLY;
                return true;
            }
            if ((unsigned int)data[0] == 1) {
                m_parseReply = ECHO_REPLY2;
                m_homePosition = (unsigned int)data[1];
                return true;
            }
            m_errorText = QString("Nie poprawna wiadomosc echo %d != [0,1]").arg(data[0]);
            m_errorBool = true;
            return false;
        }
        case MOVEHOME_REP:
        {
            if (len == 4) {
                uint32_t word = (((unsigned int)data[0])<<24) + (((unsigned int)data[1])<<16) + (((unsigned int)data[2])<<8) + (unsigned int)data[3];
                m_steps = word;
                m_parseReply = MOVEHOME_REPLY;
                return true;
            }
            m_errorText = QString("Nie poprawna dlugosc odpowiedzi movehome_rep %1").arg(len);
            m_errorBool = true;
            return false;
        }

        case POSITION_REP:
        {
            if (len == 4) {
                uint32_t word = (((unsigned int)data[0])<<24) + (((unsigned int)data[1])<<16) + (((unsigned int)data[2])<<8) + (unsigned int)data[3];
                m_steps = word;
                m_parseReply = POSITION_REPLY;
                return true;
            }
            m_errorText = QString("Nie poprawna dlugosc odpowiedzi position_rep %1").arg(len);
            m_errorBool = true;
            return false;
        }

        case SET_PARAM_REP:
        {
            m_parseReply = SETPARAMS_REPLY;
            return true;
        }

        case RESET_REP:
        {
            m_parseReply = RESET_REPLY;
            return true;
        }
        default:
            m_errorText = QString("Nieznana komenda");
            m_errorBool = true;
            return false;
    }

    return false;
}

void SerialMessage::setInvalidReply()
{
    m_parseReply = INVALID_REPLY;
}

void SerialMessage::setInProgressReply()
{
    m_parseReply = INPROGRESS_REPLY;
}

void SerialMessage::setTimeoutReply(bool write)
{
    m_parseReply = write ? TIMEOUT_WRITE_REPLY : TIMEOUT_READ_REPLY;
}

bool SerialMessage::isInvalidReply()
{
    return m_parseReply == INVALID_REPLY;
}

bool SerialMessage::isInProgressReply()
{
    return m_parseReply == INPROGRESS_REPLY;
}

bool SerialMessage::isTimeoutWriteReply()
{
    return m_parseReply == TIMEOUT_WRITE_REPLY;
}

bool SerialMessage::isTimeoutReadReply()
{
    return m_parseReply == TIMEOUT_READ_REPLY;
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
    return ret;
}

uint8_t SerialMessage::getHomePosition() const
{
    return m_homePosition;
}

SerialMessage::ParseReply SerialMessage::getParseReply() const
{
    return m_parseReply;
}

uint32_t SerialMessage::getSteps() const
{
    return m_steps;
}

