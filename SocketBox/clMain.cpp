//
// Database Editor
// clMain.cpp 15/05/2012 WVD
//
#include <QtCore/QCoreApplication>
#include <QtWidgets/QMainWindow>

#include "clSocketBox.h"
#include <iostream>
#include <list>




#include <QtCore/QString>

using namespace std;

int main (int paArgc, char * paArgv[])
{
  try
  {
    if (paArgc > 1)
    {
		printf("Amount of arguments checked of the SocketBox ... \r\n");
        QCoreApplication a( paArgc, paArgv );
        clSocketBox loSocketBox(paArgv[1]);
		loSocketBox.StartServer();
		return a.exec();
    }
    else
    {
        printf("Not enough parameters supplied to call application ...");
    }
  }
  catch ( ... )
  {
    printf("Unknown exception");
  }
  return -1;
}
