#ifndef CLMACHINEVIEW_H
#define CLMACHINEVIEW_H

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
#include <QtGui/QScreen>
#include <QtCore/QSize>
#include <QtCore/QSignalMapper>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QTextStream>
#include <QtCore/QPropertyAnimation>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QTextureImage>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>

#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QOrbitCameraController>

#include "Qt3DExtras/qt3dwindow.h"
#include "Qt3DExtras/qorbitcameracontroller.h"



#include "clIceClientLogging.h"
#include "wdMachineView.h"
#include "clIceClientServer.h"
#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"
#include "clNewObjectUI.h"
#include "clEditObjectUI.h"
#include "clClassInfo.h"
#include "clClassLoader.h"
#include "clObject.h"

#include "clMachineViewStruct.h"
#include "clMachineViewStateWidget.h"
#include "clMachineViewGraphWidget.h"
using namespace std;



class clMachineView : public QWidget
{

    Q_OBJECT;

public:

    clMachineView (clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, clClassLoader *paClassLoader, QString paConfigurationFile, QWidget* paParent = 0, const char* paName = 0); //, WFlags paFlags = 0);
    ~clMachineView ();

    Ui::wdMachineView meMachineView;

private slots:

private:

	bool readConfigFile();
	bool generateWidgetsFromConfigFile();

    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;	
	clClassLoader * meClassLoader;

	const QString meConfigFileName = "./CONFIG/machineView_01.xml";

	vector <struct_widget> meStructWidget;
	
	QHBoxLayout *meMainLayout;
	QWidget *viewport;
	QScrollArea *scrollArea;
	QGridLayout *layout;	
};

#endif
