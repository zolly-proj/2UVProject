#ifndef CLLOGSERVER_H_WVD_17052011
#define CLLOGSERVER_H_WVD_17052011

#include <Ice/Ice.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/CtrlCHandler.h>

#include <vector>
#include <iostream>
/* #include <windows.h> */
#include <string>



#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QThread>

#include "clLogServer_impl.h"
#include "clFileWriter.h"





using namespace std;

class clLogServer : public QObject
{

    Q_OBJECT;

public:
    clLogServer (QString paConfigFile,QObject * parent = 0);
    ~clLogServer ();
public:

    QString mePath;
    QString meSize;
    QString meDiskUsage;

    void doSetup(QThread &paThread);
    void doShutDown();

public slots:
    void doWork();
private:
    //Local functions
    void readConfigFile(QString paConfigFile);
    bool getAttributeFromConfigFile(QString paConfigFile,QString paAttribute, QString & paValue, QString & paReturnMessage);

    QThread meThread;
    //Variables
    Ice::CommunicatorPtr meCommunicationPtr;


    //Getters and setters
    void setPath(QString paPath);
    QString getPath();
    void setSize(QString paSize);
    QString getSize();
    void setDiskUsage(QString paDiskUsage);
    QString getDiskUsage();
};

#endif
