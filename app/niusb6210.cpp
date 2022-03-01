#include "niusb6210.h"

#include <QDebug>

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

NIDAQMxUSB6210::NIDAQMxUSB6210()
{
    error = 0;
    taskHandleRead = nullptr;
    taskHandleWrite = nullptr;
    for (int i = 0; i < 16; ++i)
        dataWrite[i] = 0;

    errBuff[2048] = { '\0' };
    read = 0;;
}

NIDAQMxUSB6210::~NIDAQMxUSB6210()
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

bool NIDAQMxUSB6210::isConnected()
{
    return taskHandleRead != nullptr /* && taskHandleWrite != nullptr */;
}

bool NIDAQMxUSB6210::configure()
{
    /*********************************************/
    // DAQmx Configure Code
    /*********************************************/
    DAQmxErrChk(DAQmxCreateTask("readValues", &taskHandleRead));
    //qDebug("%d task = %p", __LINE__, taskHandleRead);
    DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle, "USB6210/ai0, USB6210/ai1, USB6210/ai2, USB6210/ai3, USB6210/ai4, USB6210/ai5", 
        "", DAQmx_Val_Cfg_Default, 0.0, 10.0, DAQmx_Val_Volts, NULL));
    //qDebug("%d task = %p", __LINE__, taskHandleRead);
    DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle, "", 10000.0, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, 1000));

    
    
    /* NIE WSPIERA PULL UP*/
    //DAQmxErrChk(DAQmxSetDigitalPullUpPullDownStates("USB6210", "USB6210/port0, USB6210/port1/Line0", DAQmx_Val_PullUp, NULL));
    //qDebug("%d task = %p", __LINE__, taskHandleRead);
    DAQmxErrChk(DAQmxStartTask(taskHandleRead));
    //qDebug("%d task = %p", __LINE__, taskHandleRead);


    return true;

Error:
    errorFun();
    qDebug("%d %p %p %s", __LINE__, taskHandleRead, taskHandleWrite, errStr().c_str());
    return false;
}

bool NIDAQMxUSB6210::readValue(uInt16& val)
{
    //qDebug("%d task = %d", __LINE__, taskHandleRead);
    DAQmxErrChk(DAQmxReadDigitalU32(taskHandleRead, 1, 10.0, DAQmx_Val_GroupByChannel, &dataRead, 1, &read, NULL));
    val = dataRead & 0x01ff;
    return true;

Error:
    errorFun();
    qDebug("%d %d %s", __LINE__, taskHandleRead, errStr().c_str());
    return false;
}

bool NIDAQMxUSB6210::writeValue(uInt16& val)
{
    qDebug("%d write %04x", __LINE__, val);
    for (int i = 0; i < 10; ++i)
        dataWrite[i] = (val >> i) & 0x1;

    for (int i = 0; i < 10; ++i)
        qDebug("%d", dataWrite[i]);
    qDebug("==");

    DAQmxErrChk(DAQmxWriteDigitalLines(taskHandleWrite, 1, 1, 10.0, DAQmx_Val_GroupByChannel, dataWrite, NULL, NULL));

    return true;

Error:
    errorFun();
    return false;
}

void NIDAQMxUSB6210::errorFun()
{
    if (DAQmxFailed(error))
        DAQmxGetExtendedErrorInfo(errBuff, 2048);

    if (taskHandleWrite != nullptr) {
        DAQmxStopTask(taskHandleWrite);
        DAQmxClearTask(taskHandleWrite);
        taskHandleWrite = nullptr;
    }

    if (taskHandleRead != nullptr) {
        DAQmxStopTask(taskHandleRead);
        DAQmxClearTask(taskHandleRead);
        taskHandleRead = nullptr;
    }

    //if (DAQmxFailed(error))
    //    printf("DAQmx Error: %s\n", errBuff);
    //printf("End of program, press Enter key to quit\n");
    //getchar();
    //return 0;
}

std::string NIDAQMxUSB6210::errStr()
{
    return std::string(errBuff);
}