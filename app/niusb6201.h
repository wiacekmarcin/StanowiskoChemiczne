#pragma once

#include <NIDAQmx.h>

class NIDAQMxUSB6501
{
public:
	NIDAQMxUSB6501();
	~NIDAQMxUSB6501();

    bool configure();
    bool readValue(uInt16 & dataRead);
    bool writeValue(uInt16& dataWrite);

protected:
    void errorFun();

private:
    int32		error;
    TaskHandle	taskHandleRead;
    TaskHandle	taskHandleWrite;
    uInt32		dataRead;
    uInt8		dataWrite[9];
    char		errBuff[2048];
    int32		read;
};
