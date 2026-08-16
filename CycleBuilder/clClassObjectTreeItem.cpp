#include "clClassObjectTreeItem.h"
void clClassObjectTreeItem::setClassName(QString paName)
{
    meClassName = paName;
}
void clClassObjectTreeItem::setDatabaseActions(vector<clDatabaseAction> paDatabaseActions)
{
    meDatabaseActions = paDatabaseActions;
}
void clClassObjectTreeItem::setDatabaseColumns(vector<clDatabaseColumn> paDatabaseColumns)
{
    meDatabaseColumns = paDatabaseColumns;
}
vector<clDatabaseColumn> clClassObjectTreeItem::getDatabaseColumns()
{
    return meDatabaseColumns;
}
vector<clDatabaseAction> clClassObjectTreeItem::getDatabaseActions()
{
    return meDatabaseActions;
}
QString clClassObjectTreeItem::getClassName()
{
    return meClassName;
}
