#define INFO_BUFFER_SIZE 32767
#include "clLivingObjectLocator.h"

clLivingObjectLocator::clLivingObjectLocator(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QString paObjectId, QMutex * paLock, clClassLoader * paClassLoader, QObject * parent)
{
	try
	{
    meIceClientServer = paIceClientServer;
	meIceClientLogging = paIceClientLogging;
	meObjectId = paObjectId;
	meLock = paLock;
	meClassLoader = paClassLoader;
	
	//////// Getting the living object for this machine /////////////////////
	getLivingObjectLocatorProperties();
	
	
	meSocket = NULL;
	/////// Setting up the timer function ///////////////////////////////////
	meTimer = new QTimer(this);
	meTimer->setInterval(meTimerCycle.toInt());
	meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotDoIt()));
	meTimer->start();
	
	}
	catch(...)
	{
		cout << "clLivingObjectLocator::clLivingObjectLocator -> failed" << endl;
		
	}
}

clLivingObjectLocator::~clLivingObjectLocator()
{

}
bool clLivingObjectLocator::getLivingObjectLocatorProperties()
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

			 QString loClassNameTemp01 =  QString("LIVING_OBJ_LOCATION");
			
			 meIceClientServer->getAllPropertiesFromTable(  loClassNameTemp01,
															loPropertyName,
															loAlias,
															loType,
															loExtra,
															loReference,
															loReturnMessageObject);
															
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMach::getLivingObjectMachProperties -> " + loReturnMessageObject);

			
			for (int j=0; j< loPropertyName.size(); j++)
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
			
			
			for (int i = 0; i < meDatabaseColumnArr.size(); i++)
			{
				clDatabaseColumn loDatabaseColumn = meDatabaseColumnArr.at(i);
				loPropertyNames.push_back(loDatabaseColumn.getName().toStdString());
			}
			
			
			bool loGetID = false;
			
			QString loClassNameTemp02 = QString("LIVING_OBJ_LOCATION");
			
			loGetID = meIceClientServer->getFromTableDatabaseById(	loClassNameTemp02,
																	meObjectId,
																	loPropertyNames,
																	loReturnValues,
																	loReturnMessageGetById);
																	
			if (!loGetID)
			{			
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::getLivingObjectLocatorProperties -> " + loReturnMessageGetById);			
				return false;
			}
			else
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::getLivingObjectLocatorProperties -> " + loReturnMessageGetById);			
			
			//////////////////////////////// Set the connection parameters ////////////////////////////////////////////////////////////
			for (int k = 0; k < loPropertyNames.size(); k++)
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
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::getLivingObjectLocatorProperties -> error");			
		return false;
	}
}
void clLivingObjectLocator::slotDoIt()
{
	try
	{
		if (meSocket == NULL)
		{
			meSocket = new QTcpSocket( this ); // <-- needs to be a member variable: QTcpSocket * _pSocket;
			connect( meSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );
		
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::slotDoIt -> connect to host " + QString(meSocketIp + "::"+ meSocketPort));				
			
			meSocket->connectToHost(meSocketIp, meSocketPort.toInt());
			meSocket->waitForConnected();

			if (meSocket->state() != QAbstractSocket::ConnectedState ) 
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::slotDoIt() -> cannot connect to host");			
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
				
				
				QString loClassNameTemp01 = QString("LIVING_OBJ_LOCATION");
				
				meIceClientServer->updateIntoTableDatabase(     loClassNameTemp01,
																meObjectId,
																loColumns,
																loValue,
																loTypeValue,
																loReturnMessage);
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::slotDoIt -> " + loReturnMessage);				
				
				return;
			}
			else
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::slotDoIt -> SOCKET CONNECTED");								
				//////////////// Update the connection state ////////////////////////////////////////////////
				vector<std::string> loColumns;
				vector<std::string> loValue;
				vector<std::string> loTypeValue;
				
				loColumns.push_back("CONNECTION_STATE");
				loValue.push_back("0");
				loTypeValue.push_back("int");
				QString loReturnMessage;
				
				QString loClassNameTemp02 = QString("LIVING_OBJ_LOCATION");
				
				meIceClientServer->updateIntoTableDatabase(     loClassNameTemp02,
																meObjectId,
																loColumns,
																loValue,
																loTypeValue,
																loReturnMessage);
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::slotDoIt -> " + loReturnMessage);								
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
			
			QString loClassNameTemp03 = QString("LIVING_OBJ_LOCATION");
			
            loGetById = meIceClientServer->getFromTableDatabaseById(    loClassNameTemp03,
																		meObjectId,
																		loPropertyName,
																		loReturnValues,
																		loReturnMessageGetById);
			
			if(!loGetById)
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::slotDoIt -> " + loReturnMessageGetById);
				return;
			}
			else
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::slotDoIt -> " + loReturnMessageGetById);
			
			
			//////////////////////////////// Get the commands ////////////////////////////////////////////////////////////
			QString loCommands;
			for (int k = 0; k < loPropertyName.size(); k++)
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
					meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::slotDoIt -> createCommand fail, stopping slot");
					return;					
				}
				
				
				
				
				
				//////////////// Update the database for the commands ////////////////////////////////////////////////														
				vector<std::string> loColumns;
				vector<std::string> loValue;
				vector<std::string> loTypeValue;
				
				loColumns.push_back("COMMANDS_TODO");
				
				
				std::string loValueArr = "";
				
				for (int y = 0; y < loCommandList.size() - 1; y++)
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
				
				
				QString loClassNameTemp03 = QString("LIVING_OBJ_LOCATION");
				meIceClientServer->updateIntoTableDatabase(     loClassNameTemp03,
																meObjectId,
																loColumns,
																loValue,
																loTypeValue,
																loReturnMessage);
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::slotDoIt -> " + loReturnMessage);
				
			}
			else
			{
				////////////////// Create status ///////////////////////////////////////
				QString loCommand = QString("state");
				/////// Create command //////////////////////////
				if (!createCommand(loCommand,loCommandSend))
				{
					meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::slotDoIt -> createCommand fail, stopping slot");
					return;					
				}				
			}
			/////////////////////////////// If no command should be performed do state command //////////////////////
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clLivingObjectLocator::slotDoIt() command send to machine -> " + loCommandSend));
			
			//Short check before send
			if (meSocket->state() != QAbstractSocket::ConnectedState ) 
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::slotDoIt() -> cannot connect to host before command send");			
				delete meSocket;
				meSocket = NULL;
				return;
			}
			meSocket->write(loCommandSend.toUtf8());
			
			if (meSocket->state() != QAbstractSocket::ConnectedState ) 
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::slotDoIt() -> cannot connect to host after command send");			
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

