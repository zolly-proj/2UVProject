#ifndef _CLSHOWQRCODEOFOBJECT_H
#define _CLSHOWQRCODEOFOBJECT_H

#include <QtWidgets/QApplication>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtNetwork/QHostInfo>

#include <vector>
#include <iostream>
#include <string>

#include "qrcode/QrCodeGenerator.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clShowQrCodeOfObjectUI.h"
#include "clMethodCall.h"

using namespace std;

class clShowQrCodeOfObject : public clMethodCall
{
public:
    clShowQrCodeOfObject ();
    ~clShowQrCodeOfObject ();

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
	clShowQrCodeOfObjectUI *meShowQrCodeOfObjectUI;
};
#endif
extern "C" clMethodCall* CreateModule()
{
    // call the constructor of the actual implementation
    clMethodCall * module = new clShowQrCodeOfObject();
    // return the created function
    return module;
}
