#ifndef CLWORKSTATIONGRAPHWIDGET_H_WVD_28122023
#define CLWORKSTATIONGRAPHWIDGET_H_WVD_28122023

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
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QGraphicsObject>
#include <QtWidgets/QGraphicsItem>
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

#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QCategoryAxis>

#include <QtGui/QBrush>
#include <QtGui/QFont>


#include "clIceClientLogging.h"
#include "clWorkstationStatus.h"
#include "clServer_impl.h"
#include "clXMLReader.h"
#include "clDatabaseClass.h"
#include "clDatabaseTable.h"
#include "clServerThread.h"

using namespace std;
using namespace QtCharts;

class clWorkstationGraphWidget : public QChartView
{
	Q_OBJECT
public:
    clWorkstationGraphWidget(clIceClientLogging * paIceClientLogging, vector <clWorkstationStatus> * paWorkstationStatus);
    ~clWorkstationGraphWidget();

public:
	void setTimingsInGraph();
	/*
public slots:
	void slotDoIt();
	*/
private:
	void setUpWidget();

	QTimer * meTimer;

    //Communication class logging client
    clIceClientLogging * meIceClientLogging;
	vector <clWorkstationStatus> * meWorkstationStatus;


	QGraphicsItem * meGraphicsItem;
	QChart * meChart;
};
#endif
