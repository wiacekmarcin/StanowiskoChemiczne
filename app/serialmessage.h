#ifndef SERIALMESSAGE_H
#define SERIALMESSAGE_H

#include <QObject>

#include <QTimer>

/**
 * @brief The SerialMessage class
 * Klasa reprezuntująca wiadomość wysyłaną do sterownika dozowników
 * NOP_REQ i NOP_REP - pusta wiadomość, WELCOME_REQ i WELCOME_REP -
 * wiadomość powitalna, SET_PARAM_REQ i SET_PARAM_REP - ustawianie
 * parametrów, POSITION_REQ i POSITION_REP - żądanie ustawienia
 * pozycji, ECHO_REQ i ECHO_REP - wiadomość echo, MOVEHOME_REQ i
 * MOVEHOME_REP - ustawianie pozycji bazowej, RESET_REQ i RESET_REP
 * - reset sterownika silnika, ERROR_REP - bład
 *
 * @param m_errorText - info o błedzie
 * @param m_errorBool - czy był bład parsowania
 * @param m_steps - ilość kroków
 * @param m_homePosition - status pozycji bazowych dla wszystkich dozowników
 * @param m_parseReply - odpowiedż po sparsowaniu
 */
class SerialMessage
{

public:

    explicit SerialMessage();

    ~SerialMessage();

    /**
     * Typy wiadomości
     */
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

    /**
     * Możliwe statusy odpowiedzi
     */
    typedef enum _action {
        INVALID_REPLY = -4,
        TIMEOUT_WRITE_REPLY = -3,
        TIMEOUT_READ_REPLY = -2,
        INPROGRESS_REPLY = 0,
        WELCOME_REPLY = 1,
        ECHO_REPLY,
        ECHO_REPLY2,
        MOVEHOME_REPLY,
        POSITION_REPLY,
        RESET_REPLY,
        SETPARAMS_REPLY,
    } ParseReply;

    /**
     * @brief setSettingsMsg - wiadomość ustawiająca parametry
     * @param reverse1 - kierunek silnika 1
     * @param reverse2 - kierunek silnika 2
     * @param reverse3 - kierunek silnika 3
     * @param reverse4 - kierunek silnika 4
     * @param reverse5 - kierunek silnika 5
     * @param maxImp - maksymalna ilość impulsów
     * @param imptime - czas pojedyńczego impulsu ; przekłada się to na prędkość dozowania
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setSettingsMsg(bool reverse1, bool reverse2, bool reverse3, bool reverse4, bool reverse5, uint32_t maxImp, uint16_t imptime);

    /**
     * @brief setSettingsMsg - krótsza wiadomość ustawiająca parametry
     * @param revByte reprezentacja bitowa kierunku silnika w postaci XX XX XX S5  S4 S3 S2 S1
     * @param maxImp - maksymalna ilość impulsów
     * @param imptime - czas pojedyńczego impulsu ; przekłada się to na prędkość dozowania
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setSettingsMsg(uint8_t revByte, uint32_t maxImp, uint16_t imptime);

    /**
     * @brief setDefaultSettingsMsg - ustawia domyślne parametry
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setDefaultSettingsMsg();

    /**
     * @brief echoMsg - wiadomość echo
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray echoMsg();

    /**
     * @brief echoMsg2 - wiadomość echo2 pobierająca także pozycje startową dozowników
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray echoMsg2();

    /**
     * @brief welcomeMsg - wiadomość powitalna
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray welcomeMsg();

    /**
     * @brief setPositionHome - wiadomość do ustawienia pozycji bazowej
     * @param DozownikNr - numer dozownika
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setPositionHome(short DozownikNr);

    /**
     * @brief setPosition - wiadomość do ustawienia konkretnej pozycji
     * @param DozwnikNr - numer dozownika
     * @param x - ilość kroków
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setPosition(short DozwnikNr, uint32_t x);

    /**
     * @brief setReset - wiadomość do resetowania sterownika silnika
     * @return bajty reprezentujące wiadomość
     */
    static QByteArray setReset();

    /*************************************************************/
    /**
     * @brief getSteps - ilość kroków (przy pozycji bazowej czy przy ustawianiu)
     * @return
     */
    uint32_t getSteps() const;

    /**
     * @brief getParseReply zwraca typ sparsowanej wiadomości
     * @return
     */
    ParseReply getParseReply() const;

    /**
     * @brief parseCommand - parsuje komendę
     * @param arr - bajty do sparsowania
     * @return - wynik czy poprawna instrukcja
     */
    bool parseCommand(const QByteArray &arr);

    /**
     * @brief setInvalidReply - ustawia status niepoprawna wiadomosc
     * @brief setInProgressReply - ustawia wiadomosc w trakcie realizacji
     * @brief setTimeoutReply - ustawia wiadomosc w stanie timeout
     * @param write - write/read Timeout
     **/
    void setInvalidReply();
    void setInProgressReply();
    void setTimeoutReply(bool write);
     /**
     * @brief isInvalidReply
     * @brief isInProgressReply
     * @brief isTimeoutWriteReply;
     * @brief isTimeoutReadReply;
     */
    bool isInvalidReply();
    bool isInProgressReply();
    bool isTimeoutWriteReply();
    bool isTimeoutReadReply();

    /**
     * @brief getHomePosition zwraca status pozycji bazowych
     * @return byte reprezentujący pozycję bazową w postaci XX XX XX D5   D4 D3 D2 D1
     * gdzie D to kolejny dozownik
     */
    uint8_t getHomePosition() const;

protected:

    bool checkHead(const QByteArray &arr, uint8_t & cmd, uint8_t & len, QByteArray & data);
    static QByteArray prepareMessage(uint8_t cmd, uint8_t tab[], uint8_t len);

private:
    QString m_errorText;
    bool m_errorBool;

    uint32_t m_steps;
    uint8_t m_homePosition;
    ParseReply m_parseReply;

};

#endif // SERIALMESSAGE_H
