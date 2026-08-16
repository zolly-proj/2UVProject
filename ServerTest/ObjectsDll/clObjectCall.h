#ifndef _CLOBJECTCALL_H
#define _CLOBJECTCALL_H


#ifdef CLOBJECTCALL_EXPORTS
#define CLOBJECTCALL_API __declspec(dllexport)
#else
#define CLOBJECTCALL_API __declspec(dllimport)
#endif

#include <QtCore/QString>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>
#include <QtCore/QDateTime>
#include <QtNetwork/QHostInfo>

#include <vector>
#include <iostream>
#include <string>

#ifdef WIN32
#include <windows.h>
#elif defined(_WIN32)
#include <windows.h>
#elif defined(__WIN32)
#include <windows.h>
#elif defined(__WIN32__)
#include <windows.h>
#else

#endif

#include "clDatabaseColumn.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clMethodCall.h"

using namespace std;

class clObjectCall
{
public:
    clObjectCall ();
    virtual ~clObjectCall ();
public:
	virtual bool createPluginClass(clIceClientServer * paIceClientServer,clIceClientLogging * paIceClientLogging) = 0;
	virtual bool doMethod(QString paMethodName, const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue, const vector <QString> &paLogExp) = 0;
	virtual int GetReturnParameters() = 0;
	virtual bool createClassMethodsCall(vector <QString> paClassNames,vector <vector<QString>> paClassMethods,vector <clObjectCall *> paObjectCall) = 0;
	virtual bool createGeneralMethodsCall(vector <QString> paMethodsNameList,vector <clMethodCall *> paMethodCallGeneral) = 0;	
	virtual bool createDatabaseColumnsByClassNameList(vector <vector<clDatabaseColumn*>> paDatabaseColumnsByClassNameList) = 0;	
	

	
	QString ClassName;
	QString ObjectId;
	bool Initialized;
	
	vector <QString> Properties;
	vector <QString> PropertiesTypes;
	
	vector <QString> meReturnParameters;
	vector <QString> meReturnParametersValue;
	vector <QString> meReturnParametersType;

};
#endif
