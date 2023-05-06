#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QThread>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QList>
#include <QByteArray>
#include <QTimer>

#include "serialmessage.h"
#include "ustawienia.h"

class SerialDevice;
class QThread;
/**
 * @brief The SerialWorker class
 * Klasa wątku w którym chodzi sterowanie dozownikime
 * Normalnie wątek jest zawieszony do czasu podania i
 * intrukcji za pomocą funkcji command.
 * @param actTask - aktualne wykonywane zadanie, na IDLE - wątek "wisi"
 * @param mutex - ustawianie zadania chronione mutexem
 * @param newTask - zmienna informujaca, że jest nowe zadanie
 * @param sd - pointer to device
 */
class SerialWorker : public QThread
{
    Q_OBJECT
public:
    /**
     * Zadania,
     * IDLE -nic nierób, CONNECT - staraj się połączyć z urządzeniem, CONFIGURE - skonfiguruj urządzenie,
     * RESET - resetuj sterownik silników, SET_PARAMS - ustaw parametry sterownika silników,
     * SET_POSITION - ustaw konkretną pozycję dozownika, SET_HOME - ustaw dozownik w pozycji bazowej,
     * SET_CYCLE - ustaw dozownik w pozycji maksymalnej (SET_POSITION MAX) i następnie ustaw dozownik
     * w pozycji bazowej (SET_HOME), SET_STEPS - ustaw ilość kroków , jeżeli jest większa niż maksymalna
     * następuje SET_CYCLE i ustawienie SET_POSITION o ilość kroków pomniejszoną o maksymalną liczbę kroków
     * i następnie SET_HOME, SET_ECHO2 - pobranie informacji czy dozowniki są w pozycji bazowej
     */
    typedef enum _task {
        IDLE,
        CONNECT,
        CONFIGURE,
        RESET,
        SET_PARAMS,
        SET_POSITION,
        SET_HOME,
        SET_CYCLE,
        SET_STEPS,
        SET_ECHO2,
        SET_ECHO,
        CLOSE_DEVICE
    } Task;

    explicit SerialWorker(SerialDevice * device);
    ~SerialWorker();

    /**
     * @brief command
     * Ustawienie nowego zadania
     * @param task - nowe zadanie IDLE, CONNECT, CONFIGURE, RESET, SET_PARAMS, SET_POSITION, SET_HOME, SET_CYCLE,
     * SET_STEPS, SET_ECHO2
     */
    void command(Task task);

    void commandNoRepeat(Task task);

    /**
     * @brief setStop
     * Zatrzymanie pracy workera
     */
    void setStop();

protected:
    /**
     * @brief run
     * Fukcja workera
     */
    void run();


private:
    QList<Task> actTask;
    QMutex mutex;
    QWaitCondition newTask;
    bool runWorker;
    SerialDevice * sd;
};

#define SET_GET_REVERSE_DECLARE(N) \
    bool getReverse##N() const;\
    void setReverse##N(bool newReverse##N);\

/**
 * @brief The SerialDevice class
 * Klasa urzadzenia szeregowego, jest odpowiedzialna za sterowanie dozownikiem
 * @param m_portName - nazwa portu szeregowego
 * @param m_portNr - numer portu szeregowego na windowsie np 7 (czyli COM7)
 * @param m_connected - czy urządzenie jest w systemie
 * @param m_configured - czy urządzenie jest skonfigurowane
 * @param m_reverse - konfiguracja obrotów silnika
 * @param m_maxImp - maksymalna ilość impulsów
 * @param m_timeImp - czas imulsu 125 min
 * @param m_worker - obiekt który realizuje komunikację w wątku
 * @param m_ustawienia - obiekt ustawień
 * @param dozownikNr - zmienna tymczasowa, do przechowywania numer dozownika
 * @param val1, val2 - zmienna tymczasowa do przechowania np ilości kroków
 * @param homePositionMask - zmienna tymczasowa do przechowywania info o pozycji bazowej
 */
class SerialDevice : public QObject
{
    Q_OBJECT
public:
    explicit SerialDevice(Ustawienia & u, QObject *parent = nullptr);
    ~SerialDevice();

