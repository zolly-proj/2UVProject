#define INFO_BUFFER_SIZE 32767
#include "clLivingObjectMeasure.h"

clLivingObjectMeasure::clLivingObjectMeasure(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QString paObjectId, QMutex * paLock, clClassLoader * paClassLoader, QObject * parent)
{
	try
	{
    meIceClientServer = paIceClientServer;
	meIceClientLogging = paIceClientLogging;
	meObjectId = paObjectId;
	meLock = paLock;
	meClassLoader = paClassLoader;
	
	//////// Getting the living object for this machine /////////////////////
	getLivingObjectMeasureProperties();
	
	
	meSocket = NULL;
	/////// Setting up the timer function ///////////////////////////////////
	meTimer = new QTimer(this);
	meTimer->setInterval(meTimerCycle.toInt());
	meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotDoIt()));
	meTimer->start();
	
	}
	catch(...)
	{
		cout << "getLivingObjectMeasureProperties::getLivingObjectMeasureProperties -> failed" << endl;
		
	}
}

clLivingObjectMeasure::~clLivingObjectMeasure()
{

}
bool clLivingObjectMeasure::getLivingObjectMeasureProperties()
{
	
	try
	{
			/*****************************************
			* Getting the table information for the object
			*******************************************/
			 vector<std::string> loPropertyName;
			 vector<std::string> loAlias;
			 vector<std::string> loType;
			 vector<std::string> loExtra;
			 vector<std::string> loReference;
			 QString loReturnMessageObject;
	
			 
			 
			 QString loClassName = QString("living_obj_meas");
			 
			 
			 meIceClientServer->getAllPropertiesFromTable(  loClassName,
															loPropertyName,
															loAlias,
															loType,
															loExtra,
															loReference,
															loReturnMessageObject);
															
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::getLivingObjectMeasureProperties -> " + loReturnMessageObject);

			
			for (int j=0; j < (int) loPropertyName.size(); j++)
			{
				clDatabaseColumn loDatabaseColumn(  QString(loPropertyName[j].c_str()),
													QString(loAlias[j].c_str()),
													QString(loType[j].c_str()),
													QString(loExtra[j].c_str()),
													QString(loReference[j].c_str()));
				meDatabaseColumnArr.push_back(loDatabaseColumn);
			}
			
			//////////////////////////////////// Getting the values ////////////////////////////////////////////////////////////
			
			vector<std::string> loPropertyNames;
			vector<std::string> loReturnValues;
			QString loReturnMessageGetById;
			
			
			for (int i = 0; i < (int) meDatabaseColumnArr.size(); i++)
			{
				clDatabaseColumn loDatabaseColumn = meDatabaseColumnArr.at(i);
				loPropertyNames.push_back(loDatabaseColumn.getName().toStdString());
			}
			
			
			bool loGetID = false;
			QString loClassNameId = QString("living_obj_meas");
			loGetID = meIceClientServer->getFromTableDatabaseById(	loClassNameId,
																	meObjectId,
																	loPropertyNames,
																	loReturnValues,
																	loReturnMessageGetById);
																	
			if (!loGetID)
			{			
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::getLivingObjectMeasureProperties -> " + loReturnMessageGetById);			
				return false;
			}
			else
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::getLivingObjectMeasureProperties -> " + loReturnMessageGetById);			
			
			//////////////////////////////// Set the connection parameters ////////////////////////////////////////////////////////////
			for (int k = 0; k < (int) loPropertyNames.size(); k++)
			{
				if (QString(loPropertyNames.at(k).c_str()).toUpper().compare("TIMER_CYCLE") == 0)
					meTimerCycle = QString(loReturnValues.at(k).c_str());
				else if (QString(loPropertyNames.at(k).c_str()).toUpper().compare("SOCKET_PORT") == 0)
					meSocketPort = QString(loReturnValues.at(k).c_str());
				else if (QString(loPropertyNames.at(k).c_str()).toUpper().compare("SOCKET_IP") == 0)
					meSocketIp = QString(loReturnValues.at(k).c_str());
				else if (QString(loPropertyNames.at(k).c_str()).toUpper().compare("NAME") == 0)
					meObjectName = QString(loReturnValues.at(k).c_str());					
			}
			return true;
	}
	catch(...)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::getLivingObjectMeasureProperties -> error");			
		return false;
	}
}
void clLivingObjectMeasure::slotDoIt()
{
	
	try
	{
		if (meSocket == NULL)
		{
			meSocket = new QTcpSocket( this ); // <-- needs to be a member variable: QTcpSocket * _pSocket;
			connect( meSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );
		

		
			meSocket->connectToHost(meSocketIp, meSocketPort.toInt());
			meSocket->waitForConnected();

			if (meSocket->state() != QAbstractSocket::ConnectedState ) 
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::slotDoIt() -> cannot connect to host");			
				delete meSocket;
				meSocket = NULL;
				
				
				
				//////////////// Update the connection state ////////////////////////////////////////////////
				vector<std::string> loColumns;
				vector<std::string> loValue;
				vector<std::string> loTypeValue;
				
				loColumns.push_back("CONNECTION_STATE");
				loValue.push_back("0");
				loTypeValue.push_back("int");
				QString loReturnMessage;
				
				
				QString loClass = QString("living_obj_meas");
				
				
				meIceClientServer->updateIntoTableDatabase(     loClass,
																meObjectId,
																loColumns,
																loValue,
																loTypeValue,
																loReturnMessage);
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::slotDoIt -> " + loReturnMessage);				
				
				return;
			}
			else
			{
				//////////////// Update the connection state ////////////////////////////////////////////////
				vector<std::string> loColumns;
				vector<std::string> loValue;
				vector<std::string> loTypeValue;
				
				loColumns.push_back("CONNECTION_STATE");
				loValue.push_back("0");
				loTypeValue.push_back("int");
				QString loReturnMessage;
				
				QString loClass = QString("living_obj_meas");
				
				
				meIceClientServer->updateIntoTableDatabase(     loClass,
																meObjectId,
																loColumns,
																loValue,
																loTypeValue,
																loReturnMessage);
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::slotDoIt -> " + loReturnMessage);								
			}
		}
		else
		{			
			//////////////////////////////// Check if any command to perform ////////////////////////////////////////
			vector<std::string> loPropertyName;
            vector<std::string> loReturnValues;
            QString loReturnMessageGetById;
			loPropertyName.push_back("COMMANDS_TODO");
			
			bool loGetById = false;
			
			QString loClass = QString("living_obj_meas");
			
			
            loGetById = meIceClientServer->getFromTableDatabaseById(    loClass,
																		meObjectId,
																		loPropertyName,
																		loReturnValues,
																		loReturnMessageGetById);
			
			if(!loGetById)
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::slotDoIt -> " + loReturnMessageGetById);
				return;
			}
			else
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::slotDoIt -> " + loReturnMessageGetById);
			
			
			//////////////////////////////// Get the commands ////////////////////////////////////////////////////////////
			QString loCommands;
			for (int k = 0; k < (int) loPropertyName.size(); k++)
			{
				if (QString(loPropertyName.at(k).c_str()).toUpper().compare(QString("COMMANDS_TODO")) == 0)
				{
					//cout << loReturnValues.at(k).c_str() << endl;
					loCommands = QString(loReturnValues.at(k).c_str());
					break;
				}
			}
			
			
			QString loCommandSend = QString("");
			
			
			QStringList loCommandList = loCommands.split(",");
			if (loCommandList.size() > 0 && loCommands.compare("{}") != 0)
			{
				/////////////////// Perform last command in list /////////////////////////	
				QString loCommand = loCommandList.at(loCommandList.size() - 1);
				
				///// Remove last character
				loCommand = loCommand.replace("{","");
				loCommand = loCommand.replace("}","");
				/*
				int pos = loCommand.lastIndexOf(QChar('}'));
				loCommand = loCommand.left(pos);
				pos = loCommand.firstIndexOf(QChar('{'));
				loCommand = loCommand.right(pos);
				*/
				/////// Create command //////////////////////////
				if (!createCommand(loCommand,loCommandSend))
				{
					meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::slotDoIt -> createCommand fail, stopping slot");
					return;					
				}
				
				
				
				
				
				//////////////// Update the database for the commands ////////////////////////////////////////////////														
				vector<std::string> loColumns;
				vector<std::string> loValue;
				vector<std::string> loTypeValue;
				
				loColumns.push_back("COMMANDS_TODO");
				
				
				std::string loValueArr = "";
				
				for (int y = 0; y < (int) loCommandList.size() - 1; y++)
				{
						if (loValueArr == "")
						{
							loValueArr = loValueArr + QString(loCommandList.at(y)).replace("{","").toStdString();
						}
						else
						{
							loValueArr = loValueArr + "$;$" + QString(loCommandList.at(y)).replace("{","").toStdString();
						}
				}
				
				
				loValue.push_back(loValueArr);
				loTypeValue.push_back("text[]");
				QString loReturnMessage;
				
				QString loClass = QString("living_obj_meas");
				
				meIceClientServer->updateIntoTableDatabase(     loClass,
																meObjectId,
																loColumns,
																loValue,
																loTypeValue,
																loReturnMessage);
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::slotDoIt -> " + loReturnMessage);
				
			}
			else
			{
				////////////////// Create status ///////////////////////////////////////
				QString loCommand = QString("state");
				/////// Create command //////////////////////////
				if (!createCommand(loCommand,loCommandSend))
				{
					meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::slotDoIt -> createCommand fail, stopping slot");
					return;					
				}				
			}
			/////////////////////////////// If no command should be performed do state command //////////////////////
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clLivingObjectMeasure::slotDoIt() command send to machine -> " + loCommandSend));
			
			//Short check before send
			if (meSocket->state() != QAbstractSocket::ConnectedState ) 
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::slotDoIt() -> cannot connect to host");			
				delete meSocket;
				meSocket = NULL;
				return;
			}
			meSocket->write(loCommandSend.toUtf8());
			if (meSocket->state() != QAbstractSocket::ConnectedState ) 
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::slotDoIt() -> cannot connect to host");			
				delete meSocket;
				meSocket = NULL;
				return;
			}			
			meSocket->waitForBytesWritten(50);
			//meSocket->waitForReadyRead(200);
		}
	}
	catch(...)
	{
		meSocket = NULL;
		return;
	}
}

