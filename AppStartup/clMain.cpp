//
// Database Editor
// clMain.cpp 10/03/2011 WVD
//
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include "clAppStarterWidget.h"
#include <iostream>
#include <list>
using namespace std;

int main (int paArgc, char * paArgv[])
{
  try
  {
    if (paArgc > 1)
    {
        QApplication loApplication( paArgc, paArgv );
        //CalibratieScherm
        //----------------------------------
        clAppStarterWidget *loAppStarterWidget = new clAppStarterWidget(paArgv[1]);
        //loMainWin.resize(682,297);
        loAppStarterWidget->show();
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
