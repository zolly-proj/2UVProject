#include "clLogServer.h"

clLogServer::clLogServer(QString paConfigFile,QObject * parent)
{
	//Ice::CommunicatorPtr ic;
	
    try
    {
		
        readConfigFile(paConfigFile);

        //Initialize the log server
        int argc=0;
        char * argv[1];
        //////Ice::InitializationData initData;
        
		//////printf("\r\n Ice::createProperties() ... \r\n");
		//////initData.properties = Ice::createProperties();
		
		//char buf[256];
		//GetCurrentDirectoryA(256, buf);
		//printf("Current working directory = ");
		//printf(buf);
		//printf("\r\n");
		
		//QString loVariable = QString(buf);
		//loVariable = loVariable.append("\\config");
		//loVariable = QString("config.server");
		
		
		//const std::string file = loVariable.toStdString();
		
		//char * writable = new char[file.size() + 1];
		//std::copy(file.begin(), file.end(), writable);
		//writable[file.size()] = '\0'; // don't forget the terminating 0
		//printf(writable);
		//printf("\r\n");
		
		
		//initData.properties->setProperty("Endpoints", "tcp -p 5001:udp -p 5001"); 
		//////try
		//////{
		//////	initData.properties->load("config.server");
		//////}
		//////catch(const Ice::FileException& ex)
		//////{
		//////	printf(ex.what());       
		//////}
		//try
		//{
		//	initData.properties->setProperty("LogAppServerAdaptor.Endpoints","tcp -p 5001:udp -p 5001");
		//}
		//    catch(const Ice::Exception& ex)
		//{
        //    printf(ex.what());       
		//}
		//////printf("File loaded");
		//////printf("\r\n");
        meCommunicationPtr = Ice::initialize(argc, argv);
		printf("meCommunicationPtr = Ice::initialize(argc, argv); \r\n");
		//////printf("\r\n");

        //Thread example
		printf("Starting thread ... \r\n");
        doSetup(meThread);
        moveToThread(&meThread);
        meThread.start();
		printf("Thread started ... \r\n");

    }
    catch(const Ice::Exception& ex)
    {
            printf(ex.what());       
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
    }	
    catch(...)
    {
        printf("clLogServer::clLogServer(QString paConfigFile) -> error ...");
    }
}

clLogServer::~clLogServer ()
{
    doShutDown();
}
void clLogServer::doSetup(QThread &paThread)
{
		printf("START clLogServer::doSetup");
        connect(&paThread,SIGNAL(started()),this,SLOT(doWork()));
		printf("END clLogServer::doSetup");
}
void clLogServer::doShutDown()
{

        if (meCommunicationPtr != NULL)
        {
                meCommunicationPtr->shutdown();
        }

}
void clLogServer::readConfigFile(QString paConfigFile)
{
    try
    {
      QString loMessage;
      //Getting the saving directory
      QString loSafePath;
      if (!getAttributeFromConfigFile(paConfigFile,QString("SAVEDIR"),loSafePath,loMessage))
      {
          cout << "clLogServer::getAttributeFromConfigFile(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage) -> error getting savedir";
      }
      else
      {
          setPath(loSafePath);
          QByteArray loBa1 = loSafePath.toLatin1();
          char *loStringContent = loBa1.data();
          cout << loStringContent;
      }
      //Getting the size
      QString loSize;
      if (!getAttributeFromConfigFile(paConfigFile,QString("SIZE"),loSize,loMessage))
      {
          cout << "clLogServer::getAttributeFromConfigFile(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage) -> error getting size";
      }
      else
      {
          setSize(loSize);
          QByteArray loBa1 = loSize.toLatin1();
          char *loStringContent = loBa1.data();
          cout << loStringContent;
      }
      //Getting the disk usage
      QString loDiskUsage;
      if (!getAttributeFromConfigFile(paConfigFile,QString("DISKUSAGE"),loDiskUsage,loMessage))
      {
          cout << "clLogServer::getAttributeFromConfigFile(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage) -> error getting diskusage";
      }
      else
      {
          setDiskUsage(loDiskUsage);
          QByteArray loBa1 = loDiskUsage.toLatin1();
          char *loStringContent = loBa1.data();
          cout << loStringContent;
      }
    }
    catch(...)
    {
        cout << "clLogServer::readConfigFile(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage) -> error ...";
    }
}

