#ifndef URZADZENIA_H
#define URZADZENIA_H

#include <QObject>
#include <QString>
#include <QFile>

#include "nicards.h"
#include "ustawienia.h"
#include "serialdevice.h"

class QThread;


/**
 * @brief The Urzadzenia class
 * Klasa zarządzająca urządzeniami - zarówno sterownikiem dozownika ja i kartami National Instruments
 *
 * @param m_NI_Cards - klasa zawierająca sterowanie kartami National Instrument - 1 wątek cykliczny co 100ms
 * @param m_serialDev - klasa zawierająca sterowania dozownikiem - 1 wątek wiszący
 * @param m_ustawienia - referencja do klasy ustawienia - zawierająca ustawienia dla dozownika i kart sterujących
 * @param m_digCardsOk - czy udało się połączyć z urządzeniem NI Cards (dla funkcji zapisu - odczyt i tak nie działa)
 * @param m_dozownikOk - czy jest skonfigurowany dla dozownika - dla funkcji zapisu
 * @param m_inputs - zmienna przechowująca aktualne wejścia
 * @param plTimer - timery dla płomienia
 * @param imTimer - timery dla iskry mechanicznej
 * @param elTimer  - timer dla iskry elektrycznej
 * @param m_fileLog - plik do którego będą zapisywane dane logujące
 */
class Urzadzenia : public QObject
{
    Q_OBJECT
public:
    explicit Urzadzenia(Ustawienia & ustawiania, QObject *parent = nullptr);
    ~Urzadzenia();

    /**
     * @brief setThreads - ustawia wątki dla odpowiednich klas
     * @param niReads - wątek dla odczytu z kart NI
     * @param dozownik - wątek dla odczytu z sterownika dozownika
     */
    void setThreads(QThread * niReads, QThread * dozownik);

    /**
     * @brief setStop - funkcja zatrzymująca wątki
     */
    void setStop();

public slots:
    /**
     * @brief setCykle - Dozowanie wstępne (napełnianie strzykawki)
     * @param nrDoz - numer dozownika
     * @param nrCyckli - numer cykli (maksymalne 'wychylenie' i powrót do pozycji bazowej)
     */
    void setCykle(uint8_t nrDoz, uint32_t nrCyckli);

    /**
     * @brief setMl - ustaw odpowiednią ilość ml
     * @param nrDoz - numer dozownika
     * @param ml - ilość ml
     */
    void setMl(uint8_t nrDoz, uint32_t ml);

    /**
     * @brief digitalWriteAll - zapisz do wyjścia
     * @param vals - maska wyjść 1 na bicie oznacza załączenie wyjścia
     */
    void digitalWriteAll(uint16_t vals);

    /**
     * @brief digitalWrite - zapisz do wyjścia
     * @param outDevice - urządzenia do włączenia
     * @param on - czy załączyć
     */
    void digitalWrite(digitalOut outDevice, bool on);

    /**
     * @brief zaplon - procedura zapłonu iskry elektrycznej/mechanicznej/płomienia
     * @param idiskra - rodzaj iskry
     */
    void zaplon(ZaplonRodzaj idiskra);

    /**
     * @brief checkPositionHome - sprawdza czy dozowniki są na pozycji bazowej
     */
    void checkPositionHome();

    /**
     * @brief resetDozownik - resetuje kontroler silników
     */
    void resetDozownik();

signals:
    /**
     * @brief analogValueChanged - zmiana wartości analogowych
     * @param val1 - vol1 [%]
     * @param val2 - vol2 [%]
     * @param val3 - o2 [%]
     * @param val4 - co2 [%]
     * @param val5 - cisnienie komory [kPa]
     * @param val6 - temperatura komory [C]
     * @param val7 - temperatura parownika [C]
     * @param val8 - temperatura parownika [C]
     */
    void analogValueChanged(double val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8);

    /**
     * @brief digitalValueReadChanged - zmienione zostały wyjścia
     * @param vals - wyjście
     * @param open - czy zostało otwarte (0 - zamknięte, 1 - otwarte)
     */
    void digitalReadValueChanged(digitalIn vals, bool open);

    /**
     * @brief digitalAllRead - zmienione zostały wejścia
     * @param vals - maska wejść, 1 na bicie oznacza urządzenie otwarte
     */
    void digitalReadAllValueChanged(uint16_t vals);

    /**
     * @brief digitalWriteDevice - zostały zmienione wyjścia
     * @param vals - maska bitowa wyjść
     */
    void digitalWriteValueChanged(uint16_t vals);

    /**
     * @brief usb6210 - czy urządzenie zostało znaleziono i skonfigurowane
     * @param open - czy zostało znalezione
     * @param conf - czy zostało skonfigurowane
     */
    void usb6210(bool open, bool conf);

    /**
     * @brief usb6501 - czy urządzenie zostało znalezione i skonfigurowane
     * @param open - czy zostało znalezione
     * @param conf - czy zostało skonfigurowane
     */
    void usb6501(bool open, bool conf);

    /**
     * @brief dozownik - czy urządzenie zostało znalezione i skonfigurowane
     * @param open - czy zostało znalezione
     * @param conf - czy zostało skonfigurowane
     */
    void dozownik(bool open, bool conf);

    /**
     * @brief connDozownik - czy dozownik jest połączony
     * @param success - czy operacja zakończyła się powodzeniem
     */
    void connDozownik(bool success);

