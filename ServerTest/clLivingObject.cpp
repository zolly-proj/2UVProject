#define INFO_BUFFER_SIZE 32767
#include "clLivingObject.h"

clLivingObject::clLivingObject(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QMutex * paLock, clClassLoader * paClassLoader)
{
	try
	{
    meIceClientServer = paIceClientServer;
	meIceClientLogging = paIceClientLogging;
	meLock = paLock;
	meClassLoader = paClassLoader;
	//////// Getting the living object for this machine /////////////////////
	getLivingObjectsForThisWorkstation();
	
	
	}
	catch(...)
	{
		cout << "clLivingObject::clLivingObject -> failed" << endl;
		
	}
}

clLivingObject::~clLivingObject()
{

}
bool clLivingObject::getLivingObjectsForThisWorkstation()
{
	try
	{
		////////////////////////////////////////// Getting the living objects machine ///////////////////////////////////////////////////////////////
		QString loTableName("living_obj_mach");
		vector <std::string> loProperties;
		vector <std::string> loValues;
		vector <std::string> loTypeValues;
		vector <std::string> loLogExp;
		vector <std::string> loReturnIds;
		QString loReturnMessage;
		
		loProperties.push_back(QString("WORKSTATION_NAME").toStdString());
		loValues.push_back(QString(QHostInfo::localHostName()).toStdString());
		loTypeValues.push_back(QString("text").toStdString());
		loLogExp.push_back(QString("=").toStdString());
		
		QString loStartStopTemp = QString("0");
		
		if (!meIceClientServer->getFromTableDatbaseByProperty(loTableName,loStartStopTemp,loStartStopTemp,loProperties,loValues,loTypeValues,loLogExp,loReturnIds,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObject::getLivingObjectsForThisWorkstation() -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObject::getLivingObjectsForThisWorkstation() -> " + loReturnMessage);
		
		
		int loThreadNumber = 0;
		
		if (loReturnIds.size() > 0)
		{
			for (int i = 0; i < loReturnIds.size(); i++)
			{
				meThread[loThreadNumber] = new QThread();
				meLivingObjectMach[i] = new clLivingObjectMach(meIceClientServer, meIceClientLogging, QString(loReturnIds.at(i).c_str()), meLock);
				meLivingObjectMach[i]->moveToThread(meThread[loThreadNumber]);
				meThread[loThreadNumber]->start();
				loThreadNumber = loThreadNumber + 1;	
			}
		}
		////////////////////////////////////////// Getting the living objects location ///////////////////////////////////////////////////////////////
		QString loTableNameLocation("LIVING_OBJ_LOCATION");
		vector <std::string> loPropertiesLoc;
		vector <std::string> loValuesLoc;
		vector <std::string> loTypeValuesLoc;
		vector <std::string> loLogExpLoc;
		vector <std::string> loReturnIdsLoc;
		QString loReturnMessageLoc;
		
		loPropertiesLoc.push_back(QString("WORKSTATION_NAME").toStdString());
		loValuesLoc.push_back(QString(QHostInfo::localHostName()).toStdString());
		loTypeValuesLoc.push_back(QString("text").toStdString());
		loLogExpLoc.push_back(QString("=").toStdString());
		
		QString loStartStop = QString("0");
		
		
		
		if (!meIceClientServer->getFromTableDatbaseByProperty(loTableNameLocation,loStartStop,loStartStop,loPropertiesLoc,loValuesLoc,loTypeValuesLoc,loLogExpLoc,loReturnIdsLoc,loReturnMessageLoc))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObject::getLivingObjectsForThisWorkstation() -> " + loReturnMessageLoc);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObject::getLivingObjectsForThisWorkstation() -> " + loReturnMessageLoc);
				
		if (loReturnIdsLoc.size() > 0)
		{		
			for (int i = 0; i < loReturnIdsLoc.size(); i++)
			{
				
				meThread[loThreadNumber] = new QThread();
				meLivingObjectLocator[i] = new clLivingObjectLocator(meIceClientServer, meIceClientLogging, QString(loReturnIdsLoc.at(i).c_str()), meLock, meClassLoader);
				meLivingObjectLocator[i]->moveToThread(meThread[loThreadNumber]);
				meThread[loThreadNumber]->start();
				loThreadNumber = loThreadNumber + 1;		
			}		
		}
		////////////////////////////////////////// Getting the living objects measure ///////////////////////////////////////////////////////////////
		QString loTableNameMeas("LIVING_OBJ_MEAS");
		vector <std::string> loPropertiesMeas;
		vector <std::string> loValuesMeas;
		vector <std::string> loTypeValuesMeas;
		vector <std::string> loLogExpMeas;
		vector <std::string> loReturnIdsMeas;
		QString loReturnMessageMeas;
		
		loPropertiesMeas.push_back(QString("WORKSTATION_NAME").toStdString());
		loValuesMeas.push_back(QString(QHostInfo::localHostName()).toStdString());
		loTypeValuesMeas.push_back(QString("text").toStdString());
		loLogExpMeas.push_back(QString("=").toStdString());
		
		QString loStartStopMeas = QString("0");
		
		
		
		if (!meIceClientServer->getFromTableDatbaseByProperty(loTableNameMeas,loStartStopMeas,loStartStopMeas,loPropertiesMeas,loValuesMeas,loTypeValuesMeas,loLogExpMeas,loReturnIdsMeas,loReturnMessageMeas))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObject::getLivingObjectsForThisWorkstation() -> " + loReturnMessageMeas);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObject::getLivingObjectsForThisWorkstation() -> " + loReturnMessageMeas);
				
		if (loReturnIdsMeas.size() > 0)
		{		
			for (int i = 0; i < loReturnIdsMeas.size(); i++)
			{
				
				meThread[loThreadNumber] = new QThread();
				meLivingObjectMeas[i] = new clLivingObjectMeasure(meIceClientServer, meIceClientLogging, QString(loReturnIdsMeas.at(i).c_str()), meLock, meClassLoader);
				meLivingObjectMeas[i]->moveToThread(meThread[loThreadNumber]);
				meThread[loThreadNumber]->start();
				loThreadNumber = loThreadNumber + 1;		
			}		
		}		
		return true;
	}
	catch(...)
	{
		return false;
	}
}


