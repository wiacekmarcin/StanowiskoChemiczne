#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <Arduino.h>
#include "crc8.h"



#define MAXLENPROTO 17

// |      HEAD       | n1 | n2 | .... | n15 | CRC8
// | CMD 4b | LEN 4b |
// LEN(MSG) = 1 (HEAD) + LEN + 1 (CRC8)

// getVersion
// 0x10 CRC8 - req
// 0x2f [15B] CRC8 - rep

// setError
//0x4f nrE CRC8 - rep

//setPos 
//0x58 X4 X3 X2 X1 CRC8 - req
//0x61 S/K CRC8 - reply setting position in proges S=start, K=end

//set home position
//0x90 CRC8 - req 
//0xa1 s/K CRC8 - s=start, K=stop

//set parameters
//0x3a 0x01 STATUSBYTE X4 X3 X2 X1 Y4 Y3 Y2 Y1 CRC8- ustawianie parametrow 
//0x39 0x02 X4 X3 X2 X1 Y4 Y3 Y2 Y1 CRC8- ustawianie parametrow 
//0x40 CRC8 - ACK

//echo 
//0x70 CRC8 - req echo
//0x80 CRC8 - reply echo

class Message {
    public:
    typedef enum _cmd {
        SET_PARAM_REQ = 3,
        SET_PARAM_REP = 4,
        POSITION_REQ = 5,
        POSITION_REP = 6,
        MOVEHOME_REQ = 9,
        MOVEHOME_REP = 10,
        RESET_REQ = 11,
        RESET_REP = 12,
        ERROR_REP = 15,
    } CMD;

    typedef enum _work {
        NOP,
        POS_START,
        RETURN_HOME,
        SETTING
    } Work;

    void init();
    bool check(unsigned char c);
    
    Work getStatusWork() const;
    uint8_t getNrDozownika() const;
    uint32_t getSteps() const;
    bool getReverse(uint8_t s);

    void setHomeDone(uint32_t steps) const;
    void setPosDone(uint32_t steps) const;
    void setResetDone() const;

    uint32_t parseNumber(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
protected:

    bool parse();
       

    void sendMessage(uint8_t cmd, uint8_t *buf, uint8_t len) const;
    void messageWrite(uint8_t *buf, uint8_t len) const;
    void sendError(uint8_t *buf, uint8_t len) { sendMessage (ERROR_REP, buf, len); }; 
    
    // rozkaz/dlugosc | 1 byte | 2 byte | 3 byte | 4 byte | crc
    uint8_t data[MAXLENPROTO + 4];

    uint8_t posCmd;
    uint8_t rozkaz;
    uint8_t dlugosc;
    CRC8 crc;

    Work actWork;
    uint8_t nrDozownika;
    uint32_t steps;
    uint8_t reverseByte;
};

    

    

#endif // !_PROTOCOL_H_