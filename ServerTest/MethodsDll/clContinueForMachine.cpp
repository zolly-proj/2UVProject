#define INFO_BUFFER_SIZE 32767
#include "clContinueForMachine.h"
clContinueForMachine::clContinueForMachine()
{
    try
    {		
		meSocket = NULL;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
    }	
    catch(...)
    {
        printf("clPauseForMachine::clContinueForMachine(vector <QString> paParameters) -> error ...");
    }
}
clContinueForMachine::~clContinueForMachine ()
{
	try
	{
		if (meSocket != NULL)
		{	
			delete meSocket;
			meSocket = NULL;
		}
	}
	catch(...)
	{
		
	}
}
int clContinueForMachine::GetReturnParameters()
{
    try
    {
		return 0;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
			return -1;
    }	
    catch(...)
    {
		return -1;
    }
}

bool clContinueForMachine::doMethod(const vector <QString> &paParametersType, const vector <QString> &paParameters)
{
	try
	{
		if (meSocket == NULL)
		{
			meSocket = new QTcpSocket(); // <-- needs to be a member variable: QTcpSocket * _pSocket;
			//connect( meSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );
			
			
			
			
			
			//////////////////////////////// GET THE PORT AND THE IP ADRESS ////////////////////////////////////////
			vector<std::string> loPropertyName;
            vector<std::string> loReturnValues;
            QString loReturnMessageGetById;
			loPropertyName.push_back("SOCKET_IP");
			loPropertyName.push_back("SOCKET_PORT");
			
			QString loTableName = QString(paParameters.at(0));
			QString loObjectID = QString(paParameters.at(1));
			
			bool loGetById = false;
            loGetById = meIceClientServer->getFromTableDatabaseById(    loTableName,
																		loObjectID,
																		loPropertyName,
																		loReturnValues,
																		loReturnMessageGetById);
			
			if(!loGetById)
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clContinueForMachine::doMethod -> " + loReturnMessageGetById);
				return false;
			}
			else
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clContinueForMachine::doMethod -> " + loReturnMessageGetById);
			
			QString loCommands;
			for (int k = 0; k < loPropertyName.size(); k++)
			{
				if (QString(loPropertyName.at(k).c_str()).toUpper().compare(QString("SOCKET_IP")) == 0)
				{
					//cout << loReturnValues.at(k).c_str() << endl;
					meSocketIp = QString(loReturnValues.at(k).c_str());
				}
				else if (QString(loPropertyName.at(k).c_str()).toUpper().compare(QString("SOCKET_PORT")) == 0)
				{
					meSocketPort = QString(loReturnValues.at(k).c_str());
				}
			}			
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			meSocket->connectToHost(meSocketIp, meSocketPort.toInt());
			meSocket->waitForConnected();

			if (meSocket->state() != QAbstractSocket::ConnectedState ) 
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clContinueForMachine::doMethod() -> cannot connect to host");			
				delete meSocket;
				meSocket = NULL;
				return false;
			}
			
		}
		
		//Building the command		
		QString loCommandSend = 	QString		(QString("<hardwareDevice>") +
												QString("<id>") + QString::number(QRandomGenerator::global()->generate()) + QString("</id>") +
												QString("<connect do='false'></connect>") +
												QString("<disconnect do='false'></disconnect>") +
												QString("<state do='false'></state>") +
												QString("<run do='false'></run>") +
												QString("<abort do='false'></abort>") +
												QString("<hold do='false'></hold>") +
												QString("<continue do='true'></continue>") +
												QString("<sendFile do='false'></sendFile>") +
												QString("<receiveFile do='false'></receiveFile>") +
												QString("<scriptCommand do='false'></scriptCommand>") +
												QString("<optionalCommand do='false' name='SendDataCommand2'>") +
												QString("</optionalCommand>") +
												QString("</hardwareDevice>"));
		//Send to machine
		meSocket->write(loCommandSend.toUtf8());
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clContinueForMachine::doMethod() command send to machine -> " + loCommandSend));
		meSocket->waitForBytesWritten(50);
		meSocket->waitForReadyRead(300);
		
		meData = meSocket->readAll();
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clContinueForMachine::readTcpData() command recieved from machine -> " + QString::fromLocal8Bit(meData.constData())));
		return true;
	}
	catch(...)
	{
		meSocket = NULL;
		return false;
	}
}
bool clContinueForMachine::createPluginClass( clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging)	
{
    try
    {
		meIceClientServer = paIceClientServer;
		meIceClientLogging = paIceClientLogging;
		return true;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
			return false;
    }	
    catch(...)
    {
		return false;
    }
}
