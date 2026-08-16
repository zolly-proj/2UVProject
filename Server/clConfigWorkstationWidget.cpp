#include "clConfigWorkstationWidget.h"
clConfigWorkstationWidget::clConfigWorkstationWidget(clIceClientLogging * paIceClientLogging, clServerThread * paServerThread, QWidget* paParent, const char* paName)
{
    try
    {
        this->resize(682,297);
		
        createTreeViewDock();
		
		meServerThread = paServerThread;
		meIceClientLogging = paIceClientLogging;

		/*
        doSetup(meThread);
        moveToThread(&meThread);
        meThread.start();
		*/
		meClosed = false;
		
		meTimer = new QTimer(this);
        meTimer->setInterval(5000);
        meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotDoIt()));
        meTimer->start();
		
		
		
		
		meSplitter = new QSplitter;
		meSplitter->setMinimumSize(400,200);		
		meConfigWorkstationWidget.verticalLayout->addWidget(meSplitter);
		
		
		
		
    }
    catch(...)
    {

    }
}

clConfigWorkstationWidget::~clConfigWorkstationWidget ()
{
	try
	{
		meClosed = true;
		this->close();
	}
	catch(...)
	{
		
	}
}
/*****************************
* UI functions
*****************************/
void clConfigWorkstationWidget::createTreeViewDock()
{
    try
    {

		meUI = new QWidget();
		meConfigWorkstationWidget.setupUi(meUI);
		//connect(meClientLogger.btnPing, SIGNAL(clicked()),this,SLOT(slotButtonPingPressed()));
		//connect(meClientLogger.btnShutdown, SIGNAL(clicked()),this,SLOT(slotButtonShutdownPressed()));
		//connect(meClientLogger.btnSendMessage, SIGNAL(clicked()),this,SLOT(slotButtonSendMessagePressed()));
		meConfigWorkstationWidget.vtsPercentage->setMinimum(0);
		meConfigWorkstationWidget.vtsPercentage->setMaximum(99);
		meConfigWorkstationWidget.vtsPercentage->setValue(0);
		meConfigWorkstationWidget.vtsPercentage->setTickInterval(1);
		
		connect(meConfigWorkstationWidget.vtsPercentage,SIGNAL(valueChanged(int)),this,SLOT(slotVerticleSliderChanged()));

        //Create the tree widget
        meTreeView = new QDockWidget(tr("Workstation performance ..."));
        meTreeView->setWidget(meUI);
        meTreeView->setAllowedAreas(Qt::LeftDockWidgetArea
                                      | Qt::RightDockWidgetArea);
        //Local function form the main widget
        addDockWidget(Qt::RightDockWidgetArea, meTreeView);
        meTreeView->show();
		
    }
    catch(...)
    {
    }
}
/************************************************
* Do thread startup
************************************************/
void clConfigWorkstationWidget::doSetup(QThread &paThread)
{
		printf("START clConfigProductWidget::doSetup");
        connect(&paThread,SIGNAL(started()),this,SLOT(doWork()));
		printf("END clConfigProductWidget::doSetup");
}
void clConfigWorkstationWidget::slotDoIt()
{
    try
    {
		printf("START clConfigProductWidget::slotDoIt");
		
		
		meWorkstationStatus.clear();

		printf("[%d]",meServerThread->meWorkstationStatus.size());
		
		for (int l = 0; l < meServerThread->meWorkstationStatus.size(); l++)
		{
			meWorkstationStatus.push_back(meServerThread->meWorkstationStatus[l]);
		}
		
		meWorkstation.clear();
		meIceClientLogging->insertItem("1","2UVLogServer.exe","clConfigWorkstationWidget::slotDoIt ->" + QString("Amount of entrys [") + QString::number(meWorkstationStatus.size()) + QString("]"));
		//meIceClientLogging->insertItem("1","2UVLogServer.exe","clConfigProductWidget::slotDoIt ->" + QString("Amount of workstations [" + QString::number(meWorkstation.size()) + "]"));
		
		if (meWorkstationStatus.size() > 0)
		{
			for (int i = 0; i < meWorkstationStatus.size(); i++)
			{
				bool loInserted = false;
				
				meIceClientLogging->insertItem("1","2UVLogServer.exe","clConfigWorkstationWidget::slotDoIt ->" + QString("meWorkstation.size() [") + QString::number(meWorkstation.size()) + QString("]"));
				for (int j = 0; j < meWorkstation.size(); j++)
				{
					meIceClientLogging->insertItem("1","2UVLogServer.exe","clConfigWorkstationWidget::slotDoIt ->" + QString("Workstation memory name  [" + QString(meWorkstation[j][0].getWorkstationName()) + "] new name [" + meWorkstationStatus[i].getWorkstationName() + "]"));
					if (meWorkstation[j][0].getWorkstationName() == meWorkstationStatus[i].getWorkstationName())
					{
						meWorkstation[j].push_back(meWorkstationStatus[i]);
						meIceClientLogging->insertItem("1","2UVLogServer.exe","clConfigWorkstationWidget::slotDoIt ->" + QString("Workstation memory name  [" + QString(meWorkstation[j][0].getWorkstationName()) + "] pushed [" + QString::number(i) + "]"));
						loInserted = true;
					}
				}
				if(!loInserted)
				{
					vector <clWorkstationStatus> loWorkstationStatusNew;
					loWorkstationStatusNew.push_back(meWorkstationStatus[i]);
					meWorkstation.push_back(loWorkstationStatusNew);
					meIceClientLogging->insertItem("1","2UVLogServer.exe","clConfigWorkstationWidget::slotDoIt ->" + QString("New workstation detected [" + QString(meWorkstationStatus[i].getWorkstationName()) + "]"));
				}
			}
		}
		//Create the widgets
		
		meIceClientLogging->insertItem("1","2UVLogServer.exe","clConfigWorkstationWidget::slotDoIt ->QSplitter declared");

		for (int l = 0; l < meSplitter->count();l++)
		{
			meSplitter->widget(l)->deleteLater();
		}

		
		if (meWorkstation.size() > 0)
		{			
			for (int k = 0; k < meWorkstation.size(); k++)
			{
				//new clWorkstationGraphWidget(meIceClientLogging,&meWorkstation[k],meConfigWorkstationWidget.lstWidget);
				meIceClientLogging->insertItem("1","2UVLogServer.exe","clConfigWorkstationWidget::slotDoIt ->QChartView");
				QChartView *loChartView = new clWorkstationGraphWidget(meIceClientLogging,&meWorkstation[k]);
				meSplitter->addWidget(loChartView);
				meIceClientLogging->insertItem("1","2UVLogServer.exe","clConfigWorkstationWidget::slotDoIt ->Added");
			}
		}
		
		
		meIceClientLogging->insertItem("1","2UVLogServer.exe","clConfigWorkstationWidget::slotDoIt ->" + QString("End reading workstation load"));
		
		printf("END clConfigProductWidget::slotDoIt");
    }
    catch(exception &e)
    {
		meIceClientLogging->insertItem("1","2UVLogServer.exe","clConfigWorkstationWidget::slotDoIt ->" + QString(e.what()));
    }
}

void clConfigWorkstationWidget::slotVerticleSliderChanged()
{
	try
	{
			meIceClientLogging->insertItem("1","2UVLogServer.exe","clServerThread::slotVerticleSliderChanged ->" + QString("START"));
			meConfigWorkstationWidget.txtPercentage->setText(QString::number(meConfigWorkstationWidget.vtsPercentage->value()) + QString("%"));
			meConfigWorkstationWidget.pgbPercentage->setValue(meConfigWorkstationWidget.vtsPercentage->value());
			meIceClientLogging->insertItem("1","2UVLogServer.exe","clServerThread::slotVerticleSliderChanged ->" + QString("STOP"));
    }
    catch (exception& e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe","clServerThread::slotVerticleSliderChanged ->" + QString(e.what()));
    }
}