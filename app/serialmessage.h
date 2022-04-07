#ifndef SERIALMESSAGE_H
#define SERIALMESSAGE_H

#include <QObject>

#include <QTimer>

class SerialMessage
{

public:

    explicit SerialMessage();

    ~SerialMessage();

    typedef enum _cmd {
        NOP_REQ = 0,
        WELCOME_REQ = 1,
        WELCOME_REP = 2,
        SET_PARAM_REQ = 3,
        SET_PARAM_REP = 4,
        POSITION_REQ = 5,
        POSITION_REP = 6,
        ECHO_REQ = 7,
        ECHO_REP = 8,
        MOVEHOME_REQ = 9,
        MOVEHOME_REP = 10,
        RESET_REQ = 11,
        RESET_REP = 12,
        ERROR_REP = 14,
        NOP_REP = 15

    } CMD;

    typedef enum _action {
        INVALID_REPLY = -4,
        TIMEOUT_WRITE_REPLY,
        TIMEOUT_READ_REPLY,
        INPROGRESS_REPLY = 0,
        WELCOME_REPLY = 1,
        ECHO_REPLY,
        MOVEHOME_REPLY,
        POSITION_REPLY,
        RESET_REPLY,
        SETPARAMS_REPLY,
    } ParseReply;

    static QByteArray setSettingsMsg(bool reverse1, bool reverse2, bool reverse3, bool reverse4, bool reverse5, uint32_t maxImp, uint16_t imptime);
    static QByteArray setSettingsMsg(uint8_t revByte, uint32_t maxImp, uint16_t imptime);
    static QByteArray setDefaultSettingsMsg();
    static QByteArray echoMsg();
    static QByteArray welcomeMsg();
    static QByteArray setPositionHome(short DozownikNr);
    static QByteArray setPosition(short DozwnikNr, uint32_t x);
    static QByteArray setReset();

    uint32_t getSteps() const;
    short getParseReply() const;
    bool parseCommand(const QByteArray &arr);
    void setInvalidReply();
    void setInProgressReply();
    void setTimeoutReply(bool write);
    bool isInvalidReply();
    bool isInProgressReply();
    bool isTimeoutWriteReply();
    bool isTimeoutReadReply();
protected:

    bool checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len, QByteArray & data);

    static QByteArray prepareMessage(uint8_t cmd, uint8_t tab[], uint8_t len);

private:

    QByteArray cmd;
    unsigned short lenCmd;

    QString errT;
    bool errB;

    uint32_t steps;
    short parseReply;

};

#endif // SERIALMESSAGE_H
