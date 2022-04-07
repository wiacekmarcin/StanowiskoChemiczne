#include "rs232.h"

#include <initguid.h>
#include <windows.h>
#include <Setupapi.h>
#include <winreg.h>
#include <wchar.h>
#include <string.h>

#define RS232_PORTNR  32

HANDLE Cport[RS232_PORTNR];


const char *comports[RS232_PORTNR]={"\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",  "\\\\.\\COM4",
                                    "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",  "\\\\.\\COM8",
                                    "\\\\.\\COM9",  "\\\\.\\COM10", "\\\\.\\COM11", "\\\\.\\COM12",
                                    "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15", "\\\\.\\COM16",
                                    "\\\\.\\COM17", "\\\\.\\COM18", "\\\\.\\COM19", "\\\\.\\COM20",
                                    "\\\\.\\COM21", "\\\\.\\COM22", "\\\\.\\COM23", "\\\\.\\COM24",
                                    "\\\\.\\COM25", "\\\\.\\COM26", "\\\\.\\COM27", "\\\\.\\COM28",
                                    "\\\\.\\COM29", "\\\\.\\COM30", "\\\\.\\COM31", "\\\\.\\COM32"};

char mode_str[128];


int RS232_OpenComport(int comport_number, int baudrate, const char *mode, int flowctrl)
{
  if((comport_number>=RS232_PORTNR)||(comport_number<0))
  {
    printf("illegal comport number\n");
    return(1);
  }

  switch(baudrate)
  {
    case     110 : strcpy_s(mode_str, "baud=110");
                   break;
    case     300 : strcpy_s(mode_str, "baud=300");
                   break;
    case     600 : strcpy_s(mode_str, "baud=600");
                   break;
    case    1200 : strcpy_s(mode_str, "baud=1200");
                   break;
    case    2400 : strcpy_s(mode_str, "baud=2400");
                   break;
    case    4800 : strcpy_s(mode_str, "baud=4800");
                   break;
    case    9600 : strcpy_s(mode_str, "baud=9600");
                   break;
    case   19200 : strcpy_s(mode_str, "baud=19200");
                   break;
    case   38400 : strcpy_s(mode_str, "baud=38400");
                   break;
    case   57600 : strcpy_s(mode_str, "baud=57600");
                   break;
    case  115200 : strcpy_s(mode_str, "baud=115200");
                   break;
    case  128000 : strcpy_s(mode_str, "baud=128000");
                   break;
    case  256000 : strcpy_s(mode_str, "baud=256000");
                   break;
    case  500000 : strcpy_s(mode_str, "baud=500000");
                   break;
    case  921600 : strcpy_s(mode_str, "baud=921600");
                   break;
    case 1000000 : strcpy_s(mode_str, "baud=1000000");
                   break;
    case 1500000 : strcpy_s(mode_str, "baud=1500000");
                   break;
    case 2000000 : strcpy_s(mode_str, "baud=2000000");
                   break;
    case 3000000 : strcpy_s(mode_str, "baud=3000000");
                   break;
    default      : printf("invalid baudrate\n");
                   return(1);
                   break;
  }

  if(strlen(mode) != 3)
  {
    printf("invalid mode \"%s\"\n", mode);
    return(1);
  }

  switch(mode[0])
  {
    case '8': strcat_s(mode_str, " data=8");
              break;
    case '7': strcat_s(mode_str, " data=7");
              break;
    case '6': strcat_s(mode_str, " data=6");
              break;
    case '5': strcat_s(mode_str, " data=5");
              break;
    default : printf("invalid number of data-bits '%c'\n", mode[0]);
              return(1);
              break;
  }

  switch(mode[1])
  {
    case 'N':
    case 'n': strcat_s(mode_str, " parity=n");
              break;
    case 'E':
    case 'e': strcat_s(mode_str, " parity=e");
              break;
    case 'O':
    case 'o': strcat_s(mode_str, " parity=o");
              break;
    default : printf("invalid parity '%c'\n", mode[1]);
              return(1);
              break;
  }

  switch(mode[2])
  {
    case '1': strcat_s(mode_str, " stop=1");
              break;
    case '2': strcat_s(mode_str, " stop=2");
              break;
    default : printf("invalid number of stop bits '%c'\n", mode[2]);
              return(1);
              break;
  }

  if(flowctrl)
  {
    strcat_s(mode_str, " xon=off to=off odsr=off dtr=on rts=off");
  }
  else
  {
    strcat_s(mode_str, " xon=off to=off odsr=off dtr=on rts=on");
  }

/*
http://msdn.microsoft.com/en-us/library/windows/desktop/aa363145%28v=vs.85%29.aspx

http://technet.microsoft.com/en-us/library/cc732236.aspx

https://docs.microsoft.com/en-us/windows/desktop/api/winbase/ns-winbase-_dcb
*/

  Cport[comport_number] = CreateFileA(comports[comport_number],
                      GENERIC_READ|GENERIC_WRITE,
                      0,                          /* no share  */
                      NULL,                       /* no security */
                      OPEN_EXISTING,
                      0,                          /* no threads */
                      NULL);                      /* no templates */

  if(Cport[comport_number]==INVALID_HANDLE_VALUE)
  {
    printf("unable to open comport\n");
    return(1);
  }

  DCB port_settings;
  memset(&port_settings, 0, sizeof(port_settings));  /* clear the new struct  */
  port_settings.DCBlength = sizeof(port_settings);

  if(!BuildCommDCBA(mode_str, &port_settings))
  {
    printf("unable to set comport dcb settings\n");
    CloseHandle(Cport[comport_number]);
    return(1);
  }

  if(flowctrl)
  {
    port_settings.fOutxCtsFlow = TRUE;
    port_settings.fRtsControl = RTS_CONTROL_HANDSHAKE;
  }

  if(!SetCommState(Cport[comport_number], &port_settings))
  {
    printf("unable to set comport cfg settings\n");
    CloseHandle(Cport[comport_number]);
    return(1);
  }

  COMMTIMEOUTS Cptimeouts;

  Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
  Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
  Cptimeouts.ReadTotalTimeoutConstant    = 0;
  Cptimeouts.WriteTotalTimeoutMultiplier = 0;
  Cptimeouts.WriteTotalTimeoutConstant   = 0;

  if(!SetCommTimeouts(Cport[comport_number], &Cptimeouts))
  {
    printf("unable to set comport time-out settings\n");
    CloseHandle(Cport[comport_number]);
    return(1);
  }

  return(0);
}


