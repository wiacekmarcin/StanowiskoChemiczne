#include "niusb6210.h"

#ifndef L_COMP
#include <QDebug>

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

NIDAQMxUSB6210::NIDAQMxUSB6210()
{
    error = 0;
    taskHandleRead = nullptr;

    errBuff[2048] = { '\0' };
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
    qDebug("%d %p %s", __LINE__, taskHandleRead, errStr().c_str());
    return false;
}

bool NIDAQMxUSB6210::readValue(float& val1, float& val2, float& val3, float& val4, float& val5, float& val6, float& val7)
{
    if (!isConnected())
        return false;

    int32       read;
    
    float64 val[8];

    DAQmxErrChk(DAQmxReadAnalogF64(taskHandleRead, 1, 1.0, DAQmx_Val_GroupByChannel, val, 7, &read, NULL));

    val1 = val[0];
    val2 = val[1];
    val3 = val[2];
    val4 = val[3];
    val5 = val[4];
    val6 = val[5];
    val7 = val[6];

    return true;

Error:
    errorFun();
    qDebug("%d %d %s", __LINE__, taskHandleRead, errStr().c_str());
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
