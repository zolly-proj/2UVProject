#ifndef CLSERVER_IMPL_H_WVD_14022006
#define CLSERVER_IMPL_H_WVD_14022006

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>

#include <string>
#include <iostream>
#include <exception>

#include "clDatabaseClass.h"
#include "clIceClientLogging.h"
#include "ICE_ServerApplication.h"
#include "clDatabaseTable.h"
#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"
#include "clWorkstationStatus.h"

using namespace std;


class clServer_impl : public UVServerAppServer::ServerAppServer
{
	vector <clWorkstationStatus> &meWorkstationStatus;
public:
    clServer_impl(vector <clDatabaseTable> paDatabaseTable, vector <clWorkstationStatus> & paWorkstationStatus, clDatabaseClass *paDatabaseClass, clIceClientLogging *paIceClientLogging) : meWorkstationStatus(paWorkstationStatus)
    {
        meDatabaseTable = paDatabaseTable;
        meIceClientLogging = paIceClientLogging;
        meDatabaseClass = paDatabaseClass;
		//paWorkstationStatus = meWorkstationStatus;
    };
    //~clServer_impl ();
	
	
	
	
	
	
	
	
	
	
public:
    virtual bool Ping(const Ice::Current& paCurrent);
    virtual void Shutdown(const Ice::Current& paCurrent);

    virtual bool insertIntoTableDatabase(   const std::string& paTableName,
                                            const UVServerAppServer::tyStringSequence& paColumns,
                                            const UVServerAppServer::tyStringSequence& paValue,
                                            const UVServerAppServer::tyStringSequence& paTypeValue,
                                            std::string& paReturnMessage,
                                            const Ice::Current&);

    virtual bool updateIntoTableDatabase(   const std::string& paTableName,
                                            const std::string& paId,
                                            const UVServerAppServer::tyStringSequence& paColumns,
                                            const UVServerAppServer::tyStringSequence& paValue,
                                            const UVServerAppServer::tyStringSequence& paTypeValue,
                                            std::string& paReturnMessage,
                                            const Ice::Current&);

    virtual bool deleteIntoTableDatabase( const std::string& paTableName,
                                          const std::string& paId,
                                          std::string& paReturnMessage,
                                          const Ice::Current&);

    virtual bool getFromTableDatabaseGeneral(   const std::string& paTableName,
                                                const std::string& paStartValue,
                                                const std::string& paMaxValue,
                                                UVServerAppServer::tyStringSequence& paReturnId,
												UVServerAppServer::tyStringSequence& paReturnName,
                                                std::string& paMessage,
                                                const Ice::Current&);
                                                
    virtual bool getFromTableDatabaseGeneralOrdered(   const std::string& paTableName,
                                                const std::string& paStartValue,
                                                const std::string& paMaxValue,
												const std::string& paOrderBy,
                                                UVServerAppServer::tyStringSequence& paReturnId,
												UVServerAppServer::tyStringSequence& paReturnName,
                                                std::string& paMessage,
                                                const Ice::Current&);                                                

    virtual bool getFromTableDatbaseByProperty(     const std::string& paTableName,
                                                    const std::string& paStartValue,
                                                    const std::string& paMaxValue,
                                                    const UVServerAppServer::tyStringSequence& paProperties,
                                                    const UVServerAppServer::tyStringSequence& paValue,
                                                    const UVServerAppServer::tyStringSequence& paTypeValue,
                                                    const UVServerAppServer::tyStringSequence& paLogExp,
                                                    UVServerAppServer::tyStringSequence& paReturnId,
                                                    std::string& paReturnMessage,
                                                    const Ice::Current&);

    virtual bool getFromTableDatbaseByPropertyOrdered(     const std::string& paTableName,
                                                    const std::string& paStartValue,
                                                    const std::string& paMaxValue,
                                                    const UVServerAppServer::tyStringSequence& paProperties,
                                                    const UVServerAppServer::tyStringSequence& paValue,
                                                    const UVServerAppServer::tyStringSequence& paTypeValue,
                                                    const UVServerAppServer::tyStringSequence& paLogExp,
													const std::string& paOrderBy,
                                                    UVServerAppServer::tyStringSequence& paReturnId,
                                                    std::string& paReturnMessage,
                                                    const Ice::Current&);	                                                    

    virtual bool getFromTableDatbaseByPropertyRepresentProp(    const std::string& paTableName,
                                                                const std::string& paStartValue,
                                                                const std::string& paMaxValue,
                                                                const UVServerAppServer::tyStringSequence& paProperties,
                                                                const UVServerAppServer::tyStringSequence& paValue,
                                                                const UVServerAppServer::tyStringSequence& paTypeValue,
                                                                const UVServerAppServer::tyStringSequence& paLogExp,
                                                                UVServerAppServer::tyStringSequence& paPropertiesReturn,
                                                                UVServerAppServer::tyStringSequence& paVauesReturn,
                                                                std::string& paReturnMessage,
                                                                const Ice::Current&);

    virtual bool getFromTableDatabaseById(  const std::string& paTableName,
                                            const std::string& paId,
                                            const UVServerAppServer::tyStringSequence& paProperties,
                                            UVServerAppServer::tyStringSequence& paReturnValue,
                                            std::string& paReturnMessage,
                                            const Ice::Current&);

    virtual bool getAllTablesFromDatabase(  UVServerAppServer::tyStringSequence& paTables,
                                            std::string& paReturnMessage,
                                            const Ice::Current&);

    virtual bool getAllMethodsFromTable(    const std::string& paTable,
                                            UVServerAppServer::tyStringSequence& paMethodName,
                                            UVServerAppServer::tyStringSequence& paAlias,
                                            UVServerAppServer::tyStringSequence& paSource,
                                            UVServerAppServer::tyStringSequence& paSourceName,
                                            UVServerAppServer::tyStringSequence& paReturnType,
                                            std::string& paReturnMessage,
                                            const Ice::Current&);

    virtual bool getAllPropertiesFromTable( const std::string& paTable,
                                            UVServerAppServer::tyStringSequence& paPropertyName,
                                            UVServerAppServer::tyStringSequence& paAlias,
                                            UVServerAppServer::tyStringSequence& paType,
                                            UVServerAppServer::tyStringSequence& paExtra,
                                            UVServerAppServer::tyStringSequence& paReference,
                                            std::string& paReturnMessage,
                                            const Ice::Current&);
											
	virtual bool giveWorkstationClientInfo(	const std::string& paWorkstationName,
											const std::string& paPercentageMemUsage,
											const std::string& paPercentageDiskUsage,
											const std::string& paPercentageCpuUsage,
											std::string& paReturnMessage,
											const Ice::Current&);
											
	virtual bool getServerFileStructure(	std::string& paFileStructure,
											const Ice::Current&);
											
	virtual bool getServerAppStructure(		std::string& paAppStructure,
											const Ice::Current&);											
											
private:
    clIceClientLogging * meIceClientLogging;
    IceUtil::Mutex meGeneralMutex;
    //Database variable
    vector <clDatabaseTable> meDatabaseTable;
    clDatabaseClass * meDatabaseClass;
	
};

#endif
