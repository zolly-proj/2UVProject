#include "clSocketBoxThread.h"

clSocketBoxThread::clSocketBoxThread(int paID,vector <QString> *paData,QObject * parent) : QThread(parent)
{
    try
    {		
		this->meSocketDescriptor = paID;
		meDriverData = paData;
		
		
		
		////////////////////////Loading the library///////////////////////////////////////////////////////////////////
		string loLibraryName = string(meDriverData->at(0).toUtf8());
		meLibraryLib = dlopen(loLibraryName.c_str(), RTLD_LAZY);		
		
		if (meLibraryLib == NULL) {
			cout << "Library " << loLibraryName << " not found" << std::endl;
		}
		else
		{
			cout << "Library " << loLibraryName << " found" << std::endl;
			//Getting the class
			//Getting the class
			CreateModuleFn * (*create)();
			void (*destroy)(CreateModuleFn*);
			
			create = (CreateModuleFn* (*)())dlsym(meLibraryLib, "create_object");
			destroy = (void (*)(CreateModuleFn*))dlsym(meLibraryLib, "destroy_object");
			
			CreateModuleFn* meCreateModuleFn = (CreateModuleFn*)create();
			meDriverFrame=(*meCreateModuleFn)();
			//Testing the class
			if (!meDriverFrame)
			{ 
				cout << "Loading failed " << loLibraryName << std::endl;
				cout << "unable to load CreateModule";
			}
			else
			{ 
				cout << "Loading success " << loLibraryName << std::endl;
				
				vector<QString> loDriverData;
				for (int k = 0; k < (int) meDriverData->size(); k++)
				{
					loDriverData.push_back(meDriverData->at(k));
				}
				
				meDriverFrame->createPluginClass(loDriverData);
				//meDriverFrame->doConnect("TEST","TEST"); // calling function 
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		
		
		//Open the socket starts here ////////////////////////////////////////////////////////////////////////////////////
		meSocket = new QTcpSocket(this);
		//connect(meSocket, SIGNAL(readyRead()), this, SLOT(readyReadTest()),Qt::DirectConnection);
		//connect(meSocket, SIGNAL(disconnected()), this, SLOT(disconnectedTest()),Qt::DirectConnection);
		connect(meSocket, SIGNAL(readyRead()), this, SLOT(readyReadSocket()));
		connect(meSocket, SIGNAL(disconnected()), this, SLOT(disconnectedSocket()));
		
		qDebug() << meSocketDescriptor << " Starting thread";

		if(!meSocket->setSocketDescriptor(this->meSocketDescriptor))
		{
			emit error(meSocket->error());
			return;
		}
		
		qDebug() << meSocketDescriptor << " Client connected";
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
    }	
    catch(...)
    {
        printf("clSocketBoxThread::clLogServer(QString paConfigFile) -> error ...");
    }
}

clSocketBoxThread::~clSocketBoxThread ()
{
	
}


void clSocketBoxThread::run()
{
	

    // make this thread a loop
	//while (meSocket->isOpen())
	//{
		
	//}
    exec();
}

void clSocketBoxThread::readyReadSocket()
{
	try
	{
		cout << "clSocketBoxThread::readyRead->START" << endl;
		QByteArray loData = meSocket->readAll();

		qDebug() << meSocketDescriptor << " Data in: " << endl << convertDataFromSocket(loData) << endl;
		
		//call the driver class
		if (meDriverFrame->parseInputXMLcommand(loData))
		{
			vector <QString> loCommand = meDriverFrame->GetCommand();
			cout << "Command ID: [" << loCommand[0].toStdString() << "]" << endl;
			QString loCommandType = loCommand[1];
			QString loCommandContent = loCommand[2];
			
			
			if (loCommandType.compare("connect") == 0)
			{
				meDriverFrame->doConnect(loCommandContent, QString(""));
			}
			else if (loCommandType.compare("disconnect") == 0)
			{
				meDriverFrame->doDisconnect(loCommandContent, QString(""));
			}
			else if (loCommandType.compare("state") == 0)
			{
				meDriverFrame->doState(loCommandContent, QString(""));
			}
			else if (loCommandType.compare("run") == 0)
			{
				meDriverFrame->doRun(loCommandContent, QString(""));
			}
			else if (loCommandType.compare("abort") == 0)
			{
				meDriverFrame->doAbort(loCommandContent, QString(""));
			}
			else if (loCommandType.compare("hold") == 0)
			{
				meDriverFrame->doHold(loCommandContent, QString(""));
			}
			else if (loCommandType.compare("continue") == 0)
			{
				meDriverFrame->doContinue(loCommandContent, QString(""));
			}
			else if (loCommandType.compare("sendFile") == 0)
			{
				meDriverFrame->doSendFile(loCommandContent, QString(""));
			}		
			else if (loCommandType.compare("receiveFile") == 0)
			{
				meDriverFrame->doReceiveFile(loCommandContent, QString(""));
			}		
			else if (loCommandType.compare("scriptCommand") == 0)
			{
				meDriverFrame->doScriptCommand(loCommandContent, QString(""));
			}				
			else if (loCommandType.compare("optionalCommand") == 0)
			{
				meDriverFrame->doOptionalCommand(loCommandContent, QString(""));
			}
			else
			{
				cout << "Command not implemented ..." << endl;
				return;
			}
		}
		else
		{
				cout << "Command not accepted by parsing ..." << endl;	
				return;
		}
		
		vector <QString> loReplay;
		if (meDriverFrame->getReturnCommandFinished())
		{
			loReplay = meDriverFrame->GetReplay();	
		}
		
		
		
		
		
		/*
			//Functions to be overridden
		QString doConnect(QString paValue,  QString paParameters) const override;
		QString doDisconnect(QString paValue,  QString paParameters) const override;
		QString doState(QString paValue,  QString paParameters) const override;
		QString doRun(QString paValue,  QString paParameters) const override;
		QString doAbort(QString paValue,  QString paParameters) const override;
		QString doHold(QString paValue,  QString paParameters) const override;
		QString doContinue(QString paValue,  QString paParameters) const override;
		QString doSendFile(QString paValue,  QString paParameters) const override;
		QString doReceiveFile(QString paValue,  QString paParameters) const override;
		QString doScriptCommand(QString paValue,  QString paParameters) const override;
		QString doOptionalCommand(QString paValue,  QString paParameters) const override;
		*/
		
		
		
		//qDebug() << meSocketDescriptor << " Data in: " << QString(Data.toStdString().c_str());
		//qDebug() << meSocketDescriptor << " Data in: " << (Data.data());
		//qDebug() << meSocketDescriptor << " Data in: " << QString(Data.data());
		//qDebug() << meSocketDescriptor << " Data in: " << Data;	
		//QString DataAsString = QTextCodec::codecForMib(1015)->toUnicode(Data);
		//qDebug() << meSocketDescriptor << " Data in: " << DataAsString;	
		//QString qstr_test = QString::fromUtf16((ushort *)Data.data());
		//qDebug() << meSocketDescriptor << " Data in: " << qstr_test;
			
		QByteArray block = loReplay[0].toUtf8();
		/*
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_5_5);
		out << QString("ending");
		out.device()->seek(0);
		*/

		meSocket->write(block);
		
		cout << "clSocketBoxThread::readyRead->STOP" << endl;
	}
	catch(...)
	{
		cout << "Error" << endl;
	}
}
QString clSocketBoxThread::convertDataFromSocket(QByteArray paByteArray)
{
	char fixed[10000];
	int index = 0;
	QByteArray::iterator iter = paByteArray.begin();
	while(iter != paByteArray.end())
	{
		QChar c = *iter;
		if (c != '\0' && c != '\n') fixed[index++] = c.toLatin1();
		iter++;
	}
	fixed[index] = '\0';
	return QString(fixed);
}
void clSocketBoxThread::disconnectedSocket()
{
    qDebug() << meSocketDescriptor << " Disconnected";
    meSocket->deleteLater();
    exit(0);
}

