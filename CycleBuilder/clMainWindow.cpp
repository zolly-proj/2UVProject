#include "clMainWindow.h"
#define INFO_BUFFER_SIZE 32767
//Constructors
clMainWindow::clMainWindow(int paArgc, char * paArgv[], QWidget* paParent, const char* paName)
{
    try
    {
        initializeLogClient(paArgc,paArgv);

        initializeServerClient(paArgc,paArgv);
		
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

    meConfigCycle = new QAction(tr("&Configure"), this);
    meConfigCycle->setStatusTip(tr("Config product"));
    connect(meConfigCycle, SIGNAL(triggered()), this, SLOT(startConfigCycle()));
}
//Creating the menu
void clMainWindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    helpMenu = menuBar()->addMenu(tr("&Help"));

    fileMenu->addAction(meExitApplication);
    fileMenu->addAction(meSaveAss);
    fileMenu->addAction(meConfigCycle);
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
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clMainWindow::exitApplication()" + QString(e.what()));
        }
}
//Start config product
void clMainWindow::startConfigCycle()
{
        try
        {
            meCycleBuilderView = new clCycleBuilderView(meIceClientServer,meIceClientLogging);
            meWorkspace->addSubWindow(meCycleBuilderView);
            meCycleBuilderView->show();
        }
        catch(exception &e)
        {	
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clMainWindow::startConfigCycle()" + QString(e.what()));
        }
}
void clMainWindow::slotDoIt()
{
    try
    {
		QStorageInfo storage = QStorageInfo::root();
		
		string loReturnMessage;
		//TODO WVD percentage diskusage, percentage cpu usage
		meIceClientServer->giveWorkstationClientInfo(std::string(QHostInfo::localHostName().toStdString()),std::to_string(storage.bytesTotal()/1000/1000), std::string("50"), std::string("50"), loReturnMessage);
		meIceClientLogging->insertItem(QString("10"),QString(QHostInfo::localHostName()),QString("2UVServerTest.exe"),QString("clMainWindow::slotDoIt() insert values ") + QString(" [") + QString(QHostInfo::localHostName()) + QString("] ") + QString(" [") + QString("50") + QString("] ") + QString(" [") + QString("50") + QString("] ") + QString(" [") + QString("50") + QString("]"));
		meIceClientLogging->insertItem(QString("10"),QString(QHostInfo::localHostName()),QString("2UVServerTest.exe"),QString("clMainWindow::slotDoIt() ") + QString(loReturnMessage.c_str()));	
    }
    catch (exception &e)
    {
		meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clMainWindow::slotDoIt() " + QString(e.what()));
    }
}
