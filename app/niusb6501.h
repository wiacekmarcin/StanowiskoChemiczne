#ifndef _NIUSB6501_H_
#define _NIUSB6501_H_
#include "common.h"
#if !SYMULATOR
#include <QString>
#include <NIDAQmx.h>

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
    void reset();
protected:
    void errorFun();

    void del();
    void ini();

private:
    int32		error;
#ifndef L_COMP
    TaskHandle	taskHandleRead;
    TaskHandle	taskHandleWrite;
#endif
    uInt32		dataRead;
    uInt8		dataWrite[16];

    char		errBuff[2048];
    int32		read;
};
#endif
#endif
