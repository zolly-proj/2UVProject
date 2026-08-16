#ifndef _CLCREATEOPERATIONFORMACHINE_H
#define _CLCREATEOPERATIONFORMACHINE_H


#include <QtCore/QString>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>
#include <QtNetwork/QHostInfo>

#include <vector>
#include <iostream>
#include <string>

#include "clDatabaseColumn.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clMethodCall.h"

using namespace std;

class clCreateOperationForMachine : public clMethodCall
{
public:
    clCreateOperationForMachine ();
    ~clCreateOperationForMachine ();

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
};
#endif
extern "C" clMethodCall* CreateModule()
{
    // call the constructor of the actual implementation
    clMethodCall * module = new clCreateOperationForMachine();
    // return the created function
    return module;
}
