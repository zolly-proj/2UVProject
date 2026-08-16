#include "clWorkstationCycle.h"

clWorkstationCycle::clWorkstationCycle(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QString paObjectId, QMutex * paLock, clClassLoader * paClassLoader, QObject * parent)
{
	try
	{
    meIceClientServer = paIceClientServer;
	meIceClientLogging = paIceClientLogging;
	meObjectId = paObjectId;
	meLock = paLock;
	meClassLoader = paClassLoader;
	
	//////// Getting the living object for this machine /////////////////////
	getWorkstationRoutineCycles(meObjectId);
	prepareScriptingPath("./SCRIPTS","./" + meObjectId,true);

	}
	catch(...)
	{
		cout << "clLivingObjectMach::clLivingObjectMach -> failed" << endl;
		
	}
}

clWorkstationCycle::~clWorkstationCycle()
{

}
bool clWorkstationCycle::prepareScriptingPath(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
	try
	{
		QDir sourceDir(fromDir);
		QDir targetDir(toDir);		
		if(!targetDir.exists()){    /* if directory don't exists, build it */
			if(!targetDir.mkdir(targetDir.absolutePath()))
				return false;
		}

		QFileInfoList fileInfoList = sourceDir.entryInfoList();
		foreach(QFileInfo fileInfo, fileInfoList){
			if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
				continue;

			if(fileInfo.isDir()){    /* if it is directory, copy recursively*/
				if(!prepareScriptingPath(fileInfo.filePath(),  
					targetDir.filePath(fileInfo.fileName()), 
					coverFileIfExist)) 
					return false;
			}
			else{            /* if coverFileIfExist == true, remove old file first */
				if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
					targetDir.remove(fileInfo.fileName());
				}

				// files copy
				if(!QFile::copy(fileInfo.filePath(),  
					targetDir.filePath(fileInfo.fileName()))){ 
						return false;
				}
			}
		}
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::prepareScriptingPath -> " + QString(e.what()));
		return false;
    }
}


