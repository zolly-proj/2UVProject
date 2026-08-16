#undef slots
#include <Python.h>
#define slots Q_SLOTS

#ifndef CLWORKSTATIONCYCLE_H
#define CLWORKSTATIONCYCLE_H

#include <vector>
#include <iostream>
#include <string>
#include <dlfcn.h>


#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDir>
#include <QtCore/QFileInfoList>
#include <QtCore/QFile>

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#include "clDatabaseColumn.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clMethodCall.h"
#include "clMethodCallHeader.h"
#include "clClassLoader.h"

#define pyscope() \
    PyGILState_STATE gstate = PyGILState_Ensure(); \
    util::scope_guard sggstate([&]() \
    { \
        PyGILState_Release(gstate); \
    })


using namespace std;


class clWorkstationCycle : public QThread
{
    Q_OBJECT
public:
    clWorkstationCycle(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QString paObjectId, QMutex * paLock, clClassLoader * paClassLoader, QObject * parent = 0);
    ~clWorkstationCycle();


	
protected:
    void run() override;
signals:
    void resultReady(const QString &s);
private:
	bool getWorkstationRoutineCycles(QString paObjectId);
	bool performScript(char * paScriptName, char * paScriptClass, char * paScriptMethod, char *paArg01, char *paArg02, char *paArg03, char *paArg04, char *paArg05, char *paArg06, char *paArg07, char *paArg08, char *paArg09, char *paArg10);
	bool performScriptWithDebug(char * paScriptNameWithExtention, char * paScriptName, char * paScriptClass, char * paScriptMethod, char *paArg01, char *paArg02, char *paArg03, char *paArg04, char *paArg05, char *paArg06, char *paArg07, char *paArg08, char *paArg09, char *paArg10);
	
	bool readCurrentRoutine();
	bool getRoutineFromCycleRoutine();
	bool selectRoutine();
	bool performRoutine();
	bool callRoutinePythonNoDebug();
	bool callRoutinePythonWithDebug();
	bool callRoutineDLL();
	bool chooseAndSetNextRoutine();
	bool prepareScriptingPath(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
	
	clIceClientLogging * meIceClientLogging;
	clIceClientServer * meIceClientServer;
	//PyThreadState* pThreadState;
	
	enum Choice
	{
		READ_CURRENT_ROUTINE = 0,
		GET_ROUTINE_FROM_CYCLE_ROUTINE = 10,
		SELECT_ROUTINE = 20, 
		PERFORM_ROUTINE = 30,
		CALL_ROUTINE_PYTHON_NO_DEBUG = 40,
		CALL_ROUTINE_PYTHON_WITH_DEBUG = 50,
		CALL_ROUTINE_DLL = 60,
		CHOOSE_AND_SET_NEXT_ROUTINE = 70
	};
public:
	vector<clDatabaseColumn> meDatabaseColumnCycle;
	vector<clDatabaseColumn> meDatabaseColumnCycleRoutine;
	
	vector<std::string> meRoutineTables;
	vector<std::string> meCycleRoutineIds;
	
	vector<clDatabaseColumn> meCurrentDatabaseColumnRoutine;

	QString meObjectId;
	
	QString meCycleUUID;
	QString meCycleName;
	QString meCycleWorkstation;
	
	QString meCurrentCycleRoutineUUID;
	
	QString meCurrentRoutineClass;
	QString meCurrentRoutineUUID;
	QString meCurrentRoutineName;
	QString meCurrentRoutineSourceType;
	QString meCurrentRoutineSourceFile;
	QString meCurrentRoutineReturnType;
	vector <QString> meCurrentRoutineArgType;
	vector <QString> meCurrentRoutineArgValue;
	
	int meState;
	bool meDebug;
	double meResultFromFunction;
	bool meRunning;
	int meTimeOutFromCycle;
	bool meException;
	bool meQuit;
	
	QMutex * meLock;
	clClassLoader * meClassLoader;
	//PyMutex * meLock;
};
//! [0]

#endif
