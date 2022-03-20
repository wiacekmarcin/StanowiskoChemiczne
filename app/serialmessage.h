#ifndef SERIALMESSAGE_H
#define SERIALMESSAGE_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QTimer>

class SerialMessage : public QObject
{

    Q_OBJECT
public:

    explicit SerialMessage(QObject *parent = nullptr);

    ~SerialMessage();
    void closeDevice();

    typedef enum _cmd {
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

    } CMD;

    bool getReverse() const;
    void setReverse(bool newReverse);

    bool getReverse1() const;
    void setReverse1(bool newReverse);

    bool getReverse2() const;
    void setReverse2(bool newReverse);

    bool getReverse3() const;
    void setReverse3(bool newReverse);

    bool getReverse4() const;
    void setReverse4(bool newReverse);

    bool getReverse5() const;
    void setReverse5(bool newReverse);

    uint32_t getMaxImp() const;
    void setMaxImp(uint32_t newMaxImp);

    void setSettings(bool reverse, uint32_t maxImp);
    void setSettings5(bool reverse1, bool reverse2, bool reverse3, bool reverse4, bool reverse5, uint32_t maxImp);
    void connectToSerial();
    void echo();
    void setPositionHome(short DozownikNr);
    void setPosition(short DozwnikNr, uint32_t x);
    void setReset();
    void setParameters();

signals:
    void dozownik(bool conn);
    void errorSerial(QString);
    void debug(QString);
    void donePositionHome(bool ok);
    void donePosition();
    void successOpenDevice(bool success);
    void setParamsDone();
    void echoOK(bool ok);
    void resetDone();



public slots:
    void handleReadyRead();
    void serialError(const QSerialPort::SerialPortError & error);
    void response(const QByteArray &s);

protected:
    bool openDevice(const QSerialPortInfo & port);

    QByteArray welcomeMsg();
    QByteArray homePositionMsg(short DozownikNr);
    QByteArray positionMsg(short DozownikNr, uint32_t x);
    QByteArray echoMsg();

    bool checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len, QByteArray & data);
    bool parseCommand(const QByteArray &arr);
    QByteArray prepareMessage(uint8_t cmd, uint8_t tab[], uint8_t len);
    void writeMessage(const QByteArray &writeData);
    QByteArray settingsMsg(bool reverse, uint32_t maxImp);
    QByteArray settingsMsg5(uint8_t byteRev, uint32_t maxImp);
    QByteArray resetMsg();


private:
    //MasterThread mt;
    QSerialPort m_serialPort;
    QString portName;
    bool connSerial;

    QList<QByteArray> commands;
    QByteArray cmd;
    unsigned short lenCmd;

    short lenEcho;
    bool reverse;
    uint8_t revByte;
    uint32_t maxImp;

};

#endif // SERIALMESSAGE_H
