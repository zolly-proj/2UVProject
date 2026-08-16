#ifndef CLMACHINEVIEWSTATEWIDGET_H
#define CLMACHINEVIEWSTATEWIDGET_H

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





#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clClassLoader.h"
#include "clObject.h"

#include "clClickedLabel.h"
#include "clMachineViewStruct.h"

using namespace std;




class clMachineViewStateWidget : public QWidget
{

    Q_OBJECT;

public:
    clMachineViewStateWidget(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging,  clClassLoader *paClassLoader, struct_widget paStructWidget, QWidget* paParent = 0, const char* paName = 0);
    ~clMachineViewStateWidget ();
	
public slots:
	void slotShowMethodsMenu();
	void slotPerformMethod(QString paMethodName);
	void slotDoIt();
private:

    void initializeElements();

    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;	
	clClassLoader * meClassLoader;

public:
	
	QGridLayout * meMainLayout;
	QPixmap * mePixmapOnline;
	QPixmap * mePixmapRunning;
	QPixmap * mePixmapWarning;
	QPixmap * mePixmapError;
	QPixmap * mePixmapInit;
	
	QPixmap * mePixmapConnected;
	QPixmap * mePixmapNotConnected;
	
	QPixmap * mePixmapMach;
	
	clClickedLabel * meLabelInformation[100];
	QLabel * meLabel[100];
	
	QTimer * meTimer;
	
	struct_widget meStructWidget;
};

#endif