int RS232_PollComport(int comport_number, unsigned char *buf, int size)
{
  int n;

/* added the void pointer cast, otherwise gcc will complain about */
/* "warning: dereferencing type-punned pointer will break strict aliasing rules" */

  if(!ReadFile(Cport[comport_number], buf, size, (LPDWORD)((void *)&n), NULL))
  {
    return -1;
  }

  return(n);
}


int RS232_SendByte(int comport_number, unsigned char byte)
{
  int n;

  if(!WriteFile(Cport[comport_number], &byte, 1, (LPDWORD)((void *)&n), NULL))
  {
    return(1);
  }

  if(n<0)  return(1);

  return(0);
}


int RS232_SendBuf(int comport_number, unsigned char *buf, int size)
{
  int n;

  if(WriteFile(Cport[comport_number], buf, size, (LPDWORD)((void *)&n), NULL))
  {
    return(n);
  }

  return(-1);
}


void RS232_CloseComport(int comport_number)
{
  CloseHandle(Cport[comport_number]);
}

/*
http://msdn.microsoft.com/en-us/library/windows/desktop/aa363258%28v=vs.85%29.aspx
*/

int RS232_IsDCDEnabled(int comport_number)
{
  int status;

  GetCommModemStatus(Cport[comport_number], (LPDWORD)((void *)&status));

  if(status&MS_RLSD_ON) return(1);
  else return(0);
}


int RS232_IsRINGEnabled(int comport_number)
{
  int status;

  GetCommModemStatus(Cport[comport_number], (LPDWORD)((void *)&status));

  if(status&MS_RING_ON) return(1);
  else return(0);
}


int RS232_IsCTSEnabled(int comport_number)
{
  int status;

  GetCommModemStatus(Cport[comport_number], (LPDWORD)((void *)&status));

  if(status&MS_CTS_ON) return(1);
  else return(0);
}


int RS232_IsDSREnabled(int comport_number)
{
  int status;

  GetCommModemStatus(Cport[comport_number], (LPDWORD)((void *)&status));

  if(status&MS_DSR_ON) return(1);
  else return(0);
}


void RS232_enableDTR(int comport_number)
{
  EscapeCommFunction(Cport[comport_number], SETDTR);
}


void RS232_disableDTR(int comport_number)
{
  EscapeCommFunction(Cport[comport_number], CLRDTR);
}


void RS232_enableRTS(int comport_number)
{
  EscapeCommFunction(Cport[comport_number], SETRTS);
}


void RS232_disableRTS(int comport_number)
{
  EscapeCommFunction(Cport[comport_number], CLRRTS);
}

/*
https://msdn.microsoft.com/en-us/library/windows/desktop/aa363428%28v=vs.85%29.aspx
*/

