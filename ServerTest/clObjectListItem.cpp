#include "clObjectListItem.h"

void clObjectListItem::setName(QString paName)
{
    meName = paName;
}
void clObjectListItem::setAlias(QString paAlias)
{
    meAlias = paAlias;
}
void clObjectListItem::setType(QString paType)
{
    meType = paType;
}
void clObjectListItem::setExtra(QString paExtra)
{
    meExtra = paExtra;
}
void clObjectListItem::setReference(QString paReference)
{
    meReference = paReference;
}
void clObjectListItem::setValue(QString paValue)
{
    meValue = paValue;
}
void clObjectListItem::setTableName(QString paTableName)
{
    meTableName = paTableName;
}
void clObjectListItem::setId(QString paId)
{
        meId = paId;
}
QString clObjectListItem::getName(){return meName;}
QString clObjectListItem::getAlias(){return meAlias;}
QString clObjectListItem::getType(){return meType;}
QString clObjectListItem::getExtra(){return meExtra;}
QString clObjectListItem::getReference(){return meReference;}
QString clObjectListItem::getValue(){return meValue;}
QString clObjectListItem::getTableName(){return meTableName;}
QString clObjectListItem::getId(){return meId;}
