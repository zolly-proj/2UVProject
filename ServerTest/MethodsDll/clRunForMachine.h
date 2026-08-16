#ifndef _CLRUNFORMACHINE_H
#define _CLRUNFORMACHINE_H


#include <QtCore/QString>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>
#include <QtCore/QRandomGenerator>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#include <vector>
#include <iostream>
#include <string>

#include "clDatabaseColumn.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clMethodCall.h"

using namespace std;

class clRunForMachine : public clMethodCall
{
public:
    clRunForMachine ();
    ~clRunForMachine ();

	//BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
		//if(fdwReason == DLL_PROCESS_ATTACH){
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProc, 0, 0, 0);

public:
	bool createPluginClass(clIceClientServer * paIceClientServer,clIceClientLogging  * paIceClientLogging) override;
	int GetReturnParameters() override;
	bool doMethod(const vector <QString> &paParametersType, const vector <QString> &paParameters) override;
private:
	vector <QString> meParametersType;
	vector <QString> meParameters;
	clIceClientLogging * meIceClientLogging;
	clIceClientServer * meIceClientServer;
	
	
	
	
	QString meSocketPort;
	QString meSocketIp;
	QTcpSocket *meSocket;
	QByteArray meData;	
	
	
	
	
	
	
	
	
};
#endif
extern "C" clMethodCall* CreateModule()
{
    // call the constructor of the actual implementation
    clMethodCall * module = new clRunForMachine();
    // return the created function
    return module;
}
