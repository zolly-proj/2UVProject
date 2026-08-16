#include "clMainWindow.h"
/*************************
* Constructors
*************************/
clMainWindow::clMainWindow(int paArgc, char * paArgv[], char * paIniFile, QWidget* paParent, const char* paName)
{
    try
    {

        createXMLReaderServerClass(paIniFile);
        createXMLReaderDatabaseClass();

        //Client ice
        initializeLogServer(paArgc,paArgv);

		//Set up logging
		createActions();

        //Server ice
        initializeIceServer(paArgc,paArgv);
		

        //Create GUI
        meWorkspace = new QMdiArea;
        setCentralWidget(meWorkspace);
        
        createMenus();



    }
    catch(exception& e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe","clMainWindow::clMainWindow -> " + QString(e.what()));
        //cout << e.what() << endl;
    }
}
//Destructors
clMainWindow::~clMainWindow()
{
    try
    {
        meServerThread->quit();
    }
    catch(exception& e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe","clMainWindow::~clMainWindow -> " + QString(e.what()));
    }
    //Clean up the memory
    //delete meDatabaseClass;
    //delete meXMLReader;
}
/*****************************
* ICE Functions
*******************************/
void clMainWindow::initializeLogServer(int paArgc,char * paArgv[])
{
    try
    {
        Ice::InitializationData initData2;
        initData2.properties = Ice::createProperties();
        initData2.properties->load("config.client.LogServer");

        meIceCommunicator = Ice::initialize(paArgc, paArgv, initData2);

        printf("main() -> Communicator inistialized ");
        meIceClientLogging = new clIceClientLogging(meIceCommunicator);
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }
}

void clMainWindow::initializeIceServer(int paArgc, char * paArgv[])
{
    try
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe","clMainWindow::initializeIceServer -> Start");
        //Initialize the Ice Server
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        initData.properties->load("config.server");
        meServerCommunicationPtr = Ice::initialize(paArgc, paArgv, initData);

        meServerThread = new clServerThread(meXMLReaderDatabase, meIceClientLogging, meServerCommunicationPtr);
        meServerThread->start();
    }
    catch(exception& e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe","clMainWindow::initializeIceServer -> " + QString(e.what()));
    }
}

/*****************************
* UI controls
*******************************/
//Create the actions
void clMainWindow::createActions()
{
    try
    {
        meIceClientLogging->insertItem("1","2UVServer.exe","clMainWindow::createActions() -> start");

        meExitApplication = new QAction(tr("&Exit"), this);
        meExitApplication->setStatusTip(tr("Exit"));
        connect(meExitApplication, SIGNAL(triggered()), this, SLOT(exitApplication()));

        meSaveAss = new QAction(tr("&SaveAs"), this);
        meSaveAss->setStatusTip(tr("Save ass ..."));
        //connect(meSaveAss, SIGNAL(triggered()), this, SLOT(saveLogging()));

        meConfigProduct = new QAction(tr("&Configure"), this);
        meConfigProduct->setStatusTip(tr("Config product"));
        connect(meConfigProduct, SIGNAL(triggered()), this, SLOT(startProductConfig()));
		
		
		meConfigWorkstation = new QAction(tr("&Workstations"), this);
        meConfigWorkstation->setStatusTip(tr("Workstations"));
        connect(meConfigWorkstation, SIGNAL(triggered()), this, SLOT(startShowWorkstations()));
		
		

        meLogging = new QAction(tr("&Logging"),this);
        meLogging->setStatusTip(tr("Logging product"));
        connect(meLogging, SIGNAL(triggered()), this, SLOT(startLogging()));

        meIceClientLogging->insertItem("1","2UVServer.exe","clMainWindow::createActions() -> stop");
    }
    catch(exception& e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe","clMainWindow::createActions ->" + QString(e.what()));
        //cout << e.what() << endl;
    }
}
//Creating the menu
void clMainWindow::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(meExitApplication);
    fileMenu->addAction(meSaveAss);

    productMenu = menuBar()->addMenu(tr("&Product"));
    productMenu->addAction(meConfigProduct);
	
    productMenu = menuBar()->addMenu(tr("&Workstation"));	
	productMenu->addAction(meConfigWorkstation);

    editMenu = menuBar()->addMenu(tr("&Edit"));

    helpMenu = menuBar()->addMenu(tr("&Help"));
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
        catch(exception& e)
        {
            meIceClientLogging->insertItem("1","2UVLogServer.exe","clMainWindow::exitApplication ->" + QString(e.what()));
            //cout << e.what() << endl;
        }
}
//Start config product
void clMainWindow::startProductConfig()
{
        try
        {
            meConfigProductWidget = new clConfigProductWidget();
            meWorkspace->addSubWindow(meConfigProductWidget);
            meConfigProductWidget->show();
        }
        catch(exception& e)
        {
            meIceClientLogging->insertItem("1","2UVLogServer.exe","clMainWindow::startProductConfig -> " + QString(e.what()));
            //cout << e.what() << endl;
        }
}
//Start config product
void clMainWindow::startShowWorkstations()
{
        try
        {
            meConfigWorkstationWidget = new clConfigWorkstationWidget(meIceClientLogging, meServerThread);
            meWorkspace->addSubWindow(meConfigWorkstationWidget);
            meConfigWorkstationWidget->show();
        }
        catch(exception& e)
        {
            meIceClientLogging->insertItem("1","2UVLogServer.exe","clMainWindow::startShowWorkstations -> " + QString(e.what()));
            //cout << e.what() << endl;
        }
}




void clMainWindow::startLogging()
{
    try
    {
        //meLoggingWidget = new clLoggingWidget();
        //meWorkspace->addWindow(meLoggingWidget);
        //meLoggingWidget->show();
    }
    catch(exception& e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe","clMainWindow::startLogging -> " + QString(e.what()));
    }
}

/************************************************
* File access
*************************************************/
//Getting server config files
bool clMainWindow::createXMLReaderServerClass(char * paFileName)
{
    try
    {
        //padatbaseClass = clDatabaseClass(paFileName);
        QString loFileName(paFileName);
        meXMLReaderServer = new clXMLReader(loFileName);
        return true;
    }
    catch(exception& e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe","clMainWindow::createXMLReaderServerClass -> " + QString(e.what()));
        //cout << e.what() << endl;
        return false;
    }
}
//Getting database config files
bool clMainWindow::createXMLReaderDatabaseClass()
{
    try
    {
        QString loDatabasePath;
        QString loValue;
        QString loMessage;
        //Getting the user name
        if (meXMLReaderServer->getAttributeFromConfigFile("DATABASE",loValue,loMessage))
        {
            loDatabasePath = loValue;
            meXMLReaderDatabase = new clXMLReader(loDatabasePath);
        }
        else
        {
            QString loErrorMessage("bool clMainWindow::createXMLReaderDatabaseClass() -> error getting var 'DATABASE'...");
            meIceClientLogging->insertItem("1","2UVLogServer.exe",loErrorMessage);
            return false;
        }

        return true;
    }
    catch(exception& e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe","clMainWindow::createXMLReaderDatabaseClass -> " + QString(e.what()));
        //cout << e.what() << endl;
        return false;
    }
}

