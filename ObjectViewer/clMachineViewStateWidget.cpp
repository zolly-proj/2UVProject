#define INFO_BUFFER_SIZE 32767
#include "clMachineViewStateWidget.h"

clMachineViewStateWidget::clMachineViewStateWidget(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clClassLoader *paClassLoader, struct_widget paStructWidget,QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;	
	meClassLoader = paClassLoader;
	meStructWidget = paStructWidget;
    initializeElements();
    
    /////// Setting up the timer function ///////////////////////////////////
	meTimer = new QTimer(this);
	meTimer->setInterval(meStructWidget.refresh.toInt());
	meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotDoIt()));
	meTimer->start();
    
    
}
clMachineViewStateWidget::~clMachineViewStateWidget()
{
}

void clMachineViewStateWidget::slotShowMethodsMenu()
{
	try
	{
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMahineViewStateWidget::slotShowMethodsMenu -> start");
		QMenu loMenu(this);		
		QAction *loMethods[200];			
		QSignalMapper* signalMapper = new QSignalMapper(this);
		
		//clGraphicsItem * loGraphicsItem = meGraphicsItems.at(paIndex);
		for (int i = 0; i < meStructWidget.methods.size(); i++)
		{
			loMethods[i] = loMenu.addAction(meStructWidget.methods.at(i).name);
			QObject::connect(loMethods[i], SIGNAL(triggered()), signalMapper, SLOT(map()));
			signalMapper->setMapping(loMethods[i], meStructWidget.methods.at(i).name);
		
		}
		connect(signalMapper, SIGNAL(mapped(QString)),this, SLOT(slotPerformMethod(QString)));
		loMenu.exec(QCursor::pos());	
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMahineViewStateWidget::slotShowMethodsMenu -> " + QString(e.what()));
	}
}
void clMachineViewStateWidget::slotPerformMethod(QString paMethodName)
{
    try
    {			
		for (int i = 0; i < (int) meClassLoader->meMethodsGeneralNames.size(); i++)
		{
			if (meClassLoader->meMethodsGeneralNames.at(i).toUpper().compare(paMethodName.toUpper()) == 0)
			{
				vector <QString> loCurrentMethodArgType;
				loCurrentMethodArgType.push_back(QString("CLASS"));
				loCurrentMethodArgType.push_back(QString("uuid"));


				vector <QString> lloCurrentMethodParameters;

				vector <QString> loCurrentMethodArgValue;
				loCurrentMethodArgValue.push_back(meStructWidget.object_class);
				loCurrentMethodArgValue.push_back(meStructWidget.object_id);


				meClassLoader->meMethodCalls.at(i)->doMethod(loCurrentMethodArgType,loCurrentMethodArgValue);
				
				break;
			}
		}
    }
    catch(exception &e)
    {
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMahineViewStateWidget::slotPreformMethod" + QString(e.what()));
    }
}

