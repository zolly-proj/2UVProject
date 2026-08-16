#include "clServerThread.h"

clServerThread::clServerThread(clXMLReader * paXMLReaderDatabase, clIceClientLogging * paIceClientLogging, Ice::CommunicatorPtr& paServerCommnicationPtr,QObject *parent)
    : QThread(parent)
{
    try
    {
        meServerCommunicationPtr = paServerCommnicationPtr;
        meIceClientLogging = paIceClientLogging;
        meXMLReaderDatabase = paXMLReaderDatabase;

        //Read out all parameters from the xml file
        createDatabaseConfiguration();

        //Initialize classes for methods databases
        createDatabaseClass();
        openDB();

    }
    catch(exception& e)
    {
        meIceClientLogging->insertItem("1","2UVServer.exe","clServerThread::clServerThread()" + QString(e.what()));
    }
}

clServerThread::~clServerThread()
{
    try
    {
        //Destroy server connection
        meServerCommunicationPtr->destroy();
        //Close the database
        closeDB();
    }
    catch(exception& e)
    {
        meIceClientLogging->insertItem("1","2UVServer.exe","clServerThread::~clServerThread()" + QString(e.what()));
    }
}

//Running the thread
void clServerThread::run()
{
    meMutex.lock();
    try
    {
        Ice::ObjectAdapterPtr adapter = meServerCommunicationPtr->createObjectAdapter("ServerAppServerAdaptor");
        meServant = new clServer_impl(meDatabaseTable, meWorkstationStatus, meDatabaseClass, meIceClientLogging);
        adapter->add(meServant, meServerCommunicationPtr->stringToIdentity("ServerAppServer"));
        adapter->activate();
        meServerCommunicationPtr->waitForShutdown();
    }
    catch (exception& e)
    {
        //TODO
        //meIceClientLogging->insertItem("1","2UVServer.exe",QString(paException));

        meIceClientLogging->insertItem("1","2UVServer.exe","clServerThread::run ->" + QString(e.what()));
    }

    if (meServerCommunicationPtr)
    {
        meServerCommunicationPtr->destroy ();
    }
    meMutex.unlock();
}
/************************************************
* Database functions
*************************************************/
bool clServerThread::createDatabaseConfiguration()
{
        try
        {
            //initialize loLocal vars

            QString loErrorMessage;
            QString loDatabaseConfigFileName;

            //Get file path
            if (meXMLReaderDatabase->getAttributeFromConfigFile("DATABASE_CONFIG_PATH",loDatabaseConfigFileName,loErrorMessage))
            {
				//bool readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, vector <clDatabaseRoutine> & paDatabaseRoutines, QString &paReturnMessage);
                if (meXMLReaderDatabase->readDatabaseConfiguration(loDatabaseConfigFileName,meDatabaseTable,meDatabaseRoutines,loErrorMessage))
                {
                    meIceClientLogging->insertItem("1","2UVServer.exe","clServerThread::createDatabaseConfiguration() -> " + loErrorMessage);
                    return true;
                }
                else
                {
                    meIceClientLogging->insertItem("1","2UVServer.exe","clServerThread::createDatabaseConfiguration() -> " + loErrorMessage);
                    return false;
                }
            }
            else
            {
                meIceClientLogging->insertItem("1","2UVServer.exe","clServerThread::createDatabaseConfiguration() -> " + loErrorMessage);
                return false;
            }

        }
        catch(exception& e)
        {
            meIceClientLogging->insertItem("1","2UVServer.exe","clServerThread::createDatabaseConfiguration()" + QString(e.what()));
            return false;
            //cout << e.what() << endl;
        }
}
//Opening the database
void clServerThread::openDB()
{
        try
        {
            QString loMessage;
            if (meDatabaseClass->openDatabase(loMessage))
            {
                meIceClientLogging->insertItem("1","2UVServer.exe",loMessage);
            }
            else
            {
                //appendTextToLog(loMessage);
                QString loErrorMessage("clDatabaseEditorWidget::openDB() -> error ...");
                meIceClientLogging->insertItem("1","2UVServer.exe",loErrorMessage);
            }
        }
        catch(exception& e)
        {
            meIceClientLogging->insertItem("1","2UVServer.exe","clServerThread::openDB()" + QString(e.what()));
            //cout << e.what() << endl;
        }
}
//Closing the database
void clServerThread::closeDB()
{
        try
        {
            QString loMessage;
            if (meDatabaseClass->closeDatabase(loMessage))
            {
                meIceClientLogging->insertItem("1","2UVServer.exe",loMessage);
            }
            else
            {
                QString loErrorMessage("clDatabaseEditorWidget::closeDB() -> error ...");
                meIceClientLogging->insertItem("1","2UVServer.exe",loErrorMessage);
            }
        }
        catch(exception& e)
        {
            meIceClientLogging->insertItem("1","2UVServer.exe","clServerThread::closeDB()" + QString(e.what()));
            //cout << e.what() << endl;
        }
}
//Creating the datab&se class
bool clServerThread::createDatabaseClass()
{
    try
    {
        QString loValue;
        QString  loMessage;

        QString loUserName, loPassword, loDatabaseName, loHostAdress, loPort, loDatabaseTech;

        //Getting the user name
        if (meXMLReaderDatabase->getAttributeFromConfigFile("USERNAME",loValue,loMessage))
        {
            loUserName = loValue;
        }
        else
        {
            QString loErrorMessage("clMainWindow::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error getting var 'USERNAME'...");
            meIceClientLogging->insertItem("1","2UVServer.exe",loErrorMessage);
            return false;
        }

        //Getting the password
        if (meXMLReaderDatabase->getAttributeFromConfigFile("PASSWORD",loValue,loMessage))
        {
            loPassword = loValue;
        }
        else
        {
            QString loErrorMessage("clMainWindow::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error getting var 'PASSWORD'...");
            meIceClientLogging->insertItem("1","2UVServer.exe",loErrorMessage);
            return false;
        }

        //Getting the DatabaseName
        if (meXMLReaderDatabase->getAttributeFromConfigFile("DATABASENAME",loValue,loMessage))
        {
            loDatabaseName = loValue;
        }
        else
        {
            QString loErrorMessage("clMainWindow::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error getting var 'DATABASENAME'...");
            meIceClientLogging->insertItem("1","2UVServer.exe",loErrorMessage);
            return false;
        }

        //Getting the host adress
        if (meXMLReaderDatabase->getAttributeFromConfigFile("HOSTADRESS",loValue,loMessage))
        {
            loHostAdress = loValue;
        }
        else
        {
            QString loErrorMessage("clMainWindow::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error getting var 'HOSTADRESS'...");
            meIceClientLogging->insertItem("1","2UVServer.exe",loErrorMessage);
            return false;
        }

        //Getting the port
        if (meXMLReaderDatabase->getAttributeFromConfigFile("PORT",loValue,loMessage))
        {
            loPort = loValue;
        }
        else
        {
            QString loErrorMessage("clMainWindow::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error getting var 'PORT'...");
            meIceClientLogging->insertItem("1","2UVServer.exe",loErrorMessage);
            return false;
        }

        //Getting the tech
        if (meXMLReaderDatabase->getAttributeFromConfigFile("DATABASETECH",loValue,loMessage))
        {
            loDatabaseTech = loValue;
        }
        else
        {
            QString loErrorMessage("clMainWindow::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error getting var 'DATABASETECH'...");
            meIceClientLogging->insertItem("1","2UVServer.exe",loErrorMessage);
            return false;
        }

        meDatabaseClass = new clDatabaseClass(loUserName,loPassword,loDatabaseName,loHostAdress,loPort,loDatabaseTech, meDatabaseTable, meDatabaseRoutines);
        return true;
    }
    catch(exception& e)
    {
        meIceClientLogging->insertItem("1","2UVServer.exe","clServerThread::createDatabaseClass()" + QString(e.what()));
        //cout << e.what() << endl;
        return false;
    }
}