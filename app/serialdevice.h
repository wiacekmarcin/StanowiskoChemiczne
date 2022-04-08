#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QThread>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QByteArray>
#include "serialmessage.h"
#include "ustawienia.h"

class SerialDevice;
class SerialThread : public QThread
{

public:
    typedef enum _task {
        IDLE,
        CONNECT,
        CONFIGURE,
        RESET,
        SET_PARAMS,
        SET_POSITION,
        SET_HOME,
        SET_CYCLE,
        SET_STEPS
    } Task;

    explicit SerialThread(SerialDevice * sd, QObject *parent = nullptr);
    ~SerialThread();
    void command(Task);


private:
    void run() override;
    short nrZadania;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bool m_quit = false;
    Task actTask;
    SerialDevice * sd;
};

#define SET_GET_REVERSE_DECLARE(N) \
    bool getReverse##N() const;\
    void setReverse##N(bool newReverse##N);\

class SerialDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDevice(Ustawienia & u, QObject *parent = nullptr);
    ~SerialDevice();

    SET_GET_REVERSE_DECLARE(1)
    SET_GET_REVERSE_DECLARE(2)
    SET_GET_REVERSE_DECLARE(3)
    SET_GET_REVERSE_DECLARE(4)
    SET_GET_REVERSE_DECLARE(5)

    void setMaxImp(uint32_t imp);
    void setSettings(bool r1, bool r2, bool r3, bool r4, bool r5, uint32_t impMax, uint16_t timeImp);

    void setImpTime(uint16_t timeImp);
    void setPositionHome(uint8_t nrDoz);
    void setPosition(uint8_t nrDoz, uint32_t imp);
    void setReset();
    void setCykle(uint8_t nrDoz, uint32_t nrCyckli);
    void setImpulse(uint8_t nrDoz, uint64_t steps);

signals:

    void error(const QString &s);
    //void timeout(const QString &s);
    void debug(const QString &d);

    void resetDone(bool);
    void setParamsDone(bool);
    void dozownikConfigured(bool, bool);
    void errorSerial(QString);
    void setPositionHomeDone(bool);
    void setPositionDone(bool);
    void setCykleDone(bool);
    void setStepsDone(bool);
    //void successOpenDevice(bool);

protected:
    friend class SerialThread;
    void connectToSerialJob();
    bool configureDeviceJob();
    void setParamsJob();
    void setHomeJob();
    void setPosJob();
    void setResetJob();
    void setCykleJob();
    void setStepsJob();

    void closeDevice();
    bool openDevice();
    SerialMessage write(const QByteArray & req, int currentWaitWriteTimeout, int currentReadWaitTimeout);
    SerialMessage parseMessage(const QByteArray & reply);




private:
    //void run() override;

    QString m_portName;
    int portNr;
    //int handlePort;

    bool m_connected;
    bool m_configured;
    bool readError;

    QByteArray replyData;

    bool reverse1, reverse2, reverse3, reverse4, reverse5;
    uint32_t maxImp;
    uint16_t timeImp;
    SerialThread my_thread;
    Ustawienia & ustawienia;

    short dozownikNr;
    uint32_t val1;
    uint64_t val2;
};



#endif // SERIALDEVICE_H
