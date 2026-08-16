#ifndef LIVINGOBJECT_H
#define LIVINGOBJECT_H

#include <exception>
#include <string>
#include <iostream>

#include <QtCore/QThread>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QMutex>


#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clLivingObjectMach.h"
#include "clLivingObjectLocator.h"
#include "clLivingObjectMeasure.h"
#include "clClassLoader.h"

//! [0]
class clLivingObject
{

public:
    clLivingObject(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QMutex * paLock, clClassLoader * paClassLoader);
    ~clLivingObject();



private:
	bool getLivingObjectsForThisWorkstation();

	clIceClientLogging * meIceClientLogging;
	clIceClientServer * meIceClientServer;
	clLivingObjectMach * meLivingObjectMach[200];
	clLivingObjectLocator * meLivingObjectLocator[200];
	clLivingObjectMeasure * meLivingObjectMeas[200];
	QThread * meThread[200];
	QMutex * meLock;
	
	clClassLoader * meClassLoader;
};
//! [0]

#endif
