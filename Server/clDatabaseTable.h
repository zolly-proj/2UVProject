#ifndef CLDATABASETABLE_H_WVD_17052011
#define CLDATABASETABLE_H_WVD_17052011

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>
#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"

using namespace std;


class clDatabaseTable
{

public:
    clDatabaseTable (QString paTableName, vector <clDatabaseColumn> paDatabaseColumn, vector <clDatabaseAction> paDatabaseAction);
    ~clDatabaseTable ();
public:
    //public vars
    vector <clDatabaseColumn> meDatabaseColumns;
    vector <clDatabaseAction> meDatabaseActions;

    //public functions

    //Getters and setters
    vector <clDatabaseColumn> getDatabaseColumns();
    vector <clDatabaseAction> getDatabaseActions();
    QString getTableName();

private:

    //local vars
    QString meTableName;

    //getters and setters
    void setTableName(QString paTableName);
    void setDatabaseColumns(vector <clDatabaseColumn> paDatabaseColumns);
    void setDatabaseActions(vector <clDatabaseAction> paDatabaseActions);
};

#endif
