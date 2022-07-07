#include "niusb6501.h"


#if !SYMULATOR

#include <QDebug>
#include "ustawienia.h"

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

NIDAQMxUSB6501::NIDAQMxUSB6501()
{
    ini();
}

NIDAQMxUSB6501::~NIDAQMxUSB6501()
{

}

bool NIDAQMxUSB6501::isConnected()
{
    return taskHandleRead != nullptr && taskHandleWrite != nullptr ;
}

void NIDAQMxUSB6501::reset()
{
    del();
    ini();
}

bool NIDAQMxUSB6501::configure(const QString & readDevice, const QString & writeDevice)
{
    
    if (isConnected())
        return true;
    /*********************************************/
    // DAQmx Configure Code
    /*********************************************/
    DAQmxErrChk(DAQmxCreateTask("readValues", &taskHandleRead));
    
    DAQmxErrChk(DAQmxCreateDIChan(taskHandleRead, readDevice.toStdString().c_str(), "", DAQmx_Val_ChanForAllLines));
    
    DAQmxErrChk(DAQmxStartTask(taskHandleRead));
    
    DAQmxErrChk(DAQmxCreateTask("writeValue", &taskHandleWrite));
    
    DAQmxErrChk(DAQmxCreateDOChan(taskHandleWrite, writeDevice.toStdString().c_str(), "", DAQmx_Val_ChanForAllLines));
    
    DAQmxErrChk(DAQmxStartTask(taskHandleWrite));
    

    //uInt8 dataWriteNow[16] = {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    //DAQmxErrChk(DAQmxWriteDigitalLines(taskHandleWrite, 1, 1, 10.0, DAQmx_Val_GroupByChannel, dataWriteNow, NULL, NULL));

    return true;

Error:
    errorFun();
    
    return false;
}

bool NIDAQMxUSB6501::readValue(uInt16 & val)
{
    if (!isConnected())
        return false;
    
    DAQmxErrChk(DAQmxReadDigitalU32(taskHandleRead, 1, 10.0, DAQmx_Val_GroupByChannel, &dataRead, 1, &read, NULL));
    {
        uint16_t corrval = (dataRead & 0x00ff) | (((dataRead & 0xff00) >> 4));
        val = (~corrval) & 0x01ff;
    }
    
    return true;

Error:
    errorFun();
    
    return false;
}

bool NIDAQMxUSB6501::writeValue(uInt16& val)
{
    if (!isConnected())
        return false;
    
    for (int i = 0; i < Ustawienia::maxCzujekCyfrOut; ++i) {
        
        dataWrite[i] = ((~val >> i) & 0x1);
    }

    
    //        dataWrite[5],dataWrite[4],dataWrite[3],dataWrite[2],dataWrite[1],dataWrite[0]);
    //for (int i = 0; i < 10; ++i)
        
    

    DAQmxErrChk(DAQmxWriteDigitalLines(taskHandleWrite, 1, 1, 10.0, DAQmx_Val_GroupByChannel, dataWrite, NULL, NULL));
    
    return true;

Error:
    errorFun();
    return false;
}

void NIDAQMxUSB6501::errorFun()
{
    if (DAQmxFailed(error))
        DAQmxGetExtendedErrorInfo(errBuff, 2048);

    del();


    //if (DAQmxFailed(error))
    //    printf("DAQmx Error: %s\n", errBuff);
    //printf("End of program, press Enter key to quit\n");
    //getchar();
    //return 0;
}

void NIDAQMxUSB6501::del()
{
    if (taskHandleRead != nullptr) {
        DAQmxStopTask(taskHandleRead);
        DAQmxClearTask(taskHandleRead);
        taskHandleRead = nullptr;
    }

    if (taskHandleWrite != nullptr) {
        DAQmxStopTask(taskHandleWrite);
        DAQmxClearTask(taskHandleWrite);
        taskHandleWrite = nullptr;
    }
}

void NIDAQMxUSB6501::ini()
{
    error = 0;
    taskHandleRead = nullptr;
    taskHandleWrite = nullptr;
    for (int i = 0; i < 16; ++i)
        dataWrite[i] = 0;

    errBuff[2047] = { '\0' };
    read = 0;;
}

std::string NIDAQMxUSB6501::errStr()
{
    return std::string(errBuff);
}

#endif
