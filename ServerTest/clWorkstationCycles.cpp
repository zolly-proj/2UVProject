#define INFO_BUFFER_SIZE 32767
#include "clWorkstationCycles.h"

clWorkstationCycles::clWorkstationCycles(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QMutex * paLock, clClassLoader * paClassLoader)
{
	try
	{
		meIceClientServer = paIceClientServer;
		meIceClientLogging = paIceClientLogging;
		meLock = paLock;
		meClassLoader = paClassLoader;
		//////// Getting the cycles for this machine /////////////////////
		getWorkstationCycles();
	}
	catch(...)
	{
		cout << "clWorkstationCycles::clWorkstationCycles -> failed" << endl;
	}
}

clWorkstationCycles::~clWorkstationCycles()
{
	/*
	PyEval_RestoreThread( tstate ) ; // nb: this also locks the GIL
	Py_Finalize() ;
	*/
	
	//PyGILState_Ensure();
	//Py_Finalize();
	
	for (int i=0; i < 20; i++)
	{
	
	}
}
bool clWorkstationCycles::getWorkstationCycles()
{
	
	try
	{
		////////////////////////////////////////// Getting the cycles ///////////////////////////////////////////////////////////////
		QString loTableName("cycles");
		vector <std::string> loProperties;
		vector <std::string> loValues;
		vector <std::string> loTypeValues;
		vector <std::string> loLogExp;
		vector <std::string> loReturnIds;
		QString loReturnMessage;
		
		loProperties.push_back(QString("WORKSTATION_NAME").toStdString());
		loValues.push_back(string(QString(QHostInfo::localHostName()).toStdString()));
		loTypeValues.push_back(QString("varchar(255)").toStdString());
		loLogExp.push_back(QString("=").toStdString());
		
		QString loStartStop = QString("0");

		if (!meIceClientServer->getFromTableDatbaseByProperty(loTableName,loStartStop,loStartStop,loProperties,loValues,loTypeValues,loLogExp,loReturnIds,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycles::getWorkstationCycles() -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycles::getWorkstationCycles() -> " + loReturnMessage);
				
		if (loReturnIds.size() < 1) return false;
		
		//Initialise the threads
		for(int i=0; i < 20; i++)
		{
			meWorkstationCycle[i] = NULL;
		}
		
		
		// initialize Python
		//Py_Initialize() ;
		//PyEval_InitThreads() ; // nb: creates and locks the GIL
		// NOTE: We save the current thread state, and restore it when we unload,
		// so that we can clean up properly.
		//tstate = PyEval_SaveThread() ;
		
		//Set the threads
		for (int i = 0; i < loReturnIds.size(); i++)
		{
			//Starting the threads//
			meWorkstationCycle[i] = new clWorkstationCycle(meIceClientServer, meIceClientLogging, QString(loReturnIds.at(i).c_str()), meLock, meClassLoader, this);
			connect(meWorkstationCycle[i], &clWorkstationCycle::resultReady, this, &clWorkstationCycles::handleResults);
			//connect(meWorkstationCycle[i], &clWorkstationCycle::finished, meWorkstationCycle[i], &QObject::deleteLater);
			meWorkstationCycle[i]->start();
		}
		
		return true;
	}
	catch(...)
	{
		return false;
	}
}
void clWorkstationCycles::handleResults(const QString &)
{
	try
	{
		
	}
	catch(...)
	{
		
	}	
}


