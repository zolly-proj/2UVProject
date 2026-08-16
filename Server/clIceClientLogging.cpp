#define INFO_BUFFER_SIZE 32767
#include "clIceClientLogging.h"

bool clIceClientLogging::Ping()
{
    try
    {
        IceUtil::Mutex::Lock loLock (meMutex);
        return meLogAppServer->Ping();
    }
    catch(exception& e)
    {
        printf("bool clIceClientLogging::Ping() -> error ...");
        cout << e.what() << endl;
        return false;
    }
}
void clIceClientLogging::Shutdown()
{
    try
    {
        IceUtil::Mutex::Lock loLock (meMutex);
        meLogAppServer->Shutdown();
    }
    catch(...)
    {
        printf("bool clIceClientLogging::Shutdown() -> error ...");
    }
}
void clIceClientLogging::insertItem(   QString paLogNumber,
                                QString paApplicationName,
                                QString paApplicationText)
{
    try
    {
        IceUtil::Mutex::Lock loLock (meMutex);
		
		char hostname[INFO_BUFFER_SIZE];
		char username[INFO_BUFFER_SIZE];
		
		gethostname(hostname,INFO_BUFFER_SIZE);
		getlogin_r(username,INFO_BUFFER_SIZE);
		/*
		TCHAR  infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;

		
		GetComputerName(infoBuf,&bufCharCount);
                */
        meLogAppServer->insertItem(std::string(paLogNumber.toStdString()), std::string(hostname),std::string(paApplicationName.toStdString ()),std::string(paApplicationText.toStdString()));
    }
    catch(...)
    {
        printf("bool clIceClientLogging::insertItem(...) -> error ...");
    }
}
void clIceClientLogging::insertItem(   QString paLogNumber,
                                QString paApplicationName,
								QString paComputerName,
                                QString paApplicationText)
{
    try
    {
        IceUtil::Mutex::Lock loLock (meMutex);		
        meLogAppServer->insertItem(std::string(paLogNumber.toStdString()), std::string(paComputerName.toStdString()),std::string(paApplicationName.toStdString ()),std::string(paApplicationText.toStdString()));
    }
    catch(...)
    {
        printf("bool clIceClientLogging::insertItem(...) -> error ...");
    }
}