bool clLivingObjectMeasure::createCommand(QString paCommand,QString &paSocketCommand)
{
	
	try
	{
		QStringList loCommandParts = paCommand.split("$C$");
		
		QString loConnect("false");
		QString loDisconnect("false");
		QString loState("false");
		QString loRun("false");
		QString loAbort("false");
		QString loHold("false");
		QString loContinue("false");
		QString loSendFile("false");
		QString loReceiveFile("false");
		QString loScriptCommand("false");
		QString loOptionalCommand("false");
		
		QString loConnectParams("");
		QString loDisconnectParams("");
		QString loStateParams("");
		QString loRunParams("");
		QString loAbortParams("");
		QString loHoldParams("");
		QString loContinueParams("");
		QString loSendFileParams("");
		QString loReceiveFileParams("");
		QString loScriptCommandParams("");
		QString loOptionalCommandParams("");

		QString loParameters = QString("");
		if (loCommandParts.size() > 1)
		{
			for (int k = 1; k < loCommandParts.size(); k++)
			{
				loParameters = loParameters + QString("<data>") + loCommandParts.at(k) + QString("</data>");
			}
		}




		if (loCommandParts.at(0).toLower().compare(QString("connect")) == 0)
		{
			loConnect = QString("true");
			loConnectParams = loParameters;
		}
		else if(loCommandParts.at(0).toLower().compare(QString("disconnect")) == 0)
		{
			loDisconnect = QString("true");
			loDisconnectParams = loParameters;
		}
		else if(loCommandParts.at(0).toLower().compare(QString("state")) == 0)
		{
			loState = QString("true");
			loStateParams = loParameters;
		}
		else if(loCommandParts.at(0).toLower().compare(QString("run")) == 0)
		{
			loRun = QString("true");
			loRunParams = loParameters;
		}	
		else if(loCommandParts.at(0).toLower().compare(QString("abort")) == 0)
		{
			loAbort = QString("true");
			loAbortParams = loParameters;
		}		
		else if(loCommandParts.at(0).toLower().compare(QString("hold")) == 0)
		{
			loHold = QString("true");
			loHoldParams = loParameters;
		}		
		else if(loCommandParts.at(0).toLower().compare(QString("continue")) == 0)
		{
			loContinue = QString("true");
			loContinueParams = loParameters;
		}		
		else if(loCommandParts.at(0).toLower().compare(QString("sendFile")) == 0)
		{
			loSendFile = QString("true");
			loSendFileParams = loParameters;
		}		
		else if(loCommandParts.at(0).toLower().compare(QString("receiveFile")) == 0)
		{
			loReceiveFile = QString("true");
			loReceiveFileParams = loParameters;
		}		
		else if(loCommandParts.at(0).toLower().compare(QString("scriptcommand")) == 0)
		{
			loScriptCommand = QString("true");
			loScriptCommandParams = loParameters;
		}		
		else if(loCommandParts.at(0).toLower().compare(QString("optionalcommand")) == 0)
		{
			loOptionalCommand = QString("true");
			loOptionalCommandParams = loParameters;
		}
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clLivingObjectMeasure::createCommand() -> command not implemented" + loCommandParts.at(0)));
			return false;
		}
		

		
		
		paSocketCommand = QString		(QString("<hardwareDevice>") +
												QString("<id>") + QString::number(QRandomGenerator::global()->generate()) + QString("</id>") +
												QString("<connect do='") + loConnect + QString("'>") + loConnectParams + QString("</connect>") +
												QString("<disconnect do='") + loDisconnect + QString("'>") + loDisconnectParams + QString("</disconnect>") +
												QString("<state do='") + loState + QString("'>") + loStateParams + QString("</state>") +
												QString("<run do='") + loRun + QString("'>") + loRunParams + QString("</run>") +
												QString("<abort do='") + loAbort + QString("'>") + loAbortParams + QString("</abort>") +
												QString("<hold do='") + loHold + QString("'>") + loHoldParams + QString("</hold>") +
												QString("<continue do='") + loContinue + QString("'>") + loContinueParams + QString("</continue>") +
												QString("<sendFile do='") + loSendFile + QString("'>") + loSendFileParams + QString("</sendFile>") +
												QString("<receiveFile do='") + loReceiveFile + QString("'>") + loReceiveFileParams + QString("</receiveFile>") +
												QString("<scriptCommand do='") + loScriptCommand + QString("'>") + loScriptCommandParams + QString("</scriptCommand>") +
												QString("<optionalCommand do='") + loOptionalCommand + QString("' name='SendDataCommand2'>") +
												loOptionalCommandParams +
												QString("</optionalCommand>") +
											QString("</hardwareDevice>"));		
		return true;
	}
	catch(...)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::createCommand() -> error");
		return false;
	}
}


