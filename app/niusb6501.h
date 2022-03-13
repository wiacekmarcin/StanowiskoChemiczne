#pragma once
#include <string>
#ifndef L_COMP
#include <NIDAQmx.h>
#include <QString>

class NIDAQMxUSB6501
{
public:
	NIDAQMxUSB6501();
	~NIDAQMxUSB6501();

    bool configure(const QString &readDevice, const QString &writeDevice);
    bool readValue(uInt16 & dataRead);
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
#else

typedef uint16_t uInt16;
class NIDAQMxUSB6501
{
public:
    NIDAQMxUSB6501() {}
    ~NIDAQMxUSB6501() {}

    bool configure() { return true; }
    bool readValue(uInt16 & dataRead) { dataRead = 0; return true; }
    bool writeValue(uInt16& /*dataWrite*/) { return true; }
    std::string errStr() { return std::string("ee"); }

    bool isConnected() { return true; }
};
#endif

