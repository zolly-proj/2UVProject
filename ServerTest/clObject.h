#ifndef CLOBJECT_H
#define CLOBJECT_H

#include <vector>
#include <iostream>
#include <string>

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtNetwork/QHostInfo>

#include "clDatabaseColumn.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clObjectCall.h"
//#include "clObjectCallHeader.h"
#include "clMethodCall.h"
//#include "clMethodCallHeader.h"

class clObject
{

public:
    clObject(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging);
    clObject(clObject * paObject);
    ~clObject();
	
private:
	vector <QString> getReferencesFromTableInformation(QString paClassName,QString paPropertyName);
	bool createObject(QString paClassName, QString paObjectId, clObject &paObject);
	bool getTypeFromTableInformation(QString paClassName,QString paPropertyName, QString &paPropertyType);
	
	

public:

	bool get(QString paPropName, QString &paValue);
	bool get(QString paPropName, vector <QString> &paValue);
	bool get(QString paPropName, QDateTime &paValue);
	bool get(QString paPropName, vector <QDateTime> &paValue);
	bool get(QString paPropName, int &paValue);
	bool get(QString paPropName, vector <int> &paValue);	
	bool get(QString paPropName, float &paValue);
	bool get(QString paPropName, vector <float> &paValue);
	bool get(QString paPropName, double &paValue);
	bool get(QString paPropName, vector <double> &paValue);
	bool get(QString paPropName, clObject &paValue);
	bool get(QString paPropName, vector <clObject> &paValue);
	bool getParents(QString paTableName, QString paPropName, vector <clObject> &paValue);
	
	bool set(QString paPropName, QString paValue);
	bool set(QString paPropName, vector <QString> paValue);
	bool set(QString paPropName, QDateTime paValue);
	bool set(QString paPropName, vector <QDateTime> paValue);
	bool set(QString paPropName, float paValue);
	bool set(QString paPropName, vector <float> paValue);
	bool set(QString paPropName, int paValue);
	bool set(QString paPropName, vector <int> paValue);	
	bool set(QString paPropName, double paValue);
	bool set(QString paPropName, vector <double> paValue);
	bool set(QString paPropName, clObject &paValue);
	bool set(QString paPropName, vector <clObject> &paValue);
	
	bool deleteObject();
	bool insertObject(vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValues);

	//???//
	bool queryObjects(vector <QString> paProperties, vector <QString> paValue, vector <QString> paValueType, vector <QString> paLogExp, vector <clObject> &paObjects);
	bool queryAll(int paIndexFrom, int paIndexTo, vector <clObject> &paObjects);
	
	bool doMethod(QString paMethodName, const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue, const vector <QString> &paLogExp);
	int getParameters();


	clIceClientLogging * meIceClientLogging;
	clIceClientServer * meIceClientServer;

	QString ClassName;
	QString ObjectId;
	QString ObjectName;

	vector <clObjectCall *> meObjectCalls;
	vector <QString> meClassNameList;
	vector <vector<QString>> meClassMethodsList;

	vector <clMethodCall *> meMethodCalls;
	vector <QString> meMethodNamesList;

	vector <vector<clDatabaseColumn *>> meDatabaseColumnsByClassNameList;
	


	
	
	
};
//! [0]

#endif
