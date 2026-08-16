#ifndef CLCLASSOBJECTTREEITEM_H_WVD_08102011
#define CLCLASSOBJECTTREEITEM_H_WVD_08102011

#include <vector>


#include <QtCore/QString>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QTreeWidget>
#include <QtXml/QDomElement>
#include <QtNetwork/QHostInfo>

#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"

using namespace std;


class clClassObjectTreeItem : public QObject, public QTreeWidgetItem
{

    Q_OBJECT;

public:
    clClassObjectTreeItem(QWidget* paParent = 0, const char* paName = 0){}
    clClassObjectTreeItem(QTreeWidget *tree) : QTreeWidgetItem(tree)  {}
    clClassObjectTreeItem(QTreeWidget * parent, const QStringList & strings) : QTreeWidgetItem (parent,strings)  {}
    clClassObjectTreeItem(QTreeWidget * parent, QTreeWidgetItem * preceding) : QTreeWidgetItem (parent,preceding)  {}

public:
    void setClassName(QString paName);
    void setDatabaseActions(vector<clDatabaseAction> paDatabaseActions);
    void setDatabaseColumns(vector<clDatabaseColumn> paDatabaseColumns);

    QString getClassName();
    vector<clDatabaseColumn> getDatabaseColumns();
    vector<clDatabaseAction> getDatabaseActions();
	
	
	QString meUUID;
	QString meNameDisplay;
	bool meIsInfo = false;
	
	
private:

    QString meClassName;
    vector<clDatabaseAction> meDatabaseActions;
    vector<clDatabaseColumn> meDatabaseColumns;
	
	
	
};

#endif
