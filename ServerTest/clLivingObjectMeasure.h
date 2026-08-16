#ifndef CLLIVINGOBJECTMEASURE_H
#define CLLIVINGOBJECTMEASURE_H

#include <exception>
#include <string>
#include <iostream>
#include <dlfcn.h>

#include <QtCore/QThread>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>
#include <QtCore/QRandomGenerator>
#include <QtCore/QMutex>

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>

#include "clDatabaseColumn.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clObjectCall.h"
#include "clObjectCallHeader.h"
#include "clObject.h"
#include "clClassLoader.h"

//! [0]
class clLivingObjectMeasure : public QObject
{
    Q_OBJECT;
public:
    clLivingObjectMeasure(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QString paObjectId, QMutex * paLock, clClassLoader  * paClassLoader, QObject * parent = 0);
    ~clLivingObjectMeasure();

public slots:
	void slotDoIt();
	void readTcpData();

private:
	bool getLivingObjectMeasureProperties();
	bool createCommand(QString paCommand, QString &paSocketCommand);
	clObjectCall* callObjectDLL(QString paLibName);
	
	clIceClientLogging * meIceClientLogging;
	clIceClientServer * meIceClientServer;
	QString meObjectId;
	QString meObjectName;
	
	QString meTimerCycle = "1000";
	QString meSocketPort;
	QString meSocketIp;
	QTimer* meTimer;
	
	vector<clDatabaseColumn> meDatabaseColumnArr;
	
	QTcpSocket *meSocket;
	QByteArray meData;
	
	clClassLoader * meClassLoader;
	QMutex * meLock;
};
//! [0]

#endif
