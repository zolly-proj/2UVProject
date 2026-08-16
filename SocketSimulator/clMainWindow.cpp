#include "clMainWindow.h"
#define INFO_BUFFER_SIZE 32767

clMainWindow::clMainWindow(int paArgc, char * paArgv[], QWidget* paParent, const char* paName)
{
    try
    {
        initializeLogClient(paArgc,paArgv);

        initializeServerClient(paArgc,paArgv);
		
		initializeServerSocketCommunicator(paArgc,paArgv);
		
        meWorkspace = new QMdiArea;
        setCentralWidget(meWorkspace);
        createActions();
        createMenus();
		
		meTimer = new QTimer(this);
        meTimer->setInterval(10000);
        meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotDoIt()));
        meTimer->start();		
    }
    catch(...)
    {

    }
}
//Destructors
clMainWindow::~clMainWindow()
{

}
/*****************************
* ICE FUNCTIONS
*******************************/
void clMainWindow::initializeLogClient(int paArgc, char * paArgv[])
{
    try
    {
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
void clMainWindow::initializeWorkstationCycles(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging)
{
	try
	{
		meWorkstationCycles = new clWorkstationCycles(meIceClientServer, meIceClientLogging);
	}
	catch(...)
	{
		cout << "Workstation cycles not initialized" << endl;
	}
}

/*****************************
* UI controls
*******************************/
//Create the actions
void clMainWindow::createActions()
{
    meExitApplication = new QAction(tr("&Exit"), this);
    meExitApplication->setStatusTip(tr("Exit"));
    connect(meExitApplication, SIGNAL(triggered()), this, SLOT(exitApplication()));

    meSaveAss = new QAction(tr("&SaveAs"), this);
    meSaveAss->setStatusTip(tr("Save ass ..."));
    //connect(meSaveAss, SIGNAL(triggered()), this, SLOT(saveLogging()));

    meConfigProduct = new QAction(tr("&Configure"), this);
    meConfigProduct->setStatusTip(tr("Config product"));
    connect(meConfigProduct, SIGNAL(triggered()), this, SLOT(startProductConfig()));

    meCycles = new QAction(tr("&Cycles"), this);
    meCycles->setStatusTip(tr("Cycles for this workstation"));
    connect(meCycles, SIGNAL(triggered()), this, SLOT(startCycles()));	
	
}
//Creating the menu
void clMainWindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    editMenu = menuBar()->addMenu(tr("&Edit"));
    productMenu = menuBar()->addMenu(tr("&Product"));
    helpMenu = menuBar()->addMenu(tr("&Help"));

    fileMenu->addAction(meExitApplication);
    fileMenu->addAction(meSaveAss);
    productMenu->addAction(meConfigProduct);
	productMenu->addAction(meCycles);
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
			
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clMainWindow::exitApplication()" + QString(e.what()));
        }
}
//Start config product
void clMainWindow::startProductConfig()
{
        try
        {
            meSocketCommunicatorView = new clSocketCommunicatorView(meIceClientServer,meIceClientLogging);
            meWorkspace->addSubWindow(meSocketCommunictorView);
            meSocketCommunictorView->show();
        }
        catch(exception &e)
        {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clMainWindow::exitApplication()" + QString(e.what()));
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
			//if(uname(&name)) exit(-1);
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clMainWindow::exitApplication()" + QString(e.what()));
    }
}
