#ifndef _CLERRORTABLE_H
#define _CLERRORTABLE_H


#include <QtCore/QString>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>
#include <QtCore/QRandomGenerator>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>

#include <vector>
#include <iostream>
#include <string>

#include "clDatabaseColumn.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clObjectCall.h"
#include "clMethodCall.h"

using namespace std;

class clErrorTable : public clObjectCall
{
public:
    clErrorTable ();
    ~clErrorTable ();
	
public:
	bool createPluginClass(clIceClientServer * paIceClientServer,clIceClientLogging  * paIceClientLogging) override;
	bool createClassMethodsCall(vector <QString> paClassNames,vector <vector<QString>> paClassMethods,vector <clObjectCall *> paObjectCall) override;
	bool createGeneralMethodsCall(vector <QString> paMethodsNameList,vector <clMethodCall *> paMethodCallGeneral) override;	
	bool createDatabaseColumnsByClassNameList(vector <vector<clDatabaseColumn*>> paDatabaseColumnsByClassNameList) override;	
	int GetReturnParameters() override;
	bool doMethod(QString paMethodName, const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue, const vector <QString> &paLogExp) override;
private:

	bool getTextFromErrorNumber(const vector <QString> &paParametersType, const vector <QString> &paParameters);
	bool getErrorNumberFromText(const vector <QString> &paParametersType, const vector <QString> &paParameters);
	bool initializeErrorSearch(QString paErrorTable, vector <QString> &paErrorNumber, vector <QString> &paErrorText, vector <QString> &paMethod);
	bool findErrorInText(QString paError, vector <QString> &paErrorNumber, vector <QString> &paErrorText, vector <QString> &paMethod);
	bool findTextInError(QString paError, vector <QString> &paErrorNumber, vector <QString> &paErrorText, vector <QString> &paMethod);
	
	vector <QString> meParametersType;
	vector <QString> meParameters;
	clIceClientLogging * meIceClientLogging;
	clIceClientServer * meIceClientServer;	
	
	//Loaded dll's to call the methods
	vector <QString> meClassNames;
	vector <vector<QString>> meClassMethods;
	vector <clObjectCall *> meObjectCall;
	vector <vector<clDatabaseColumn *>> meDatabaseColumnsByClassNameList;
	vector <QString> meMethodsNameList;
	vector <clMethodCall *> meMethodCallGeneral;	
	
};
#endif
extern "C" clObjectCall* CreateModuleObject()
{
    // call the constructor of the actual implementation
    clObjectCall * module = new clErrorTable();
    // return the created function
    return module;
}
