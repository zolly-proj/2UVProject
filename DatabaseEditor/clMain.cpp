//
// Database Editor
// clMain.cpp 10/03/2011 WVD
//
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include "clDatabaseEditorWidget.h"
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
        QMainWindow loMainWin;
        clDatabaseEditorWidget *loDatabaseEditorWidget = new clDatabaseEditorWidget(paArgv[1]);
        loMainWin.setCentralWidget(loDatabaseEditorWidget);
        loMainWin.resize(682,297);
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
