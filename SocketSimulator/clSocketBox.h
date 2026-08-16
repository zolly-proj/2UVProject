#ifndef CLSOCKETBOX_H
#define CLSOCKETBOX_H

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

#include "clSocketBoxThread.h"

using namespace std;

class clSocketBox : public QTcpServer
{
    Q_OBJECT;
public:
    explicit clSocketBox(QString paConfigFile,QObject * parent = 0);
    ~clSocketBox ();
	
	void StartServer();
	
protected:
	void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;
signals:

public slots:

//protected:
    //void incomingConnection(int socketDescriptor);
	
private:
    void readConfigFile(QString paConfigFile);
	bool getAttributeFromConfigFileGeneral(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage);
	bool getDataFromConfigFile(QString paConfigFile,QString paElement,vector <QString> & paData, QString & paReturnMessage);

	QString meIp;
	QString meName;
	QString mePort;
	QString meError;
	vector <QString> meData;
	
	clSocketBoxThread * meThread;
};

#endif
