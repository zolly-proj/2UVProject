#ifndef CLPARAMETERVIEW_H
#define CLPARAMETERVIEW_H

#undef slots
#include <Python.h>
#define slots Q_SLOTS


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
#include <QtCore/QSignalMapper>
#include <QtCore/QThread>
#include <QtCore/QTextStream>
#include <QtNetwork/QHostInfo>




#include "clIceClientLogging.h"
#include "wdObjectView.h"
#include "clIceClientServer.h"
//#include "clTreeClass.h"
#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"
//#include "clClassObjectTreeItem.h"
//#include "clObjectListItem.h"
//#include "clChangePropertieText.h"
#include "clNewObjectUI.h"
#include "clEditObjectUI.h"
#include "clClassInfo.h"
//#include "clGraphwidget.h"
//#include "clMethodCall.h"
//#include "clMethodCallHeader.h"
#include "clClassLoader.h"
#include "clObject.h"


using namespace std;

class clObjectViewer : public QWidget
{

    Q_OBJECT;

public:

    clObjectViewer (clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, clClassLoader *paClassLoader, QString paConfigurationFile, QWidget* paParent = 0, const char* paName = 0); //, WFlags paFlags = 0);
    ~clObjectViewer ();

    Ui::wdObjectView meObjectView;
    //QThread meThread;


	

private slots:
    void slotButtonRefreshPressed();


    void slotTreeClassItemMenu(QTreeWidgetItem *item, int i);

    void slotTreeClassItemPressed(QTreeWidgetItem *item, int index);
    //void slotNewElement(clClassObjectTreeItem *paConvertedItemParent);
    void slotNewElement();
    void slotDeleteElement();
	void slotEditElement();
	
	void slotClassInfo();
	void slotCallAction(const QString &paActionName);
	void slotCallMethod(const QString &paActionName);
	void slotCallClassMethod(const QString &paActionName);
private:

	//bool performScript(char * paScriptName, char * paScriptClass, char * paScriptMethod, char *paArg01);
	//bool performScriptWithDebug(char * paScriptNameWithExtension, char * paScriptName, char * paScriptClass, char * paScriptMethod, char *paArg01);
	//bool callRoutineDLL(QString paObjectUUID, QString paCurrentMehtodSourceFile, QString paClassName);
	//bool callRoutinePythonNoDebug(QString paObjectUUID, QString paCurrentMethodSourceFile, QString paClass, QString paMethod);
	//bool callRoutinePythonWithDebug(QString paObjectUUID, QString paCurrentMethodSourceFile, QString paClass, QString paMethod);
	
    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;	
	clClassLoader * meClassLoader;
	
    bool createClassesInTreeView(vector<std::string> paTables);
    bool removeElementsFromTreeView();

    bool addElementsToQTreeWidget(QTreeWidgetItem * paItem, vector<std::string> paReturnIds, vector<std::string> paReturnNames);
	bool addElementToQTreeWidgetSub(QTreeWidgetItem * paParent, QString paID, QString paTable);	
	bool addElementsToQTreeWidgetSub(QTreeWidgetItem * paParent, QString paIDs, QString paTable);	
	
	QString getRelatedIcon(QString paClassName);
	void readMappingIcons();
	bool readXMLconfigFile();
	bool classInConfig(std::string paClassName);
	bool propertyInConfigFile(QString paClassName, QString paPropertyName);
	bool methodInConfigFile(QString paClassName, QString paMethodName);
	
	vector <QString> meIcon;
	vector <QString> meClass;
	
	//For read of the configuration file
	QString meClassExposingFile;
	vector <QString> meClassesConfigured;
	vector <vector<QString>> meClassMethodsConfigured;
	vector <vector<QString>> meClassPropsConfigured;
};

#endif
