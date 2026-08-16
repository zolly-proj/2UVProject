#include "clDatabaseOracle.h"

clDatabaseOracle::clDatabaseOracle(QString paUserName, QString paPassword, QString paDatabaseName, QString paHostAdress, QString paPort)
{
    setUserName(paUserName);
    setPassword(paPassword);
    setDatabaseName(paDatabaseName);
    setHostAdress(paHostAdress);
    setPort(paPort);
}

clDatabaseOracle::~clDatabaseOracle()
{

}
/******************************
* Database operations
********************************/
//Opening the database
bool clDatabaseOracle::openDatabase(QString & paMessage)
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
//Closing the database
bool clDatabaseOracle::closeDatabase(QString & paMessage)
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
void clDatabaseOracle::setUserName(QString paUserName){meUserName = paUserName;}
QString clDatabaseOracle::getUserName(){return meUserName;}

void clDatabaseOracle::setPassword(QString paPassword){mePassword = paPassword;}
QString clDatabaseOracle::getPassword(){return mePassword;}

void clDatabaseOracle::setDatabaseName(QString paDatabaseName){meDatabaseName = paDatabaseName;}
QString clDatabaseOracle::getDataseName(){return meDatabaseName;}

void clDatabaseOracle::setHostAdress(QString paHostAdress){meHostAdress = paHostAdress;}
QString clDatabaseOracle::getHostAdress(){return meHostAdress;}

void clDatabaseOracle::setPort(QString paPort){mePort = paPort;}
QString clDatabaseOracle::getPort(){return mePort;}
