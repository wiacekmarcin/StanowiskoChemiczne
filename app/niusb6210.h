#pragma once
#include <string>
#if !defined L_COMP

#include <NIDAQmx.h>
#include <QString>

class NIDAQMxUSB6210
{
public:
    NIDAQMxUSB6210();
    ~NIDAQMxUSB6210();

    bool configure(const QString &deviceString);
    bool readValue(float & val1, float& val2, float& val3, float& val4, float& val5, float& val6, float& val7);
    
    std::string errStr();

    bool isConnected();
protected:
    void errorFun();

private:
    int32		error;
    TaskHandle	taskHandleRead;
    char		errBuff[2048];
};
#else
typedef uint16_t uInt16;
class NIDAQMxUSB6210
{
public:
    NIDAQMxUSB6210() { }
    ~NIDAQMxUSB6210() { }

    bool configure() { return true; }
    bool readValue(float & val1, float& val2, float& val3, float& val4, float& val5, float& val6, float& val7)
    { val1 = 0; val2 = 0; val3 = 0; val4 = 0; val5 = 0; val6 = 0; val7 = 0; return true; }

    std::string errStr() { return std::string(""); }

    bool isConnected() { return true; };
};


#endif
