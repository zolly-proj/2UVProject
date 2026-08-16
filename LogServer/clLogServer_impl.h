#ifndef LOGSERVER_IMPL_H_WVD_14022006
#define LOGSERVER_IMPL_H_WVD_14022006

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>

#include <string>
#include <iostream>

#include "ICE_LogApplication.h"
#include "clFileWriter.h"

using namespace std;


class clLogServer_impl : public UVLogAppServer::LogAppServer
{
public:
    clLogServer_impl(clFileWriter &paFileWriter) : meFileWriter(paFileWriter)
    {
    }; //, WFlags paFlags = 0);
    //~clTestAppServer_impl ();
public:
    virtual bool Ping(const Ice::Current&) override;
    virtual void Shutdown(const Ice::Current&) override;
    virtual void insertItem(const std::string& paLogNumber,
							const std::string& paComputerName,
                            const std::string& paApplicationName,
                            const std::string& paApplicationText,
                            const Ice::Current &) override;
private:
    IceUtil::Mutex meGeneralMutex;
    clFileWriter &meFileWriter;
    //ItemObjectDef meObjectPass;
};

#endif
