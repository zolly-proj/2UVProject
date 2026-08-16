//
// Database Editor
// clMain.cpp 10/03/2011 WVD
//
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCore/QString>
#include <QtCore/QByteArray>


#include "clMainWindow.h"
#include <iostream>
#include <list>




using namespace std;

int main (int paArgc, char * paArgv[])
{
  try
  {
	printf("Server start ...");
    //QString str1 = QString(paArgc);
    //QByteArray ba = str1.toLocal8Bit();
    //const char *c_str2 = ba.data();
	//printf(c_str2);
    if (paArgc > 1)
    {
        QApplication loApplication( paArgc, paArgv );
        clMainWindow loMainWin(paArgc,paArgv,paArgv[1]);
        loMainWin.show();
        return loApplication.exec();
    }
    else
    {
        printf("Not enough parameters supplied to call application ...");
    }
  }
  catch ( ... )
  {
    cout << "Unknown exception" << endl;
  }
  return -1;
}
