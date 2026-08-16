#ifndef CLMAINWINDOW_H_WVD_08102011
#define CLMAINWINDOW_H_WVD_08102011

#include <string>
#include <iostream>
#include <vector>
#include <exception>



#include "clDatabaseClass.h"
#include "clXMLReader.h"


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
#include <QtCore/QByteArray>
#include <QtCore/QThread>


#include "clConfigProductWidget.h"
#include "clConfigWorkstationWidget.h"
#include "clIceClientLogging.h"
#include "clServer_impl.h"
#include "clServerThread.h"

using namespace std;


class clMainWindow : public QMainWindow
{
    Q_OBJECT;

public:
    clMainWindow (int paArgc,char * paArgv[], char * paIniFile, QWidget* paParent = 0, const char* paName = 0);
    ~clMainWindow ();

public slots:

    void exitApplication();
    void startProductConfig();
    void startLogging();
	void startShowWorkstations();


private:
    void createMenus();
    void createActions();
    void initializeLogServer(int paArgc,char * paArgv[]);
    void initializeIceServer(int paArgc,char * paArgv[]);

    //Config file functions
    bool createXMLReaderServerClass(char * paFileName);
    bool createXMLReaderDatabaseClass();

    QMdiArea *meWorkspace;

    //Private qt vars
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *productMenu;
    QMenu *helpMenu;

    //File actions

    QAction *meExitApplication;
    QAction *meSaveAss;
    QAction *meConfigProduct;
	QAction *meConfigWorkstation;
    QAction *meLogging;


    //Private products
    QMainWindow *meConfigProductWidget;
	QMainWindow *meConfigWorkstationWidget;
    QMainWindow *meLoggingWidget;

    //XML FILES
    clXMLReader * meXMLReaderServer;
    clXMLReader * meXMLReaderDatabase;

    //Threading for server
    clServerThread * meServerThread;
    Ice::CommunicatorPtr meServerCommunicationPtr;
    //Ice communicators for logging
	clIceClientLogging * meIceClientLogging;	
	Ice::CommunicatorPtr meIceCommunicator;





};

#endif
