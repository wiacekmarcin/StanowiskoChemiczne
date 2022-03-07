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
    return taskHandleRead != nullptr /* && taskHandleWrite != nullptr */;
}

bool NIDAQMxUSB6210::configure()
{
    /*********************************************/
    // DAQmx Configure Code
    /*********************************************/
    DAQmxErrChk(DAQmxCreateTask("readValues", &taskHandleRead));
    //qDebug("%d task = %p", __LINE__, taskHandleRead);
    //DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandleRead, "USB6210/ai0, USB6210/ai1, USB6210/ai2, USB6210/ai3, USB6210/ai4, USB6210/ai5",
    //    "", DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));

   // DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandleRead, "USB6210/ai0, USB6210/ai1, USB6210/ai2, USB6210/ai3, USB6210/ai4, USB6210/ai5",
    DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandleRead, "USB6210/ai0, USB6210/ai1, USB6210/ai2, USB6210/ai3, USB6210/ai4, USB6210/ai5, USB6210/ai6",
            "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL));
    //qDebug("%d task = %p", __LINE__, taskHandleRead);
    //DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandleRead, "", 20.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 400));

    
    
    //qDebug("%d task = %p", __LINE__, taskHandleRead);
    DAQmxErrChk(DAQmxStartTask(taskHandleRead));
    //qDebug("%d task = %p", __LINE__, taskHandleRead);


    return true;

Error:
    errorFun();
    qDebug("%d %p %s", __LINE__, taskHandleRead, errStr().c_str());
    return false;
}

//UDALO SIE..... odczytywac z wielu zrodel


bool NIDAQMxUSB6210::readValue(float& val1, float& val2, float& val3, float& val4, float& val5, float& val6, float& val7)
{
    if (!isConnected())
        return false;

    int32       read;
    
    float64 val[8];
    qDebug("%d task = %d", __LINE__, taskHandleRead);

    DAQmxErrChk(DAQmxReadAnalogF64(taskHandleRead, 1, 1.0, DAQmx_Val_GroupByChannel, val, 7, &read, NULL));
    

    
    
    qDebug("Acquired %d points\n", (int)read);
    //qDebug("Acquired %d points %f %f\n", (int)read, val[0], val[1]);
    
    //qDebug("val %f %f %f %f %f %f\n", val[0], val[1], val[2], val[3], val[4], val[5]);
    //for (int i = 0; i < 6; i++) {
    //    qDebug("%d %f", i, val[i]);
    //}

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
#endif
