#include "clSocketBox.h"

clSocketBox::clSocketBox(QString paConfigFile,QObject * parent) : QTcpServer(parent)
{
    try
    {		
        readConfigFile(paConfigFile);

    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
    }	
    catch(...)
    {
        printf("clSocketBox::clSocketBox(QString paConfigFile) -> error ...");
    }
}

clSocketBox::~clSocketBox ()
{
}

void clSocketBox::StartServer()
{	

	//connect (this,SIGNAL (newConnection ()),this,SLOT(newConnections())); 


    if(!this->listen(QHostAddress::Any,mePort.toInt()))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening...";
    }
}


//void clSocketBox::newConnections(int socketDescriptor)
//{
	
//    qDebug() << socketDescriptor << " Connecting...";
//    clSocketBoxThread *meThread = new clSocketBoxThread(socketDescriptor, this);
//    connect(meThread, SIGNAL(finished()), meThread, SLOT(deleteLater()));
//    meThread->start();	
//}

void clSocketBox::incomingConnection(qintptr socketDescriptor)
{
	meThread = NULL;
    qDebug() << socketDescriptor << " Connecting...";
    meThread = new clSocketBoxThread(socketDescriptor, &meData, this);
    connect(meThread, SIGNAL(finished()), meThread, SLOT(deleteLater()));
    meThread->start();	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// Reading the config file functions //////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void clSocketBox::readConfigFile(QString paConfigFile)
{
    try
    {
      QString loMessage;
	  QString loValue;
      
      if (!getAttributeFromConfigFileGeneral(paConfigFile,QString("name"),loValue,loMessage))
      {
          cout << "clSocketBox::getAttributeFromConfigFile(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage) -> error getting name";
      }
      else
      {
          meName = loValue;
          QByteArray loBa1 = loValue.toLatin1();
          char *loStringContent = loBa1.data();
          cout << loStringContent << endl;
      }
	  
      if (!getAttributeFromConfigFileGeneral(paConfigFile,QString("ip"),loValue,loMessage))
      {
          cout << "clSocketBox::getAttributeFromConfigFile(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage) -> error getting ip";
      }
      else
      {
          meIp = loValue;
          QByteArray loBa1 = loValue.toLatin1();
          char *loStringContent = loBa1.data();
          cout << loStringContent << endl;
      }
	  
      if (!getAttributeFromConfigFileGeneral(paConfigFile,QString("port"),loValue,loMessage))
      {
          cout << "clSocketBox::getAttributeFromConfigFile(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage) -> error getting port";
      }
      else
      {
          mePort = loValue;
          QByteArray loBa1 = loValue.toLatin1();
          char *loStringContent = loBa1.data();
          cout << mePort.toInt() << endl;
      }

      if (!getAttributeFromConfigFileGeneral(paConfigFile,QString("error"),loValue,loMessage))
      {
          cout << "clSocketBox::getAttributeFromConfigFile(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage) -> error getting error";
      }
      else
      {
          meError = loValue;
          QByteArray loBa1 = loValue.toLatin1();
          char *loStringContent = loBa1.data();
          cout << loStringContent << endl;
      }
	
	  vector <QString> loData;
	  if (!getDataFromConfigFile(paConfigFile,QString("specificData"),loData, loMessage))
	  {
		  cout << "clSocketBox::getAttributeFromConfigFile(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage) -> error getting data[]";
	  }
	  else
	  {		
	      meData = loData;
		  for (int k = 0; k < loData.size(); k++)
		  {
			  QByteArray loBa1 = loData[k].toLatin1();
			  char *loStringContent = loBa1.data();
			  cout << loStringContent << endl;			  
		  }
	  }
    }
    catch(...)
    {
        cout << "clSocketBox::readConfigFile(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage) -> error ...";
    }
}

bool clSocketBox::getAttributeFromConfigFileGeneral(QString paConfigFile,QString paElement, QString & paValue, QString & paReturnMessage)
{
    try
    {
        QFile loFile(paConfigFile);
        if ( !loFile.open( QIODevice::ReadOnly ) ) 
		{
            QString loTemp("clSocketBox::getAttributeFromConfigFile(QString paElement, QString * paValue, QString * paReturnMessage) -> Cound not open file '" + paElement + "' ...");
            paReturnMessage = loTemp;
            return false;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) 
		{
            QString loTemp("clSocketBox::getAttributeFromConfigFile(QString paElement, QString * paValue, QString * paReturnMessage) -> Cound not parse file '" + paElement + "' ...");
            paReturnMessage = loTemp;
            loFile.close();
            return false;
        }

        // create the tree view out of the DOM
       QDomElement loDocElem = loDomDocument.documentElement();

       QDomNode loDomNode = loDocElem.firstChild();
       while( !loDomNode.isNull() ) 
	   {
          if(loDomNode.nodeName() == QString("general"))
          {
			 QDomElement loTablesElem = loDomNode.toElement(); 
			//Get elemnt name, port, ip and error
			if (!loTablesElem.isNull())
			{			
				int j = 1;
				QDomNode loTableNode = loTablesElem.firstChild();

				//For each ROUTINE
				while(!loTableNode.isNull())
				{
					if (loTableNode.nodeName() == QString(paElement))
					{
						paValue = loTableNode.toElement().text(); 
						break;
					}
				  loTableNode = loTableNode.nextSibling();
				}
			}
		  }
		  loDomNode = loDomNode.nextSibling();
       }
       return true;
    }
    catch(...)
    {
        QString loTemp("clSocketBox::getAttributeFromConfigFile(QString paElement, QString * paValue, QString * paReturnMessage) -> error ...");
        paReturnMessage = loTemp;
        return false;
    }
}

bool clSocketBox::getDataFromConfigFile(QString paConfigFile,QString paElement,vector <QString> & paData, QString & paReturnMessage)
{
	try
	{
        QFile loFile(paConfigFile);
        if ( !loFile.open( QIODevice::ReadOnly ) ) 
		{
            QString loTemp("clSocketBox::getAttributeFromConfigFile(QString paElement, QString * paValue, QString * paReturnMessage) -> Cound not open file '" + paElement + "' ...");
            paReturnMessage = loTemp;
            return false;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) 
		{
            QString loTemp("clSocketBox::getAttributeFromConfigFile(QString paAttribute, QString * paValue, QString * paReturnMessage) -> Cound not parse file '" + paElement + "' ...");
            paReturnMessage = loTemp;
            loFile.close();
            return false;
        }


        // create the tree view out of the DOM
		QDomElement loDocElem = loDomDocument.documentElement();

		QDomNode loDomNode = loDocElem.firstChild();
		while( !loDomNode.isNull() ) 
		{
			if(loDomNode.nodeName() == paElement)
			{
				QDomElement loTablesElem = loDomNode.toElement(); 
				//Get elemnt name, port, ip and error
				if (!loTablesElem.isNull())
				{			
					QDomNode loTableNode = loTablesElem.firstChild();
					//For each Data
					while(!loTableNode.isNull())
					{
						paData.push_back(loTableNode.toElement().text());
						loTableNode = loTableNode.nextSibling();
					}
				}
			}
			loDomNode = loDomNode.nextSibling();
		}
       return true;		
	}
    catch(...)
    {
        QString loTemp("clSocketBox::getDataFromConfigFile(QString paConfigFile,QString paElement,vector <QString> & paData, QString & loMessage) -> error ...");
        paReturnMessage = loTemp;
        return false;
    }
	
}