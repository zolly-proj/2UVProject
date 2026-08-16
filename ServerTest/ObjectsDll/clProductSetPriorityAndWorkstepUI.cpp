#define INFO_BUFFER_SIZE 32767
#include "clProductSetPriorityAndWorkstepUI.h"

clProductSetPriorityAndWorkstepUI::clProductSetPriorityAndWorkstepUI(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <vector<clDatabaseColumn *>> paDatabaseColumnsByClassNameList, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;
	meDatabaseColumnsByClassNameList = paDatabaseColumnsByClassNameList;
    
    meMainLayout = new QVBoxLayout;
	layout = new QGridLayout;
	
	
	//Create a widget and set its layout as your new layout created above
	viewport = new QWidget;
	viewport->setLayout(layout);

	meLabels[0] = new QLabel("Product:");
	meLabels[1] = new QLabel;
	meLabels[2] = new QLabel("Index:");
	meLabels[3] = new QLabel;
	meLabels[4] = new QLabel("Priority:");
	meLabels[5] = new QLabel("State:");
	meLabels[6] = new QLabel("Workstep state:");
	meLabels[7] = new QLabel("Worksteps:");

	meButtons[1] = new QPushButton(tr("Close"));
	meButtons[0] = new QPushButton(tr("Save"));
	connect(meButtons[0], SIGNAL(clicked()), this, SLOT(slotButtonSaveAssPressed()));
    connect(meButtons[1], SIGNAL(clicked()), this, SLOT(slotButtonCancelPressed()));
	
	
	
	clDatabaseColumn *loDatabaseColumnProdState = getDatabaseColumn("PRODUCT_STATE");
	clDatabaseColumn *loDatabaseColumnProdWsState = getDatabaseColumn("PRODUCT_WS_STATE");
	
	meComboBox[0] = new clComboBoxExtend(meIceClientLogging,*loDatabaseColumnProdState);	
	meComboBox[1] = new clComboBoxExtend(meIceClientLogging,*loDatabaseColumnProdWsState);	
	meLineEdits[0] = new QLineEdit;
	meTreeWidget = new QTreeWidget;
	
    layout->addWidget(meLabels[0],1,0);
	layout->addWidget(meLabels[1],1,1);

    layout->addWidget(meLabels[2],2,0);
	layout->addWidget(meLabels[3],2,1);
	
	layout->addWidget(meLabels[4],3,0);
	layout->addWidget(meLineEdits[0],3,1);
	
	layout->addWidget(meLabels[5],4,0);
	layout->addWidget(meComboBox[0],4,1);

	layout->addWidget(meLabels[6],5,0);
	layout->addWidget(meComboBox[1],5,1);
	
	layout->addWidget(meLabels[7],6,0);
	layout->addWidget(meTreeWidget,6,1);

	layout->addWidget(meButtons[0],7,0);
	layout->addWidget(meButtons[1],7,1);
	
	//Add the viewport to the scroll area
	scrollArea = new QScrollArea;
	scrollArea->setWidget(viewport);
	
    meMainLayout->addWidget(scrollArea);
    setLayout(meMainLayout);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}
clProductSetPriorityAndWorkstepUI::~clProductSetPriorityAndWorkstepUI()
{
}
void clProductSetPriorityAndWorkstepUI::slotButtonSaveAssPressed()
{
	try
	{	
		//Update batch priority
		meObjectBatch->set("PRODUCT_PRIORITY",meLineEdits[0]->text().toInt());
		
		meObjectProduct->set("PRODUCT_STATE",meComboBox[0]->getValue().toInt());
		meObjectProduct->set("PRODUCT_WS_STATE",meComboBox[1]->getValue().toInt());
		
		QList<QTreeWidgetItem*> loList = meTreeWidget->selectedItems();
		QTreeWidgetItem *loItem = NULL;
			
		int loRows = loList.count();
		for (int i = 0; i < loRows; i++)
		{
				
			loItem = loList.takeAt(i);
			break;
		}
		
		if (loItem != NULL)
		{
			QString loWsId;
			clClassObjectTreeItem * loClassObjectTreeItem = (clClassObjectTreeItem*) loItem;
			loWsId = loClassObjectTreeItem->meUUID;
			
			for (int i = 0; i < meObjectsWs.size(); i++)
			{
				if (meObjectsWs.at(i)->ObjectId.compare(loWsId) == 0)
				{
					if (i > 0 && i < meObjectsWs.size() - 1)
					{
						clObject loObjectTemp = new clObject(meObjectsWs.at(i));
						clObject loObjectPrev = new clObject(meObjectsWs.at(i-1));
						clObject loObjectNext = new clObject(meObjectsWs.at(i+1));
						meObjectProduct->set("CURRENT_WORKSTEP",loObjectTemp);
						meObjectProduct->set("PREVIOUS_WORKSTEP",loObjectPrev);
						meObjectProduct->set("NEXT_WORKSTEP",loObjectNext);
						
					}
					else if (i == 0 && i < meObjectsWs.size() - 1)
					{
						clObject loObjectTemp = new clObject(meObjectsWs.at(i));
						clObject loObjectNext = new clObject(meObjectsWs.at(i+1));						
						meObjectProduct->set("CURRENT_WORKSTEP",loObjectTemp);
						meObjectProduct->set("NEXT_WORKSTEP",loObjectNext);
						//Empty object
						clObject loObject(meIceClientServer, meIceClientLogging);
						meObjectProduct->set("PREVIOUS_WORKSTEP",loObject);
						
					}
					else if (i == 0 && i == meObjectsWs.size() - 1)
					{
						clObject loObjectTemp = new clObject(meObjectsWs.at(i));
						meObjectProduct->set("CURRENT_WORKSTEP",loObjectTemp);
						//Empty objects
						clObject loObject(meIceClientServer, meIceClientLogging);
						meObjectProduct->set("PREVIOUS_WORKSTEP",loObject);
						meObjectProduct->set("NEXT_WORKSTEP",loObject);						
					}
					else if (i > 0 && i == meObjectsWs.size() - 1)
					{
						clObject loObjectTemp = new clObject(meObjectsWs.at(i));
						clObject loObjectPrev = new clObject(meObjectsWs.at(i-1));						
						meObjectProduct->set("CURRENT_WORKSTEP",loObjectTemp);
						meObjectProduct->set("PREVIOUS_WORKSTEP",loObjectPrev);
						//Empty object
						clObject loObject(meIceClientServer, meIceClientLogging);						
						meObjectProduct->set("NEXT_WORKSTEP",loObject);
						
					}
				}
			}
		}
		
		//Closing the application
		this->done(0);
		
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clExportToExcelUI::slotButtonSaveAssPressed() -> " + QString(e.what()));
    }
}

