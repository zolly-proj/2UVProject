#ifndef _CLCREATEQRCODEOFOBJECT_H
#define _CLCREATEQRCODEOFOBJECT_H

#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtNetwork/QHostInfo>

#include <vector>
#include <iostream>
#include <string>

#include "qrcode/QrCodeGenerator.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clMethodCall.h"

using namespace std;

class clCreateQrCodeOfObject : public clMethodCall
{
public:
    clCreateQrCodeOfObject ();
    ~clCreateQrCodeOfObject ();

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
    clMethodCall * module = new clCreateQrCodeOfObject();
    // return the created function
    return module;
}
