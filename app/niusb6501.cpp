#include "niusb6501.h"

#include <QDebug>

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

NIDAQMxUSB6501::NIDAQMxUSB6501()
{
    error = 0;
    taskHandleRead = nullptr;
    taskHandleWrite = nullptr;
    for (int i = 0; i < 16; ++i)
        dataWrite[i] = 0;

    errBuff[2048] = { '\0' };
    read = 0;;
}

NIDAQMxUSB6501::~NIDAQMxUSB6501()
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

bool NIDAQMxUSB6501::isConnected()
{
    return taskHandleRead != nullptr /* && taskHandleWrite != nullptr */;
}

bool NIDAQMxUSB6501::configure()
{
    /*********************************************/
    // DAQmx Configure Code
    /*********************************************/
    DAQmxErrChk(DAQmxCreateTask("readValues", &taskHandleRead));
    //qDebug("%d task = %p", __LINE__, taskHandleRead);
    DAQmxErrChk(DAQmxCreateDIChan(taskHandleRead, "USB6501/port0, USB6501/port1/Line0", "", DAQmx_Val_ChanForAllLines));
    //qDebug("%d task = %p", __LINE__, taskHandleRead);
    /* NIE WSPIERA PULL UP*/
    //DAQmxErrChk(DAQmxSetDigitalPullUpPullDownStates("USB6501", "USB6501/port0, USB6501/port1/Line0", DAQmx_Val_PullUp, NULL));
    //qDebug("%d task = %p", __LINE__, taskHandleRead);
    DAQmxErrChk(DAQmxStartTask(taskHandleRead));
    //qDebug("%d task = %p", __LINE__, taskHandleRead);

    
    DAQmxErrChk(DAQmxCreateTask("writeValue", &taskHandleWrite));
    DAQmxErrChk(DAQmxCreateDOChan(taskHandleWrite, "USB6501/port2/Line0:6,USB6501/port1/Line4:7", "", DAQmx_Val_ChanForAllLines));
    DAQmxErrChk(DAQmxStartTask(taskHandleWrite));

    //DAQmxErrChk(DAQmxWriteDigitalLines(taskHandleWrite, 1, 1, 10.0, DAQmx_Val_GroupByChannel, dataWrite, NULL, NULL));

    return true;

Error:
    errorFun();
    qDebug("%d %p %p %s", __LINE__, taskHandleRead, taskHandleWrite, errStr().c_str());
    return false;
}

bool NIDAQMxUSB6501::readValue(uInt16 & val)
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

bool NIDAQMxUSB6501::writeValue(uInt16& val)
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

void NIDAQMxUSB6501::errorFun()
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

std::string NIDAQMxUSB6501::errStr()
{
    return std::string(errBuff);
}