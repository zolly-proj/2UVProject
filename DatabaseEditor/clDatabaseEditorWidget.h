#ifndef CLDATABASEEDITORWIDGET_H_WVD_10032011
#define CLDATABASEEDITORWIDGET_H_WVD_10032011

#undef slots
#include <Python.h>
#define slots Q_SLOTS

#include <string>
#include <iostream>
#include <fstream>

#include "wdgDatabaseEditor.h"
#include "clDatabaseClass.h"
#include "clDatabaseRoutine.h"
#include "clDatabaseColumn.h"

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QMutex>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QAction>
#include <QtCore/QByteArray>
#include <QtCore/QThread>
#include <QtNetwork/QHostInfo>



#include "libpq-fe.h"
#include "clXMLReader.h"

#include "clDatabaseTable.h"
using namespace std;


class clDatabaseEditorWidget : public QMainWindow
{

    Q_OBJECT;

public:
    clDatabaseEditorWidget (char * paIniFile, QWidget* paParent = 0, const char* paName = 0); //, WFlags paFlags = 0);
    ~clDatabaseEditorWidget ();

    Ui::wdgDatabaseEditor meDatabaseEditor;
    QThread meThread;

public slots:
    //Slots Camera setup
        /*
    void slotButtonPressed();
    void slotWriteToDB();
    void slotGetFromDB();
    */
    void openDB();
    void closeDB();
    void updateDB();
    void backupDB();
    void exitApplication();
    void saveLogging();
private:
    //private UI methods
    void createMenus();
    void createActions();
    void appendTextToLog(const QString paString);
    void setProgressBar(int paTotal,int paNumber);
    QString saveAs();


    //private local methods
    //bool createDatabaseClass(clDatabaseClass * paDatabaseClass, clXMLReader & paXMLReader);
    bool createDatabaseClass();
    //bool createXMLReaderClass(char * paFileName, clXMLReader & paXMLReader);
    bool createXMLReaderClass(char * paFileName);

    //private Datbase methods
    void CloseConn(PGconn *conn);
    PGconn * ConnectDB();
    bool backupDatabase(QString paFileName, QString paDatbaseName, QString paUserName);
    bool updateDatabase(vector <clDatabaseTable> paDatabaseTable,QString & paMessage);
    //private python methods
    bool performScript(char * paScriptName, char * paScriptClass, char * paScriptMethod, char* arg01, char* arg02, char* arg03, char* arg04, char* arg05, char* arg06, char* arg07, char* arg08, char* arg09, char* arg10);

    //Private local vars
    clDatabaseClass * meDatabaseClass;
    clXMLReader * meXMLReader;

    //Private postgres vars
    PGconn * meDatabaseConnection;

    //Private qt vars
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;

    //File actions
    QAction *meActOpenDb;
    QAction *meActCloseDB;
    QAction *meExitApplication;
    QAction *meUpdateDatabase;
    QAction *meBackupDatabase;
    QAction *meSaveAss;
    
    int meResultFromFunction;
    QMutex meLock;
};

#endif
