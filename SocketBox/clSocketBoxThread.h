#ifndef CLSOCKETBOXTHREAD_H
#define CLSOCKETBOXTHREAD_H

#include <vector>
#include <iostream>
#include <string>
#include <dlfcn.h>

#include <QtCore/QObject>
#include <QtCore/QDebug>
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
#include <QtCore/QTextCodec>

#include "clDriverFrameHeader.h"
#include "clDriverFrame.h"
using namespace std;

class clSocketBoxThread : public QThread
{
    Q_OBJECT;
public:
    explicit clSocketBoxThread(int iID,	vector <QString> *paData, QObject *parent = 0);
    ~clSocketBoxThread ();
	void run();
	QString convertDataFromSocket(QByteArray paByteArray);
signals:
    void error(QTcpSocket::SocketError socketerror);
    
public slots:
    void readyReadSocket();
    void disconnectedSocket();
	
private:
    QTcpSocket *meSocket;
    int meSocketDescriptor;
	//clDriverFrameImplementation *meDriverFrame;
	vector <QString> *meDriverData;
		
	void* meLibraryLib;	
	clDriverFrame* meDriverFrame;	
};
#endif
