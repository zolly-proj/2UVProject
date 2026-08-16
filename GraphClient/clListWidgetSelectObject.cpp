#define INFO_BUFFER_SIZE 32767
#include "clListWidgetSelectObject.h"

clListWidgetSelectObject::clListWidgetSelectObject(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <clDatabaseColumn> paDatabaseColumns, clDatabaseColumn paDatabaseColumn, QWidget* paParent, const char* paName) : meDatabaseColumn(paDatabaseColumn)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;
    meDatabaseColumns = paDatabaseColumns;
	

    initializeElements();
    createGridGroupBox();

    meMainLayout = new QVBoxLayout();
    meMainLayout->addWidget(meGridGroupBox);
    setLayout(meMainLayout);


}
clListWidgetSelectObject::~clListWidgetSelectObject()
{
}
void clListWidgetSelectObject::initializeElements()
{
    try
    {

        //for(int i = 0; i < NumGridRows; i++)
        //{
        //    meLineEdits[i] = NULL;
        //}

    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);			
		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clNewObjectUI::initializeElements" + QString(e.what()));
    }
}


void clListWidgetSelectObject::createGridGroupBox()
{
    meGridGroupBox = new QGroupBox(tr("Values"));
    QGridLayout *layout = new QGridLayout;

	QString loSelectedClass = meDatabaseColumn.getReference();

	
    meTreeWidget = new QTreeWidget;
	meTreeWidget->setColumnCount(3);
	
	
	QStringList loClasses = loSelectedClass.split("$;$");
	if (loClasses.size() > 1)
	{

		//*****************************
        //* Getting the tables *
        //********************************
        vector<std::string> loTables;
        QString loMessage;
        meIceClientServer->getAllTablesFromDatabase(loTables,loMessage);
		
		for (int k = 0; k < loTables.size(); k++)
		{
			QString loTableName;
			loTableName = QString(loTables[k].c_str());
			if (loTableName.toUpper().contains("ADD_"))
			{
				//Do nothing
			}
			else if (loTableName.toUpper().contains("A_"))
			{
				//Do nothing
				
			}
			else
			{
				for (int t = 0; t < loClasses.size(); t++)
				{
					if(loTableName.toUpper().compare(loClasses.at(t).toUpper()) == 0)
					{
				
						QTreeWidgetItem *loTreeWidgetItem = new QTreeWidgetItem(meTreeWidget);
						loTreeWidgetItem->setText(0, loTableName);
						
						//Getting the objectsID's from the database
						vector<std::string> loReturnIdsTable;
						vector<std::string> loReturnNamesTable;
						QString loReturnMessageTable;

						meIceClientServer->getFromTableDatabaseGeneral( loTableName,
																		QString("0"),
																		QString("1000"),
																		loReturnIdsTable,
																		loReturnNamesTable,
																		loReturnMessageTable);												
						/////////////////////////////////////////////
						for (int kk = 0; kk < loReturnIdsTable.size(); kk++)
						{
							//Creating sub items
							QTreeWidgetItem *loItem1 = new QTreeWidgetItem();
							loItem1->setText(1,QString(loReturnIdsTable[kk].c_str()));
							loItem1->setText(2,QString(loReturnNamesTable[kk].c_str()));
							 
							//Adding the sub item
							loTreeWidgetItem->addChild(loItem1);
						}
					}
				}
			}	
		}
	}
	else
	{
		//Getting the objectsID's from the database
		vector<std::string> loReturnIds;
		vector<std::string> loReturnNames;
		QString loReturnMessage;

		meIceClientServer->getFromTableDatabaseGeneral( loSelectedClass,
														QString("0"),
														QString("1000"),
														loReturnIds,
														loReturnNames,
														loReturnMessage);
		/////////////////////////////////////////////		

		QTreeWidgetItem *loTreeWidgetItem = new QTreeWidgetItem(meTreeWidget);
		loTreeWidgetItem->setText(0, loSelectedClass);
			
		for (int i = 0; i < loReturnIds.size(); i++)
		{
			//Creating sub items
			QTreeWidgetItem *loItem1 = new QTreeWidgetItem();
			loItem1->setText(1,QString(loReturnIds[i].c_str()));
			loItem1->setText(2,QString(loReturnNames[i].c_str()));
			 
			//Adding the sub item
			loTreeWidgetItem->addChild(loItem1);
		}
	}

	  
    layout->addWidget(meTreeWidget, 0, 0);
	
    meButtons[0] = new QPushButton(QString("OK"));
    meButtons[1] = new QPushButton(QString("Cancel"));
    connect(meButtons[0], SIGNAL(clicked()), this, SLOT(slotButtonOKPressed()));
    connect(meButtons[1], SIGNAL(clicked()), this, SLOT(slotButtonCancelPressed()));
    layout->addWidget(meButtons[0], 1, 0);
    layout->addWidget(meButtons[1], 1, 1);

    meGridGroupBox->setLayout(layout);
}
void clListWidgetSelectObject::slotButtonOKPressed()
{
    try
    {
		//QList<QTreeWidgetItem *> loSelectedItems = meTreeWidget->selectedItems();
		//for (int i = 0; i <  loSelectedItems.size(); i++)
		//{
		//	cout << "PINHG" << endl;
		//	meValues[i] = new QString(((QTreeWidgetItem)loSelectedItems[i]).text(1));
		//				cout << meValues[i]->toStdString() << endl;
		//				cout << meTreeWidget->currentItem->
						
		//				cout << ((QTreeWidgetItem)loSelectedItems[i]).text(0).toStdString() << endl;
		//}
		if (meTreeWidget->currentItem() != NULL)
		{
			QTreeWidgetItem *loItem = meTreeWidget->currentItem();
			meValues[0] = new QString(loItem->text(1));
			//cout << loItem->text(1).toStdString() << endl;
		}
        this->done(0);
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);			
		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clNewObjectUI::slotButtonOKPressed() -> " + QString(e.what()));
    }
}

void clListWidgetSelectObject::slotButtonCancelPressed()
{
    try
    {
        this->done(1);
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);			
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clNewObjectUI::slotButtonCancelPressed() -> " + QString(e.what()));
    }
}


