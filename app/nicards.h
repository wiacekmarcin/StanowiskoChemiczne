#ifndef NICARDS_H
#define NICARDS_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "ustawienia.h"
#include "niusb6210.h"
#include "niusb6501.h"
#include <QTimer>

#if SYMULATOR

class TestAnalog {
public:
    bool configure(const QString &) { return true; }
    bool readValue(float & val1, float& val2, float& val3, float& val4, float& val5, float& val6, float& val7){
        val1 = val2 = val3 = val4 = val5 = val6 = val7 = 0; return true;
    }
    std::string errStr() { return ""; }
    bool isConnected() { return true; }
    void reset() { };
};

class TestDigital {
public:
    bool configure(const QString &, const QString &) {return true; }
    bool readValue(uint16_t & dataRead) { dataRead = 0; return true; }
    bool writeValue(const uint16_t&) { return true; }
    std::string errStr() { return ""; }
    bool isConnected() { return true; }
    void reset() { };
};

#endif

/**
 * @brief The NICards class
 * Klasa która zarządza odczytem i zapisem do klas National Instrumental
 * @param anConf - czy skonfigurowana jest karta analogowa
 * @param digConf - czy skonfigurowana jest karta cyfrowa
 * @param m_quit - czy zakończyć wątek
 * @param readAnalString - stały string do konfiguracji karty analogowej
 * @param writeDigString - stały string do konfiguracji karty cyfrowej - zapis
 * @param readDigString - stały string do konfiguracji karty cyfrowej - odczyt
 * @param analogDevice - nazwa karty analogwej;
 * @param digitalDevice - nazwa karty cyfrowej
 * @param digitalConfReadString - string do konfiguracji karty cyfrowej odczyt
 * @param digitalConfWriteString - string do konfiguraji karty cyfrowej zapis
 * @param prevInputs - poprzedni stan wejść cyfrowych - impuls tylko jak zmieniły się wejścia;
 */

class NICards : public QThread
{
    Q_OBJECT
public:
    explicit NICards();
    ~NICards();

    /**
     * @brief setThreads - ustawiam wątek
     * @param thr1 - wątek w którym będzie pracował zapis i odczyt z kart cyfrowych i analogowych
     */
    void setThreads(QThread * thr1);

    /**
     * @brief setStop - koniec pracy workera
     */
    void setStop();
    
    /**
     * @brief digitalWrite zapisz pojedyńcze wejście
     * @param out - maska bitowa pojedyńczego wejścia
     * @param val - wartość wyjścia - true zapal przekaźnik
     */
    void digitalWrite(digitalOut out, bool val);

    /**
     * @brief digitalWriteAll
     * @param out - maska bitowa wszystkich wejść jeżeli na danym bicie jest 1
     * oznacza załączenie danego wejścia
     */
    void digitalWriteAll(uint16_t out) { maskOutput = out; }

    /**
     * @brief getDigitalWrite - pobiera maskę włączonych wyjść
     * @return maska włączonych wyjść
     */
    uint16_t getDigitalWrite() const { return maskOutput; }

    /**
     * @brief getDigitalRead pobiera aktualny stan wejść
     * @return aktualny stan wejść
     */
    uint16_t getDigitalRead() const { return maskInput; }

signals:
    /**
     * @brief digitalReadValueChanged - odczytano wejścia
     * @param vals - maska bitowa wejść
     */
    void digitalReadValueChanged(uint16_t vals);

    /**
     * @brief analogValueChanged - zmiana wartości analogowych
     * @param val0 - vol1 [V]
     * @param val1 - vol2 [V]
     * @param val2 - o2 [V]
     * @param val3 - co2 [V]
     * @param val4 - cisnienie komory [V]
     * @param val5 - temperatura komory [V]
     * @param val6 - temperatura parownika [V]
     */
    void analogValueChanged(double val0, double val1, double val2, double val3, double val4, double val5, double val6);

    /**
     * @brief error - wystąpił błąd
     * @param s - komunikat błędu
     */
    void error(const QString &s);

    /**
     * @brief debug - informacje debugowe
     * @param d - komunikat
     */
    void debug(const QString &d);

    /**
     * @brief usb6210 - sygnał o konfiguracji urządzenia
     * @brief usb6501 - sygnał o konfiguracji urządzenia
     * @param open - znaleziono
     * @param conf - skonfigurowano
     */
    void usb6210(bool open, bool conf);
    void usb6501(bool ok, bool conf);

protected slots:
    /**
     * @brief analogConfigure - konfiguracja karty analogowej
     */
    void analogConfigure();

    /**
     * @brief digitalConfigure - konfiguracja karty cyfrowej
     */
    void digitalConfigure();


protected:
    /**
     * @brief run - główna pętla wątku
     */
    void run() override;

    /**
     * @brief find - funkcja znajduje urzadzenia
     * @return - czy udało się znaleźć karty
     */
    bool find();



    /**
     * @brief resetDevice - reset kart
     * @param analog - karta analogowa
     * @param digital - karta cyfrowa
     */
    void resetDevice(bool analog, bool digital);

    /**
     * @brief readAnalog - odczytaj dane analogowe
     */
    void readAnalog();

    /**
     * @brief readDigital - odczytaj dane cyfrowe
     */
    void readDigital();

    /**
     * @brief writeDigital - zapisz dane cyfrowe
     */
    void writeDigital();

private:
    uint16_t maskInput;
    uint16_t maskOutput;

    QMutex m_mutex;
#if SYMULATOR
    TestAnalog analog;
    TestDigital digital;
#else
    NIDAQMxUSB6210 analog;
    NIDAQMxUSB6501 digital;
#endif
    bool m_anConf;
    bool m_digConf;
    bool m_quit;

    const QString readAnalString;
    const QString writeDigString;
    const QString readDigString;

    QString analogDevice;
    QString analogConfString;
    QString digitalDevice;
    QString digitalConfReadString;
    QString digitalConfWriteString;

    uint16_t prevInputs;
    uint16_t prevOutputs;

    QTimer anTimer;
    QTimer digTimer;
};
#endif


