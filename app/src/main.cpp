#include "glowneokno.h"
#include <QApplication>
//#include <QWindowsStyle>

#include <stdio.h>
#include <NIDAQmx.h>

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

int main(void)
{
    int32		error=0;
    TaskHandle	taskHandle=0;
    uInt32		data;
    char		errBuff[2048]={'\0'};
    int32		read;

    /*********************************************/
    // DAQmx Configure Code
    /*********************************************/
    DAQmxErrChk (DAQmxCreateTask("",&taskHandle));
    DAQmxErrChk (DAQmxCreateDIChan(taskHandle,"Dev1/port2","",DAQmx_Val_ChanForAllLines));

    /*********************************************/
    // DAQmx Start Code
    /*********************************************/
    DAQmxErrChk (DAQmxStartTask(taskHandle));

    for (int i = 0; i < 100; i++) {
    /*********************************************/
    // DAQmx Read Code
    /*********************************************/
    DAQmxErrChk (DAQmxReadDigitalU32(taskHandle,1,10.0,DAQmx_Val_GroupByChannel,&data,1,&read,NULL));

    printf("Data acquired: 0x%X\n",(unsigned)data);
    }

Error:
    if( DAQmxFailed(error) )
        DAQmxGetExtendedErrorInfo(errBuff,2048);
    if( taskHandle!=0 ) {
        /*********************************************/
        // DAQmx Stop Code
        /*********************************************/
        DAQmxStopTask(taskHandle);
        DAQmxClearTask(taskHandle);
    }
    if( DAQmxFailed(error) )
        printf("DAQmx Error: %s\n",errBuff);
    printf("End of program, press Enter key to quit\n");
    getchar();
    return 0;
}

/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("Star Runner");
    //a.setStyle("Windows");
    //QApplication::setStyle(QWindowsStyle());

    GlowneOkno w;
    w.show();

    return a.exec();
}
*/
