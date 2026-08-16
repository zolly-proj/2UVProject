#ifndef CLSERVERTHREAD_H
#define CLSERVERTHREAD_H

#include <string>
#include <exception>
#include <iostream>
#include <vector>

#include <IceUtil/IceUtil.h>
#include <Ice/Ice.h>

#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtCore/QThread>

#include "clIceClientLogging.h"
#include "clServer_impl.h"
#include "clXMLReader.h"
#include "clDatabaseClass.h"
#include "clDatabaseTable.h"
#include "clWorkstationStatus.h"

class clServerThread : public QThread
{
    Q_OBJECT

public:
    clServerThread(clXMLReader * paXMLReaderDatabase, clIceClientLogging * paIceClientLogging, Ice::CommunicatorPtr& paServerCommnicationPtr, QObject *parent = 0);
    ~clServerThread();
	
	void calculateVectorWorkstationInfo();
	//vector <clWorkstationStatus> getWorkstationInfo();

	vector <clWorkstationStatus> meWorkstationStatus;
	UVServerAppServer::ServerAppServerPtr meServant;
protected:
    void run();

private:
    //Local variables
    QMutex meMutex;

    //Communication pointer server
    Ice::CommunicatorPtr meServerCommunicationPtr;

    //Communication class logging client
    clIceClientLogging *meIceClientLogging;

    //Variables
    //Private local vars
    clDatabaseClass * meDatabaseClass;
    clXMLReader * meXMLReaderDatabase;
	vector <clDatabaseRoutine> meDatabaseRoutines;
    vector <clDatabaseTable> meDatabaseTable;
	


	


    //Database functions
    bool createDatabaseConfiguration();
    bool createDatabaseClass();
    void openDB();
    void closeDB();
};
#endif
