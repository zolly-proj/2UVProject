#include "clDatabaseMySql.h"

clDatabaseMySql::clDatabaseMySql(QString paUserName, QString paPassword, QString paDatabaseName, QString paHostAdress, QString paPort)
{
    setUserName(paUserName);
    setPassword(paPassword);
    setDatabaseName(paDatabaseName);
    setHostAdress(paHostAdress);
    setPort(paPort);
}

clDatabaseMySql::~clDatabaseMySql()
{

}
/******************************
* Database operations
********************************/
bool clDatabaseMySql::openDatabase(QString & paMessage)
{
    try
    {
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool clDatabaseMySql::closeDatabase(QString & paMessage)
{
    try
    {
        return true;
    }
    catch(...)
    {
        return false;
    }
}


/******************************
* Getters and setters
*******************************/
void clDatabaseMySql::setUserName(QString paUserName){meUserName = paUserName;}
QString clDatabaseMySql::getUserName(){return meUserName;}

void clDatabaseMySql::setPassword(QString paPassword){mePassword = paPassword;}
QString clDatabaseMySql::getPassword(){return mePassword;}

void clDatabaseMySql::setDatabaseName(QString paDatabaseName){meDatabaseName = paDatabaseName;}
QString clDatabaseMySql::getDataseName(){return meDatabaseName;}

void clDatabaseMySql::setHostAdress(QString paHostAdress){meHostAdress = paHostAdress;}
QString clDatabaseMySql::getHostAdress(){return meHostAdress;}

void clDatabaseMySql::setPort(QString paPort){mePort = paPort;}
QString clDatabaseMySql::getPort(){return mePort;}
