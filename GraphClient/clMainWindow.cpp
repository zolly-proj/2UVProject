#include "clMainWindow.h"
#define INFO_BUFFER_SIZE 32767
//Constructors
clMainWindow::clMainWindow(int paArgc, char * paArgv[], QWidget* paParent, const char* paName)
{
    try
    {
        initializeLogClient(paArgc,paArgv);

        initializeServerClient(paArgc,paArgv);

		meGraphView = new clGraphView(meIceClientServer, meIceClientLogging, paArgv[1]);
 
        setCentralWidget(meGraphView);

		meTimer = new QTimer(this);
        meTimer->setInterval(10000);
        meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotDoIt()));
        meTimer->start();		
    }
    catch(...)
    {
		cout << "Constructor failed ..." << endl;
    }
}
//Destructors
clMainWindow::~clMainWindow()
{
}
/*****************************
* Chart functions
*******************************/





/*****************************
* ICE FUNCTIONS
*******************************/
void clMainWindow::initializeLogClient(int paArgc, char * paArgv[])
{
    try
    {
		/*
		int loArgc = 2;
		char * loArgv[1];
		loArgv[0] = paArgv[0];
		loArgv[1] = paArgv[1];
		*/
        Ice::InitializationData initData2;
        initData2.properties = Ice::createProperties();
        initData2.properties->load("config.client.LogServer");
        meIceCommunicatorLogging = Ice::initialize(paArgc, paArgv, initData2);
        meIceClientLogging = new clIceClientLogging(meIceCommunicatorLogging);
    }
    catch(...)
    {
        cout << "Logging not initialized" << endl;
    }
}

void clMainWindow::initializeServerClient(int paArgc, char * paArgv[])
{
    try
    {
		/*
		int loArgc = 2;
		char * loArgv[1];
		loArgv[0] = paArgv[0];
		loArgv[1] = paArgv[2];
		*/
		
        Ice::InitializationData initData2;
        initData2.properties = Ice::createProperties();
        initData2.properties->load("config.client.Server");
        meIceCommunicatorServer = Ice::initialize(paArgc, paArgv, initData2);
        printf("main() -> Server Communicator inistialized ");

        meIceClientServer = new clIceClientServer(meIceCommunicatorServer,meIceClientLogging);
    }
    catch(...)
    {
        cout << "Logging not initialized" << endl;
    }
}
/***********************************
* UI slots
***************************************/
//Exit the application
void clMainWindow::exitApplication()
{
        try
        {
            this->close();
        }
        catch(exception &e)
        {
            meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clMainWindow::exitApplication()" + QString(e.what()));
        }
}

void clMainWindow::slotDoIt()
{	
    try
    {
		
		
		QStorageInfo storage = QStorageInfo::root();
		/*
			bool giveWorkstationClientInfo(	string paWorkstationName,
											string paPercentageMemUsage,
											string paPercentageDiskUsage,
											string paPercentageCpuUsage,
											out string paReturnMessage);	
		 */
		
		string loReturnMessage;
		//TODO WVD percentage diskusage, percentage cpu usage
		meIceClientServer->giveWorkstationClientInfo(std::string(QHostInfo::localHostName().toStdString()),std::to_string(storage.bytesTotal()/1000/1000), std::string("50"), std::string("50"), loReturnMessage);
		meIceClientLogging->insertItem(QString("10"),QString(QHostInfo::localHostName()),QString("2UVServerTest.exe"),QString("clMainWindow::slotDoIt() insert values ") + QString(" [") + QString(QHostInfo::localHostName()) + QString("] ") + QString(" [") + QString("50") + QString("] ") + QString(" [") + QString("50") + QString("] ") + QString(" [") + QString("50") + QString("]"));
		meIceClientLogging->insertItem(QString("10"),QString(QHostInfo::localHostName()),QString("2UVServerTest.exe"),QString("clMainWindow::slotDoIt() ") + QString(loReturnMessage.c_str()));
		
		
    }
    catch (exception &e)
    {
			meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clMainWindow::exitApplication()" + QString(e.what()));
    }
}
