#include <NIDAQmx.h>
#include <string>

class NIDAQMxUSB6210
{
public:
    NIDAQMxUSB6210();
    ~NIDAQMxUSB6210();

    bool configure();
    bool readValue(float & val1, float& val2, float& val3, float& val4, float& val5, float& val6);
    
    std::string errStr();

    bool isConnected();
protected:
    void errorFun();

private:
    int32		error;
    TaskHandle	taskHandleRead;
    float64		dataRead[6*100*10];
    char		errBuff[2048];
};
#pragma once
