#ifndef CLMAINWINDOW_H_WVD_08102011
#define CLMAINWINDOW_H_WVD_08102011

#include <string>
#include <iostream>
#include <vector>
#include <exception>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>



#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#elif defined(_WIN32)
#include <windows.h>
#include <tchar.h>
#elif defined(__WIN32)
#include <windows.h>
#include <tchar.h>
#elif defined(__WIN32__)
#include <windows.h>
#include <tchar.h>
#else
//Include something else
#endif


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
#include <QtCore/QStorageInfo>
#include <QtNetwork/QHostInfo>


#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clParameterView.h"
#include "clCycleView.h"
#include "clLivingObject.h"
#include "clWorkstationCycles.h"
#include "clClassLoader.h"

#define ULONGLONG unsigned long
#define FILETIME unsigned long
using namespace std;



class clMainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    clMainWindow (int paArgc, char * paArgv[], QWidget* paParent = 0, const char* paName = 0);
    ~clMainWindow ();
public:


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

	clSocketCommunicatorView *meSocketCommunicatorView;
	void initializeSocketCommunicatorView(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging);
	
	FILETIME prevSysIdle, prevSysKernel, prevSysUser;	
};
#endif
