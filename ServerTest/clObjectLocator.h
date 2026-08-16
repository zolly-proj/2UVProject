#ifndef CLOBJECTLOCATOR_H
#define CLOBJECTLOCATOR_H

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
#include <QtCore/QSignalMapper>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QTextStream>
#include <QtCore/QPropertyAnimation>
#include <QtNetwork/QHostInfo>

//#include <Qt3DCore/QEntity>
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
#include "wdObjectLocator.h"
#include "clIceClientServer.h"
#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"
#include "clNewObjectUI.h"
#include "clEditObjectUI.h"
#include "clClassInfo.h"
#include "clClassLoader.h"
#include "clObject.h"


using namespace std;



class clObjectLocator : public QWidget
{

    Q_OBJECT;

public:

    clObjectLocator (clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging, clClassLoader *paClassLoader, QString paConfigurationFile, QWidget* paParent = 0, const char* paName = 0); //, WFlags paFlags = 0);
    ~clObjectLocator ();

    Ui::wdObjectLocator meObjectLocator;

private slots:
    void slotButtonRefreshPressed();
	void slotTreeClassItemPressed(QTreeWidgetItem *item, int index);
	void slotDoIt();
private:
    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;	
	clClassLoader * meClassLoader;
	
	bool fillObjectLocations();
	bool addElementsToQTreeWidget(QTreeWidgetItem * paItem, vector<std::string> paReturnIds, vector<std::string> paReturnNames);	
	bool createClassesInTreeView(vector<std::string> paTables);
	bool removeElementsFromObjectLocations();
	
	bool showLocatorMapForLocation(QString paUUID);
	Qt3DCore::QEntity* createPointEntity(Qt3DCore::QEntity* rootEntity, QString paName, QString paSSID, QString paKind, QString paQRcode, QString paUUID, QString paXcoord, QString paYcoord, QString paZcoord, int paId);
	Qt3DCore::QEntity* createPlane(Qt3DCore::QEntity* rootEntity, QString paPlaneImage);
	Qt3DCore::QEntity* createLineEntity(Qt3DCore::QEntity* rootEntity);
	Qt3DExtras::Qt3DWindow* meViewPort = NULL;
	QWidget* meContainer = NULL;
	
	//Points
	// Sphere
	vector<Qt3DCore::QEntity *> sphereEntity;
	vector<Qt3DExtras::QSphereMesh*> sphereMesh;
	vector<Qt3DCore::QTransform*> transform; 
	// Material
	vector<Qt3DExtras::QPhongMaterial*> material;
	//Extra info
	vector<QString> meName;
	vector<QString> meUUID;
	QString meDisplayedView = "";
	
	
	QTimer* meTimer;
	
	
	QString getRelatedIcon(QString paClassName);
	void readMappingIcons();	
	vector <QString> meIcon;
	vector <QString> meClass;
	
	float meX_SIZE = 100;
	float meY_SIZE = 100;

};

#endif
