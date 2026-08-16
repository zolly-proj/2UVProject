#ifndef CLDATABASEPOSTGRES_H_WVD_18052011
#define CLDATABASEPOSTGRES_H_WVD_18052011

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QByteArray>

#include "libpq-fe.h"

#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"
#include "clDatabaseTable.h"



using namespace std;


class clDatabasePostgres
{

public:
    clDatabasePostgres (QString paUserName, QString paPassword, QString paDatabaseName, QString paHostAdress, QString paPort);
    ~clDatabasePostgres ();
public:

    bool openDatabase(QString &paMessage);
    bool closeDatabase(QString &paMessage);

    bool checkDatabaseColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExists, bool &paTypeMatch, QString &paMessage);
    bool checkDatabaseTableExist(QString paTableName, bool &paExist, QString &paMessage);

    bool createDatabaseTable(QString paTableName, QString &paMessage);

    bool dropDatabaseTable(QString paTableName, QString &paMessage);

    bool createDatabaseColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, QString paReference, QString &paMessage);
    //bool checkDatabaseColumnExists(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExists, bool &paTypeMatch, QString &paMessage);
    bool updateDatabaseColumn(QString paTableName, QString paCollName, QString paType, QString paExtra, QString paReference, QString &paMessage);
    bool dropDatabaseColumn(QString paTableName, QString paCollName, QString &paMessage);

    bool getDatabaseColumns(QString paTableName,vector <clDatabaseColumn> &paDatabaseColumns, QString &paMessage);

    bool getDatabaseTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage);

    bool insertIntoTableDatabase(QString paTableName,vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paMessage);
    bool updateIntoTableDatabase(QString paTableName,QString paId, vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paMessage);
    bool deleteIntoTableDatabase(QString paTableName,QString paId, QString &paMessage);

    bool getFromTableDatabaseGeneral(QString paTableName,QString paStartValue, QString paMaxValue, vector <QString> &paReturnId, QString &paMessage);
    bool getFromTableDatabaseByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue,vector <QString> paLogExp, vector <QString> &paReturnId, QString &paMessage);
    bool getFromTableDatabaseById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue, QString &paMessage);

private:

    //Getters and setters
    void setUserName(QString paUserName);
    QString getUserName();
    void setPassword(QString paPassword);
    QString getPassword();
    void setDatabaseName(QString paDatabaseName);
    QString getDataseName();
    void setHostAdress(QString paHostAdress);
    QString getHostAdress();
    void setPort(QString paPort);
    QString getPort();
    void setDatabaseTech(QString paDatabaseTech);
    QString getDatabaseTech();

    //Private local functions
    bool convertTypeName( QString paType, QString paUdt_Name, QString paLength_char, QString paXMLType, QString &paMessage);
    bool buildConditions(vector <QString> paProperties,vector <QString> paValue,vector <QString> paTypeValue,vector <QString> paLogExp,QString &loConditions);
    bool getColumnsString(vector <QString> paColumns, QString &paResult);
    bool getValuesString(vector <QString> paValues, vector <QString> paTypeValue, QString &paResult);
    bool getValueColumnsString(vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paResult);



    //private database functions
    bool CloseConn(QString &paMessage);
    bool ConnectDB(QString &paMessage);

    bool checkTableExist(QString paTableName, bool &paExist, QString &paMessage);
    bool createTable(QString paTableName, QString &paMessage);

    bool dropTable(QString paTableName, QString &paMessage);
    bool getTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage);

    bool getColumns(QString paTableName, vector <clDatabaseColumn> &paDatabaseColumns, QString &paMessage);



    bool checkColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExist, bool &paTypeMatch, QString &paMessage);
    bool createColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, QString paReference, QString &paMessage);
    bool updateColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, QString paReference, QString &paMessage);
    bool dropColumn(QString paTableName, QString paCollName, QString &paMessage);

    bool insertIntoTable(QString paTableName,vector <QString> paColumns, vector <QString> paValues,vector <QString> paTypeValues, QString &paMessage);
    bool updateIntoTable(QString paTableName,QString paId, vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paMessage);
    bool deleteIntoTable(QString paTableName,QString paId, QString &paMessage);

    bool getFromTableGeneral(QString paTableName,QString paStartValue, QString paMaxValue, vector <QString> &paReturnId, QString &paMessage);
    bool getFromTableByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue,vector <QString> paLogExp, vector <QString> &paReturnId,QString &paMessage);
    bool getFromTableById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue, QString &paMessage);


    //private database variables
    QString meUserName;
    QString mePassword;
    QString meDatabaseName;
    QString meHostAdress;
    QString mePort;
    PGconn * meDatabaseConnection;

};

#endif
