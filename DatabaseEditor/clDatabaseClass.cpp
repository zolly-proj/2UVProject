#include "clDatabaseClass.h"

clDatabaseClass::clDatabaseClass(QString paUserName, QString paPassword, QString paDatabaseName, QString paHostAdress, QString paPort, QString paDatabaseTech)
{
    setUserName(paUserName);
    setPassword(paPassword);
    setDatabaseName(paDatabaseName);
    setHostAdress(paHostAdress);
    setPort(paPort);
    setDatabaseTech(paDatabaseTech);
	
	QString loInit = QString("");
    initializeDatabases(loInit);
}

clDatabaseClass::~clDatabaseClass()
{

}
/******************************
* Database operations
********************************/
//Initialize the database
bool clDatabaseClass::initializeDatabases(QString &paMessage)
{
    try
    {
        bool loConversion;
        int loTech;

        //Check the conversion of database technologie
        getDatabaseTech().toInt(&loConversion,10);
        if (loConversion)
        {
            //loTech = getDatabaseTech().toInt(&ok, 16);       // hex == 255, ok == true
            loTech = getDatabaseTech().toInt(&loConversion,10);
            setIDatabaseTech(loTech);

        }
        else
        {
            QString loTemp("clDatabaseClass::initializeDatabases(QString & paMessage) -> Cound not convert tech '" + getDatabaseTech() + "' to integer ...");
            paMessage = loTemp;
            return false;
        }

        switch(getIDatabaseTech())
        {
            case 1:
                    meDatabasePostgres = new clDatabasePostgres(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    meDatabaseMySql = NULL;
                    meDatabaseOracle = NULL;
                    break;
            case 2:
                    meDatabasePostgres = NULL;
                    meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    meDatabaseOracle = NULL;
                    break;
            case 3:
                    meDatabasePostgres = NULL;
                    meDatabaseMySql = NULL;
                    meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:

                    QString loTemp("clDatabaseClass::initializeDatabases(QString & paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }

        return true;
    }
    catch(...)
    {
        QString loTemp("clDatabaseClass::initializeDatabases(QString & paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Opening the database
bool clDatabaseClass::openDatabase(QString & paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:
                    return meDatabasePostgres->openDatabase(paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("clDatabaseClass::openDatabase(QString & paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("clDatabaseClass::openDatabase(QString & paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Closing the database
bool clDatabaseClass::closeDatabase(QString & paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:

                    return meDatabasePostgres->closeDatabase(paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("clDatabaseClass::closeDatabase(QString & paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("clDatabaseClass::closeDatabase(QString & paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Create a column
bool clDatabaseClass::createDatabaseColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, QString paReference, QString &paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:
                    return meDatabasePostgres->createDatabaseColumn(paTableName,paCollName,paType,paExtra,paReference,paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("clDatabaseClass::createDatabaseColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, bool paReference, QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("clDatabaseClass::createDatabaseColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, bool paReference, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Check if column exists
bool clDatabaseClass::checkDatabaseColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExists, bool &paTypeMatch, QString &paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:
                    return meDatabasePostgres->checkDatabaseColumnExist(paTableName,paCollName,paType,paExtra,paExists,paTypeMatch,paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("clDatabaseClass::checkDatabaseColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExists, bool &paTypeMatch, QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("clDatabaseClass::checkDatabaseColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExists, bool &paTypeMatch, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Create a table
bool clDatabaseClass::createDatabaseTable(QString paTableName, QString &paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:
                    return meDatabasePostgres->createDatabaseTable(paTableName, paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("clDatabaseClass::createDatabaseTable(QString paTableName, QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("clDatabaseClass::createDatabaseTable(QString paTableName, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Check if the table exists
bool clDatabaseClass::checkDatabaseTableExist(QString paTableName, bool &paExist, QString &paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:
                    meDatabasePostgres->checkDatabaseTableExist(paTableName,paExist,paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("clDatabaseClass::checkDatabaseTableExist(QString paTableName, bool &paExist, QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("clDatabaseClass::checkDatabaseTableExist(QString paTableName, bool &paExist, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Update a database column
bool clDatabaseClass::updateDatabaseColumn(QString paTableName, QString paCollName, QString paType, QString paExtra, QString paReference, QString &paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:
                    meDatabasePostgres->updateDatabaseColumn(paTableName,paCollName,paType,paExtra,paReference,paMessage);
                    //meDatabasePostgres->checkDatabaseTableExist(paTableName,paExist,paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("clDatabaseClass::updateDatbasaseColumn(QString paTableName, QString paCollName, QString paType, QSTrin paExtra, QString paReference, QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("clDatabaseClass::updateDatbasaseColumn(QString paTableName, QString paCollName, QString paType, QSTrin paExtra, QString paReference, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Drop a database column
bool clDatabaseClass::dropDatabaseColumn(QString paTableName, QString paCollName, QString & paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:
                    meDatabasePostgres->dropDatabaseColumn(paTableName,paCollName,paMessage);
                    //meDatabasePostgres->checkDatabaseTableExist(paTableName,paExist,paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("clDatabaseClass::dropDatabaseColumn(QString paTableName, QString paCollName, QString & paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("clDatabaseClass::dropDatabaseColumn(QString paTableName, QString paCollName, QString & paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Drop database table
bool clDatabaseClass::dropDatabaseTable(QString paTableName,QString &paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:
                    meDatabasePostgres->dropDatabaseTable(paTableName,paMessage);
                    //meDatabasePostgres->checkDatabaseTableExist(paTableName,paExist,paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("clDatabaseClass::dropDatabaseTable(QString paTableName,QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("clDatabaseClass::dropDatabaseTable(QString paTableName,QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }

}
bool clDatabaseClass::getDatabaseColumns(QString paTableName, vector <clDatabaseColumn> &paDatabaseColumns, QString &paMessage)
{

    try
    {

        switch(getIDatabaseTech())
        {
            case 1:
                    return meDatabasePostgres->getDatabaseColumns(paTableName, paDatabaseColumns, paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("vector <clDatabaseColumn> clDatabaseClass::getDatabaseColumns(QString paTableName, QString paCollName, QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("vector <clDatabaseColumn> clDatabaseClass::getDatabaseColumns(QString paTableName, QString paCollName, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }

}

bool clDatabaseClass::getDatabaseTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage)
{
    try
    {

        switch(getIDatabaseTech())
        {
            case 1:
                    return meDatabasePostgres->getDatabaseTables(paDatabaseTables, paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("bool clDatabaseClass::getDatabaseTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("bool clDatabaseClass::getDatabaseTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Insert into a table
bool clDatabaseClass::insertIntoTableDatabase(QString paTableName,vector <QString> paColumns, vector <QString> paValues,vector <QString> paTypeValue, QString &paMessage)
{
    try
    {

        switch(getIDatabaseTech())
        {
            case 1:
                    return meDatabasePostgres->insertIntoTableDatabase(paTableName, paColumns, paValues, paTypeValue, paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("bool clDatabaseClass::insertIntoTableDatabase(QString paTableName,vector <QString> paColumns, vector <QString> paValues, QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("bool clDatabaseClass::insertIntoTableDatabase(QString paTableName,vector <QString> paColumns, vector <QString> paValues, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Update into a table
bool clDatabaseClass::updateIntoTableDatabase(QString paTableName,QString paId, vector <QString> paColumns, vector <QString> paValues, vector<QString> paTypeValue, QString &paMessage)
{
    try
    {

        switch(getIDatabaseTech())
        {
            case 1:
                    return meDatabasePostgres->updateIntoTableDatabase(paTableName, paId, paColumns, paValues, paTypeValue, paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("bool clDatabaseClass::updateIntoTableDatabase(QString paTableName,QString paId, vector <QString> paColumns, vector <QString> paValues, QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("bool clDatabaseClass::updateIntoTableDatabase(QString paTableName,QString paId, vector <QString> paColumns, vector <QString> paValues, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Update into a table

bool clDatabaseClass::deleteIntoTableDatabase(QString paTableName,QString paId, QString &paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:
                    return meDatabasePostgres->deleteIntoTableDatabase(paTableName, paId, paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("bool clDatabaseClass::deleteIntoTableDatabase(QString paTableName,QString paId, QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("bool clDatabaseClass::deleteIntoTableDatabase(QString paTableName,QString paId, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Get From Database General
bool clDatabaseClass::getFromTableDatabaseGeneral(QString paTableName,QString paStartValue, QString paMaxValue, vector <QString> &paReturnId, QString &paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:
                    return meDatabasePostgres->getFromTableDatabaseGeneral( paTableName, paStartValue, paMaxValue, paReturnId, paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("bool clDatabaseClass::getFromTableDatabaseGeneral(QString paTableName,QString paStartValue, QString paMaxValue, QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("bool clDatabaseClass::getFromTableDatabaseGeneral(QString paTableName,QString paStartValue, QString paMaxValue, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Get From Database by property
bool clDatabaseClass::getFromTableDatabaseByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue, vector <QString> paLogExp, vector <QString> &paReturnId,QString &paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:
                    return meDatabasePostgres->getFromTableDatabaseByProperty( paTableName, paStartValue, paMaxValue, paProperties, paValue, paTypeValue, paLogExp, paReturnId, paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("bool clDatabaseClass::getFromTableDatabaseByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> loTypeValue,vector <QString> paLogExp, vector <QString> &paReturnId,QString &paMessage) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("bool clDatabaseClass::getFromTableDatabaseByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> loTypeValue,vector <QString> paLogExp, vector <QString> &paReturnId,QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Get From Database by id
bool clDatabaseClass::getFromTableDatabaseById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue, QString &paMessage)
{
    try
    {
        switch(getIDatabaseTech())
        {
            case 1:
                    return meDatabasePostgres->getFromTableDatabaseById( paTableName, paId, paProperties, paReturnValue, paMessage);
                    break;
            case 2:
                    //meDatabaseMySql = new clDatabaseMySql(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            case 3:
                    //meDatabaseOracle = new clDatabaseOracle(getUserName(),getPassword(),getDataseName(),getHostAdress(),getPort());
                    break;
            default:
                    QString loTemp("bool clDatabaseClass::getFromTableDatabaseById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue) -> Tech '" + getDatabaseTech() + "' not supported ...");
                    paMessage = loTemp;
                    return false;
                    break;
        }
        return true;
    }
    catch(...)
    {
        QString loTemp("bool clDatabaseClass::getFromTableDatabaseById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue) -> error ...");
        paMessage = loTemp;
        return false;
    }
}

/******************************
* Getters and setters
*******************************/
void clDatabaseClass::setUserName(QString paUserName){meUserName = paUserName;}
QString clDatabaseClass::getUserName(){return meUserName;}

void clDatabaseClass::setPassword(QString paPassword){mePassword = paPassword;}
QString clDatabaseClass::getPassword(){return mePassword;}

void clDatabaseClass::setDatabaseName(QString paDatabaseName){meDatabaseName = paDatabaseName;}
QString clDatabaseClass::getDataseName(){return meDatabaseName;}

void clDatabaseClass::setHostAdress(QString paHostAdress){meHostAdress = paHostAdress;}
QString clDatabaseClass::getHostAdress(){return meHostAdress;}

void clDatabaseClass::setPort(QString paPort){mePort = paPort;}
QString clDatabaseClass::getPort(){return mePort;}

void clDatabaseClass::setDatabaseTech(QString paDatabaseTech){meDatabaseTech = paDatabaseTech;}
QString clDatabaseClass::getDatabaseTech(){return meDatabaseTech;}

void clDatabaseClass::setIDatabaseTech(int paTech){meTech = paTech;}
int clDatabaseClass::getIDatabaseTech(){return meTech;}

