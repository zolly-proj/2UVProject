#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

#include "clClientApplication.h"
#include "clIceClient.h"
#include <iostream>
#include <list>

int main(int paArgc, char* paArgv[])
{
	try
	{
		Ice::InitializationData initData2;
		initData2.properties = Ice::createProperties();
		initData2.properties->load("config.client");
		Ice::CommunicatorPtr loIceCommunicator = Ice::initialize(paArgc, paArgv, initData2);

		printf("main() -> Communicator inistialized ");
		clIceClient loIceclient( loIceCommunicator);

		QApplication loApplication( paArgc, paArgv);


		clClientApplication * loClientApplication = new clClientApplication( loIceclient);

		QMainWindow loTestWin;
		loTestWin.setCentralWidget(loClientApplication);
		loTestWin.resize(440,467);
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