void RS232_flushRX(int comport_number)
{
  PurgeComm(Cport[comport_number], PURGE_RXCLEAR | PURGE_RXABORT);
}


void RS232_flushTX(int comport_number)
{
  PurgeComm(Cport[comport_number], PURGE_TXCLEAR | PURGE_TXABORT);
}


void RS232_flushRXTX(int comport_number)
{
  PurgeComm(Cport[comport_number], PURGE_RXCLEAR | PURGE_RXABORT);
  PurgeComm(Cport[comport_number], PURGE_TXCLEAR | PURGE_TXABORT);
}

void RS232_cputs(int comport_number, const char *text)  /* sends a string to serial port */
{
  while(*text != 0)   RS232_SendByte(comport_number, *(text++));
}


/* return index in comports matching to device name or -1 if not found */
int RS232_GetPortnr(const char *devname)
{
  int i;

  char str[32];

#if defined(__linux__) || defined(__FreeBSD__)   /* Linux & FreeBSD */
  strcpy_s(str, "/dev/");
#else  /* windows */
  strcpy_s(str, "\\\\.\\");
#endif
  strncat_s(str, devname, 16);
  str[31] = 0;

  for(i=0; i<RS232_PORTNR; i++)
  {
    if(!strcmp(comports[i], str))
    {
      return i;
    }
  }

  return -1;  /* device not found */
}

void GetComPortUsb(char *pszComePort, const char const * vid, const char const * pid)
{
    HDEVINFO DeviceInfoSet;
    DWORD DeviceIndex =0;
    SP_DEVINFO_DATA DeviceInfoData;
    char DevEnum[4] = {'U', 'S', 'B', 0};
    char ExpectedDeviceId[80] = {0}; //Store hardware id
    BYTE szBuffer[1024] = {0};
    DEVPROPTYPE ulPropertyType;
    DWORD dwSize = 0;
    DWORD Error = 0;
    //create device hardware id
    sprintf(ExpectedDeviceId,"vid_%s&pid_%s", vid, pid);
    //strcpy_s(ExpectedDeviceId, "vid_");
    //strcpy_s(ExpectedDeviceId + 4, vid);
    //strcpy_s(&ExpectedDeviceId[8], "&pid_");
    //strcpy_s(&ExpectedDeviceId[13], pid);
    printf("expected %s", ExpectedDeviceId);
    //SetupDiGetClassDevs returns a handle to a device information set
    DeviceInfoSet = SetupDiGetClassDevs(
                        NULL,
                        (PCWSTR)DevEnum,
                        NULL,
                        DIGCF_ALLCLASSES | DIGCF_PRESENT);
    if (DeviceInfoSet == INVALID_HANDLE_VALUE)
        return;
    //Fills a block of memory with zeros
    ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    //Receive information about an enumerated device
    while (SetupDiEnumDeviceInfo(
                DeviceInfoSet,
                DeviceIndex,
                &DeviceInfoData))
    {
        DeviceIndex++;
        //Retrieves a specified Plug and Play device property
        if (SetupDiGetDeviceRegistryProperty (DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID,
                                              &ulPropertyType, (BYTE*)szBuffer,
                                              sizeof(szBuffer),   // The size, in bytes
                                              &dwSize))
        {
            HKEY hDeviceRegistryKey;
            //Get the key
            hDeviceRegistryKey = SetupDiOpenDevRegKey(DeviceInfoSet, &DeviceInfoData,DICS_FLAG_GLOBAL, 0,DIREG_DEV, KEY_READ);
            if (hDeviceRegistryKey == INVALID_HANDLE_VALUE)
            {
                Error = GetLastError();
                break; //Not able to open registry
            }
            else
            {
                // Read in the name of the port
                char pszPortName[32];
                DWORD dwSize = sizeof(pszPortName);
                DWORD dwType = 0;
                if( (RegQueryValueEx(hDeviceRegistryKey, L"PortName", NULL, &dwType, (LPBYTE) pszPortName, &dwSize) == ERROR_SUCCESS) && (dwType == REG_SZ))
                {
                    // Check if it really is a com port
                    if( _strnicmp( pszPortName, "COM", 3) == 0)
                    {
                        int nPortNr = atoi( pszPortName + 3 );
                        if( nPortNr != 0 )
                        {
                            strcpy_s(pszComePort,32,pszPortName);
                        }
                    }
                }
                // Close the key now that we are finished with it
                RegCloseKey(hDeviceRegistryKey);
            }
        }
    }
    if (DeviceInfoSet)
    {
        SetupDiDestroyDeviceInfoList(DeviceInfoSet);
    }
}