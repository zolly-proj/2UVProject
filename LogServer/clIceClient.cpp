#include "clIceClient.h"

bool clIceClient::Ping()
{
    try
    {
        IceUtil::Mutex::Lock loLock (meMutex);
		printf("bool clIceClient::Ping() -> Mutex set ...");
		if (meLogAppServer == NULL)
		{
				printf("clIceClient::clIceClient -> wouter toch");
			
		}
		
        return meLogAppServer->Ping();
    }
    catch(exception& e)
    {
        printf("bool clIceClient::Ping() -> error ...");
        cout << e.what() << endl;
        return false;
    }
}
void clIceClient::Shutdown()
{
    try
    {
        IceUtil::Mutex::Lock loLock (meMutex);
		printf("void clIceClient::Shutdown() -> Mutex set ...");
        meLogAppServer->Shutdown();
    }
    catch(exception& e)
    {
        printf("void clIceClient::Shutdown() -> error ...");
		cout << e.what() << endl;
    }
}
void clIceClient::insertItem(   QString paLogNumber,
								QString paComputerName,
                                QString paApplicationName,
                                QString paApplicationText)
{
    try
    {
        IceUtil::Mutex::Lock loLock (meMutex);
		printf("void clIceClient::insertItem() -> Mutex set ...");
        meLogAppServer->insertItem(std::string(paLogNumber.toStdString()),std::string(paComputerName.toStdString()), std::string(paApplicationName.toStdString()),std::string(paApplicationText.toStdString()));
    }
    catch(exception& e)
    {
        printf("bool clIceClient::insertItem(...) -> error ...");
		cout << e.what() << endl;
    }
}
