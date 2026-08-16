#ifndef CLSOCKETBOXCLIENT_H
#define CLSOCKETBOXCLIENT_H

#include <vector>
#include <iostream>
#include <string>

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>


#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDataStream>


using namespace std;

class clSocketBoxClient : public QObject
{
	Q_OBJECT;
public:
    clSocketBoxClient(QString paIp,QString paPort, QObject * parent = 0);
    ~clSocketBoxClient ();
	
	void sendToServer();
public slots:
	void readTcpData();
private:
	QString meIp;
	QString mePort;
	
	QTcpSocket *pSocket;
	
	
	QByteArray meData;
};

#endif
