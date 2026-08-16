#ifndef CLWORKSTATIONCYCLES_H
#define CLWORKSTATIONCYCLES_H

#undef slots
#include <Python.h>
#define slots Q_SLOTS


#include <exception>
#include <string>
#include <iostream>

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtCore/QDateTime>


#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clWorkstationCycle.h"

//! [0]
class clWorkstationCycles : public QObject
{
	Q_OBJECT	
public:
    clWorkstationCycles(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QMutex * paLock, clClassLoader * paClassLoader);
    ~clWorkstationCycles();

	clWorkstationCycle * meWorkstationCycle[20];
public slots:
    void handleResults(const QString &);	
signals:
    void operate(const QString &);	
private:
	bool getWorkstationCycles();

	clIceClientLogging * meIceClientLogging;
	clIceClientServer * meIceClientServer;
	
	
	QMutex * meLock;

	clClassLoader * meClassLoader;
	//PyThreadState* tstate;

};
//! [0]

#endif
