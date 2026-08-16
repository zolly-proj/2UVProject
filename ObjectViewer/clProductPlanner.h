#ifndef CLPRODUCTPLANNER_H
#define CLPRODUCTPLANNER_H

#undef slots
#include <Python.h>
#define slots Q_SLOTS

#include <algorithm>
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
//#include <QtCore/qtalgorithms>
#include <QtCore/QSignalMapper>
#include <QtCore/QThread>
#include <QtCore/QTextStream>




#include "clIceClientLogging.h"
#include "wdProductPlanner.h"
#include "clIceClientServer.h"
#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"
#include "clNewObjectUI.h"
#include "clEditObjectUI.h"
#include "clClassInfo.h"
#include "clClassLoader.h"
#include "clObject.h"


using namespace std;



struct struct_product
{
	QString meUUID;
	QString meName;
	QString meClassName;
	int mePriority;
	
	vector <QString> meWsUuid;
	vector <int> meTech;
	vector <QDateTime> meStartTime;
	vector <QDateTime> meStopTime;
	vector <QTime> meTime;

	vector <QString> meMachGroupUuid;
};

struct struct_workplace
{
	QString meUUID;
	QString meName;
	QString meClassName;
	QDateTime meFreeDateTime;
	int meTech;
	bool meStandAloneMach = false;
	
	vector<QString> meMachGroupUUID;
	
	vector<QString> mePlanName;
	vector<QString> mePlanUUID;
	vector<int> meOverlapTime;
	vector<int> mePrePrep;
	vector<int> mePostPrep;
	
	vector<QString> meShiftName;
	vector<QString> meShiftUUID;
	vector<QDateTime> meStartMonday;
	vector<QDateTime> meStopMonday;
	vector<QDateTime> meStartTues;
	vector<QDateTime> meStopTues;
	vector<QDateTime> meStartWedn;
	vector<QDateTime> meStopWedn;
	vector<QDateTime> meStartThur;
	vector<QDateTime> meStopThur;
	vector<QDateTime> meStartFrid;
	vector<QDateTime> meStopFrid;
	vector<QDateTime> meStartSat;
	vector<QDateTime> meStopSat;
	vector<QDateTime> meStartSun;
	vector<QDateTime> meStopSun;
};


class clProductPlanner : public QWidget
{

    Q_OBJECT;

public:

    clProductPlanner (clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, clClassLoader *paClassLoader, QString paConfigurationFile, QWidget* paParent = 0, const char* paName = 0); //, WFlags paFlags = 0);
    ~clProductPlanner ();

    Ui::wdProductPlanner meProductPlanner;

private slots:
    void slotButtonPlanPressed();
	
private:
    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;	
	clClassLoader * meClassLoader;
	
	QString getRelatedIcon(QString paClassName);
	void readMappingIcons();
	
	bool fillMachineWindow();
	bool fillProductWindow();
	bool createClassesInTreeView(vector<std::string> paTables);
	bool createProductsInTreeView(vector<std::string> paTables);
	bool removeElementsFromMachineView();
	bool removeElementsFromProductView();
	static bool compareProductByPriority(const struct_product& a, const struct_product& b);
	static bool compareWorkplaceByDateTime(const struct_workplace* a, const struct_workplace* b);
	
	vector <QString> meIcon;
	vector <QString> meClass;

};

#endif
