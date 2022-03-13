#include "niusb6501.h"

#ifndef L_COMP
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

bool NIDAQMxUSB6501::configure(const QString & readDevice, const QString & writeDevice)
{
    qDebug("%s %s", readDevice.toStdString().c_str(), writeDevice.toStdString().c_str());
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

    return true;

Error:
    errorFun();
    qDebug("%d %p %p %s", __LINE__, taskHandleRead, taskHandleWrite, errStr().c_str());
    return false;
}

bool NIDAQMxUSB6501::readValue(uInt16 & val)
{
    if (!isConnected())
        return false;

    //qDebug("%d task = %d", __LINE__, taskHandleRead);
    DAQmxErrChk(DAQmxReadDigitalU32(taskHandleRead, 1, 10.0, DAQmx_Val_GroupByChannel, &dataRead, 1, &read, NULL));

    uint16_t corrval = (dataRead & 0x00ff) | (((dataRead & 0xff00) >> 5));
    val = (~corrval) & 0x01ff;

    //qDebug("%04x %04x %04x ", dataRead, corrval, val);
    return true;

Error:
    errorFun();
    qDebug("%d %d %s", __LINE__, taskHandleRead, errStr().c_str());
    return false;
}

bool NIDAQMxUSB6501::writeValue(uInt16& val)
{
    if (!isConnected())
        return false;

    qDebug("%d write %04x", __LINE__, val);
    for (int i = 0; i < 10; ++i) {
        qDebug("%d val_wr=%d", __LINE__, ((~val >> i) & 0x1));
        dataWrite[i] = ((~val >> i) & 0x1);
    }

    for (int i = 0; i < 10; ++i)
        //qDebug("%d", dataWrite[i]);
    //qDebug("==");

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
#endif
