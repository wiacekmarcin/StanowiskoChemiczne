#include <NIDAQmx.h>
#include <string>

class NIDAQMxUSB6210
{
public:
    NIDAQMxUSB6210();
    ~NIDAQMxUSB6210();

    bool configure();
    bool readValue(uInt16& dataRead);
    bool writeValue(uInt16& dataWrite);
    std::string errStr();

    bool isConnected();
protected:
    void errorFun();

private:
    int32		error;
    TaskHandle	taskHandleRead;
    TaskHandle	taskHandleWrite;
    uInt32		dataRead;
    uInt8		dataWrite[16];
    char		errBuff[2048];
    int32		read;
};
#pragma once
