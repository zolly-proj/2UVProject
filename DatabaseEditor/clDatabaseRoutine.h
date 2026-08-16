#ifndef CLDATABASEROUTINE_H_WVD_03122023
#define CLDATABASEROUTINE_H_WVD_03122023

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>


using namespace std;


class clDatabaseRoutine
{

public:
    clDatabaseRoutine(QString paName, QString paAlias, QString paSource, QString paSourceName, QString paReturnType, vector <QString> Objects);
    ~clDatabaseRoutine();
public:
    //public database functions
    QString getName();
    QString getAlias();
    QString getSource();
    QString getSourceName();
    QString getReturnType();
	vector <QString> getObjects(); 
private:
    QString meName;
    QString meAlias;
    QString meSource;
    QString meSourceName;
    QString meReturnType;
	vector <QString> meObjects;


    void setName(QString paName);
    void setAlias(QString paAlias);
    void setSource(QString paSource);
    void setSourceName(QString paSourceName);
    void setReturnType(QString paReturnType);
	void setObjects(vector <QString> paObjects);
};

#endif
