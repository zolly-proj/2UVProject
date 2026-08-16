#ifndef CLCYCLEBUILDERVIEW_H
#define CLCYCLEBUILDERVIEW_H

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
#include <QtWidgets/QGraphicsView>
#include <QtNetwork/QHostInfo>
#include <QtCore/QStorageInfo>



#include "clIceClientLogging.h"
#include "wdCycleBuilderView.h"
#include "clIceClientServer.h"
#include "clDatabaseColumn.h"
#include "clDatabaseRow.h"
#include "clClassObjectTreeItem.h"
#include "clNewObjectUI.h"
#include "clEditObjectUI.h"
#include "clExtendGraphicsView.h"



using namespace std;

class clCycleBuilderView : public QWidget
{

    Q_OBJECT;

public:

    clCycleBuilderView (clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, QWidget* paParent = 0, const char* paName = 0); //, WFlags paFlags = 0);
    ~clCycleBuilderView ();

    Ui::wdCycleBuilderView meCycleBuilderView;
    //QThread meThread;

private slots:
    //Slots Camera setup
	void slotButtonRefreshPressed();
	void slotButtonCreateCycleRoutinePressed();
	void slotButtonSaveCycle();
	void slotTreeClassItemPressedCycles(QTreeWidgetItem *item, int index);
	void slotTreeClassItemPressedRoutines(QTreeWidgetItem *item, int index);
	void slotTreeClassItemMenu(QTreeWidgetItem *item, int i);
	void slotTreeClassItemMenuRoutineCycles(QTreeWidgetItem *item, int i)	;

    void slotNewElement();
    void slotDeleteElement();
	void slotEditElement();	
    void slotNewElementCycles();
    void slotDeleteElementCycles();
	void slotEditElementCycles();	
	void slotDeleteElementRoutineCycles();	
private:
	QString getRelatedIcon(QString paClassName);
	void readMappingIcons();
	bool createClassesInTreeView(vector<std::string> paTables);
	bool removeElementsFromTreeView();
	bool addElementsToQTreeWidget(QTreeWidgetItem * paItem, vector<std::string> paReturnIds, vector<std::string> paReturnNames);
	vector <clDatabaseRow> createClassesInTreeViewCycleRoutine(QString paTable, QString paCycleID);
	
	
	
	clExtendGraphicsView *meGraphicsView;
	
    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;

	vector <QString> meIcon;
	vector <QString> meClass;

};

#endif
