#pragma once

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
#if !defined L_COMP
    int32		error;
    TaskHandle	taskHandleRead;
#endif
    char		errBuff[2048];
};