bool clWorkstationCycle::getWorkstationRoutineCycles(QString paObjectId)
{
	
	try
	{
		/*****************************************
		* Getting the table information for CYCLE_ROUTINE
		*******************************************/		
        vector<std::string> loPropertyNameCycleRoutine;
		vector<std::string> loAliasCycleRoutine;
		vector<std::string> loTypeCycleRoutine;
		vector<std::string> loExtraCycleRoutine;
		vector<std::string> loReferenceCycleRoutine;
        QString loReturnMessageCycleRoutine;
		bool loGetColumns = false;

		QString loTableNameRoutine = QString("CYCLE_ROUTINE");


		loGetColumns = meIceClientServer->getAllPropertiesFromTable(loTableNameRoutine,loPropertyNameCycleRoutine,loAliasCycleRoutine,loTypeCycleRoutine,loExtraCycleRoutine,loReferenceCycleRoutine,loReturnMessageCycleRoutine);
		if (!loGetColumns)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getWorkstationRoutineCycles-> " + loReturnMessageCycleRoutine);
			return false;
		}
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getWorkstationRoutineCycles-> " + loReturnMessageCycleRoutine);
		
		
		for (int i = 0; i < loPropertyNameCycleRoutine.size(); i++)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getWorkstationRoutineCycles-> -> Propertie name [" + QString(loPropertyNameCycleRoutine[i].c_str()) + "]");
			clDatabaseColumn test(								QString(loPropertyNameCycleRoutine[i].c_str()),
																QString(loAliasCycleRoutine[i].c_str()),
																QString(loTypeCycleRoutine[i].c_str()),
																QString(loExtraCycleRoutine[i].c_str()),
																QString(loReferenceCycleRoutine[i].c_str()));
																
			meDatabaseColumnCycleRoutine.push_back(test);
		}
		/*****************************************
		* Getting the table information for CYCLES
		*******************************************/		
        vector<std::string> loPropertyNameCycle;
		vector<std::string> loAliasCycle;
		vector<std::string> loTypeCycle;
		vector<std::string> loExtraCycle;
		vector<std::string> loReferenceCycle;
        QString loReturnMessageCycle;
		bool loGetColumnsCycle = false;
		
		QString loTableNameCycle = QString("CYCLES");


		loGetColumnsCycle = meIceClientServer->getAllPropertiesFromTable(loTableNameCycle,loPropertyNameCycle,loAliasCycle,loTypeCycle,loExtraCycle,loReferenceCycle,loReturnMessageCycle);
		if (!loGetColumnsCycle)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getWorkstationRoutineCycles-> " + loReturnMessageCycle);
			return false;
		}
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getWorkstationRoutineCycles-> " + loReturnMessageCycle);
		
		
		for (int i = 0; i < loPropertyNameCycle.size(); i++)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getWorkstationRoutineCycles-> -> Propertie name [" + QString(loPropertyNameCycle[i].c_str()) + "]");
			clDatabaseColumn test(								QString(loPropertyNameCycle[i].c_str()),
																QString(loAliasCycle[i].c_str()),
																QString(loTypeCycle[i].c_str()),
																QString(loExtraCycle[i].c_str()),
																QString(loReferenceCycle[i].c_str()));
																
			meDatabaseColumnCycle.push_back(test);
		}		
		/*****************************************
		* Getting all the ROUTINE tables
		*******************************************/
        vector<std::string> loTables;
        QString loMessage;
		QString loTable;
        meIceClientServer->getAllTablesFromDatabase(loTables,loMessage);

        for(int i=0; i < loTables.size(); i++)
        {
			
            loTable = QString(loTables[i].c_str());
			if (!loTable.contains("RTI_", Qt::CaseInsensitive) ||(loTable.indexOf(QString("a_")) == 0))
			{
				loTables.erase(loTables.begin() + i);
				i = i - 1;
			}
        }		
		
		if (loTables.size() < 1) return false;
		
		meRoutineTables = loTables;
		/*****************************************
		* Getting the table information for ROUTINE
		*******************************************/		
		/*
        vector<std::string> loPropertyNameRoutine;
		vector<std::string> loAliasRoutine;
		vector<std::string> loTypeRoutine;
		vector<std::string> loExtraRoutine;
		vector<std::string> loReferenceRoutine;
        QString loReturnMessageRoutine;
		bool loGetColumnsRoutine = false;
		
		loGetColumnsRoutine = meIceClientServer->getAllPropertiesFromTable(QString(loTables.at(0).c_str()),loPropertyNameRoutine,loAliasRoutine,loTypeRoutine,loExtraRoutine,loReferenceRoutine,loReturnMessageRoutine);
		if (!loGetColumnsRoutine)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getWorkstationRoutineCycles-> " + loReturnMessageRoutine);
			return false;
		}
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getWorkstationRoutineCycles-> " + loReturnMessageRoutine);
		
		
		for (int i = 0; i < loPropertyNameRoutine.size(); i++)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getWorkstationRoutineCycles-> -> Propertie name [" + QString(loPropertyNameRoutine[i].c_str()) + "]");
			clDatabaseColumn test(								QString(loPropertyNameRoutine[i].c_str()),
																QString(loAliasRoutine[i].c_str()),
																QString(loTypeRoutine[i].c_str()),
																QString(loExtraRoutine[i].c_str()),
																QString(loReferenceRoutine[i].c_str()));
																
			meDatabaseColumnRoutine.push_back(test);
		}				
		
		*/
		
		/*****************************************
		* Getting the CYCLE_ROUTINE objects for this cycle
		*******************************************/
		QString loTableName("CYCLE_ROUTINE");
		vector <std::string> loProperties;
		vector <std::string> loValues;
		vector <std::string> loTypeValues;
		vector <std::string> loLogExp;
		vector <std::string> loReturnIds;
		QString loReturnMessage;
		
		loProperties.push_back(QString("CYCLE").toStdString());
		loValues.push_back(meObjectId.toStdString());
		loTypeValues.push_back(QString("uuid").toStdString());
		loLogExp.push_back(QString("=").toStdString());
		
		QString loStartStop = QString("0");

		if (!meIceClientServer->getFromTableDatbaseByProperty(loTableName,loStartStop,loStartStop,loProperties,loValues,loTypeValues,loLogExp,loReturnIds,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycles::getWorkstationCycles() -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycles::getWorkstationCycles() -> " + loReturnMessage);
		
		if(loReturnIds.size() < 1)return false;
		meCycleRoutineIds = loReturnIds;
		
		
		//TODO WVA YES START THE CYCLE//
		meException = false;
		meQuit = false;
		meState = 0;
		meDebug = false;
		
		return true;
	}
	catch(...)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::addElementsToQTreeWidgetSub -> error");
		return false;
	}
}
void clWorkstationCycle::run()
{

	try
	{
		//Setting the variables//
		meQuit = false;
		meException = false;
		meRunning = false;
		meTimeOutFromCycle = 50;
		//Should we do this ????
		meState = READ_CURRENT_ROUTINE;
		
		
		
		//Perform the routine//
		while(!meException && !meQuit)
		{
			meRunning = true;
			//cout << endl << "This is the thread [" << meObjectId.toStdString() << endl;
			
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::run()-> cycle name [" + meCycleName + "] is in state[" + QString::number(meState) + "]");
			
			switch (meState)
			{
				case READ_CURRENT_ROUTINE:
								if (!readCurrentRoutine())
								{	
									meException = true;
									meState = READ_CURRENT_ROUTINE;
								}
								else
									meState = GET_ROUTINE_FROM_CYCLE_ROUTINE;
								break;
				case GET_ROUTINE_FROM_CYCLE_ROUTINE:
								if (!getRoutineFromCycleRoutine())
								{
									meState = READ_CURRENT_ROUTINE;
									meException = true;
								}
								else meState = SELECT_ROUTINE;
								break;
				case SELECT_ROUTINE:
								if (!selectRoutine())
								{
									meState = READ_CURRENT_ROUTINE;
									meException = true;									
								}
								else meState = PERFORM_ROUTINE;
								break;
				case PERFORM_ROUTINE:
								/**********************************************************************
								* Select the method call
								**********************************************************/
								if (meCurrentRoutineSourceType.toUpper().compare(QString("DLL")) == 0)
								{
									meState = CALL_ROUTINE_DLL;
								}
								else if (meCurrentRoutineSourceType.toUpper().compare(QString("PYTHON")) == 0 && meDebug)
								{
									meState = CALL_ROUTINE_PYTHON_WITH_DEBUG;
								}
								else if (meCurrentRoutineSourceType.toUpper().compare(QString("PYTHON")) == 0 && !meDebug)
								{
									meState = CALL_ROUTINE_PYTHON_NO_DEBUG;
								}
								else 
								{
									meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::run()-> source file type [" + meCurrentRoutineSourceType + "] not supported");
									meException = true;
									meState = READ_CURRENT_ROUTINE;
									
								}
								break;
				case CALL_ROUTINE_PYTHON_NO_DEBUG:
								if(!callRoutinePythonNoDebug())
								{
									meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::run()-> callRoutinePythonNoDebug failed");
									meException = true;
									meState = PERFORM_ROUTINE;
									
								}
								else
								{ 
									if (meResultFromFunction == -1)
									{
										meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::run()-> callRoutinePythonNoDebug script returned -1");
										meException = true;
										meState = PERFORM_ROUTINE;
																				
									}
									else meState = CHOOSE_AND_SET_NEXT_ROUTINE;
								}
								break;
				case CALL_ROUTINE_PYTHON_WITH_DEBUG:
								if(!callRoutinePythonWithDebug())
								{
									meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::run()-> callRoutinePythonWithDebug failed");
									meException = true;
									meState = PERFORM_ROUTINE;
									
								}
								else
								{ 
									if (meResultFromFunction == -1)
									{
										meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::run()-> callRoutinePythonWithDebug script returned -1");
										meException = true;
										meState = PERFORM_ROUTINE;
																				
									}
									else meState = CHOOSE_AND_SET_NEXT_ROUTINE;
								}
								break;
				case CALL_ROUTINE_DLL:
								if(!callRoutineDLL())
								{
									meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::run()-> callRoutineDLL failed");
									meException = true;
									meState = PERFORM_ROUTINE;
									
								}
								else
								{ 
									if (meResultFromFunction == -1)
									{
										meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::run()-> callRoutineDLL dll returned -1");
										meException = true;
										meState = PERFORM_ROUTINE;
																				
									}
									else meState = CHOOSE_AND_SET_NEXT_ROUTINE;
								}
								break;
				case CHOOSE_AND_SET_NEXT_ROUTINE:
								if (!chooseAndSetNextRoutine())
								{
									meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::run()-> chooseAndSetNextRoutine failed");
									meException = true;
									meState = PERFORM_ROUTINE;
									
									//Set the nex routine to empty
								}
								else meState = READ_CURRENT_ROUTINE;
								break;
				default:
					meState = READ_CURRENT_ROUTINE;
			}
			
			QThread::msleep(meTimeOutFromCycle);
		}
		//Setting the variables//
		meRunning = false;
		cout << "thread finished" << endl;
		//Signal the the the thread is done//
		//emit resultReady(QString("finished signal"));
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::cycle() -> " + QString(e.what()));
		//Setting the variables//
		meRunning = false;
		meException = true;
    }
}
bool clWorkstationCycle::readCurrentRoutine()
{
	try
	{
		/*****************************************
		* Getting the CURRENT_ROUTINE from cycle
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back("CURRENT_ROUTINE");
		

		QString loTableNameCycles = QString("CYCLES");


		if (!meIceClientServer->getFromTableDatabaseById(loTableNameCycles,meObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::readCurrentRoutine() -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::readCurrentRoutine() -> " + loReturnMessage);
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::readCurrentRoutine() -> no results");
			return false;			
		}
		
		/*****************************************
		* Check if current cycle_routine is empty
		*******************************************/			
		QString loCurrentCycleRoutine = QString(loReturnValues[0].c_str());
		if (loCurrentCycleRoutine.compare(QString("")) == 0)
		{
			/*****************************************
			* Getting the first CYCLE_ROUTINE object for this cycle
			*******************************************/
			QString loTableNameCR("CYCLE_ROUTINE");
			vector <std::string> loPropertiesCR;
			vector <std::string> loValuesCR;
			vector <std::string> loTypeValuesCR;
			vector <std::string> loLogExpCR;
			vector <std::string> loReturnIdsCR;
			QString loReturnMessageCR;
			
			loPropertiesCR.push_back(QString("CYCLE").toStdString());
			loValuesCR.push_back(meObjectId.toStdString());
			loTypeValuesCR.push_back(QString("uuid").toStdString());
			loLogExpCR.push_back(QString("=").toStdString());

			loPropertiesCR.push_back(QString("PARENT_CLASS").toStdString());
			loValuesCR.push_back(QString("CYCLES").toStdString());
			loTypeValuesCR.push_back(QString("varchar(255)").toStdString());
			loLogExpCR.push_back(QString("=").toStdString());
			
			QString loStartStop = QString("0");

			if (!meIceClientServer->getFromTableDatbaseByProperty(loTableNameCR,loStartStop,loStartStop,loPropertiesCR,loValuesCR,loTypeValuesCR,loLogExpCR,loReturnIdsCR,loReturnMessageCR))
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::readCurrentRoutine() -> " + loReturnMessageCR);
				return false;
			}
			else
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::readCurrentRoutine() -> " + loReturnMessageCR);
			
			if(loReturnIdsCR.size() < 1)
			{
				return false;
			}
			meCurrentCycleRoutineUUID = QString(loReturnIdsCR[0].c_str());
		}
		else
		{
			/***************************************************************
			* Set the selected CYCLE_ROUTINE object for this cycle
			*****************************************************************/
			meCurrentCycleRoutineUUID = loCurrentCycleRoutine;
		}	
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::readCurrentRoutine() -> " + QString(e.what()));
		return false;
    }
}
bool clWorkstationCycle::getRoutineFromCycleRoutine()
{
	try
	{
		/*****************************************
		* Getting the ROUTINE from the CURRENT_ROUTINE
		*******************************************/
		
		vector <std::string> loPropertiesRCR;
		vector <std::string> loReturnValuesRCR;
		QString loReturnMessageRCR;

		loPropertiesRCR.push_back("ROUTINE");
		loPropertiesRCR.push_back("ROUTINE_CLASS");
		
		QString loTableNameCycleRoutine = QString("CYCLE_ROUTINE");


		if (!meIceClientServer->getFromTableDatabaseById(loTableNameCycleRoutine,meCurrentCycleRoutineUUID,loPropertiesRCR,loReturnValuesRCR,loReturnMessageRCR))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getRoutineFromCycleRoutine() -> " + loReturnMessageRCR);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getRoutineFromCycleRoutine() -> " + loReturnMessageRCR);
		
		if (loReturnValuesRCR.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getRoutineFromCycleRoutine() -> no results");
			return false;			
		}
		/*****************************************
		* Check if routine is valid
		*******************************************/
		meCurrentRoutineUUID = QString(loReturnValuesRCR[0].c_str());
		meCurrentRoutineClass = QString(loReturnValuesRCR[1].c_str());

		if (meCurrentRoutineUUID.compare(QString("")) == 0 || meCurrentRoutineClass.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::run() -> no [ROUTINE] UUID or ROUTINE_CLASS in selected [CYCLE_ROUTINE]");
			return false;									
		}
		else
		{
			meState = SELECT_ROUTINE;
		}		
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::getRoutineFromCycleRoutine() -> " + QString(e.what()));
		return false;
    }
}
bool clWorkstationCycle::selectRoutine()
{
	try
	{
		/**************************************************************
		* Getting all the properties from the table ROUTINE
		****************************************************************/		
		vector<std::string> loPropertyNamePR;
		vector<std::string> loAliasPR;
		vector<std::string> loTypePR;
		vector<std::string> loExtraPR;
		vector<std::string> loReferencePR;
		QString loReturnMessagePR;

		if (!meIceClientServer->getAllPropertiesFromTable(QString(meCurrentRoutineClass),loPropertyNamePR,loAliasPR,loTypePR,loExtraPR,loReferencePR,loReturnMessagePR))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::selectRoutine()-> " + loReturnMessagePR);
			return false;
		}
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::selectRoutine()-> " + loReturnMessagePR);
		
		meCurrentDatabaseColumnRoutine.clear();
		for (int i = 0; i < loPropertyNamePR.size(); i++)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::selectRoutine()-> Propertie name [" + QString(loPropertyNamePR[i].c_str()) + "]");
			clDatabaseColumn test(								QString(loPropertyNamePR[i].c_str()),
																QString(loAliasPR[i].c_str()),
																QString(loTypePR[i].c_str()),
																QString(loExtraPR[i].c_str()),
																QString(loReferencePR[i].c_str()));
																
			meCurrentDatabaseColumnRoutine.push_back(test);
		}						
		/**************************************************************
		* Getting all the properties from the table ROUTINE where UUID
		****************************************************************/						
		
		vector <std::string> loReturnValuesPRWU;
		QString loReturnMessagePropPRWU;

		if (!meIceClientServer->getFromTableDatabaseById(meCurrentRoutineClass,meCurrentRoutineUUID,loPropertyNamePR,loReturnValuesPRWU,loReturnMessagePropPRWU))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::selectRoutine() -> " + loReturnMessagePropPRWU);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::selectRoutine() -> " + loReturnMessagePropPRWU);
		
		if (loReturnValuesPRWU.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::selectRoutine() -> no results");
			return false;			
		}			
		/**************************************************************
		* Save the result of the selected routine (create class WVA ?)
		****************************************************************/
		meCurrentRoutineArgType.clear();
		meCurrentRoutineArgValue.clear();
		
		for (int i = 0; i < loPropertyNamePR.size(); i++)
		{	
			QString loColumnName = QString(loPropertyNamePR.at(i).c_str());
			
			if (loColumnName.toUpper().compare(QString("PKEY")) == 0)
			{
				//Already set so do nothing//
				//meCurrentRoutineUUID = QString(loReturnValues[i].c_ctr());
			}
			else if (loColumnName.toUpper().compare(QString("NAME")) == 0)
			{
				meCurrentRoutineName = QString(loReturnValuesPRWU.at(i).c_str());
			}
			else if (loColumnName.toUpper().compare(QString("SOURCE_TYPE")) == 0)
			{
				meCurrentRoutineSourceType = QString(loReturnValuesPRWU.at(i).c_str());
			}
			else if (loColumnName.toUpper().compare(QString("SOURCE_FILE")) == 0)
			{
				meCurrentRoutineSourceFile = QString(loReturnValuesPRWU.at(i).c_str());
			}
			else if (loColumnName.toUpper().compare(QString("METHOD_NAME")) == 0)
			{
				meCurrentRoutineReturnType = QString(loReturnValuesPRWU.at(i).c_str());
			}
			else
			{
				meCurrentRoutineArgType.push_back(QString(loPropertyNamePR.at(i).c_str()));
				meCurrentRoutineArgValue.push_back(QString(loReturnValuesPRWU.at(i).c_str()));
			}
		}
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::selectRoutine() -> " + QString(e.what()));
		return false;
    }
}
bool clWorkstationCycle::performRoutine()
{
	try
	{
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performRoutine() -> " + QString(e.what()));
		return false;
    }
}
bool clWorkstationCycle::callRoutinePythonNoDebug()
{

	try
	{
		
		
		char * loArg01 = NULL;
		char * loArg02 = NULL;
		char * loArg03 = NULL;
		char * loArg04 = NULL;
		char * loArg05 = NULL;
		char * loArg06 = NULL;
		char * loArg07 = NULL;
		char * loArg08 = NULL;
		char * loArg09 = NULL;
		char * loArg10 = NULL;
		QByteArray byteArray[10];
		
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::callRoutinePythonNoDebug() -> cycle [" + meObjectId + "] has #args[" + QString::number(meCurrentRoutineArgValue.size())  + "] ...");
		
		
		for(int i = 0; i < meCurrentRoutineArgValue.size(); i++)
		{
			byteArray[i] = QString(meCurrentRoutineArgValue.at(i)).toUtf8();
			
			switch(i)
			{
				case 0:
					loArg01 = byteArray[i].data();
					break;
				case 1:
					loArg02 = byteArray[i].data();
					break;
				case 2:
					loArg03 = byteArray[i].data();
					break;
				case 3:
					loArg04 = byteArray[i].data();
					break;
				case 4:
					loArg05 = byteArray[i].data();
					break;
				case 5:
					loArg06 = byteArray[i].data();
					break;
				case 6:
					loArg07 = byteArray[i].data();
					break;
				case 7:
					loArg08 = byteArray[i].data();
					break;
				case 8:
					loArg09 = byteArray[i].data();
					break;
				case 9:
					loArg10 = byteArray[i].data();
					break;					
				default:
					meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::callRoutinePythonNoDebug() -> To many arguments ...");
			}
		}
		QString loTemp = QString(meCurrentRoutineSourceFile);	
        if (!performScript(QString(loTemp.replace(".py","")).toUtf8().data(),meCurrentRoutineClass.toUpper().toUtf8().data(),meCurrentRoutineReturnType.toUtf8().data(),loArg01, loArg02, loArg03, loArg04, loArg05, loArg06, loArg07, loArg08, loArg09, loArg10))
		{
			/*
			if (loArg01 != NULL) delete loArg01;
			if (loArg02 != NULL) delete loArg02;
			if (loArg03 != NULL) delete loArg03;
			if (loArg04 != NULL) delete loArg04;
			if (loArg05 != NULL) delete loArg05;
			if (loArg06 != NULL) delete loArg06;
			if (loArg07 != NULL) delete loArg07;
			if (loArg08 != NULL) delete loArg08;
			if (loArg09 != NULL) delete loArg09;
			if (loArg10 != NULL) delete loArg10;
			*/
			return false;
		}
		cout << endl << "Do something with the return value" << endl;
		/*
		if (loArg01 != NULL) delete loArg01;
		if (loArg02 != NULL) delete loArg02;
		if (loArg03 != NULL) delete loArg03;
		if (loArg04 != NULL) delete loArg04;
		if (loArg05 != NULL) delete loArg05;
		if (loArg06 != NULL) delete loArg06;
		if (loArg07 != NULL) delete loArg07;
		if (loArg08 != NULL) delete loArg08;
		if (loArg09 != NULL) delete loArg09;
		if (loArg10 != NULL) delete loArg10;
		*/
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::callRoutinePythonNoDebug() -> " + QString(e.what()));
		return false;
    }
}
bool clWorkstationCycle::callRoutinePythonWithDebug()
{

	try
	{
		
		
		char * loArg01 = NULL;
		char * loArg02 = NULL;
		char * loArg03 = NULL;
		char * loArg04 = NULL;
		char * loArg05 = NULL;
		char * loArg06 = NULL;
		char * loArg07 = NULL;
		char * loArg08 = NULL;
		char * loArg09 = NULL;
		char * loArg10 = NULL;
		QByteArray byteArray[10];
		
		
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::callRoutinePythonNoDebug() -> cycle [" + meObjectId + "] has #args[" + QString::number(meCurrentRoutineArgValue.size())  + "] ...");
		
		
		for(int i = 0; i < meCurrentRoutineArgValue.size(); i++)
		{
			byteArray[i] = QString(meCurrentRoutineArgValue.at(i)).toUtf8();
			
			switch(i)
			{
				case 0:
					loArg01 = byteArray[i].data();
					break;
				case 1:
					loArg02 = byteArray[i].data();
					break;
				case 2:
					loArg03 = byteArray[i].data();
					break;
				case 3:
					loArg04 = byteArray[i].data();
					break;
				case 4:
					loArg05 = byteArray[i].data();
					break;
				case 5:
					loArg06 = byteArray[i].data();
					break;
				case 66:
					loArg07 = byteArray[i].data();
					break;
				case 7:
					loArg08 = byteArray[i].data();
					break;
				case 8:
					loArg09 = byteArray[i].data();
					break;
				case 9:
					loArg10 = byteArray[i].data();
					break;					
				default:
					meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::callRoutinePythonWithDebug() -> To many arguments ...");
			}
		}
		QString meTemp = QString(meCurrentRoutineSourceFile);
		meTemp.replace(".py","");
        if (!performScriptWithDebug(meCurrentRoutineSourceFile.toUtf8().data(), meTemp.toUtf8().data(),meCurrentRoutineClass.toUpper().toUtf8().data(),meCurrentRoutineReturnType.toUtf8().data(),loArg01, loArg02, loArg03, loArg04, loArg05, loArg06, loArg07, loArg08, loArg09, loArg10))
		{
			/*
			if (loArg01 != NULL) delete loArg01;
			if (loArg02 != NULL) delete loArg02;
			if (loArg03 != NULL) delete loArg03;
			if (loArg04 != NULL) delete loArg04;
			if (loArg05 != NULL) delete loArg05;
			if (loArg06 != NULL) delete loArg06;
			if (loArg07 != NULL) delete loArg07;
			if (loArg08 != NULL) delete loArg08;
			if (loArg09 != NULL) delete loArg09;
			if (loArg10 != NULL) delete loArg10;			
			*/
			return false;
		}
		cout << endl << "Do something with the return value" << endl;
		/*
		if (loArg01 != NULL) delete loArg01;
		if (loArg02 != NULL) delete loArg02;
		if (loArg03 != NULL) delete loArg03;
		if (loArg04 != NULL) delete loArg04;
		if (loArg05 != NULL) delete loArg05;
		if (loArg06 != NULL) delete loArg06;
		if (loArg07 != NULL) delete loArg07;
		if (loArg08 != NULL) delete loArg08;
		if (loArg09 != NULL) delete loArg09;
		if (loArg10 != NULL) delete loArg10;		
		*/
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::callRoutinePythonWithDebug() -> " + QString(e.what()));
		return false;
    }
}
bool clWorkstationCycle::callRoutineDLL()
{
	try
	{
		clMethodCall * loMethodCall = nullptr;
		for (int i = 0; i < meClassLoader->meClassDllNamesGeneral.size(); i++)
		{
			if (meClassLoader->meClassDllNamesGeneral.at(i).toUpper().compare(meCurrentRoutineSourceFile.toUpper()) == 0)
			{
				loMethodCall =  meClassLoader->meMethodCalls.at(i);
				break;
			}
		}
		
		if (loMethodCall != nullptr)
		{
			loMethodCall->doMethod(meCurrentRoutineArgType, meCurrentRoutineArgValue);
			meResultFromFunction = loMethodCall->GetReturnParameters();
		}
		else
		{
			return false;
		}	
		return true;
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::callRoutineDLL -> " + QString(e.what()));
		return false;
	}
}
bool clWorkstationCycle::chooseAndSetNextRoutine()
{

	try
	{
		/*****************************************
		* Getting the ROUTINE from the CURRENT_ROUTINE
		*******************************************/
		
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back("CHILDREN");
		loProperties.push_back("CHILD_CLASS");
		

		QString loTableNameCycleRountine = QString("CYCLE_ROUTINE");

		if (!meIceClientServer->getFromTableDatabaseById(loTableNameCycleRountine,meCurrentCycleRoutineUUID,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::chooseAndSetNextRoutine() -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::chooseAndSetNextRoutine() -> " + loReturnMessage);
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::chooseAndSetNextRoutine() -> no results");
			return false;			
		}
		
		QString loChildren = QString(loReturnValues[0].c_str());
		loChildren = loChildren.replace("{","").replace("}","");
		QString loChildClass = QString(loReturnValues[1].c_str());
		
		//Restart cycle post empty UUID
		if (loChildClass.toUpper().compare(QString("CYCLES")) == 0 && (loChildren.toUpper().compare(QString("")) == 0 || meResultFromFunction == 0))
		{
			vector<std::string> loColumns;
			vector<std::string> loValue;
			vector<std::string> loTypeValue;
			QString loReturnMessage;
			
			loColumns.push_back("CURRENT_ROUTINE");
			loValue.push_back("");
			loTypeValue.push_back("uuid");


			QString loTableNameCycles = QString("CYCLES");

			if (!meIceClientServer->updateIntoTableDatabase(	loTableNameCycles,
																meObjectId,
																loColumns,
																loValue,
																loTypeValue,
																loReturnMessage))
																{
																	meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::chooseAndSetNextRoutine() -> " + loReturnMessage);
																	return false;
																}
			else
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::chooseAndSetNextRoutine() -> " + loReturnMessage);
		}
		//Error ending pos empty UUID
		else if ( loChildren.toUpper().compare(QString("")) == 0 && loChildClass.toUpper().compare(QString("CYCLES")) != 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::chooseAndSetNextRoutine() -> No next routine can be selected for CYCLE_ROUTINE with UUID [" + meCurrentCycleRoutineUUID + "]");
			return false;
		}
		//Select child routine
		else
		{
			QStringList loUUIDlist = loChildren.split(",");
			if ((meResultFromFunction <= loUUIDlist.size()) && meResultFromFunction > 0)
			{
				vector<std::string> loColumns;
				vector<std::string> loValue;
				vector<std::string> loTypeValue;
				QString loReturnMessage;
				
				loColumns.push_back("CURRENT_ROUTINE");
				loValue.push_back(QString(loUUIDlist.at((int)meResultFromFunction - 1)).toStdString());
				loTypeValue.push_back("uuid");


				QString loTableNameCycles = QString("CYCLES");

				if (!meIceClientServer->updateIntoTableDatabase(	loTableNameCycles,
																	meObjectId,
																	loColumns,
																	loValue,
																	loTypeValue,
																	loReturnMessage))
																	{
																		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::chooseAndSetNextRoutine() -> " + loReturnMessage);
																		return false;
																	}
				else
					meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::chooseAndSetNextRoutine() -> " + loReturnMessage);
				
			}
			else
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::chooseAndSetNextRoutine() -> Selected routine is out of range for children of CYCLE_ROUTINE with UUID [" + meCurrentCycleRoutineUUID + "]" + " and return value [" + QString::number(meResultFromFunction) + "]");
				return false;				
			}
			
		}
		
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::chooseAndSetNextRoutine() -> " + QString(e.what()));
		return false;
    }
}
bool clWorkstationCycle::performScript(char * paScriptName, char * paScriptClass, char * paScriptMethod, char *paArg01, char *paArg02, char *paArg03, char *paArg04, char *paArg05, char *paArg06, char *paArg07, char *paArg08, char *paArg09, char *paArg10)
{
		PyGILState_STATE state;
	
		
        try
        {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScript-> cycle [" + meObjectId + "] initialising python");
			meLock->lock();
			
			state = PyGILState_Ensure();

			PyRun_SimpleString("import sys");
			
			PyObject *pName, *pModule, *pDict, *pClass, *pInstance, *pValue;

			QString loObjectPath = QString("sys.path.append('./SCRIPTS/')");
			//QString loObjectPath = QString("sys.path.append('./" + meObjectId + QString("/')"));
			QByteArray loObjectPathTemp = loObjectPath.toLocal8Bit();
			PyRun_SimpleString(loObjectPathTemp.data());
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScript-> cycle [" + meObjectId + "] path imported -> " + loObjectPath);
			
			loObjectPath = QString("sys.path.append('./SCRIPTS/Lib/')");
			//loObjectPath = QString("sys.path.append('./") + meObjectId + QString("/Lib/')");
			loObjectPathTemp = loObjectPath.toLocal8Bit();			
			PyRun_SimpleString(loObjectPathTemp.data());
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScript-> cycle [" + meObjectId + "] libs imported -> " + loObjectPath);
			
            pName = PyUnicode_FromString(paScriptName);
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScript-> python pName created for object [" + meObjectId + "]");
            pModule = PyImport_Import(pName);
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScript-> python pModule created for object [" + meObjectId + "]");
            pDict = PyModule_GetDict(pModule);
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScript-> python pDict created for object [" + meObjectId + "]");
            pClass = PyDict_GetItemString(pDict, paScriptClass);
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScript-> python pClass created for object [" + meObjectId + "]");

            // Create an instance of the class

            if (PyCallable_Check(pClass))
            {
                pInstance = PyObject_CallObject(pClass, NULL);
            }
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScript-> python pInstance created for object [" + meObjectId + "]");

            switch (meCurrentRoutineArgValue.size())
            {
                    case 0:
                        pValue = PyObject_CallMethod(pInstance, paScriptMethod, NULL);
                        break;
                    case 1:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(s)", paArg01);
                        break;
                    case 2:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ss)", paArg01, paArg02);
                        break;
                    case 3:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sss)", paArg01, paArg02, paArg03);
                        break;
                    case 4:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssss)", paArg01, paArg02, paArg03,paArg04);
                        break;
                    case 5:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssss)", paArg01, paArg02, paArg03,paArg04,paArg05);
                        break;
                    case 6:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06);
                        break;
                    case 7:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07);
                        break;
                    case 8:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08);
                        break;
                    case 9:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08,paArg09);
                        break;
                    case 10:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08,paArg09,paArg10);
                        break;
                    default:
                        pValue = PyObject_CallMethod(pInstance, paScriptMethod, NULL);
                        break;
            }
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clWorkstationCycle::performScript-> python Function [" + QString(paScriptClass) + "] called for object [" + meObjectId + "]"));
			
            if (pValue != NULL)
            {
				meResultFromFunction = PyFloat_AsDouble(pValue);
                printf("Return of call : %d\n", (int) meResultFromFunction);
            }
            else
            {
                PyErr_Print();
				meResultFromFunction = -1;
				printf("Return of call : %d\n", (int) meResultFromFunction);
				PyErr_Clear();
				PyRun_SimpleString("import gc");
				PyRun_SimpleString("gc.collect()");				
				
				if (pInstance != NULL) Py_DECREF(pInstance);
				//if (pClass != NULL) Py_DECREF(pClass);
				//if (pDict != NULL) Py_DECREF(pDict);
				if (pModule != NULL) Py_DECREF(pModule);
				if (pName != NULL) Py_DECREF(pName);				
				
				if (!PyGC_IsEnabled()) PyGC_Enable();
				PyGC_Collect();
				PyGILState_Release(state);
				meLock->unlock();		
				
				return false;
            }
			

			
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clWorkstationCycle::performScript-> python Objects cleaned for object [" + meObjectId + "]"));
			PyErr_Clear();
			PyRun_SimpleString("import gc");
			PyRun_SimpleString("gc.collect()");


			//if (pValue != NULL) Py_DECREF(pValue);
			if (pInstance != NULL) Py_DECREF(pInstance);
			//if (pClass != NULL) Py_DECREF(pClass);
			//if (pDict != NULL) Py_DECREF(pDict);
			if (pModule != NULL) Py_DECREF(pModule);
			if (pName != NULL) Py_DECREF(pName);				
			if (!PyGC_IsEnabled()) PyGC_Enable();
			PyGC_Collect();			
			PyGILState_Release(state);
			meLock->unlock();

            return true;
        }
        catch(exception &e)
        {
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScript() -> " + QString(e.what()));
			PyGILState_Release(state);
			meLock->unlock();
            return false;
        }
}