    /**
     * @brief setThreads
     * Ustawia wątek do obsługi procesu
     * @param trh
     */
    void setThread(QThread * trh);

    /**
     * @brief setStop
     * Zatrzymanie pracy workera
     */
    void setStop();

    /**
     * @brief
     * Get i Set dla zmiennej m_reverse
     */
    SET_GET_REVERSE_DECLARE(1)
    SET_GET_REVERSE_DECLARE(2)
    SET_GET_REVERSE_DECLARE(3)
    SET_GET_REVERSE_DECLARE(4)
    SET_GET_REVERSE_DECLARE(5)

    /**
     * @brief setMaxImp Ustawia maksymalną ilość impulsów
     * @param imp
     */
    void setMaxImp(uint32_t imp);

    /**
     * @brief setSettings - ustawia parametry sterownika silnika
     * @param r1 - kierunek obrotów 1 silnika
     * @param r2 - kierunek obrotów 2 silnika
     * @param r3 - kierunek obrotów 3 silnika
     * @param r4 - kierunek obrotów 4 silnika
     * @param r5 - kierunek obrotów 5 silnika
     * @param impMax - maksymalna ilość impulsów
     * @param timeImp - czas impulsów
     */
    void setSettings(bool r1, bool r2, bool r3, bool r4, bool r5, uint32_t impMax, uint16_t timeImp);

    /**
     * @brief setImpTime - ustawia czas impulsów silnika krokowego (prędkość napełniania)
     * @param timeImp - czas impulsów
     */
    void setImpTime(uint16_t timeImp);

    /************ Funkcje sterujące *************************************/
    /**
     * @brief setPositionHome - ustawia pozycje bazową dla dozownika
     * @param nrDoz - numer dozownika
     */
    void setPositionHome(uint8_t nrDoz);

    /**
     * @brief setSteps - ustawia ilość kroków dla przesunięcia
     * @param nrDoz - numer dozownika
     * @param imp - ilość impulsów, może być większ niż ilość maksymalna
     * wtedy następuje zjazd do pozycji bazowej i dalsze dozowanie
     */
    void setSteps(uint8_t nrDoz, uint64_t imp);

    /**
     * @brief setReset - resetuje sterownik silnika
     */
    void setReset();

    /**
     * @brief setCykle - ustawia ilość pelnych cykli czyli ustawienia pozycji maksymalnej i powrót do pozycji bazowej
     * @param nrDoz - numer dozownika
     * @param nrCyckli - ilość cykli
     */
    void setCykle(uint8_t nrDoz, uint32_t nrCyckli);

    /**
     * @brief setPosition - ustawia pozycje ilość kroków jest mniejsz niż maksymalna
     * @param nrDoz - numer dozownika
     * @param steps - ilość kroków
     */
    void setPosition(uint8_t nrDoz, uint32_t steps);

    /**
     * @brief checkPositionHome - sprawdza pozycje bazową dla wszystkich dozowników
     */
    void checkPositionHome();


signals:
    /**
     * @brief error wystąpił bład
     * @param s info o błędzie
     */
    void error(const QString &s);

    /**
     * @brief debug Informacje debugowe
     * @param d
     */
    void debug(const QString &d);

    /**
     * @brief resetDone - resetowanie sterownika zakończone
     * @param success - true powodzenie, false niepowodzenie
     */
    void resetDone(bool success);

    /**
     * @brief setParamsDone - ustawianie parametrów zakończone
     * @param success - true powodzenie, false niepowodzenie
     */
    void setParamsDone(bool success);

    /**
     * @brief dozownikConfigured - znaleziono sterownik dozowników i skonfigurowano
     * @param conn - połączone
     * @param conf - skonfigurowano
     */
    void dozownikConfigured(bool conn, bool conf);

    /**
     * @brief setPositionHomeDone - ustawianie pozycji bazowej zakończone
     * @param success - true powodzenie, false niepowodzenie
     */
    void setPositionHomeDone(bool success);

