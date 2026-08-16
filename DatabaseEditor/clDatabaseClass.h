#ifndef CLDATABASECLASS_H_WVD_17052011
#define CLDATABASECLASS_H_WVD_17052011

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>

#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"
#include "clDatabaseTable.h"

#include "clDatabasePostgres.h"
#include "clDatabaseMySql.h"
#include "clDatabaseOracle.h"

using namespace std;


class clDatabaseClass
{

public:
    clDatabaseClass (QString paUserName, QString paPassword, QString paDatabaseName, QString paHostAdress, QString paPort, QString loDatabaseTech);
    ~clDatabaseClass ();
public:
    //public database functions
    bool openDatabase(QString &paMessage);
    bool closeDatabase(QString &paMessage);


    bool createDatabaseTable(QString paTableName, QString &paMessage);
    bool checkDatabaseTableExist( QString paTableName, bool &paExist, QString &paMessage);
    bool getDatabaseTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage);
    bool dropDatabaseTable(QString paTableName, QString &paMessage);

    bool createDatabaseColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, QString paReference, QString &paMessage);
    bool checkDatabaseColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExists, bool &paTypeMatch, QString &paMessage);
    bool updateDatabaseColumn(QString paTableName, QString paCollName, QString paType, QString paExtra, QString paReference, QString &paMessage);
    bool dropDatabaseColumn(QString paTableName, QString paCollName, QString &paMessage);
    bool getDatabaseColumns(QString paTableName, vector <clDatabaseColumn> &paDatabaseColumns, QString &paMessage);

    bool insertIntoTableDatabase(QString paTableName,vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paMessage);
    bool updateIntoTableDatabase(QString paTableName,QString paId, vector <QString> paColumns, vector <QString> paValues, vector<QString> paTypeValue, QString &paMessage);
    bool deleteIntoTableDatabase(QString paTableName,QString paId, QString &paMessage);
    bool getFromTableDatabaseGeneral(QString paTableName,QString paStartValue, QString paMaxValue,vector <QString> &paReturnId, QString &paMessage);
    bool getFromTableDatabaseByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue,vector <QString> paTypeValue, vector <QString> paLogExp, vector <QString> &paReturnId, QString &paMessage);
    bool getFromTableDatabaseById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue, QString &paMessage);

    //Getters
    QString getUserName();
    QString getPassword();
    QString getDataseName();
    QString getHostAdress();
    QString getPort();
    QString getDatabaseTech();
    int getIDatabaseTech();

private:

    //Setters
    void setUserName(QString paUserName);
    void setPassword(QString paPassword);
    void setDatabaseName(QString paDatabaseName);
    void setHostAdress(QString paHostAdress);
    void setPort(QString paPort);
    void setDatabaseTech(QString paDatabaseTech);
    void setIDatabaseTech(int paTech);

    //Database functions
    bool initializeDatabases(QString &paMessage);

    //Database variables
    QString meUserName;
    QString mePassword;
    QString meDatabaseName;
    QString meHostAdress;
    QString mePort;
    QString meDatabaseTech;

    //Local variables
    int meTech;

    //Database classes
    clDatabaseMySql * meDatabaseMySql;
    clDatabaseOracle * meDatabaseOracle;
    clDatabasePostgres * meDatabasePostgres;
};

#endif