    /**
     * @brief resetDone - resetowanie sterownika silnika zakończyła się sukcesem
     * @param success - czy operacja zakończyła się powodzeniem
     */
    void resetDone(bool success);

    /**
     * @brief setParamsDone - ustawianie parametrów dozownika zakończyło się sukcesem
     * @param success - czy operacja zakończyła się powodzeniem
     */
    void setParamsDone(bool success);

    /**
     * @brief setPositionHomeDone - ustawianie pozycji bazowej
     * @param success - czy operacja zakończyła się powodzeniem
     */
    void setPositionHomeDone(bool success);

    /**
     * @brief setPositionDone - ustawianie danej pozycji
     * @param success - czy operacja zakończyła się powodzeniem
     */
    void setPositionDone(bool success);

    /**
     * @brief setCykleDone - wstępne dozowanie
     * @param success - czy operacja zakończyła się powodzeniem
     */
    void setCykleDone(bool success);

    /**
     * @brief setStepsDone - ustaw żądana ilość kroków (ml)
     * @param success - czy operacja zakończyła się powodzeniem
     */
    void setStepsDone(bool success);

    /**
     * @brief checkPositionHomeDone - sprawdzenie czy dany dozownik jest na pozycji bazowej
     * @param success - czy operacja zakończyła się powodzeniem
     * @param d1 - dozownik 1 w pozycji bazowej
     * @param d2 - dozownik 2 w pozycji bazowej
     * @param d3 - dozownik 3 w pozycji bazowej
     * @param d4 - dozownik 4 w pozycji bazowej
     * @param d5 - dozownik 5 w pozycji bazowej
     */
    void checkPositionHomeDone(bool ok, bool d1, bool d2, bool d3, bool d4, bool d5);

    void log(const QString & d);
public slots:
    /**
     * @brief ni_analogValueChanged - przeychwocony sygnał analogValueChanged i wartośći
     * przemnożone przez odpowiednie współczynniki i
     * @param val0 - vol1 [V]
     * @param val1 - vol2 [V]
     * @param val2 - o2 [V]
     * @param val3 - co2 [V]
     * @param val4 - cisnienie komory [V]
     * @param val5 - temperatura komory [V]
     * @param val6 - temperatura parownika [V]
     */
    void ni_analogValueChanged(double val0, double val1, double val2, double val3, double val4, double val5, double val6);

    /**
     * @brief ni_digitalRead
     * @param vals - maska bitowa wejść
     */
     void ni_digitalReadAllValueChanged(uint16_t vals);
    
    /** @brief żądanie odczytu wszystkich zmiennych
     */
    void readInputs();


    void gui_error(const QString &s);

    void gui_debug(const QString &d);

    void test_error(const QString &s);

    void test_debug(const QString &d);

private slots:

    /**
     * @brief ni_error - funkcja odbierający error z nicards
     * @param s - komunikat błedu
     */
    void ni_error(const QString &s);

    /**
     * @brief ni_debug - funkcja odbierająca debug z nicards
     * @param d - komunikat
     */
    void ni_debug(const QString &d);

    /**
     * @brief ds_error - funkcja odbierający error z nicards
     * @param s - komunikat błedu
     */
    void ds_error(const QString &s);

    /**
     * @brief ds_debug - funkcja odbierająca debug z nicards
     * @param d - komunikat
     */
    void ds_debug(const QString &d);

    /**
     * @brief urz_error - funkcja odbierający error z nicards
     * @param s - komunikat błedu
     */
    void urz_error(const QString &s);

    /**
     * @brief urz_debug - funkcja odbierająca debug z nicards
     * @param d - komunikat
     */
    void urz_debug(const QString &d);


    /**
     * @brief ni_usb6501 - czy karta cyfrowa jest w prawidłowo skonfigurowana
     * @param open
     * @param conf
     */
    void ni_usb6501(bool open, bool conf);

    /**
     * @brief ds_dozownikConfigured - czy dozownik został skonfigurowany
     * @param open
     * @param conf
     */
    void ds_dozownikConfigured(bool open, bool conf);

    void triggerKamery();

protected:
    /**
     * @brief runIskraElektryczna - uruchomienie procedury iskry elektrycznej
     */
    void runIskraElektryczna();

    /**
     * @brief runIskraMechaniczna - uruchomienie procedury iskry mechanicznej
     */
    void runIskraMechaniczna();

    /**
     * @brief runPlomien - uruchomienie procedury płomienia
     */
    void runPlomien();

private slots:
    /**
     * @brief runIskraElektryczna1
     * @brief runIskraElektryczna2()
     * @brief runIskraElektryczna2()
     * @brief runIskraMechaniczna1
     * @brief runPlomien1
     * Kolejne procedury wyzwalaniu zapłonu
     */
    void runIskraElektryczna1();
    void runIskraElektryczna2();
    void runIskraMechaniczna1();
    void runPlomien1();

protected:
    void logSlot(const QString & msg);

private:

     NICards m_NI_Cards;
     SerialDevice m_serialDev;
     Ustawienia & m_ustawienia;

     bool m_digCardsOk;
     bool m_dozownikOk;

     uint16_t m_inputs;

     QTimer plTimer;
     QTimer imTimer;
     QTimer elTimer;

     QFile * m_logFile;
     uint16_t prevInputs;

     QMutex mutex;
};

#endif // URZADZENIA_H

