#include "clMainWindow.h"
#define INFO_BUFFER_SIZE 32767
//Constructors
clMainWindow::clMainWindow(int paArgc, char * paArgv[], QWidget* paParent, const char* paName)
{
    try
    {
        initializeLogClient(paArgc,paArgv);

        initializeServerClient(paArgc,paArgv);

		initializeClassLoader(meIceClientServer, meIceClientLogging);
		
		initializePython();
		
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
    //connect(workspace, SIGNAL(windowActivated(QWidget *)),
    //        this, SLOT(updateMenus()));
    //createActions();
    //createMenus();
    //createToolBars();
    //createStatusBar();
    //setWindowTitle(tr("MDI Editor"));
    //setWindowIcon(QPixmap(":/images/icon.png"));
    }
}
//Destructors
clMainWindow::~clMainWindow()
{
    //Clean up the memory
    //delete meDatabaseClass;
    //delete meXMLReader;
    PyGILState_Ensure();
	Py_Finalize();
}
/********************************
 * 
 * *****************************/
void clMainWindow::initializePython()
{
	try
	{
		// initialize Python
		Py_Initialize() ;
		PyEval_InitThreads() ; // nb: creates and locks the GIL
		// NOTE: We save the current thread state, and restore it when we unload,
		// so that we can clean up properly.
		tstate = PyEval_SaveThread() ;
		
		initializeLivingObject(meIceClientServer, meIceClientLogging);
		initializeWorkstationCycles(meIceClientServer, meIceClientLogging);
		
	}
	catch(...)
	{
		cout << "Python not initialized" << endl;
	}
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
void clMainWindow::initializeLivingObject(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging)
{
	try
	{
		meLivingObjects = new clLivingObject(meIceClientServer, meIceClientLogging, &meLock, meClassLoader);
	}
	catch(...)
	{
		cout << "Living objects not initialized" << endl;
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
void clMainWindow::initializeWorkstationCycles(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging)
{
	try
	{
		meWorkstationCycles = new clWorkstationCycles(meIceClientServer, meIceClientLogging, &meLock, meClassLoader);
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
            meParameterView = new clParameterView(meIceClientServer,meIceClientLogging,meClassLoader, &meLock);
            meWorkspace->addSubWindow(meParameterView);
            meParameterView->show();
        }
        catch(exception &e)
        {

			
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clMainWindow::startProductConfig()" + QString(e.what()));
        }
}
void clMainWindow::startCycles()
{
        try
        {
            meCycleView = new clCycleView(meIceClientServer,meIceClientLogging, meWorkstationCycles);
            meWorkspace->addSubWindow(meCycleView);
            meCycleView->show();
        }
        catch(exception &e)
        {			
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clMainWindow::startProductConfig()" + QString(e.what()));
        }
}
void clMainWindow::slotDoIt()
{
    try
    {

/*
		//Getting the mem usage
		//memory_load
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof (statex);
		GlobalMemoryStatusEx (&statex);
		// it already contains the percentage
		
		auto memory_load = statex.dwMemoryLoad;

		std::string memory_load_str;

		#ifndef UNICODE
			memory_load_str = memory_load;
		#else
			std::wstring wStr = memory_load;
			memory_load_str = std::string(wStr.begin(), wStr.end());
		#endif
		
		const char* str = memory_load_str.c_str();
		char diskUsage = str[0];
		
		
		
		
		
		
		// or calculate it from other field if need more digits.

		
		//Getting the disk usage
		//p1 freebytesavailable
		//p2 Total number of bytes
		//p3 Total number of free bytes
		ULARGE_INTEGER p1, p2, p3;
		GetDiskFreeSpaceEx("C:/", &p1, &p2, &p3);
		__int64 l2 = p2.QuadPart / 100;
		__int64 l3 = p3.QuadPart / l2;
		
		std::stringstream stream_l3;
		stream_l3 << l3;
		
		//Getting the cpu load of the computer multiply by 100
		double val;
		getUsage(val);
		
		string loReturnMessage;
		//computername, memusage, diskusage, cpuusage, returnmessage
		meIceClientServer->giveWorkstationClientInfo(std::string(infoBuf),std::to_string((int)diskUsage), std::string(stream_l3.str()), std::string(std::to_string(val)), loReturnMessage);
		meIceClientLogging->insertItem(QString("10"),QString(QHostInfo::localHostName()),QString("2UVServerTest.exe"),QString("clMainWindow::slotDoIt() insert values ") + QString(" [") + QString(std::string(infoBuf).c_str()) + QString("] ") + QString(" [") + QString(std::to_string((int)diskUsage).c_str()) + QString("] ") + QString(" [") + QString(std::string(stream_l3.str()).c_str()) + QString("] ") + QString(" [") + QString(std::string(to_string(val)).c_str()) + QString("]"));
		meIceClientLogging->insertItem(QString("10"),QString(QHostInfo::localHostName()),QString("2UVServerTest.exe"),QString("clMainWindow::slotDoIt() ") + QString(loReturnMessage.c_str()));
        */
    }
    catch (exception &e)
    {
		meIceClientLogging->insertItem("10","blanck","2UVServerTest.exe","clMainWindow::slotDoIt() " + QString(e.what()));
    }
}


int clMainWindow::getUsage(double &val)
{
    /*
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
    */
    return 1;
}


// TIME DIFF FUNC
//ULONGLONG clMainWindow::SubtractTimes(const FILETIME one, const FILETIME two)
//{
    /*
    LARGE_INTEGER a, b;
    a.LowPart = one.dwLowDateTime;
    a.HighPart = one.dwHighDateTime;

    b.LowPart = two.dwLowDateTime;
    b.HighPart = two.dwHighDateTime;
    return a.QuadPart - b.QuadPart;
    */
//}
