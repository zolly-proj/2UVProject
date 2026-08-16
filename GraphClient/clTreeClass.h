#ifndef TREECLASS_H
#define TREECLASS_H

#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QStringList>

//! [0]
class clTreeClass
{

public:
    clTreeClass(const QList<QVariant> &data, clTreeClass *parent = 0);
    ~clTreeClass();

    void appendChild(clTreeClass *child);

    clTreeClass *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    clTreeClass *parent();

private:
    QList<clTreeClass*> childItems;
    QList<QVariant> itemData;
    clTreeClass *parentItem;
};
//! [0]

#endif
