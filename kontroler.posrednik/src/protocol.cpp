#include "protocol.hpp"

//10 imp silnika na 1 mm
#include "platform.h"


void Message::init() 
{
    posCmd[0] = 0;
    posCmd[1] = 0;
    rozkaz[0] = 0;
    rozkaz[1] = 0;
    dlugosc[0] = 0;
    dlugosc[1] = 0;
    crc[0].restart();
    crc[1].restart();
}

bool Message::check(uint8_t s, unsigned char c)
{
    data[s][posCmd[s]++] = c;
    if (posCmd[s]-1 == 0) {    
        crc[s].restart();
        crc[s].add(data[s][0]);
        rozkaz[s] = data[s][0] >> 4;
        dlugosc[s] = data[s][0] & 0x0f;
        return false;
    }
    
    if (posCmd[s] == dlugosc[s] + 2) {
        uint8_t c = crc[s].getCRC();
        if (data[s][posCmd[s]-1] == c) {
            posCmd[s] = 0;
            bool r = parse(s);
            if (!r)
                //sendError("ZLY ROZKAZ", 10);
                ;
            return r;
        }
        posCmd[s] = 0;
        //sendError("ZLE CRC ", 7);
        return false;

    }

    crc[s].add(data[s][posCmd[s]-1]);    
    
    
    if (posCmd[s] == MAXLENPROTO) {
        posCmd[s] = 0;
        //sendError("ZBYT DUZA WIAD.", 15);
        return false;    
    }
    return false;
}

bool Message::check1(unsigned char c) 
{ 
    return check(0, c); 
}

bool Message::check2(unsigned char c) 
{ 
    return check(1, c); 
}

Message::Work Message::getStatusWork() const 
{ 
    return actWork; 
}

bool Message::parse1() {

    switch(rozkaz[0]) {
        
        // get info |HEAD| CRC
        case WELCOME_REQ:   //get info 
        {                          //1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  
            uint8_t sendData[15] = {'K','O','N','T','R','O','L','E','R','P','O','S','R','E', 'D'};
            sendMessage1(WELCOME_REP, sendData, 15);
            //actWork = NOP;
            return true;
        }
        case POSITION_REQ: 
        {
            sendRawMessage2(data[0], dlugosc[0] + 2);            
            //actWork = POS_START;
            return true;
        }
        case ECHO_REQ: 
        {
            sendMessage1(ECHO_REP, nullptr, 0);
            //actWork = NOP;
            return true;
        }
        case MOVEHOME_REQ:
        {
            sendRawMessage2(data[0], dlugosc[0]+2);
            //actWork = RETURN_HOME;
            return true;
        }
        case SET_PARAM_REQ:
        {
            //actWork = SET_PARAM;
            sendRawMessage2(data[0], dlugosc[0]+2);
            return true;
        }
        
        default:
            break;

    }
    rozkaz[0] = 0;
    return false;
}

bool Message::parse2() {

    switch(rozkaz[1]) {
        
        // get info |HEAD| CRC
        case POSITION_REP: 
        {
            sendRawMessage1(data[1], dlugosc[1] + 2);            
            //actWork = NOP;
            return true;
        }
        case MOVEHOME_REP:
        {
            sendRawMessage1(data[1], dlugosc[1]+2);
            //actWork = NOP;
            return true;
        }
        case SET_PARAM_REP:
        {
            //actWork = NOP;
            sendRawMessage1(data[1], dlugosc[1]+2);
            return true;
        }
        
        default:
            break;

    }
    rozkaz[1] = 0;
    return false;
}

void Message::sendMessage(uint8_t s, uint8_t cmd, uint8_t* buf, uint8_t len)
{
    if (len > 15)
        return;
    
    uint8_t sendData[MAXLENPROTO] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    if (len > 0)
        memcpy(sendData+1, buf, len);
    sendData[0] = cmd << 4 | len ;
    crc[s].restart();
    crc[s].add(sendData, len+1);
    sendData[len+1] = crc[s].getCRC();
    if (s == 0)
        messageWrite1(sendData, len+2);
    else
        messageWrite2(sendData, len+2);
}

void Message::sendRawMessage(uint8_t s, uint8_t* buf, uint8_t len)
{
    if (s == 0)
        messageWrite1(buf, len);
    else
        messageWrite2(buf, len);
}

void Message::messageWrite1(uint8_t* buf, uint8_t len)
{
    Serial.write(buf, len);
}

void Message::messageWrite2(uint8_t* buf, uint8_t len)
{
    Serial1.write(buf, len);
}

