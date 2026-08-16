#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include "clClientApplication.h"
#include <iostream>
#include <list>

int main(int paArgc, char* paArgv[])
{
	try
	{


		QApplication loApplication( paArgc, paArgv);


		clClientApplication * loClientApplication = new clClientApplication();

		QMainWindow loTestWin;
		loTestWin.setCentralWidget(loClientApplication);
		//loTestWin.resize(440,467);
		loTestWin.show();
		return loApplication.exec();
	}
    catch(exception& e)
    {
        printf("Client::constructor -> error ...");
		cout << e.what() << endl;
		return 0;
    }
    
}
