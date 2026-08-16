//
// Database Editor
// clMain.cpp 15/05/2012 WVD
//
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include "clLogServer.h"
#include <iostream>
#include <list>
/* #include <conio.h> */



#include <QtCore/QString>

using namespace std;

int main (int paArgc, char * paArgv[])
{
  try
  {
    if (paArgc > 1)
    {
		printf("Amount of arguments checked of the log server ... \r\n");
        QApplication loApplication( paArgc, paArgv );
		printf("QApplication initialized ... \r\n");
        clLogServer *loLogServer = new clLogServer(paArgv[1]);
		printf("LogServer initialized... \r\n");
        printf("Press any key then enter to continue... \r\n");
        char userContinue;
        while (QString(userContinue) != QString("q"))
        {
            userContinue = getchar();
        }
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
