#include "clDatabaseTable.h"

clDatabaseTable::clDatabaseTable(QString paTableName, vector <clDatabaseColumn> paDatabaseColumn, vector <clDatabaseAction> paDatabaseAction)
{
        setTableName(paTableName);
        setDatabaseActions(paDatabaseAction);
        setDatabaseColumns(paDatabaseColumn);
}

clDatabaseTable::~clDatabaseTable()
{

}

//Getters and setters
void clDatabaseTable::setTableName(QString paTableName){ meTableName = paTableName; }
QString clDatabaseTable::getTableName(){ return meTableName; }


//Getters
vector <clDatabaseColumn> clDatabaseTable::getDatabaseColumns()
{
    return meDatabaseColumns;
}
vector <clDatabaseAction> clDatabaseTable::getDatabaseActions()
{
    return meDatabaseActions;
}
void clDatabaseTable::setDatabaseColumns(vector <clDatabaseColumn> paDatabaseColumns)
{
    meDatabaseColumns = paDatabaseColumns;
}
void clDatabaseTable::setDatabaseActions(vector <clDatabaseAction> paDatabaseActions)
{
    meDatabaseActions = paDatabaseActions;
}
