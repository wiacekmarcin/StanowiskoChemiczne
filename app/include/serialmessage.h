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

signals:
    void successOpenDevice(bool);
    void deviceName(QString);

    void controllerOK();

    void startingPosition();
    void donePosition();
    void errorReadFromRadio();
    void readedFromRadio(int val);
    void echoOK();

    void startingHome();
    void doneHome();
    void errorHome();

    void setParams();
    void setParamsDone();

    void errorSerial(QString);
    //void timeoutSerial(QString, bool, QString);

    void debug(QString);

public slots:
    void handleReadyRead();
    void serialError(const QSerialPort::SerialPortError & error);


    void connectToSerial();
    void echo();
    void setPositionHome(short DozownikNr);
    void setSettings(bool reverse, uint32_t maxImp);
    void doneSettings();
    void setPosition(short DozwnikNr, uint32_t x);


    void response(const QByteArray &s);
    //void errorRead(const QString &s);
    //void timeoutRead(const QString &s);
    //void errorWrite(const QString &s);
    //void timeoutWrite(const QString &s);
    //void findPort(const QString &s);


protected:
    bool openDevice(const QSerialPortInfo & port);

    QByteArray welcomeMsg();
    QByteArray homePositionMsg();
    QByteArray positionMsg(uint32_t x);
    QByteArray echoMsg();

    bool checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len, QByteArray & data);
    bool parseCommand(const QByteArray &arr);
    QByteArray prepareMessage(uint8_t cmd, uint8_t tab[], uint8_t len);
    void writeMessage(const QByteArray &writeData);
    QByteArray settingsMsg(bool reverse, uint32_t maxImp);


private:
    //MasterThread mt;
    QSerialPort m_serialPort;
    QString portName;
    bool connSerial;

    QList<QByteArray> commands;
    QByteArray cmd;
    unsigned short lenCmd;

};

#endif // SERIALMESSAGE_H
