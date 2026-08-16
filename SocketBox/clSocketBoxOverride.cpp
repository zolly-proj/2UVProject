#ifndef _ICE_CLIENT_H
#define _ICE_CLIENT_H

#
#include <exception>
#include <iostream>
#include <string>

using namespace std;

#include "ICE_LogApplication.h"

class clIceClient
{
public:
    clIceClient (Ice::CommunicatorPtr paCommunicator)
    {
        try
        {
			string loServerRegistration = "LogAppServerAdaptor";
			Ice::ObjectPrx base = paCommunicator->stringToProxy("LogAppServer:tcp -p 5001:udp -p 5001");
            meLogAppServer = UVLogAppServer::LogAppServerPrx::uncheckedCast(base);
			
			if (meLogAppServer == NULL)
			{
					printf("clIceClient::clIceClient -> wouter toch");
			}		

			printf("clIceClient::clIceClient -> constructor done");
        }
        catch (Ice::Exception &paException)
        {
            printf(paException.what());
        }
        catch(...)
        {
             printf("clIceClient (Ice::CommunicatorPtr paCommunicator)-> error");
        }
    }
private:
    UVLogAppServer::LogAppServerPrx meLogAppServer;
    IceUtil::Mutex meMutex;
public:
    bool Ping();
    void Shutdown();
    void insertItem(QString paLogNumber,
					QString paComputerName,
                    QString paApplicationName,
                    QString paApplicationText);
};

#endif
