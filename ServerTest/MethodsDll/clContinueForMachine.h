#ifndef _CLCONTINUEFORMACHINE_H
#define _CLCONTINUEFORMACHINE_H

#include <QtCore/QString>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>
#include <QtCore/QRandomGenerator>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>

#include <vector>
#include <iostream>
#include <string>

#include "clDatabaseColumn.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clMethodCall.h"

using namespace std;

class clContinueForMachine : public clMethodCall
{
public:
    clContinueForMachine ();
    ~clContinueForMachine ();

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
    clMethodCall * module = new clContinueForMachine();
    // return the created function
    return module;
}
