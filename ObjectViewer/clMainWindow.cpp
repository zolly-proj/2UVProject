#include "clMainWindow.h"
#define INFO_BUFFER_SIZE 32767
//Constructors
clMainWindow::clMainWindow(int paArgc, char * paArgv[], QWidget* paParent, const char* paName)
{
    try
    {
		if (paArgc > 0)
		{
			meConfigurationFile = QString(paArgv[1]);
		}
		
        initializeLogClient(paArgc,paArgv);
        initializeServerClient(paArgc,paArgv);
		initializeClassLoader(meIceClientServer, meIceClientLogging);
		
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
void clMainWindow::initializeClassLoader(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging)
{
	try
	{
		meClassLoader = new clClassLoader(meIceClientServer, meIceClientLogging);
	}
	catch(...)
	{
		cout << "ClassLoader not initialized" << endl;
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

    meConfigProduct = new QAction(tr("&Configure"), this);
    meConfigProduct->setStatusTip(tr("Config product"));
    connect(meConfigProduct, SIGNAL(triggered()), this, SLOT(startProductConfig()));	
	
	meActionProductPlanner = new QAction(tr("&Planning"), this);
    meActionProductPlanner->setStatusTip(tr("Product planner"));
    connect(meActionProductPlanner, SIGNAL(triggered()), this, SLOT(startProductPlanner()));	

	meActionObjectLocator = new QAction(tr("&Object locator"), this);
    meActionObjectLocator->setStatusTip(tr("Object locator"));
    connect(meActionObjectLocator, SIGNAL(triggered()), this, SLOT(startObjectLocator()));		
    
	meActionSchedulePlanner = new QAction(tr("&Schedule planner"), this);
    meActionSchedulePlanner->setStatusTip(tr("Schedule planner"));
    connect(meActionSchedulePlanner, SIGNAL(triggered()), this, SLOT(startSchedulePlanner()));    
    
	meActionMachineView = new QAction(tr("&Machine view"), this);
    meActionMachineView->setStatusTip(tr("Machine view"));
    connect(meActionMachineView, SIGNAL(triggered()), this, SLOT(startMachineView()));    
        
}
//Creating the menu
void clMainWindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    productMenu = menuBar()->addMenu(tr("&Product"));
    helpMenu = menuBar()->addMenu(tr("&Help"));

    fileMenu->addAction(meExitApplication);
    productMenu->addAction(meConfigProduct);
	productMenu->addAction(meActionProductPlanner);
	productMenu->addAction(meActionObjectLocator);
	productMenu->addAction(meActionSchedulePlanner);
	productMenu->addAction(meActionMachineView);
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
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectView.exe","clMainWindow::exitApplication()" + QString(e.what()));
        }
}
//Start config product
void clMainWindow::startProductConfig()
{
        try
        {
            meObjectViewer = new clObjectViewer(meIceClientServer,meIceClientLogging,meClassLoader, meConfigurationFile);
            meWorkspace->addSubWindow(meObjectViewer);
            meObjectViewer->show();
        }
        catch(exception &e)
        {	
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectView.exe","clMainWindow::startProductConfig()" + QString(e.what()));
        }
}

void clMainWindow::startProductPlanner()
{
        try
        {
            meProductPlanner = new clProductPlanner(meIceClientServer,meIceClientLogging,meClassLoader, meConfigurationFile);
            meWorkspace->addSubWindow(meProductPlanner);
            meProductPlanner->show();
        }
        catch(exception &e)
        {	
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectView.exe","clMainWindow::startProductPlanner()" + QString(e.what()));
        }
}
void clMainWindow::startSchedulePlanner()
{
        try
        {
            meSchedulePlanner = new clSchedulePlanner(meIceClientServer,meIceClientLogging,meClassLoader, meConfigurationFile);
            meWorkspace->addSubWindow(meSchedulePlanner);
            meSchedulePlanner->show();
        }
        catch(exception &e)
        {	
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectView.exe","clMainWindow::startSchedulePlanner()" + QString(e.what()));
        }
}
void clMainWindow::startObjectLocator()
{
        try
        {
            meObjectLocator = new clObjectLocator(meIceClientServer,meIceClientLogging,meClassLoader, meConfigurationFile);
            meWorkspace->addSubWindow(meObjectLocator);
            meObjectLocator->show();
        }
        catch(exception &e)
        {	
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectView.exe","clMainWindow::startObjectLocator()" + QString(e.what()));
        }
}

void clMainWindow::startMachineView()
{
        try
        {
            meMachineView = new clMachineView(meIceClientServer,meIceClientLogging,meClassLoader, meConfigurationFile);
            meWorkspace->addSubWindow(meMachineView);
            meMachineView->show();
        }
        catch(exception &e)
        {	
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectView.exe","clMainWindow::startMachineView()" + QString(e.what()));
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

/*
int clMainWindow::getUsage(double &val)
{
    FILETIME sysIdle, sysKernel, sysUser;
    // sysKernel include IdleTime
    if (GetSystemTimes(&sysIdle, &sysKernel, &sysUser) == 0) // GetSystemTimes func FAILED return value is zero;
        return 0;

    if (prevSysIdle.dwLowDateTime != 0 && prevSysIdle.dwHighDateTime != 0)
    {
        ULONGLONG sysIdleDiff, sysKernelDiff, sysUserDiff;
        sysIdleDiff = SubtractTimes(sysIdle, prevSysIdle);
        sysKernelDiff = SubtractTimes(sysKernel, prevSysKernel);
        sysUserDiff = SubtractTimes(sysUser, prevSysUser);

        ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
        ULONGLONG kernelTotal = sysKernelDiff - sysIdleDiff; // kernelTime - IdleTime = kernelTime, because sysKernel include IdleTime

        if (sysTotal > 0) // sometimes kernelTime > idleTime
            val = (double)(((kernelTotal + sysUserDiff) * 100.0) / sysTotal);
    }

    prevSysIdle = sysIdle;
    prevSysKernel = sysKernel;
    prevSysUser = sysUser;
    return 1;
}
*/

// TIME DIFF FUNC
/*
ULONGLONG clMainWindow::SubtractTimes(const FILETIME one, const FILETIME two)
{
    LARGE_INTEGER a, b;
    a.LowPart = one.dwLowDateTime;
    a.HighPart = one.dwHighDateTime;

    b.LowPart = two.dwLowDateTime;
    b.HighPart = two.dwHighDateTime;
    return a.QuadPart - b.QuadPart;
}
*/
