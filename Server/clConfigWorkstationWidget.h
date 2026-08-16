#ifndef CLCONFIGPWORKSTATIONWIDGET_H_WVD_13102011
#define CLCONFIGPWORKSTATIONWIDGET_H_WVD_13102011

#include <string>
#include <iostream>
#include <vector>

#include <Ice/Ice.h>
#include <IceUtil/Mutex.h>
#include <IceUtil/CtrlCHandler.h>
#include <IceUtil/IceUtil.h>

#include <QtCore/QString>
#include <QtCore/QDateTime>

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
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QAbstractItemView>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include "wdConfigWorkstationWidget.h"
#include "clWorkstationGraphWidget.h"


#include "ICE_ServerApplication.h"
#include "clIceClientLogging.h"
#include "clServer_impl.h"
#include "clServerThread.h"
#include "clXMLReader.h"
#include "clDatabaseClass.h"
#include "clDatabaseTable.h"

using namespace std;


class clConfigWorkstationWidget : public QMainWindow
{
    Q_OBJECT;

public:
    clConfigWorkstationWidget(clIceClientLogging * paIceClientLogging, clServerThread * paServerThread, QWidget* paParent = 0, const char* paName = 0);
    ~clConfigWorkstationWidget ();
public:

	Ui::wdConfigWorkstationWidget meConfigWorkstationWidget;
	void doSetup(QThread &paThread);

public slots:
	void slotVerticleSliderChanged();
	void slotDoIt();
private:
    //UI fucntions
    void createTreeViewDock();

	QThread meThread;
	QTimer *meTimer;
	
	//Private variables
    //Communication pointer server
    clServerThread *meServerThread;

    //Communication class logging client
    clIceClientLogging *meIceClientLogging;
	
	vector <clWorkstationStatus> meWorkstationStatus;
	vector <clWorkstationGraphWidget> *meWorstationGraphWidgets;
	vector<vector<clWorkstationStatus>> meWorkstation;
    QDockWidget *meTreeView;
    QTreeWidget *meTreeWidget;
	QWidget *meUI;
	QSplitter *meSplitter;
	bool meClosed;
	
};

#endif