void clProductSetPriorityAndWorkstepUI::slotButtonCancelPressed()
{
    try
    {	
	
        this->done(1);
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clExportToExcelUI::slotButtonCancelPressed() -> " + QString(e.what()));
    }
}
bool clProductSetPriorityAndWorkstepUI::setContentOfUI(clObject *paProduct, clObject *paBatch, vector <clObject*> paWorksteps)
{
	try
    {
		meObjectProduct = paProduct;
		meObjectBatch = paBatch;
		meObjectsWs = paWorksteps;
		
		
		
		//Getting the priority of the batch
		int loPriority = 0;
		if (paBatch->get("PRODUCT_PRIORITY",loPriority))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::setContentOfUI -> got the priority of the batch");
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::setContentOfUI -> could not get the priority of the batch");
		
		int loStateWs = 0;
		if (paProduct->get("PRODUCT_WS_STATE",loStateWs))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::setContentOfUI -> got the state of the product");
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::setContentOfUI -> could not get the state of the product");

		int loStateProd = 0;
		if (paProduct->get("PRODUCT_STATE",loStateProd))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::setContentOfUI -> got the state of the product");
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::setContentOfUI -> could not get the state of the product");

		int loProdIndex = 0;
		if (paProduct->get("PRODUCT_INDEX",loProdIndex))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::setContentOfUI -> got the index of the product");
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::setContentOfUI -> could not get the index of the product");
		
		QString loProdName;
		if (paProduct->get("NAME",loProdName))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::setContentOfUI -> got the name of the product");
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::setContentOfUI -> could not get the name of the product");
				
		
		
		meComboBox[0]->setValue(QString::number(loStateProd));		
		meComboBox[1]->setValue(QString::number(loStateWs));

		meLabels[1]->setText(loProdName);
		meLabels[3]->setText(QString::number(loProdIndex));
				
		meLineEdits[0]->setText(QString::number(loPriority));		
		
		
		
		meTreeWidget->setColumnCount(2);

        QStringList headers;
        headers << tr("Name") << tr("UUID");
        meTreeWidget->setHeaderLabels(headers);
		
		for (int i = 0; i < paWorksteps.size(); i++)
		{
			clClassObjectTreeItem *newItem;
			newItem = new clClassObjectTreeItem(meTreeWidget);
			newItem->meUUID = paWorksteps.at(i)->ObjectId;
			newItem->setText(0,paWorksteps.at(i)->ObjectName);
			newItem->setText(1,paWorksteps.at(i)->ObjectId);
		} 

		clObject loObject(meIceClientServer, meIceClientLogging);
		if (paProduct->get("CURRENT_WORKSTEP",loObject))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::setContentOfUI -> got the current ws of product");
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::setContentOfUI -> could not get the current ws of the product");		
		//-------------------------------------------
		//Set current ws selected
		//-------------------------------------------
		if (loObject.ObjectId.compare(QString("")) == 0)
		{
			//Select nothing
		}
		else
		{
			for (int i = 0; i < meTreeWidget->topLevelItemCount(); i++)
			{
				clClassObjectTreeItem * loItem = (clClassObjectTreeItem*) meTreeWidget->topLevelItem(i); 
				if (loObject.ObjectId.compare(loItem->meUUID) == 0)
				{
					meTreeWidget->topLevelItem(i)->setSelected(true);
				}
			}
		}
		
		return true;
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clExportToExcelUI::slotButtonCancelPressed() -> " + QString(e.what()));
		return false;
    }
}
clDatabaseColumn * clProductSetPriorityAndWorkstepUI::getDatabaseColumn(QString paColumnName)
{
	try
    {	
		for (int i = 0; i < meDatabaseColumnsByClassNameList.size(); i++)
		{
			for (int j = 0; j < meDatabaseColumnsByClassNameList.at(i).size(); j++)
			{
				if (paColumnName.toUpper().compare(meDatabaseColumnsByClassNameList.at(i).at(j)->getName().toUpper()) == 0)
				{
					return meDatabaseColumnsByClassNameList.at(i).at(j);
				}
			}
		}
        return NULL;
    }
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProductSetPriorityAndWorkstepUI::getDatabaseColumn -> " + QString(e.what()));
		return NULL;
    }	
}
