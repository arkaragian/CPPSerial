#include <stdio.h>
#include <tchar.h>
#include <windows.h>

namespace CPPSerial {
  void PrintCommState (DCB dcb) {
    //Print some of the DCB structure values
    _tprintf (TEXT ("\nBaudRate = %lu, ByteSize = %d, Parity = %d, StopBits = %d\n"),
              dcb.BaudRate,
              dcb.ByteSize,
              dcb.Parity,
              dcb.StopBits);
  }
}   //namespace CPPSerial

int main () {
  DCB         dcb;
  HANDLE      hCom;
  BOOL        fSuccess;
  const char* PortName   = "COM1";
  TCHAR*      pcCommPort = TEXT ((char*)PortName);   //Most systems have a COM1 port

  //Open a handle to the specified com port.
  hCom = CreateFile (pcCommPort, GENERIC_READ | GENERIC_WRITE,
                     0,               //must be opened with exclusive-access
                     NULL,            //default security attributes
                     OPEN_EXISTING,   //must use OPEN_EXISTING
                     0,               //not overlapped I/O
                     NULL);           //hTemplate must be NULL for comm devices

  if (hCom == INVALID_HANDLE_VALUE) {
    //Handle the error.
    printf ("CreateFile failed with error %lu.\n", GetLastError ());
    return (1);
  }

  //Initialize the DCB structure.
  SecureZeroMemory (&dcb, sizeof (DCB));
  dcb.DCBlength = sizeof (DCB);

  //Build on the current configuration by first retrieving all current
  //settings.
  fSuccess = GetCommState (hCom, &dcb);

  if (!fSuccess) {
    //Handle the error.
    printf ("GetCommState failed with error %lu.\n", GetLastError ());
    return (2);
  }

  CPPSerial::PrintCommState (dcb);   //Output to console

  //Fill in some DCB values and set the com state:
  //57,600 bps, 8 data bits, no parity, and 1 stop bit.
  dcb.BaudRate = CBR_57600;    //baud rate
  dcb.ByteSize = 8;            //data size, xmit and rcv
  dcb.Parity   = NOPARITY;     //parity bit
  dcb.StopBits = ONESTOPBIT;   //stop bit

  fSuccess = SetCommState (hCom, &dcb);

  if (!fSuccess) {
    //Handle the error.
    printf ("SetCommState failed with error %lu.\n", GetLastError ());
    return (3);
  }

  //Get the comm config again.
  fSuccess = GetCommState (hCom, &dcb);

  if (!fSuccess) {
    //Handle the error.
    printf ("GetCommState failed with error %lu.\n", GetLastError ());
    return (2);
  }

  CPPSerial::PrintCommState (dcb);   //Output to console

  _tprintf (TEXT ("Serial port %s successfully reconfigured.\n"), pcCommPort);
  return 0;
}