void clMachineViewStateWidget::initializeElements()
{
    try
    {
		meMainLayout = new QGridLayout();
		QImage loImgMachine(meStructWidget.value);
		mePixmapMach = new QPixmap(QPixmap::fromImage(loImgMachine));
		meLabelInformation[0] = new clClickedLabel();
		meLabelInformation[0]->setPixmap(mePixmapMach->scaled(meStructWidget.width.toInt(),meStructWidget.height.toInt()));
		
		connect(meLabelInformation[0], SIGNAL(clicked()), this, SLOT(slotShowMethodsMenu()));

		
		meMainLayout->addWidget(meLabelInformation[0],0,0);
		
		mePixmapOnline = new QPixmap(QPixmap::fromImage(QImage("./ICONS/machview_online.png")));
		mePixmapRunning = new QPixmap(QPixmap::fromImage(QImage("./ICONS/machview_running.png")));
		mePixmapWarning = new QPixmap(QPixmap::fromImage(QImage("./ICONS/machview_warning.png")));
		mePixmapError = new QPixmap(QPixmap::fromImage(QImage("./ICONS/machview_error.png")));
		mePixmapInit = new QPixmap(QPixmap::fromImage(QImage("./ICONS/machview_init.png")));
		
		mePixmapConnected = new QPixmap(QPixmap::fromImage(QImage("./ICONS/Online.png")));
		mePixmapNotConnected = new QPixmap(QPixmap::fromImage(QImage("./ICONS/Offline.png")));

		meLabelInformation[1] = new clClickedLabel();
		meLabelInformation[1]->setPixmap(mePixmapNotConnected->scaled(meStructWidget.width.toInt(),10));
				
		meLabelInformation[2] = new clClickedLabel();
		meLabelInformation[2]->setPixmap(mePixmapInit->scaled(meStructWidget.width.toInt(),10));
		meLabel[0] = new QLabel("Initialized ...");
		
		meMainLayout->addWidget(meLabelInformation[1],1,0);
		meMainLayout->addWidget(meLabelInformation[2],2,0);
		meMainLayout->addWidget(meLabel[0],3,0);		
		setLayout(meMainLayout);
		
    }
    catch(exception &e)
    {
		
		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMahineViewStateWidget::initializeElements" + QString(e.what()));
    }
}
void clMachineViewStateWidget::slotDoIt()
{
	try
	{
		clObject loObject = meClassLoader->getObject(meStructWidget.object_class);
		loObject.ClassName = meStructWidget.object_class;
		loObject.ObjectId = meStructWidget.object_id;
		
		int loRunningState;		
		if (loObject.get("DEVICE_STATE",loRunningState))
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMachineViewStateWidget::slotDoIt -> getting the DEIVCE_STATE ok");
		else
		{
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMachineViewStateWidget::slotDoIt -> getting the DEVICE_STATE NOK");
			return;
		}

		int loFileTransfer;
		if (loObject.get("DEVICE_FILETRANSFER",loFileTransfer))
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMachineViewStateWidget::slotDoIt -> getting the DEIVCE_FILESTRANSFER ok");
		else
		{
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMachineViewStateWidget::slotDoIt -> getting the DEVICE_FILETRANSFER NOK");
			return;
		}		
		
		int loOnlineOffline;
		if (loObject.get("CONNECTION_STATE",loOnlineOffline))
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMachineViewStateWidget::slotDoIt -> getting the CONNECTION_STATE ok");
		else
		{
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMachineViewStateWidget::slotDoIt -> getting the CONNECTION_STATE NOK");
			return;
		}	
		
		
		
		switch(loOnlineOffline)
		{
			case 0:
				meLabelInformation[1]->setPixmap(mePixmapNotConnected->scaled(meStructWidget.width.toInt(),10));
				break;
			case 1:
				meLabelInformation[1]->setPixmap(mePixmapConnected->scaled(meStructWidget.width.toInt(),10));
				break;
			default:
				meLabelInformation[1]->setPixmap(mePixmapNotConnected->scaled(meStructWidget.width.toInt(),10));
				break;
		}	
		
		/*
		* 	<enum name="DEVICE_STATE">
		<state value="0" text="nothing"/>
		<state value="10" text="waiting"/>
		<state value="20" text="running"/>
		<state value="30" text="paused"/>
		<state value="40" text="not connected"/>
		<state value="100" text="undefined"/>
		<state value="110" text="error"/>
		</enum>	
		*/
		switch(loRunningState)
		{
			case 0:
				meLabelInformation[2]->setPixmap(mePixmapInit->scaled(meStructWidget.width.toInt(),10));
				meLabel[0]->setText("Initialized ...");
				break;
			case 10:
				meLabelInformation[2]->setPixmap(mePixmapOnline->scaled(meStructWidget.width.toInt(),10));
				meLabel[0]->setText("Waiting ... ");
				break;
			case 20:
				meLabelInformation[2]->setPixmap(mePixmapRunning->scaled(meStructWidget.width.toInt(),10));
				meLabel[0]->setText("Running ...");
				break;
			case 30:
				meLabelInformation[2]->setPixmap(mePixmapWarning->scaled(meStructWidget.width.toInt(),10));
				meLabel[0]->setText("Paused ... ");
				break;
			case 40:
				meLabelInformation[2]->setPixmap(mePixmapInit->scaled(meStructWidget.width.toInt(),10));
				meLabel[0]->setText("Not connected ...");
				break;
			case 100:
				meLabelInformation[2]->setPixmap(mePixmapInit->scaled(meStructWidget.width.toInt(),10));
				meLabel[0]->setText("Undefined ...");
				break;
			case 110:
				meLabelInformation[2]->setPixmap(mePixmapError->scaled(meStructWidget.width.toInt(),10));
				meLabel[0]->setText("Error ...");
				break;
			default:
				meLabelInformation[2]->setPixmap(mePixmapInit->scaled(meStructWidget.width.toInt(),10));
				meLabel[0]->setText("Initialized ...");
				break;
		}
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMahineViewStateWidget::slotDoIt" + QString(e.what()));
    }		
}