    /**
     * @brief setPositionDone - ustawianie pozycji zakończone
     * @param success - true powodzenie, false niepowodzenie
     */
    void setPositionDone(bool success);

    /**
     * @brief setCykleDone - cykle (ustawianie maks pozycji i powrót do bazy) zakończone
     * @param success - true powodzenie, false niepowodzenie
     */
    void setCykleDone(bool success);

    /**
     * @brief setStepsDone - ustawianie ilości kroków (dozowanie) zakończone
     * @param success - true powodzenie, false niepowodzenie
     */
    void setStepsDone(bool success);

    /**
     * @brief checkPositionHomeDone - sprawdzanie pozycji bazowej dla dozownikow
     * @param ok - true powodzenie, false niepowodzenie
     * @param h1 - true, dozownik 1 w pozycji bazowej
     * @param h2 - true, dozownik 2 w pozycji bazowej
     * @param h3 - true, dozownik 3 w pozycji bazowej
     * @param h4 - true, dozownik 4 w pozycji bazowej
     * @param h5 - true, dozownik 5 w pozycji bazowej
     */
    void checkPositionHomeDone(bool ok, bool h1, bool h2, bool h3, bool h4, bool h5);

    /*********************  JOBY ******************************************/
protected:
    friend class SerialWorker;
    /**
     * @brief connectToSerialJob - wyszukiwanie i otworzenie sterownika
     */
    void connectToSerialJob();

    void setConnected(bool conn);
    bool getConnected();

    /**
     * @brief configureDeviceJob - konfiguracja sterownika dozowników
     * @return
     */
    bool configureDeviceJob();

    /**
     * @brief setParamsJob - ustawia paremetry sterownika silnika
     */
    void setParamsJob();

    /**
     * @brief setHomeJob - ustawia pozycję bazową dla wybranego dozownika
     */
    void setHomeJob();

    /**
     * @brief setPosJob - ustawia żądana pozycję
     */
    void setPosJob();

    /**
     * @brief setResetJob - resetuje sterownik silników
     */
    void setResetJob();

    /**
     * @brief setCykleJob ustawia kilka dozownik w pozycji maksymalnej i bazowej
     */
    void setCykleJob();

    /**
     * @brief setStepsJob - ustawia ilość kroków większą niż maksymalna ilość kroków
     */
    void setStepsJob();

    /**
     * @brief setCheckHomeJob - sprawdza czy sterownik żyje
     */
    void setCheckHomeJob();

    /**
     * @brief setEchoJob - sprawdza czy sterownik żyje
     */
    void setEchoJob();
    /**
     * @brief setCloseDeviceJob - sprawdza czy sterownik żyje
     */
    void setCloseDeviceJob();
    /********************************** INNE FUNKCJE *************************/
    /**
     * @brief closeDevice zamyka urzadzenia
     */
    void closeDevice();

    /**
     * @brief openDevice - otwiera urzadzenie
     * @return
     */
    bool openDevice();

    /**
     * @brief write - zapisz
     * @param req - dane do zapisu
     * @param currentWaitWriteTimeout
     * @param currentReadWaitTimeout
     * @return
     */
    SerialMessage write(const QByteArray & req, int currentWaitWriteTimeout, int currentReadWaitTimeout);

    /**
     * @brief parseMessage
     * @param reply
     * @return
     */
    SerialMessage parseMessage(const QByteArray & reply);

protected slots:
    void updateDevice();


private:
    /* Ustawienia obiektu */
    QString m_portName;
    int m_portNr;
    bool m_connected;
    bool m_configured;
    Ustawienia & m_ustawienia;
    SerialWorker m_worker;

    /* Ustawianie dozownika */
    bool m_reverse1, m_reverse2, m_reverse3, m_reverse4, m_reverse5;
    uint32_t m_maxImp;
    uint16_t m_timeImp;

    /* Zmienne pomocnicze do komunikacji z dozownikami */
    short dozownikNr;
    uint32_t val1;
    uint64_t val2;
    uint8_t homePositionMask;

    QTimer checkTimer;
    QMutex mutex; 
};



#endif // SERIALDEVICE_H