bool clLivingObjectLocator::createCommand(QString paCommand,QString &paSocketCommand)
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
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clLivingObjectMach::createCommand() -> command not implemented" + loCommandParts.at(0)));
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
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::createCommand() -> error");
		return false;
	}
}


void clLivingObjectLocator::readTcpData()
{
	try
	{
		//Short check before send
		
		if (meSocket->state() != QAbstractSocket::ConnectedState ) 
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData() -> cannot connect to host");			
			delete meSocket;
			meSocket = NULL;
			return;
		}
		
		meData = meSocket->readAll();
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clLivingObjectLocator::readTcpData() command recieved from machine -> " + QString::fromLocal8Bit(meData.constData())));
		//// Parse the input TODO WVA///////////
		//<returnHardwareDevice><id>0</id><state>W0R0P0D0C0T0</state><datas></datas><error>Connection Socket Error on host:[' + self.host + '] port:[' + self.port + ']...</error></returnHardwareDevice>

        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( meData ) ) 
		{
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clLivingObjectLocator::readTcpData() command recieved from machine -> replay XML not valid ..."));
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
	   /*
		<returnHardwareDevice>
			<id>' + self.commandID + '</id>
			<state>' + returnState + '</state>
			<datas>
				<object name="5689"><x value="30.5"/><y value="30.5"/><z value="1.0"/></object>
				<object name="1234"><x value="60.5"/><y value="50.5"/><z value="0.5"/></object>
			</datas>
			<error>' + self.error + '</error>
		</returnHardwareDevice>')
	   */
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
					
					////////////////////////////////Get the state of the machine ////////////////////////////////////////
					vector<std::string> loPropertyName;
					vector<std::string> loReturnValues;
					QString loReturnMessageGetById;
					loPropertyName.push_back("DEVICE_STATE");
					loPropertyName.push_back("DEVICE_FILETRANSFER");
					
					QString loClassNameTemp01 = QString("LIVING_OBJ_LOCATION");
					
					bool loGetById = false;
					loGetById = meIceClientServer->getFromTableDatabaseById(    loClassNameTemp01,
																				meObjectId,
																				loPropertyName,
																				loReturnValues,
																				loReturnMessageGetById);
					
					if(!loGetById)
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData -> " + loReturnMessageGetById);
						return;
					}
					else
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData -> " + loReturnMessageGetById);
					
					if(loReturnValues.size() < 1)
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData -> result on fetched");
						return;
					}
					loStateEnumPrevious = QString(loReturnValues.at(0).c_str());
					loStateDataTransferPrevious = QString(loReturnValues.at(1).c_str());					
				}
			}
			if(loTablesNode.nodeName() == "error")
			{
				QDomElement loTablesElem = loTablesNode.toElement();
				loErrorText = loTablesElem.text();
				//Message detected
				
				if (loErrorText.compare("") != 0)
				{
					////////////////////////////////////// Get the error table ////////////////////////////////////////////////
					vector<std::string> loPropertyName;
					vector<std::string> loReturnValues;
					QString loReturnMessageGetById;
					loPropertyName.push_back("ERROR_TABLE");
					
					bool loGetById = false;
					QString loClassNameTemp02 = QString("LIVING_OBJ_LOCATION");
					loGetById = meIceClientServer->getFromTableDatabaseById(    loClassNameTemp02,
																				meObjectId,
																				loPropertyName,
																				loReturnValues,
																				loReturnMessageGetById);
					
					if(!loGetById)
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData -> " + loReturnMessageGetById);
						return;
					}
					else
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData -> " + loReturnMessageGetById);

					if (loReturnValues.size() < 1)meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData -> No error table found ... ");
					
					if (loReturnValues.size() > 0)
					{
						///////////////////////////////// Checking the error table object call///////////////////////////////////
						
						clObjectCall * loObjectCall = callObjectDLL(QString("libclErrorTable.so"));
						
						
						vector <QString> loParametersType;
						vector <QString> loParameters;
						vector <QString> loParametersValue;
						vector <QString> loLogExp;
						
						loParameters.push_back(QString("ERRORS_LIVING_OBJ_LOCATION"));
						loParametersValue.push_back(QString("ERRORS_LIVING_OBJ_LOCATION"));
						loParametersType.push_back(QString("CLASS"));
						loParameters.push_back(QString("PKEY"));
						loParametersValue.push_back(QString(loReturnValues.at(0).c_str()));
						loParametersType.push_back(QString("uuid"));
						loParameters.push_back(QString("OPERATION_ERROR_INFORMATION"));
						loParametersValue.push_back(loErrorText);
						loParametersType.push_back(QString("text"));
						
						loObjectCall->doMethod(QString("getErrorNumberFromText"),loParametersType, loParameters, loParametersValue, loLogExp);
						
						vector <QString> loReturnParameters = loObjectCall->meReturnParameters;
						//vector <QString> loReturnParametersType = loObjectCall->meReturnParametersType;
						
						//////////////////////////////////// Do something with the error information //////////////////////////////
						if (loReturnParameters.size() < 1)
						{
							meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clLivingObjectLocator::readTcpData -> Error found [" + loErrorText + "] but no number found"));
							loStateEnum = 110;
						}
						else
						{
							// Check the error number //
							// 00000 is error
							// 10000 is warning
							// 20000 is information
							meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clLivingObjectLocator::readTcpData -> Error found [" + loErrorText + "] with number [" + loReturnParameters.at(0) + "]"));
							int loErrorCode = loReturnParameters.at(0).toInt();
							if (loErrorCode < 10000) loStateEnum = 110;
							if (loErrorCode >= 10000 && loErrorCode < 20000) loWarning = true;
							if (loErrorCode >= 10000 && loErrorCode < 20000) loInformation = true;
						}
					}
				}
				
			}
			if (loTablesNode.nodeName() == "datas")
			{
				QDomElement loTablesElem = loTablesNode.toElement();
				QDomNode loDatasNode = loTablesElem.firstChild();
				
				while( !loDatasNode.isNull() ) 
				{
					if(loDatasNode.nodeName() == "object")
					{
						
						QDomElement loDatasElem = loDatasNode.toElement();
						QString loName = loDatasElem.attribute("name");
						
						QDomNode loCoordinatesNode = loDatasElem.firstChild();
						
						double loCoordX = 0;
						double loCoordY = 0;
						double loCoordZ = 0;
						
						while( !loCoordinatesNode.isNull() ) 
						{
							if(loCoordinatesNode.nodeName() == "x")
							{
								QDomElement loCoordinatesElem = loCoordinatesNode.toElement();
								loCoordX = loCoordinatesElem.attribute("value").toDouble();								
							}								
							else if(loCoordinatesNode.nodeName() == "y")
							{
								QDomElement loCoordinatesElem = loCoordinatesNode.toElement();
								loCoordY = loCoordinatesElem.attribute("value").toDouble();																
							}
							else if(loCoordinatesNode.nodeName() == "z")
							{
								QDomElement loCoordinatesElem = loCoordinatesNode.toElement();
								loCoordZ = loCoordinatesElem.attribute("value").toDouble();																
							}
							loCoordinatesNode = loCoordinatesNode.nextSibling();
						}
						//Check if the coordinates or not null and name is not null
						if(loCoordX != 0 && loCoordY != 0 && loCoordZ!= 0 && loName.compare("") != 0)
						{
							///////////////////////
							//Update database
							///////////////////////
							QString loTableName("OBJECT_TO_LOCATE");
							vector <std::string> loProperties;
							vector <std::string> loValues;
							vector <std::string> loTypeValues;
							vector <std::string> loLogExp;
							vector <std::string> loReturnIds;
							QString loReturnMessage;
							
							loProperties.push_back("NAME");
							loValues.push_back(loName.toStdString());
							loTypeValues.push_back(QString("text").toStdString());
							loLogExp.push_back(QString("=").toStdString());
							
							QString loStartStop = QString("0");
							
							if (!meIceClientServer->getFromTableDatbaseByProperty(loTableName,loStartStop,loStartStop,loProperties,loValues,loTypeValues,loLogExp,loReturnIds,loReturnMessage))
							{
								meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData() -> " + loReturnMessage);
								return;
							}
							if (loReturnIds.size() > 0)
							{
								//Update coord in table
								vector<std::string> loPropertyName;
								vector<std::string> loValues;
								vector<std::string> loTypeValues;
								QString loReturnMessageUpdate;
								loPropertyName.push_back("COORD_X");
								loPropertyName.push_back("COORD_Y");
								loPropertyName.push_back("COORD_Z");
								loPropertyName.push_back("OBJ_LOCATION_ID");
								
								loValues.push_back(QString::number(loCoordX).toStdString());
								loTypeValues.push_back("float8");
								loValues.push_back(QString::number(loCoordY).toStdString());
								loTypeValues.push_back("float8");			
								loValues.push_back(QString::number(loCoordZ).toStdString());
								loTypeValues.push_back("float8");			
								loValues.push_back(meObjectId.toStdString());
								loTypeValues.push_back("uuid");			
								
								QString loClassNameTemp03 = QString("OBJECT_TO_LOCATE");
								QString loUUID = QString(loReturnIds.at(0).c_str()); 
								
								
								if (!meIceClientServer->updateIntoTableDatabase( 			loClassNameTemp03,
																							loUUID,
																							loPropertyName,
																							loValues,
																							loTypeValues,
																							loReturnMessageUpdate))
								{
									meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData() -> " + loReturnMessageUpdate);
								}
							}
						}
					}
					loDatasNode = loDatasNode.nextSibling();
				}
			}			
			
			
			loTablesNode = loTablesNode.nextSibling();	
	   }
	   
		////////////////////////////////////////////////// Compare the state of the machine //////////////////////////////////////////		
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clLivingObjectLocator::readTcpData -> Previous State[" + loStateEnumPrevious + "] Previous DataTransfer[" + loStateDataTransferPrevious+ "]"));
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clLivingObjectLocator::readTcpData -> Current State[" + QString::number(loStateEnum) + "] Current DataTransfer[" + QString::number(loStateDataTransfer) + "]"));
		
		if (loStateEnum == loStateEnumPrevious.toInt())loChangeState = false;
		else loChangeState = true;
		if (loStateDataTransfer == loStateDataTransferPrevious.toInt())loChangeDataTransfer = false;
		else loChangeDataTransfer = true;
		
		if (loChangeState && loStateEnumPrevious.toInt() != 30 && (loStateEnum == 20 || loStateEnum == 110)) loStartOperation = true;
		if (loChangeState && (loStateEnumPrevious.toInt() == 20 || loStateEnumPrevious.toInt() == 30) && (loStateEnum == 10 || loStateEnum == 110)) loCloseOperation = true;
		if (loChangeState && (loStateEnumPrevious.toInt() == 20 || loStateEnumPrevious.toInt() == 30 || loStateEnumPrevious.toInt() == 10) && (loStateEnum == 110)) loCloseOperation = true;
		
	   ////////////////////////////////Update the state values on the living object ////////////////////////////////////////
	   if (loChangeState || loChangeDataTransfer)
	   {
			vector<std::string> loPropertyName;
			vector<std::string> loValues;
			vector<std::string> loTypeValues;
			QString loReturnMessageUpdate;
			loPropertyName.push_back("DEVICE_STATE");
			loPropertyName.push_back("DEVICE_FILETRANSFER");
			
			bool loUpdateDb = false;
			loValues.push_back(std::to_string(loStateEnum));
			loTypeValues.push_back("int");
			loValues.push_back(std::to_string(loStateDataTransfer));
			loTypeValues.push_back("int");			
			QString loClassNameTemp03 = QString("LIVING_OBJ_LOCATION");
			
			loUpdateDb = meIceClientServer->updateIntoTableDatabase( 	loClassNameTemp03,
																		meObjectId,
																		loPropertyName,
																		loValues,
																		loTypeValues,
																		loReturnMessageUpdate);	
			if (!loUpdateDb)
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData -> " + loReturnMessageUpdate);		   
				return;
			}
			else
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData -> " + loReturnMessageUpdate);		   
	   }
	   
	   //////////////////////////////// Check if operation has to be closed or opened ////////////////////////////////////////
	   if (loStartOperation)
	   {
			///////////////////////////////// over dll ///////////////////////////////////
			clObjectCall * loObjectCall = callObjectDLL(QString("clOperation.dll"));

			vector <QString> loParametersType;
			vector <QString> loParameters;
			vector <QString> loParametersValue;
			vector <QString> loLogExp;
			
			loParameters.push_back(QString("OPERATION"));
			loParametersValue.push_back(QString("OPERATION"));
			loParametersType.push_back(QString("CLASS"));
			
			loParameters.push_back("NAME");
			loParametersValue.push_back(QString("System " + meObjectName));
			loParametersType.push_back("varchar(255)");

			loParameters.push_back("OBJECT");
			loParametersValue.push_back(meObjectId);
			loParametersType.push_back("uuid");

			loParameters.push_back("OBJECTS");
			loParametersValue.push_back(meObjectId);
			loParametersType.push_back("uuid[]");
			
			loParameters.push_back("DATUM_START");			
			loParametersValue.push_back(QString((QDateTime::currentDateTime()).toString("yyyy-MM-dd HH:mm:ss.zzz")));
			loParametersType.push_back("timestamp");
			
			loParameters.push_back("OPERATION_SOURCE");
			loParametersValue.push_back("40");
			loParametersType.push_back("int");			

			loParameters.push_back("OPERATION_STATE");
			loParametersValue.push_back("10");
			loParametersType.push_back("int");
			
			loObjectCall->doMethod(QString("openOperation"),loParametersType, loParameters, loParametersValue, loLogExp);
			
			vector <QString> loReturnParameters = loObjectCall->meReturnParameters;
			vector <QString> loReturnParametersType = loObjectCall->meReturnParametersType;
	   }
	   
	   if (loCloseOperation)
	   {
			//Close an existing operation	
			vector <string> loParametersType;
			vector <string> loParameters;
			vector <string> loParametersValue;
			vector <string> loLogExp;
			vector <std::string> loReturnIds;
			QString loReturnMessage;			

			loParameters.push_back(QString("NAME").toStdString());
			loParametersValue.push_back(QString("System " + meObjectName).toStdString());
			loParametersType.push_back(QString("varchar(255)").toStdString());
			loLogExp.push_back(QString("=").toStdString());
			
			loParameters.push_back(QString("OBJECT").toStdString());
			loParametersValue.push_back(meObjectId.toStdString());
			loParametersType.push_back(QString("uuid").toStdString());
			loLogExp.push_back(QString("=").toStdString());

			loParameters.push_back(QString("OBJECTS").toStdString());
			loParametersValue.push_back(meObjectId.toStdString());
			loParametersType.push_back(QString("uuid[]").toStdString());
			loLogExp.push_back(QString("=").toStdString());

			loParameters.push_back("OPERATION_SOURCE");
			loParametersValue.push_back("40");
			loParametersType.push_back("int");			
			loLogExp.push_back(QString("=").toStdString());
			
			loParameters.push_back("OPERATION_STATE");
			loParametersValue.push_back("10");
			loParametersType.push_back("int");
			loLogExp.push_back(QString("=").toStdString());			
			
			QString loClassNameTemp04 = QString("OPERATION");
			QString loStartStop = QString("0");
			
			if (!meIceClientServer->getFromTableDatbaseByProperty(loClassNameTemp04,loStartStop,loStartStop,loParameters,loParametersValue,loParametersType,loLogExp,loReturnIds,loReturnMessage))
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData -> " + loReturnMessage);
				return;
			}
			else
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectLocator::readTcpData -> " + loReturnMessage);
			

			/////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//Close the operations
			for (int j = 0; j < loReturnIds.size(); j++)
			{
				clObjectCall * loObjectCallCloseOperation = callObjectDLL(QString("libclOperation.so"));
				loObjectCallCloseOperation->createPluginClass(meIceClientServer, meIceClientLogging);
		
		
				vector <QString> loParametersTypeClose;
				vector <QString> loParametersClose;
				vector <QString> loParametersValueClose;
				vector <QString> loLogExpClose;
				
				loParametersClose.push_back(QString("OPERATION"));
				loParametersValueClose.push_back(QString("OPERATION"));
				loParametersTypeClose.push_back("CLASS");

				loParametersClose.push_back("PKEY");
				loParametersValueClose.push_back(loReturnIds.at(j).c_str());
				loParametersTypeClose.push_back("uuid");				

				
				loParametersClose.push_back("DATUM_STOP");
				loParametersValueClose.push_back(QString((QDateTime::currentDateTime()).toString("yyyy-MM-dd HH:mm:ss.zzz")));
				loParametersTypeClose.push_back("timestamp");
				
				loParametersClose.push_back("OPERATION_STATE");
				loParametersValueClose.push_back("20");
				loParametersTypeClose.push_back("int");			

				loParametersClose.push_back("OPERATION_CLOSED_CAUSE");
				if (loStateEnum == 110) loParametersValueClose.push_back("20");
				else loParametersValueClose.push_back("10");
				loParametersTypeClose.push_back("int");

				loParametersClose.push_back("OPERATION_ERROR_INFORMATION");
				loParametersValueClose.push_back(loErrorText);
				loParametersTypeClose.push_back("text");
				
				loObjectCallCloseOperation->doMethod(QString("closeOperation"),loParametersTypeClose, loParametersClose, loParametersValueClose, loLogExpClose);
					
				
			}
		}
		   
	   
	   
	}
	catch(...)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMach::readTcpData() -> error");
	}
}
clObjectCall * clLivingObjectLocator::callObjectDLL(QString paName)
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
