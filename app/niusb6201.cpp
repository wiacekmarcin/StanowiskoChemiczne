#include "niusb6201.h"

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

NIDAQMxUSB6501::NIDAQMxUSB6501()
{
    error = 0;
    taskHandleRead = 0;
    taskHandleWrite = 0;
    for (int i = 0; i < 9; ++i)
        dataWrite[i] = 0;

    errBuff[2048] = { '\0' };
    read = 0;;
}

NIDAQMxUSB6501::~NIDAQMxUSB6501()
{
    if (taskHandleRead != 0) {
        DAQmxStopTask(taskHandleRead);
        DAQmxClearTask(taskHandleRead);
        taskHandleRead = 0;
    }
    if (taskHandleWrite != 0) {
        DAQmxStopTask(taskHandleWrite);
        DAQmxClearTask(taskHandleWrite);
        taskHandleWrite = 0;
    }
}

bool NIDAQMxUSB6501::configure()
{
    /*********************************************/
    // DAQmx Configure Code
    /*********************************************/
    DAQmxErrChk(DAQmxCreateTask("", &taskHandleRead));
    DAQmxErrChk(DAQmxCreateDIChan(taskHandleRead, "USB6501/port0,USB6501/port1/Line0,", "", DAQmx_Val_ChanForAllLines));
    
    DAQmxErrChk(DAQmxStartTask(taskHandleWrite));

    DAQmxErrChk(DAQmxCreateTask("", &taskHandleWrite));
    DAQmxErrChk(DAQmxCreateDOChan(taskHandleWrite, "USB6501/port2/Line0:6,USB6501/port1/Line4:5,", "", DAQmx_Val_ChanForAllLines));

    DAQmxErrChk(DAQmxStartTask(taskHandleWrite));


    return true;

Error:
    errorFun();
    return false;
}

bool NIDAQMxUSB6501::readValue(uInt16 & val)
{
    DAQmxErrChk(DAQmxReadDigitalU32(taskHandleRead, 1, 10.0, DAQmx_Val_GroupByChannel, &dataRead, 1, &read, NULL));
    val = dataRead & 0x01ff;
    return true;

Error:
    errorFun();
    return false;
}

bool NIDAQMxUSB6501::writeValue(uInt16& val)
{
    for (int i = 0; i < 9; ++i)
        dataWrite[i] = (val >> i) & 0x1;


    DAQmxErrChk(DAQmxWriteDigitalLines(taskHandleRead, 1, 1, 10.0, DAQmx_Val_GroupByChannel, dataWrite, NULL, NULL));

    return true;

Error:
    errorFun();
    return false;
}

void NIDAQMxUSB6501::errorFun()
{
    if (DAQmxFailed(error))
        DAQmxGetExtendedErrorInfo(errBuff, 2048);

    if (taskHandleWrite != 0) {
        DAQmxStopTask(taskHandleWrite);
        DAQmxClearTask(taskHandleWrite);
        taskHandleWrite = 0;
    }
    
    if (taskHandleRead != 0) {
        DAQmxStopTask(taskHandleRead);
        DAQmxClearTask(taskHandleRead);
        taskHandleRead = 0;
    }

    //if (DAQmxFailed(error))
    //    printf("DAQmx Error: %s\n", errBuff);
    //printf("End of program, press Enter key to quit\n");
    //getchar();
    //return 0;
}