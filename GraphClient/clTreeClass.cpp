#include "clTreeClass.h"

clTreeClass::clTreeClass(const QList<QVariant> &data, clTreeClass *parent)
{
    parentItem = parent;
    itemData = data;
}

clTreeClass::~clTreeClass()
{
    qDeleteAll(childItems);
}

void clTreeClass::appendChild(clTreeClass *item)
{
    childItems.append(item);
}

clTreeClass *clTreeClass::child(int row)
{
    return childItems.value(row);
}

int clTreeClass::childCount() const
{
    return childItems.count();
}

int clTreeClass::columnCount() const
{
    return itemData.count();
}

QVariant clTreeClass::data(int column) const
{
    return itemData.value(column);
}

clTreeClass *clTreeClass::parent()
{
    return parentItem;
}

int clTreeClass::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<clTreeClass*>(this));

    return 0;
}
