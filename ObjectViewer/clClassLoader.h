#ifndef CLCLASSLOADER_H
#define CLCLASSLOADER_H

#include <vector>
#include <iostream>
#include <string>
#include <dlfcn.h>

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QFile>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtNetwork/QHostInfo>

#include "clDatabaseColumn.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clObjectCall.h"
#include "clObjectCallHeader.h"
#include "clMethodCall.h"
#include "clMethodCallHeader.h"
#include "clObject.h"
//#include "clDLLoader.h"
//#include "clIDLLoader.h"

using namespace std;

//! [0]
class clClassLoader : public QObject
{
	Q_OBJECT	
public:
    clClassLoader(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging);
    ~clClassLoader();
public slots:
    
signals:
    
private:
	bool initializeLibrarys();
	
	bool readClassesFromXmlFile();
	bool readMethodsFromXmlFileGeneral();
	
	bool loadClasses();
	bool loadMethodsGeneral();
	
	bool setClassesDllVariables();
	bool setMethodsDllVariables();	
	
	clObjectCall * callObjectDLL(QString paCurrentMethodSourceFile, int paLibNumber);
	clMethodCall * callMethodDLLGeneral(QString paCurrentMethodSourceFile, int paLibNumber);
	//clObjectCall clClassLoader::setClassName(dlloader::DLLoader& dlloader,QString paClassName);

	bool cleanObjectDLL(int paLibNumber);
	bool cleanObjectDLLGeneral(int paLibNumber);

	clIceClientLogging * meIceClientLogging;
	clIceClientServer * meIceClientServer;	
	



public:	
	vector <QString> meClassNames;
	vector <QString> meClassDllNames;
	vector <vector<QString>> meClassMethods;
	vector<vector<clDatabaseColumn *>> meDatabaseColumnsByClassName;
	vector <QString> meClassPaths;

	vector <QString> meMethodsGeneralNames;
	vector <QString> meClassDllNamesGeneral;
	vector <QString> meClassPathsGeneral;

	clObject getObject(QString meClassName);

	vector <void *> meLibraryLib;
	vector <void *> meLibraryLibGeneral;

	vector <clObjectCall *> meObjectCalls;
	vector <clMethodCall *> meMethodCalls;
};
//! [0]

#endif
