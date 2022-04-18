#include "niusb6210.h"

#if !SYMULATOR

#include <QDebug>


#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else


NIDAQMxUSB6210::NIDAQMxUSB6210()
{
    error = 0;
    taskHandleRead = nullptr;
    errBuff[2047] = { '\0' };
}

NIDAQMxUSB6210::~NIDAQMxUSB6210()
{
    if (taskHandleRead != nullptr) {
        DAQmxStopTask(taskHandleRead);
        DAQmxClearTask(taskHandleRead);
        taskHandleRead = nullptr;
    }
}

bool NIDAQMxUSB6210::isConnected()
{
    return taskHandleRead != nullptr ;
}

bool NIDAQMxUSB6210::configure(const QString & deviceString)
{
    if (isConnected())
        return true;
    
    /*********************************************/
    // DAQmx Configure Code
    /*********************************************/
    DAQmxErrChk(DAQmxCreateTask("readAnalogValues", &taskHandleRead));
    
    DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandleRead, deviceString.toStdString().c_str(),
            "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL));
    
    DAQmxErrChk(DAQmxStartTask(taskHandleRead));
    

    return true;

Error:
    errorFun();
    
    return false;
}

bool NIDAQMxUSB6210::readValue(float& val1, float& val2, float& val3, float& val4, float& val5, float& val6, float& val7)
{
    static short tryRead = 0;
    
    if (!isConnected())
        return false;
    int32       read;
    
    float64 val[8];
    tryRead++;
    DAQmxErrChk(DAQmxReadAnalogF64(taskHandleRead, 1, 0.1, DAQmx_Val_GroupByChannel, val, 7, &read, NULL));
    tryRead = 0;
    val1 = val[0];
    val2 = val[1];
    val3 = val[2];
    val4 = val[3];
    val5 = val[4];
    val6 = val[5];
    val7 = val[6];

    
    return true;

Error:
    if (tryRead == 1) {
        return readValue(val1, val2, val3, val4, val5, val6, val7);
    }
    errorFun();
    
    return false;
}

void NIDAQMxUSB6210::errorFun()
{
    if (DAQmxFailed(error))
        DAQmxGetExtendedErrorInfo(errBuff, 2048);

    if (taskHandleRead != nullptr) {
        DAQmxStopTask(taskHandleRead);
        DAQmxClearTask(taskHandleRead);
        taskHandleRead = nullptr;
    }
}

std::string NIDAQMxUSB6210::errStr()
{
    return std::string(errBuff);
}

#endif

