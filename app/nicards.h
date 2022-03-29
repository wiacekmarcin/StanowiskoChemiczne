#ifndef NICARDS_H
#define NICARDS_H

#ifndef SYMULATOR

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "ustawienia.h"
#include "niusb6210.h"
#include "niusb6501.h"


class NICards : public QThread
{
    Q_OBJECT
public:
    explicit NICards(QObject *parent = nullptr);
    ~NICards();

    

    void digitalWrite(uint16_t out, bool val);

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

    NIDAQMxUSB6210 analog;
    bool anConf;
    NIDAQMxUSB6501 digital;
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
#endif


