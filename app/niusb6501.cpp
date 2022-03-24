#include "niusb6501.h"


#include <QDebug>
#ifndef L_COMP
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else
#endif

NIDAQMxUSB6501::NIDAQMxUSB6501()
{
    error = 0;
#ifndef L_COMP
    taskHandleRead = nullptr;
    taskHandleWrite = nullptr;
#endif
    for (int i = 0; i < 16; ++i)
        dataWrite[i] = 0;

    errBuff[2047] = { '\0' };
    read = 0;;
}

NIDAQMxUSB6501::~NIDAQMxUSB6501()
{
#ifndef L_COMP
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
#endif
}

bool NIDAQMxUSB6501::isConnected()
{
#ifndef L_COMP
    return taskHandleRead != nullptr && taskHandleWrite != nullptr ;
#else
    return false;
#endif
}

bool NIDAQMxUSB6501::configure(const QString & readDevice, const QString & writeDevice)
{
#ifndef L_COMP
    qDebug("%s:%d %s %s", __FILE__, __LINE__, readDevice.toStdString().c_str(), writeDevice.toStdString().c_str());
    if (isConnected())
        return true;
    /*********************************************/
    // DAQmx Configure Code
    /*********************************************/
    DAQmxErrChk(DAQmxCreateTask("readValues", &taskHandleRead));
    //qDebug("%s:%d",__FILE__,__LINE__);
    DAQmxErrChk(DAQmxCreateDIChan(taskHandleRead, readDevice.toStdString().c_str(), "", DAQmx_Val_ChanForAllLines));
    //qDebug("%s:%d",__FILE__,__LINE__);
    DAQmxErrChk(DAQmxStartTask(taskHandleRead));
    //qDebug("%s:%d",__FILE__,__LINE__);
    DAQmxErrChk(DAQmxCreateTask("writeValue", &taskHandleWrite));
    //qDebug("%s:%d",__FILE__,__LINE__);
    DAQmxErrChk(DAQmxCreateDOChan(taskHandleWrite, writeDevice.toStdString().c_str(), "", DAQmx_Val_ChanForAllLines));
    //qDebug("%s:%d",__FILE__,__LINE__);
    DAQmxErrChk(DAQmxStartTask(taskHandleWrite));
    //qDebug("%s:%d",__FILE__,__LINE__);

    uInt8 dataWriteNow[16] = {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    DAQmxErrChk(DAQmxWriteDigitalLines(taskHandleWrite, 1, 1, 10.0, DAQmx_Val_GroupByChannel, dataWriteNow, NULL, NULL));

    return true;

Error:
    errorFun();
    qDebug("%d %p %p %s", __LINE__, taskHandleRead, taskHandleWrite, errStr().c_str());
#endif
    return false;
}

bool NIDAQMxUSB6501::readValue(uInt16 & val)
{
    if (!isConnected())
        return false;
#ifndef L_COMP
    //qDebug("%d task = %d", __LINE__, taskHandleRead);
    DAQmxErrChk(DAQmxReadDigitalU32(taskHandleRead, 1, 10.0, DAQmx_Val_GroupByChannel, &dataRead, 1, &read, NULL));

    uint16_t corrval = (dataRead & 0x00ff) | (((dataRead & 0xff00) >> 4));
    val = (~corrval) & 0x01ff;

    //qDebug("%s:%d inR=%04x corrV=%04x val=%04x ", __FILE__, __LINE__, dataRead, corrval, val);
    return true;

Error:
    errorFun();
    qDebug("%d %d %s", __LINE__, taskHandleRead, errStr().c_str());
#endif
    return false;
}

bool NIDAQMxUSB6501::writeValue(uInt16& val)
{
    if (!isConnected())
        return false;
#ifndef L_COMP
    //qDebug("%d write %04x", __LINE__, val);
    for (int i = 0; i < 10; ++i) {
        //qDebug("%s:%d val_wr[%02x,%04x]=%d", __FILE__, __LINE__, ~val, 0x1 << i, ((~val >> i) & 0x1));
        dataWrite[i] = ((~val >> i) & 0x1);
    }

    //for (int i = 0; i < 10; ++i)
        //qDebug("%d", dataWrite[i]);
    //qDebug("==");

    DAQmxErrChk(DAQmxWriteDigitalLines(taskHandleWrite, 1, 1, 10.0, DAQmx_Val_GroupByChannel, dataWrite, NULL, NULL));
    //qDebug("%s:%d",__FILE__,__LINE__);
    return true;

Error:
    errorFun();
#endif
    return false;
}

void NIDAQMxUSB6501::errorFun()
{
#ifndef L_COMP
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
#endif
}

std::string NIDAQMxUSB6501::errStr()
{
    return std::string(errBuff);
}

