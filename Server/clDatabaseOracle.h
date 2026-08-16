#ifndef CLDATABASEORACLE_H_WVD_18052011
#define CLDATABASEORACLE_H_WVD_18052011

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>


using namespace std;


class clDatabaseOracle
{

public:
    clDatabaseOracle (QString paUserName, QString paPassword, QString paDatabaseName, QString paHostAdress, QString paPort);
    ~clDatabaseOracle ();
public:

    bool openDatabase(QString &paMessage);
    bool closeDatabase(QString &paMessage);




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

    //Database variables
    QString meUserName;
    QString mePassword;
    QString meDatabaseName;
    QString meHostAdress;
    QString mePort;
};

#endif
