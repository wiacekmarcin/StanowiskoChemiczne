#include "protocol.hpp"
#include "silnik.hpp"


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
    //Serial.print("recv=");
    //Serial.print((char)c);
    //Serial.print(" ");
    //Serial.println(c, HEX);

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
            if (!r) {
                sendError("ZLY ROZKAZ", 10);
                Serial.println("Bad command");
            }
            return r;
        }
        posCmd = 0;
        sendError("ZLE CRC ", 7);
        Serial.println("Bad command");
        return false;

    }

    crc.add(data[posCmd-1]);    
    
    
    if (posCmd == MAXLENPROTO) {
        posCmd = 0;
        sendError("ZBYT DUZA WIAD.", 15);
        Serial.println("Too big message");
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
            nrDozownika = data[1];
            steps = Silnik::maxSteps = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | data[5];
            actWork = POS_START;
            return true;
        }
        
        case MOVEHOME_REQ:
        {
            nrDozownika = data[1];
            actWork = RETURN_HOME;
            return true;
        }
        case SET_PARAM_REQ:
        {
            reverseByte = data[1];
            Silnik::maxSteps = (data[2] << 24) + (data[3] << 16) + (data[4] << 8) + data[5];
            Silnik::mls_motor = (data[6] << 8) + data[7];
            if (Silnik::mls_motor == 0)
                Silnik::mls_motor = 200;
            Serial.print("MaxSteps=");
            Serial.print(Silnik::maxSteps, DEC);
            Serial.print("mls_motor=");
            Serial.println(Silnik::mls_motor, DEC);
            sendMessage(SET_PARAM_REP, NULL, 0);
            actWork = SETTING;
            return true;
        }
        
        default:
            sendError("NIEZNANA WIAD.", 14);
            Serial.println("Unknown Message");
            break;

    }
    rozkaz = 0;
    return false;
}

void Message::sendMessage(uint8_t cmd, uint8_t* buf, uint8_t len) const
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

void Message::messageWrite(uint8_t* buf, uint8_t len) const
{
    Serial1.write(buf, len);
}

uint8_t Message::getNrDozownika() const 
{
    return nrDozownika;
}

uint32_t Message::getSteps() const 
{
    return steps;
}

void Message::setHomeDone(uint32_t steps) const
{
    uint8_t tab[4];
    tab[0] = (steps >> 24) & 0xff;
    tab[1] = (steps >> 16) & 0xff;
    tab[2] = (steps >> 8) & 0xff;
    tab[3] = steps & 0xff;
    sendMessage(MOVEHOME_REP, tab, 4);
}

void Message::setPosDone(uint32_t steps) const
{
    uint8_t tab[4];
    tab[0] = (steps >> 24) & 0xff;
    tab[1] = (steps >> 16) & 0xff;
    tab[2] = (steps >> 8) & 0xff;
    tab[3] = steps & 0xff;
    sendMessage(POSITION_REP, tab, 4);
}

void Message::setResetDone() const
{
    sendMessage(RESET_REP, nullptr, 0);
}

bool Message::getReverse(uint8_t s)
{
    return reverseByte & (0x1 << s);
}