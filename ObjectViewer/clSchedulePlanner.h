#ifndef CLSCHEDULEPLANNER_H
#define CLSCHEDULEPLANNER_H

#undef slots
#include <Python.h>
#define slots Q_SLOTS

#include <algorithm>
#include <exception>
#include <string>
#include <iostream>

#include <QtGui/QMouseEvent>
#include <QtWidgets/QGraphicsSceneMouseEvent>
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
#include <QtCore/QObject>
//#include <QtCore/qtalgorithms>
#include <QtCore/QSignalMapper>
#include <QtCore/QThread>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>



#include "clIceClientLogging.h"
#include "wdSchedulePlanner.h"
#include "clIceClientServer.h"
#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"
#include "clNewObjectUI.h"
#include "clEditObjectUI.h"
#include "clClassInfo.h"
#include "clClassLoader.h"
#include "clObject.h"

#include "clTimeLine.h"
#include "clGraphicsScene.h"
#include "clGraphicsView.h"
#include "clGraphicsItem.h"

using namespace std;




class clSchedulePlanner : public QWidget
{

    Q_OBJECT;

public:

    clSchedulePlanner (clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, clClassLoader *paClassLoader, QString paConfigurationFile, QWidget* paParent = 0, const char* paName = 0); //, WFlags paFlags = 0);
    ~clSchedulePlanner ();

    Ui::wdSchedulePlanner meSchedulePlanner;

public slots:
    void slotButtonPlanPressed();
	void slotButtonSavePressed();
	void slotItemDoubleClicked(QGraphicsItem * paItem);
	void slotButtonNewPressed();
	void slotButtonRefreshPressed();
	void slotDeleteObject();
	void slotEditObject();
private:
    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;	
	clClassLoader * meClassLoader;
	
	void fillEmployeeView();
	//bool addElementsToQTreeWidget(QTreeWidgetItem * paItem, vector<std::string> paReturnIds, vector<std::string> paReturnNames);	
	bool createClassesInTreeView(vector<std::string> paTables);
	bool removeElementsFromEmployeeView();
	
	
	bool fillWorkingArea();
	bool fillTimeTableView(QString paUUID, QString paName, int paIndex);
	vector <clObject> returnObjectsOfTableForId(QString paTableName, QString paId);
	
	vector <QGraphicsScene *> meScene;
    vector <clGraphicsView *> meView;
    vector <clTimeLine *> meTimeline;
    vector <clGraphicsItem *> meGraphicsItems;
    clGraphicsItem * meSelectedGraphicsItem;
	
	QString getRelatedIcon(QString paClassName);
	void readMappingIcons();	
	vector <QString> meIcon;
	vector <QString> meClass;
	
	const int meSpacerInTimeline = 22;	
	
	//TODO change to me
	QDateTime loMinDateTime;
	QDateTime loMaxDateTime;
	
	vector<QDateTimeEdit *>  meDateTimeEdit;
	QComboBox * meCboClassName;
};

#endif
