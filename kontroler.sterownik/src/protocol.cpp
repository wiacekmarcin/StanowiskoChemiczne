#include "protocol.hpp"



void Message::init() 
{
    posCmd = 0;
    rozkaz = 0;
    dlugosc = 0;
    crc.restart();
}

bool Message::check(unsigned char c)
{
    data[posCmd++] = c;
    if (posCmd-1 == 0) {    
        crc.restart();
        crc.add(data[0]);
        rozkaz = data[0] >> 4;
        dlugosc = data[0] & 0x0f;
        return false;
    }
    
    if (posCmd == dlugosc + 2) {
        uint8_t c = crc.getCRC();
        if (data[posCmd-1] == c) {
            posCmd = 0;
            bool r = parse();
            if (!r)
                //sendError("ZLY ROZKAZ", 10);
                ;
            return r;
        }
        posCmd = 0;
        //sendError("ZLE CRC ", 7);
        return false;

    }

    crc.add(data[posCmd-1]);    
    
    
    if (posCmd == MAXLENPROTO) {
        posCmd = 0;
        //sendError("ZBYT DUZA WIAD.", 15);
        return false;    
    }
    return false;
}

Message::Work Message::getStatusWork() const 
{ 
    return actWork; 
}

bool Message::parse() {

    switch(rozkaz) {
        
        // get info |HEAD| CRC
        
        case POSITION_REQ: 
        {
            
            actWork = POS_START;
            return true;
        }
        
        case MOVEHOME_REQ:
        {
            
            actWork = RETURN_HOME;
            return true;
        }
        case SET_PARAM_REQ:
        {
            actWork = SET_PARAM;
            return true;
        }
        
        default:
            break;

    }
    rozkaz = 0;
    return false;
}

void Message::sendMessage(uint8_t cmd, uint8_t* buf, uint8_t len)
{
    if (len > 15)
        return;
    
    uint8_t sendData[MAXLENPROTO] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    if (len > 0)
        memcpy(sendData+1, buf, len);
    sendData[0] = cmd << 4 | len ;
    CRC8 lcrc;
    lcrc.restart();
    lcrc.add(sendData, len+1);
    sendData[len+1] = lcrc.getCRC();
    messageWrite(sendData, len+2);
}

void Message::messageWrite(uint8_t* buf, uint8_t len)
{
    Serial1.write(buf, len);
}