bool clLogServer::getAttributeFromConfigFile(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage)
{
    try
    {
        QFile loFile(paConfigFile);
        if ( !loFile.open( QIODevice::ReadOnly ) ) {
            QString loTemp("clLogServer::getAttributeFromConfigFile(QString paAttribute, QString * paValue, QString * paReturnMessage) -> Cound not open file '" + paAttribute + "' ...");

            paReturnMessage = loTemp;
            return false;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) {
            QString loTemp("clLogServer::getAttributeFromConfigFile(QString paAttribute, QString * paValue, QString * paReturnMessage) -> Cound not parse file '" + paAttribute + "' ...");
            paReturnMessage = loTemp;
            loFile.close();
            return false;
        }

        // create the tree view out of the DOM
       QDomElement loDocElem = loDomDocument.documentElement();

       QDomNode loDomNode = loDocElem.firstChild();
       while( !loDomNode.isNull() ) {

          if(loDomNode.nodeName() == paAttribute)
          {
              QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
              if( !loDocElemChild.isNull() )// the node was really an element.
              {
                  if (loDocElemChild.hasAttribute(QString("value")))
                  {
                        //QString loValue = loDocElemChild.attribute(paAttribute);
                        //paValue = &loValue;
                        paValue = loDocElemChild.attribute("value");
                        QString loTemp("Getting attribute '" + paAttribute + "' with value '" + paValue + "'");
                        paReturnMessage = loTemp;
                        return true;
                        break;
                  }
              }
          }
          loDomNode = loDomNode.nextSibling();
       }

        QString loTempLast("clLogServer::getAttributeFromConfigFile(QString paAttribute, QString * paValue, QString * paReturnMessage) -> No attribute '" + paAttribute + "' found ...");
        paReturnMessage = loTempLast;
        return true;
    }
    catch(...)
    {
        QString loTemp("clLogServer::getAttributeFromConfigFile(QString paAttribute, QString * paValue, QString * paReturnMessage) -> error ...");
        paReturnMessage = loTemp;
        return false;
    }
}

void clLogServer::doWork()
{
    try
    {
		printf("START clLogServer::doWork");
		int argc=0;
        char * argv[1];
		Ice::CommunicatorHolder ich(argc, argv, "config.server");
        meCommunicationPtr = ich.communicator();
		
		
		
        Ice::ObjectAdapterPtr adapter = meCommunicationPtr->createObjectAdapterWithEndpoints("LogAppServerAdaptor", "tcp -p 5001:udp -p 5001");
        clFileWriter loFileWriter(getPath(),getSize(),getDiskUsage());
        UVLogAppServer::LogAppServerPtr loServant = new clLogServer_impl(loFileWriter);
        //adapter->add(loServant, meCommunicationPtr->stringToIdentity("LogAppServer"));
		adapter->add(loServant, meCommunicationPtr->stringToIdentity("LogAppServer"));
        adapter->activate();
		printf("WAIT clLogServer::doWork");
        meCommunicationPtr->waitForShutdown();
		printf("END clLogServer::doWork");
    }
    catch (Ice::Exception& paException)
    {
        cout << paException << endl;
    }

    if (meCommunicationPtr)
    {
        meCommunicationPtr->destroy ();
    }
    printf("Thread is finisched");
}

//Getters and setters
void clLogServer::setPath(QString paPath){ mePath = paPath;}
QString clLogServer::getPath(){return mePath;}
void clLogServer::setSize(QString paSize){ meSize = paSize;}
QString clLogServer::getSize(){return meSize;}
void clLogServer::setDiskUsage(QString paDiskUsage){meDiskUsage = paDiskUsage;}
QString clLogServer::getDiskUsage(){return meDiskUsage;}
