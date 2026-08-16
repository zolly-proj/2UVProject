#ifndef CLPARAMETERVIEW_H
#define CLPARAMETERVIEW_H

#include <exception>
#include <string>
#include <iostream>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMessageBox>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/qtalgorithms>
#include <QtCore/QSignalMapper>
#include <QtCore/QThread>
#include <QtCore/QTextStream>



#include "clIceClientLogging.h"
#include "wdParameterView.h"
#include "clIceClientServer.h"
#include "clTreeClass.h"
#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"
#include "clClassObjectTreeItem.h"
#include "clObjectListItem.h"
#include "clChangePropertieText.h"
#include "clNewObjectUI.h"
#include "clEditObjectUI.h"
#include "clClassInfo.h"
#include "clGraphwidget.h"


using namespace std;

class clParameterView : public QWidget
{

    Q_OBJECT;

public:

    clParameterView (clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QWidget* paParent = 0, const char* paName = 0); //, WFlags paFlags = 0);
    ~clParameterView ();

    Ui::wdParameterView meParameterView;
    //QThread meThread;

private slots:
    //Slots Camera setup
    void slotButtonRefreshPressed();
    void slotButtonReadPressed();
    void slotButtonWritePressed();
    void slotButtonMethodPressed();

    void slotTreeClassItemMenu(QTreeWidgetItem *item, int i);

    void slotTreeClassItemPressed(QTreeWidgetItem *item, int index);
    void slotListPropertieItemPressed(QListWidgetItem *item);


    //void slotNewElement(clClassObjectTreeItem *paConvertedItemParent);
    void slotNewElement();
    void slotDeleteElement();
	void slotEditElement();
	void slotClassInfo();

private:
    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;

	GraphWidget *meGraphWidget;


    bool createClassesInTreeView(vector<std::string> paTables);
    bool removeElementsFromTreeView();
	bool removeElementsFromActions();
    bool addElementsToQTreeWidget(QTreeWidgetItem * paItem, vector<std::string> paReturnIds, vector<std::string> paReturnNames);
	
	bool addElementToQTreeWidgetSub(QTreeWidgetItem * paParent, QString paID, QString paTable);	
	bool addElementsToQTreeWidgetSub(QTreeWidgetItem * paParent, QString paIDs, QString paTable);	
	
	bool fillListViewWithActions(vector<std::string> paActionIDs, vector <std::string> paActionNames, QString paClassName);
    bool fillListViewWithProperties(vector<std::string> paPropertyName, vector<std::string> paPropertyValue, clClassObjectTreeItem *paConvertedItem, clClassObjectTreeItem * paObject);
	
	QString getRelatedIcon(QString paClassName);
	void readMappingIcons();
    /*
    bool fillTableViewWithProperties(vector<std::string> loPropertyName, vector<std::string> loAlias, vector<std::string> loReturnValues, vector<std::string> loType,vector<std::string> loReference);
    bool fillTableViewWithMethods(vector<std::string> paMethodName,vector<std::string> paAliasName,vector<std::string> paSoure,vector<std::string> paSourceName,vector<std::string> paReturnType);
    */
	vector <QString> meIcon;
	vector <QString> meClass;
};

#endif
