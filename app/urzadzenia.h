#ifndef URZADZENIA_H
#define URZADZENIA_H

#include <QObject>
#include <QString>

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
 */
class Urzadzenia : public QObject
{
    Q_OBJECT
public:
    explicit Urzadzenia(Ustawienia & ustawiania, QObject *parent = nullptr);

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
    void zaplon(rodzajZaplonu idiskra);

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


public slots:
    void ni_analogValueChanged(double val0, double val1, double val2, double val3, double val4, double val5, double val6);
    void ni_digitalRead(uint16_t vals);
    void digitalWriteDebug(uint16_t vals);
    void readInputs();

private slots:

    void ni_error(const QString &s);
    void ni_debug(const QString &d);
    void ni_usb6501(bool open, bool conf);

    void ds_error(const QString &s);
    void ds_debug(const QString &d);
    void ds_errorSerial(const QString &s);
    void ds_dozownikConfigured(bool open, bool conf);

protected:
    void runIskraElektryczna();
    void runIskraMechaniczna();
    void runPlomien();
    void setSteps(uint8_t nrDoz, uint32_t impuls);

private slots:
    void runIskraElektryczna1();
    void runIskraElektryczna2();
    void runIskraMechaniczna1();
    void runPlomien1();

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
};

#endif // URZADZENIA_H