void clLivingObjectMeasure::readTcpData()
{
	
	try
	{
		//Short check before send
		if (meSocket->state() != QAbstractSocket::ConnectedState ) 
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::readTcpData() -> cannot connect to host");			
			delete meSocket;
			meSocket = NULL;
			return;
		}
		meData = meSocket->readAll();
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clLivingObjectMeasure::readTcpData() command recieved from machine -> " + QString::fromLocal8Bit(meData.constData())));
		//// Parse the input TODO WVA///////////
		//<returnHardwareDevice><id>0</id><state>W0R0P0D0C0T0</state><datas><data>measurevalue01</data><data>measurevalue02</data></datas><error>Connection Socket Error on host:[' + self.host + '] port:[' + self.port + ']...</error></returnHardwareDevice>

        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( meData ) ) 
		{
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clLivingObjectMeasure::readTcpData() command recieved from machine -> replay XML not valid ..."));
            return;
        }

       QDomElement loDocElem = loDomDocument.documentElement();
		
       QDomNode loTablesNode = loDocElem.firstChild();
	   
	   
	   
	   
		//Set the state of the machine
		int loStateEnum = 0;
		int loStateDataTransfer = 0;
		
		bool loTriggerError = false;
		bool loChangeDataTransfer = false;
		bool loChangeState = false;
		bool loStartOperation = false;
		bool loCloseOperation = false;	   
		bool loWarning = false;
		bool loInformation = false;
		QString loStateEnumPrevious;
		QString loStateDataTransferPrevious;
		QString loErrorText = QString("");		
	   
	   
		//Data passed
		vector <float> loDatasFloatInside;
		vector <QString> loDatasStringInside;
	   
	   
       while( !loTablesNode.isNull() ) 
	   {
			if(loTablesNode.nodeName() == "state")
			{
				QDomElement loTablesElem = loTablesNode.toElement();
				QString loState = loTablesElem.text();
				if (loState.compare(QString("")) != 0)
				{
					////////////////////////////////// Parse state -> W0R0P0D0C0T123456789 ///////////////////////////////
					bool loWaiting, loRunning, loDataTransfer, loConnected, loPaused;
					
					//Get the status of the machine
					if(loState.contains("W0"))loWaiting = false;
					if(loState.contains("W1"))loWaiting = true;
					if(loState.contains("R0"))loRunning = false;
					if(loState.contains("R1"))loRunning = true;
					if(loState.contains("D0"))loDataTransfer = false;
					if(loState.contains("D1"))loDataTransfer = true;
					if(loState.contains("C0"))loConnected = false;
					if(loState.contains("C1"))loConnected = true;
					if(loState.contains("P0"))loPaused = false;
					if(loState.contains("P1"))loPaused = true;
					
					if (!loWaiting && loRunning && loConnected && !loPaused)loStateEnum = 20;
					else if (!loWaiting && loRunning && loConnected && loPaused)loStateEnum = 30;
					else if (loWaiting && !loRunning && loConnected && !loPaused)loStateEnum = 10;
					else if (loWaiting && !loRunning && loConnected && loPaused)loStateEnum = 30;
					else if (!loConnected)loStateEnum = 40;
					else if (!loWaiting && !loRunning && loConnected)loStateEnum = 100;
					else if (loWaiting && loRunning && loConnected)loStateEnum = 100;
					
					if (!loDataTransfer)loStateDataTransfer = 0;
					if (loDataTransfer)loStateDataTransfer = 10;					
				}				
			}
			if(loTablesNode.nodeName() == "datas")
			{
				QDomElement loTablesElem = loTablesNode.toElement();
				
				QDomNode loDatasNodes = loTablesElem.firstChild();
				
				while( !loDatasNodes.isNull() ) 
				{
				   if (loDatasNodes.nodeName() == "data")
				   {
					   QDomElement loDatasElement = loDatasNodes.toElement();
					   
					   try
					   {
						   loDatasFloatInside.push_back(loDatasElement.text().toFloat());
					   }
					   catch(exception &exe01)
					   {
						   meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::readTcpData -> could not convert data to float " + QString(exe01.what()));
					   }
					   try
					   {
						   loDatasStringInside.push_back(loDatasElement.text());
					   }
					   catch(exception &exe02)
					   {
						   meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::readTcpData -> " + QString(exe02.what()));
					   }
					   
				   }
				   loDatasNodes = loDatasNodes.nextSibling();
				}
			}
			loTablesNode = loTablesNode.nextSibling();	
	   }
	   if ((loDatasStringInside.size() > 0) || (loDatasFloatInside.size() > 0))
	   {
			//Getting the product object
			clObject loObject(meIceClientServer,meIceClientLogging);
			loObject.ClassName = QString("OPERATION");
		
			loObject.meObjectCalls = meClassLoader->meObjectCalls;
			loObject.meClassNameList = meClassLoader->meClassNames;
			loObject.meClassMethodsList = meClassLoader->meClassMethods;
			
			loObject.meMethodNamesList = meClassLoader->meMethodsGeneralNames;
			loObject.meMethodCalls = meClassLoader->meMethodCalls;
			
			loObject.meDatabaseColumnsByClassNameList = meClassLoader->meDatabaseColumnsByClassName;
					
			//bool clObject::insertObject(vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValues)

			vector <QString> loParametersTypeValues;
			vector <QString> loParametersColumns;
			vector <QString> loParametersValues;

			loParametersColumns.push_back("NAME");
			loParametersValues.push_back(QString("System " + meObjectName));
			loParametersTypeValues.push_back("varchar(255)");

			loParametersColumns.push_back("OBJECT");
			loParametersValues.push_back(meObjectId);
			loParametersTypeValues.push_back("uuid");

			loParametersColumns.push_back("OBJECTS");
			loParametersValues.push_back(meObjectId);
			loParametersTypeValues.push_back("uuid[]");
				
			loParametersColumns.push_back("DATUM_START");
			loParametersValues.push_back(QString((QDateTime::currentDateTime()).toString("yyyy-MM-dd HH:mm:ss.zzz")));
			loParametersTypeValues.push_back("timestamp(3)");

			loParametersColumns.push_back("DATUM_STOP");
			loParametersValues.push_back(QString((QDateTime::currentDateTime()).toString("yyyy-MM-dd HH:mm:ss.zzz")));
			loParametersTypeValues.push_back("timestamp(3)");
					
			loParametersColumns.push_back("OPERATION_STATE");
			loParametersValues.push_back("20");
			loParametersTypeValues.push_back("int");			

			loParametersColumns.push_back("OPERATION_CLOSED_CAUSE");
			if (loStateEnum == 110) loParametersValues.push_back("20");
			else loParametersValues.push_back("10");
			loParametersTypeValues.push_back("int");

			loParametersColumns.push_back("OPERATION_ERROR_INFORMATION");
			loParametersValues.push_back(loErrorText);
			loParametersTypeValues.push_back("text");


			QString loFloatValues = QString("");
			for (int l = 0; l < loDatasFloatInside.size() ;l++)
			{
				if (l == 0) loFloatValues = loFloatValues + QString::number(loDatasFloatInside.at(l));
				else loFloatValues = loFloatValues + QString("$;$") + QString::number(loDatasFloatInside.at(l)); 
			}
			loParametersColumns.push_back("MEASURED_FLOAT_VALUE");
			loParametersValues.push_back(loFloatValues);
			loParametersTypeValues.push_back("float8[]");
					
			QString loTextValues = QString("");
			for (int l = 0; l < loDatasStringInside.size() ;l++)
			{
				if (l == 0) loTextValues = loTextValues + QString(loDatasStringInside.at(l));
				else loTextValues = loTextValues + QString("$;$") + QString(loDatasStringInside.at(l)); 					
			}				
			loParametersColumns.push_back("MEASURED_TEXT_VALUE");
			loParametersValues.push_back(loTextValues);
			loParametersTypeValues.push_back("text[]");				

			//	bool insertObject(vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValues);
			loObject.insertObject(loParametersColumns, loParametersValues, loParametersTypeValues);	
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::readTcpData() measure value inserted");
		}
	}
	catch(...)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMeasure::readTcpData() -> error");
	}
}
/*********************************************************************
* Object calls
**********************************************************************/
clObjectCall * clLivingObjectMeasure::callObjectDLL(QString paName)
{
	try
	{
		for (int i = 0; i < meClassLoader->meClassDllNames.size(); i++)
		{
			if (meClassLoader->meClassDllNames.at(i).toUpper().compare(paName.toUpper()) == 0)
			{
				return meClassLoader->meObjectCalls.at(i);
				break;
			}
		}

		return nullptr;
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::callRoutineDLL -> " + QString(e.what()));
		return nullptr;
	}
}
