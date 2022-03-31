#ifndef NICARDS_H
#define NICARDS_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "ustawienia.h"
#include "niusb6210.h"
#include "niusb6501.h"


#if SYMULATOR

class TestAnalog {
public:
    bool configure(const QString &) { return true; }
    bool readValue(float & val1, float& val2, float& val3, float& val4, float& val5, float& val6, float& val7){
        val1 = val2 = val3 = val4 = val5 = val6 = val7 = 0; return true;
    }
    std::string errStr() { return ""; }
    bool isConnected() { return true; }
};

class TestDigital {
public:
    bool configure(const QString &, const QString &) {return true; }
    bool readValue(uint16_t & dataRead) { dataRead = 0; return true; }
    bool writeValue(const uint16_t&) { return true; }
    std::string errStr() { return ""; }
    bool isConnected() { return true; }
};

#endif

class NICards : public QThread
{
    Q_OBJECT
public:
    explicit NICards(QObject *parent = nullptr);
    ~NICards();

    

    void digitalWrite(uint16_t out, bool val);

    void digitalWriteDebug(uint16_t out) { maskOutput = out; }
    uint16_t getDigitalWrite() const { return maskOutput; }


signals:
    void digitalRead(uint16_t vals);
    void error(const QString &s);
    void debug(const QString &d);

    void usb6210(bool open, bool conf);
    void usb6501(bool ok, bool conf);
    void analogValueChanged(double val0, double val1, double val2, double val3, double val4, double val5, double val6);

protected:
    void run() override;
    void find();
    void analogConfigure();
    void digitalConfigure();
    void resetDevice(bool analog, bool digital);

    void readAnalog();
    void readDigital();
    void writeDigital();

private:
    uint16_t maskInput;
    uint16_t maskOutput;

    QMutex m_mutex;
#if !SYMULATOR
    NIDAQMxUSB6210 analog;
    NIDAQMxUSB6501 digital;
#else
    TestAnalog analog;
    TestDigital digital;
#endif
    bool anConf;

    bool digConf;

    bool m_quit;
    const QString readAnalString;
    const QString writeDigString;
    const QString readDigString;

    QString analogDevice;
    QString analogConfString;
    QString digitalDevice;
    QString digitalConfReadString;
    QString digitalConfWriteString;

};
#endif