bool clWorkstationCycle::performScriptWithDebug(char * paScriptNameWithExtension, char * paScriptName, char * paScriptClass, char * paScriptMethod, char *paArg01, char *paArg02, char *paArg03, char *paArg04, char *paArg05, char *paArg06, char *paArg07, char *paArg08, char *paArg09, char *paArg10)
{
		PyGILState_STATE state;		
        try
        {
			/*
            STARTUPINFO startupInfo = { 0 };
            PROCESS_INFORMATION processInformation = { 0 };
            startupInfo.cb = sizeof(startupInfo);
			bool loSucces = false;
			
			
			
			meLock->lock();
			
			
            //loSucces = CreateProcess(NULL,const_cast<char *>(("python -c \"import winpdb;winpdb.main()\" " + QString(meCurrentRoutineSourceFile).toStdString()).c_str()), NULL, NULL, FALSE, 0, NULL,NULL, &startupInfo, &processInformation);
			loSucces = CreateProcess(NULL,const_cast<char *>((".\\THIRD_PARTY\\HapDebuggerBin\\HapDebugger.exe " + QString(".\\" + meObjectId + "\\" + meCurrentRoutineSourceFile).toStdString()).c_str()), NULL, NULL, FALSE, 0, NULL,NULL, &startupInfo, &processInformation);
			
			
			if (!loSucces)
			{
				cout << endl << "No success with the function " << endl;
			}
			else
			{
				WaitForSingleObject( processInformation.hProcess, INFINITE );
				CloseHandle( processInformation.hProcess );
				CloseHandle( processInformation.hThread );
			}

			
			
			state = PyGILState_Ensure();

			PyRun_SimpleString("import sys");
			
			PyObject *pName, *pModule, *pDict, *pClass, *pInstance, *pValue;		
			
			QString loObjectPath = QString("sys.path.append(\"./" + meObjectId + QString("/\")"));
			QByteArray loObjectPathTemp = loObjectPath.toLocal8Bit();
			PyRun_SimpleString(loObjectPathTemp.data());			
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScriptWithDebug-> cycle [" + meObjectId + "] path imported -> " + loObjectPath);
			
			loObjectPath = QString("sys.path.append(\"./") + meObjectId + QString("/Lib/\")");
			loObjectPathTemp = loObjectPath.toLocal8Bit();			
			PyRun_SimpleString(loObjectPathTemp.data());
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScriptWithDebug-> cycle [" + meObjectId + "] libs imported -> " + loObjectPath);

            pName = PyUnicode_FromString(paScriptName);
            pModule = PyImport_Import(pName);
            pDict = PyModule_GetDict(pModule);
            pClass = PyDict_GetItemString(pDict, paScriptClass);

            // Create an instance of the class

            if (PyCallable_Check(pClass))
            {
                pInstance = PyObject_CallObject(pClass, NULL);
            }

            switch (meCurrentRoutineArgValue.size())
            {
                    case 0:
                        pValue = PyObject_CallMethod(pInstance, paScriptMethod, NULL);
                        break;
                    case 1:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(s)", paArg01);
                        break;
                    case 2:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ss)", paArg01, paArg02);
                        break;
                    case 3:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sss)", paArg01, paArg02, paArg03);
                        break;
                    case 4:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssss)", paArg01, paArg02, paArg03,paArg04);
                        break;
                    case 5:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssss)", paArg01, paArg02, paArg03,paArg04,paArg05);
                        break;
                    case 6:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06);
                        break;
                    case 7:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07);
                        break;
                    case 8:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08);
                        break;
                    case 9:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08,paArg09);
                        break;
                    case 10:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08,paArg09,paArg10);
                        break;
                    default:
                        pValue = PyObject_CallMethod(pInstance, paScriptMethod, NULL);
                        break;
            }
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clWorkstationCycle::performScriptWithDebug-> python Function [" + QString(paScriptClass) + "] called for object [" + meObjectId + "]"));
			
            if (pValue != NULL)
            {
				meResultFromFunction = PyFloat_AsDouble(pValue);
                printf("Return of call : %d\n", (int) meResultFromFunction);
            }
            else
            {
                PyErr_Print();
				meResultFromFunction = -1;
				printf("Return of call : %d\n", (int) meResultFromFunction);
				PyErr_Clear();
				PyRun_SimpleString("import gc");
				PyRun_SimpleString("gc.collect()");				
				
				if (pInstance != NULL) Py_DECREF(pInstance);
				//if (pClass != NULL) Py_DECREF(pClass);
				//if (pDict != NULL) Py_DECREF(pDict);
				if (pModule != NULL) Py_DECREF(pModule);
				if (pName != NULL) Py_DECREF(pName);				
				
				if (!PyGC_IsEnabled()) PyGC_Enable();
				PyGC_Collect();
				PyGILState_Release(state);
				meLock->unlock();		
				
				return false;
            }
			

			
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clWorkstationCycle::performScriptWithDebug-> python Objects cleaned for object [" + meObjectId + "]"));
			PyErr_Clear();
			PyRun_SimpleString("import gc");
			PyRun_SimpleString("gc.collect()");


			//if (pValue != NULL) Py_DECREF(pValue);
			if (pInstance != NULL) Py_DECREF(pInstance);
			//if (pClass != NULL) Py_DECREF(pClass);
			//if (pDict != NULL) Py_DECREF(pDict);
			if (pModule != NULL) Py_DECREF(pModule);
			if (pName != NULL) Py_DECREF(pName);				
			if (!PyGC_IsEnabled()) PyGC_Enable();
			PyGC_Collect();			
			PyGILState_Release(state);
			meLock->unlock();
*/
            return true;
        }
        catch(exception &e)
        {
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScriptWithDebug() -> " + QString(e.what()));
			PyGILState_Release(state);
			meLock->unlock();
            return false;
        }
}


