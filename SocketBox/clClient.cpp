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

#include "clSocketBoxClient.h"

using namespace std;

int main (int paArgc, char * paArgv[])
{
  try
  {
    if (paArgc > 2)
    {
		printf("Amount of arguments checked of the SocketBox ... \r\n");
        QCoreApplication a( paArgc, paArgv );
        clSocketBoxClient loSocketBoxClient(paArgv[1], paArgv[2]);
		loSocketBoxClient.sendToServer();
	}
  }
  catch(...)
  {
	  cout << "No" << endl;
	  
  }
}
