#ifndef _ICE_CLIENT_LOGGING_H
#define _ICE_CLIENT_LOGGING_H

#include <unistd.h>
#include <limits.h>

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceUtil/Mutex.h>

#include <QtCore/QString>
#include <QtCore/QMutex>

#include <exception>
#include <iostream>
#include <string>

using namespace std;

#include "ICE_LogApplication.h"

class clIceClientLogging
{
public:
    clIceClientLogging (Ice::CommunicatorPtr paCommunicator)
    {
        try
        {
			string loServerRegistration = "LogAppServerAdaptor";
            //Ice::ObjectPrx loBaseProxy = paCommunicator->propertyToProxy("LogAppServerAdaptor.Proxy");
			Ice::ObjectPrx base = paCommunicator->stringToProxy("LogAppServer:tcp -p 5001:udp -p 5001");
            meLogAppServer = UVLogAppServer::LogAppServerPrx::uncheckedCast(base);
			
			
			printf("clIceClientLogging::clIceClientLogging -> constructor done");
        }
        catch (Ice::Exception &paException)
        {
            cout << paException << endl;
        }
        catch(...)
        {
             printf("clIceClientLogging (Ice::CommunicatorPtr paCommunicator)-> error");
        }
    }
private:

    IceUtil::Mutex meMutex;
public:
    bool Ping();
    void Shutdown();
    void insertItem(QString paLogNumber,
                    QString paApplicationName,
					QString paComputerName,
                    QString paApplicationText);
	void insertItem(QString paLogNumber,
                    QString paApplicationName,
                    QString paApplicationText);
    UVLogAppServer::LogAppServerPrx meLogAppServer;					
};

#endif
