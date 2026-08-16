#ifndef CLMAINWINDOW_H_WVD_08102011
#define CLMAINWINDOW_H_WVD_08102011

#undef slots
#include <Python.h>
#define slots Q_SLOTS

#include <string>
#include <iostream>
#include <vector>
#include <exception>
#include <stdio.h>
#include <stdlib.h>


#include <QtCore/QString>
#include <QtCore/QDateTime>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QListWidget>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QMutex>



#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clParameterView.h"
#include "clCycleView.h"
#include "clLivingObject.h"
#include "clWorkstationCycles.h"
#include "clClassLoader.h"


using namespace std;


class clMainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    clMainWindow (int paArgc, char * paArgv[], QWidget* paParent = 0, const char* paName = 0);
    ~clMainWindow ();
public:
	QMutex meLock;
	PyThreadState* tstate;
public slots:

    void exitApplication();
    void startProductConfig();
	void startCycles();
	void slotDoIt();
private:
    void createMenus();
    void createActions();
	int getUsage(double &val);
	//ULONGLONG SubtractTimes(const FILETIME one, const FILETIME two);


    QMdiArea *meWorkspace;


    //Products
    clParameterView *meParameterView;
	clCycleView *meCycleView;

	QTimer * meTimer;



    //Private qt vars
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *productMenu;
    QMenu *helpMenu;

    //File actions

    QAction *meExitApplication;
    QAction *meSaveAss;
    QAction *meConfigProduct;
	QAction *meCycles;


    //Ice communicators for logging
    clIceClientLogging *meIceClientLogging;
    Ice::CommunicatorPtr meIceCommunicatorLogging;
    void initializeLogClient(int paArgc, char * paArgv[]);

    //Ice communicator for server
    clIceClientServer *meIceClientServer;
    Ice::CommunicatorPtr meIceCommunicatorServer;
    void initializeServerClient(int paArgc, char * paArgv[]);

	void initializePython();

	clLivingObject *meLivingObjects;
	void initializeLivingObject(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging);
	
	clWorkstationCycles *meWorkstationCycles;
	void initializeWorkstationCycles(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging);

	clClassLoader *meClassLoader;
	void initializeClassLoader(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging);

	//FILETIME prevSysIdle, prevSysKernel, prevSysUser;
};

#endif
