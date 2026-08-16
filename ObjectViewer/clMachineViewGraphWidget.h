#ifndef CLMACHINEVIEWGRAPHWIDGET_H
#define CLMACHINEVIEWGRAPHWIDGET_H

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
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QDateTimeEdit>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QSignalMapper>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clClassLoader.h"
#include "clObject.h"

#include "clClickedLabel.h"
#include "clMachineViewStruct.h"

using namespace std;
using namespace QtCharts;



class clMachineViewGraphWidget : public QWidget
{

    Q_OBJECT;

public:
    clMachineViewGraphWidget(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging,  clClassLoader *paClassLoader, struct_widget paStructWidget, QWidget* paParent = 0, const char* paName = 0);
    ~clMachineViewGraphWidget ();
	
public slots:
	void slotDoIt();
private:

    void initializeElements();

    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;	
	clClassLoader * meClassLoader;

public:
	
	QGridLayout * meMainLayout;

	QPieSeries * mePieSeries;
	QChart * meChart;
	QChartView * meChartView;
	QPieSlice * mePieSlice_Running;
	QPieSlice * mePieSlice_Offline;
	
	QRadioButton * meRadioDaily;
	QRadioButton * meRadioWeekly;
	QRadioButton * meRadioMonthly;
	
	
	QTimer * meTimer;
	
	struct_widget meStructWidget;
};
#endif
