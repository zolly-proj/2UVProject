#define INFO_BUFFER_SIZE 32767
#include "clParameterView.h"

clParameterView::clParameterView(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;

    meParameterView.setupUi(this);
	
	readMappingIcons();



    connect(meParameterView.btnRefresh, SIGNAL(clicked()),this,SLOT(slotButtonRefreshPressed()));
    connect(meParameterView.btnRead, SIGNAL(clicked()),this,SLOT(slotButtonReadPressed()));
    connect(meParameterView.btnWrite, SIGNAL(clicked()),this,SLOT(slotButtonWritePressed()));
    connect(meParameterView.btnMethod, SIGNAL(clicked()),this,SLOT(slotButtonMethodPressed()));
    connect(meParameterView.tvwObjects,SIGNAL(itemClicked(QTreeWidgetItem*, int)),this,SLOT(slotTreeClassItemPressed(QTreeWidgetItem*,int)));
    connect(meParameterView.lstProperties,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotListPropertieItemPressed(QListWidgetItem*)));
    connect(meParameterView.tvwObjects,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(slotTreeClassItemMenu(QTreeWidgetItem*,int)));
	

	
	
	

	
	meGraphWidget = new GraphWidget;
	meParameterView.horizontalLayout->addWidget(meGraphWidget);
	
}

clParameterView::~clParameterView()
{
}
void clParameterView::readMappingIcons()
{
	try
	{
		QFile inputFile(".\\ICONS\\mapping.txt");
		if (inputFile.open(QIODevice::ReadOnly))
		{
		   QTextStream in(&inputFile);
		   while (!in.atEnd())
		   {
			  QString line = in.readLine();
			  QStringList loElements = line.split(";");
			  if (loElements.size() != 0)
			  {
				  if (loElements.size() == 2)
				  {
					  meIcon.push_back(loElements.at(0));
					  meClass.push_back(loElements.at(1));
				  }
			  }
		   }
		   inputFile.close();
		}
	}
	catch(...)
	{
			cout << "Mapping problem ..." << endl;
	}
}
void clParameterView::slotButtonRefreshPressed()
{
    try
    {
        //*****************************
        //* Getting the tables *
        //********************************
        vector<std::string> loTables;
        QString loMessage;

        meIceClientServer->getAllTablesFromDatabase(loTables,loMessage);

        QString loMessageTable = QString("TABLE:");
        for(int i=0; i < loTables.size(); i++)
        {
            loMessageTable = loMessageTable + QString("[") + QString(loTables[i].c_str()) + QString("]");
        }
		
				// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);
		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotButtonRefreshPressed()" + loMessage);
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotButtonRefreshPressed()" + loMessageTable);


        //***********************************
        //* Fill listview with tables
        //**************************************
        if (!createClassesInTreeView(loTables))
        {
			// Get and display the name of the computer.
			TCHAR infoBuf[INFO_BUFFER_SIZE];
			DWORD  bufCharCount = INFO_BUFFER_SIZE;
			GetComputerName(infoBuf,&bufCharCount);
			
            meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","createClassesInTreeView(loTables) Could not create tables in treeview");
        }
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotButtonRefreshPressed() -> " + QString(e.what()));
    }
}

void clParameterView::slotButtonReadPressed()
{
    try
    {



    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotButtonRefreshPressed() -> " + QString(e.what()));
    }
}
void clParameterView::slotButtonWritePressed()
{
    try
    {


        QList<clObjectListItem *> loList;

        int loRows = meParameterView.lstProperties->count();
        for (int i = 0; i < loRows; i++)
        {
            loList << (clObjectListItem*) meParameterView.lstProperties->item(i);
        }


        vector<std::string> loColumns;
        vector<std::string> loValue;
        vector<std::string> loTypeValue;
        QString loTableName;
        QString loId;
        QString loReturnMessage;

        for (int i = 0; i < loList.size(); i++)
        {
            clObjectListItem *loObjectListItem = loList.at(i);

            if (!(loObjectListItem->getName().toUpper().compare(QString("PKEY")) == 0))
            {
                loColumns.push_back(string(loObjectListItem->getName().toUtf8()));
                loValue.push_back(string(loObjectListItem->getValue().toUtf8()));
                loTypeValue.push_back(string(loObjectListItem->getType().toUtf8()));
                loTableName = loObjectListItem->getTableName();
                loId = loObjectListItem->getId();
            }
        }


        meIceClientServer->updateIntoTableDatabase(     loTableName,
                                                        loId,
                                                        loColumns,
                                                        loValue,
                                                        loTypeValue,
                                                        loReturnMessage);
														
														
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotButtonWritePressed() -> " + loReturnMessage);

    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotButtonWritePressed() -> " + QString(e.what()));
    }
}
void clParameterView::slotButtonMethodPressed()
{
    try
    {

    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotButtonMethodPressed() -> " + QString(e.what()));
    }
}
void clParameterView::slotTreeClassItemPressed(QTreeWidgetItem *item, int index)
{
    try
    {
		clClassObjectTreeItem *loClassObjectTreeItem = (clClassObjectTreeItem *)item;
        //Class is pressed so get the objects
        if( item->parent() == NULL)
        {
            vector<std::string> loReturnIds;
			vector<std::string> loReturnNames;
            QString loReturnMessage;

            meIceClientServer->getFromTableDatabaseGeneral( item->text(0),
                                                            QString("0"),
                                                            QString("1000"),
                                                            loReturnIds,
															loReturnNames,
                                                            loReturnMessage);
			// Get and display the name of the computer.
			TCHAR infoBuf[INFO_BUFFER_SIZE];
			DWORD  bufCharCount = INFO_BUFFER_SIZE;
			GetComputerName(infoBuf,&bufCharCount);
			
            meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + loReturnMessage);

            addElementsToQTreeWidget(item, loReturnIds, loReturnNames);

        }
		else if (loClassObjectTreeItem->meIsInfo)
		{
				//Do nothing this is info
		}
        //Element pressed -> get all properties of the element
        else
        {
            vector<std::string> loPropertyName;
            vector<std::string> loReturnValues;
            QString loReturnMessageGetById;

            QTreeWidgetItem *loParent;
            loParent = item->parent();





			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////// Filling the list view with properties ////////////////////////////////////////////////////////////////
            clClassObjectTreeItem *loConvertedItem;
			clClassObjectTreeItem *loElement;
            loConvertedItem = (clClassObjectTreeItem *) loParent;
			loElement = (clClassObjectTreeItem *) item;
			
			
			
            //for (int i = 0; i < loConvertedItem->getDatabaseColumns().size(); i++)
			for (int i = 0; i < loElement->getDatabaseColumns().size(); i++)
            {
                //loPropertyName.push_back(string(loConvertedItem->getDatabaseColumns()[i].getName().toUtf8()));
				loPropertyName.push_back(string(loElement->getDatabaseColumns()[i].getName().toUtf8()));
            }
			
			bool loGetById = false;
            loGetById = meIceClientServer->getFromTableDatabaseById(    loElement->getClassName(),
																		loElement->meUUID,
																		loPropertyName,
																		loReturnValues,
																		loReturnMessageGetById);
			// Get and display the name of the computer.
			TCHAR infoBuf[INFO_BUFFER_SIZE];
			DWORD  bufCharCount = INFO_BUFFER_SIZE;
			GetComputerName(infoBuf,&bufCharCount);															
            meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + loReturnMessageGetById);
			
			if(!loGetById) return;
			if(loReturnValues.size() == 0)
			{
				QMessageBox msgBox;
				msgBox.setText("Item deleted in the mean time ...");
				msgBox.exec();
				return;
			} 
			
            fillListViewWithProperties(loPropertyName,loReturnValues,loConvertedItem,loElement);
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////// Filling the list view with methods ////////////////////////////////////////////////////////////////
            vector <std::string> loReturnIdActions;
			vector <std::string> loReturnNameActions;
			QString loReturnMessageActions;
			
			meIceClientServer->getFromTableDatabaseGeneral( QString("a_" + loElement->getClassName()),
															QString("0"),
															QString("1000"),
															loReturnIdActions,
															loReturnNameActions,
															loReturnMessageActions);
			meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + loReturnMessageActions);
			fillListViewWithActions(loReturnIdActions,loReturnNameActions,QString("a_" + loElement->getClassName()));
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////			
			
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////// Add Sub level /////////////////////////////////////////////////////////////////////
			//Remove the childeren bofore refressing
			loElement->takeChildren();
			
			

			vector<clDatabaseColumn> loDatabaseColumns = loElement->getDatabaseColumns();
			
			for (int l = 0; l < loDatabaseColumns.size(); l++)
			{

				clDatabaseColumn loDatabaseColumn = loDatabaseColumns.at(l);
				if (loDatabaseColumn.getReference().toLower().compare(QString("no")) == 0)
				{
					//Do nothing
				}
				else
				{
					//TODO implement
					QStringList loClasses = loDatabaseColumn.getReference().split("$;$");
					//if (loDatabaseColumn.getReference().toLower().compare(QString("all")) == 0)
					if (loClasses.size() > 1)
					{
						for (int t = 0; t < loClasses.size();t++)
						{
							if (loDatabaseColumn.getType().toLower().compare(QString("uuid[]")) == 0)
							{
								for (int y = 0; y < loPropertyName.size(); y++)
								{
									if (QString(loPropertyName.at(y).c_str()).toLower().compare(loDatabaseColumn.getName().toLower()) == 0)
									{
										//Add object as treeitem
										addElementsToQTreeWidgetSub(loElement, loReturnValues.at(y).c_str(),loClasses.at(t));
										
									}
								}
							}
							else if(loDatabaseColumn.getType().toLower().compare(QString("uuid")) == 0)
							{
								for (int y = 0; y < loPropertyName.size(); y++)
								{
									if (QString(loPropertyName.at(y).c_str()).toLower().compare(loDatabaseColumn.getName().toLower()) == 0)
									{
										//Add object as treeitem
										addElementToQTreeWidgetSub(loElement, loReturnValues.at(y).c_str(),loClasses.at(t));
									}
								}
							}	
						}	
					}
					else
					{
							if (loDatabaseColumn.getType().toLower().compare(QString("uuid[]")) == 0)
							{
								for (int y = 0; y < loPropertyName.size(); y++)
								{
									if (QString(loPropertyName.at(y).c_str()).toLower().compare(loDatabaseColumn.getName().toLower()) == 0)
									{
										//Add object as treeitem
										addElementsToQTreeWidgetSub(loElement, loReturnValues.at(y).c_str(),loDatabaseColumn.getReference());
										
									}
								}
							}
							else if(loDatabaseColumn.getType().toLower().compare(QString("uuid")) == 0)
							{
								for (int y = 0; y < loPropertyName.size(); y++)
								{
									if (QString(loPropertyName.at(y).c_str()).toLower().compare(loDatabaseColumn.getName().toLower()) == 0)
									{
										//Add object as treeitem
										addElementToQTreeWidgetSub(loElement, loReturnValues.at(y).c_str(),loDatabaseColumn.getReference());
										
									}
								}
							}							
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        }
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + QString(e.what()));
    }
}
void clParameterView::slotListPropertieItemPressed(QListWidgetItem *item)
{
    try
    {
        clObjectListItem *loObjectListItem;
        loObjectListItem = (clObjectListItem*) item;

        /*
        switch(string(loObjectListItem->getType().toUtf8()))
        {
            case "": break;
            case "": break;
            case "": break;
            case "": break;
            case "": break;
            case "": break;
            default: break;
        }
        */

        clChangePropertieText *loChangePropertieText;

        loChangePropertieText = new clChangePropertieText(meIceClientServer,meIceClientLogging,loObjectListItem,this);
        loChangePropertieText->exec();

        loObjectListItem->setText(loObjectListItem->getValue());
    }
    catch(exception &e)
    {
		
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotListPropertieItemPressed -> " + QString(e.what()));
    }
}

//Item Clicked
void clParameterView::slotTreeClassItemMenu(QTreeWidgetItem *item, int i)
{
    try
    {
        //Class is pressed so get the objects
        if( item->parent() == NULL)
        {
            //Getting the propertys
            clClassObjectTreeItem *loConvertedItemParent;
            loConvertedItemParent = (clClassObjectTreeItem *) item;





            QMenu menu(this);
            QAction *action=menu.addAction("NEW");
            /*
            QSignalMapper *loSignalMapper = new QSignalMapper(action);
            loSignalMapper->setMapping(action,loConvertedItemParent);
            connect(action,SIGNAL(triggered()),loSignalMapper,SLOT(map()));
            connect(loSignalMapper,SIGNAL(mapped(clClassObjectTreeItem*)),SLOT(slotNewElement(clClassObjectTreeItem*)));
            */





            QObject::connect(action, SIGNAL(triggered()),this,SLOT(slotNewElement()));
            menu.exec(QCursor::pos());
        }
        //Element pressed -> get all properties of the element
        else
        {
            /*
            clClassObjectTreeItem *loTableParent;
            loTableParent = (clClassObjectTreeItem *) item->parent();
            */
            QMenu loMenu(this);
			QAction *loEdit=loMenu.addAction("EDIT");
            QAction *loDelete=loMenu.addAction("DELETE");
			QAction *loClassProperties=loMenu.addAction("CLASS INFO");


            clClassObjectTreeItem *loConvertedItem;
            loConvertedItem = (clClassObjectTreeItem *) item->parent();

            vector <clDatabaseAction> loDatabaseAction = loConvertedItem->getDatabaseActions();
            QAction *loMethods[100];
            for (int i = 0; i < loDatabaseAction.size(); i ++)
            {
                loMethods[i] = loMenu.addAction(loDatabaseAction[i].getName());
            }
			QObject::connect(loEdit,SIGNAL(triggered()),this,SLOT(slotEditElement()));
            QObject::connect(loDelete,SIGNAL(triggered()),this,SLOT(slotDeleteElement()));
			QObject::connect(loClassProperties,SIGNAL(triggered()),this,SLOT(slotClassInfo()));
            loMenu.exec(QCursor::pos());
            /*
            QTreeWidgetItem *loParent;
            loParent = item->parent();

            //Getting the propertys
            clClassObjectTreeItem *loConvertedItem;
            loConvertedItem = (clClassObjectTreeItem *) loParent;
            */
        }
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotTreeClassItemMenu -> " + QString(e.what()));
    }
}
void clParameterView::slotClassInfo()
{
	//TODO
	// Get and display the name of the computer.
	TCHAR infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	GetComputerName(infoBuf,&bufCharCount);	
	
	try
	{
			//Class is pressed so get the objects
			QList<QTreeWidgetItem*> loList = meParameterView.tvwObjects->selectedItems();
			QTreeWidgetItem *loItem;
			
			int loRows = loList.count();
			for (int i = 0; i < loRows; i++)
			{
				
				loItem = loList.takeAt(i);
				break;
			}

			vector<std::string> loPropertyNames;
			vector<std::string> loReturnValues;
			QString loReturnMessageGetById;
			
			clClassObjectTreeItem *loConvertedItem;
			loConvertedItem = (clClassObjectTreeItem *) loItem;

			//Not on property
			if (loConvertedItem->meIsInfo) return;
			
			clClassInfo *loClassInfo = new clClassInfo(loConvertedItem->getClassName(),loConvertedItem->getDatabaseColumns());
			
			if (loClassInfo->exec() == 0)
			{
						
			}
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotClassInfo -> " + QString(e.what()));
    }			
			
			
		
	
	
}
void clParameterView::slotEditElement()
{
    try
    {
		//TODO
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);	
		
		
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList = meParameterView.tvwObjects->selectedItems();
        QTreeWidgetItem *item;
		
        int loRows = loList.count();
        for (int i = 0; i < loRows; i++)
        {
			
            item = loList.takeAt(i);
			break;
        }

		//************************************
		//* Getting the propertys
		//************************************
		QTreeWidgetItem *loParent;
		loParent = item->parent();
		clClassObjectTreeItem *loConvertedItem;
		clClassObjectTreeItem *loItem;
		loItem = (clClassObjectTreeItem *) item;
		loConvertedItem = (clClassObjectTreeItem *) loParent;
			
		if (loItem->meIsInfo) return;
		//for (int i = 0; i < loConvertedItem->getDatabaseColumns().size(); i++)
		//{
		//	loPropertyNames.push_back(string(loConvertedItem->getDatabaseColumns()[i].getName().toUtf8()));
		//}
		
			
		//*********************************
        //* Getting the table information *
        //*********************************
        vector<std::string> loPropertyName;
		vector<std::string> loAlias;
		vector<std::string> loType;
		vector<std::string> loExtra;
		vector<std::string> loReference;
        QString loMessage;
		meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotEditElement()-> Table name=" + loItem->getClassName());		
		meIceClientServer->getAllPropertiesFromTable(loItem->getClassName(),loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotEditElement()-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		for (int i = 0; i < loPropertyName.size(); i++)
		{
			meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotEditElement()-> Propertie name [" + QString(loPropertyName[i].c_str()) + "]");
			clDatabaseColumn test(								QString(loPropertyName[i].c_str()),
																QString(loAlias[i].c_str()),
																QString(loType[i].c_str()),
																QString(loExtra[i].c_str()),
																QString(loReference[i].c_str()));
																
			loDatabaseColumn.push_back(test);
		}
		
		
		/////////////////////////////////////
		// Getting the information content //
		/////////////////////////////////////
		vector<std::string> loReturnValues;
		QString loReturnMessageGetById;
		meIceClientServer->getFromTableDatabaseById(    loItem->getClassName(),
														loItem->meUUID,
														loPropertyName,
														loReturnValues,
														loReturnMessageGetById);
														
		meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotEditElement -> " + loReturnMessageGetById);
		
		
		
		
		/////////////////////////////////////
		
        clEditObjectUI *loEditObjectUI;
        loEditObjectUI = new clEditObjectUI(meIceClientServer,meIceClientLogging,loDatabaseColumn, loReturnValues);
		cout << "test" << endl;
        if (loEditObjectUI->exec() == 0)
		{
		

            QString loReturnMessage;

            vector<std::string> loColumns;
            vector<std::string> loValue;
            vector<std::string> loTypeValue;


            for (int j = 1; j < loDatabaseColumn.size();j++)
            {
                if(loDatabaseColumn[j].getName().toUpper().compare("PKEY") == 0)
                {

                }
                else
                {
                    loColumns.push_back(string(loDatabaseColumn[j].getName().toUtf8()));
                    loValue.push_back(string(loEditObjectUI->meValues[j]->toUtf8()));
                    loTypeValue.push_back(string(loDatabaseColumn[j].getType().toUtf8()));
                }
            }
			
			
	
			
			
			
			cout << "before update" << endl;
            meIceClientServer->updateIntoTableDatabase(    loItem->getClassName(),
															loItem->meUUID,
                                                            loColumns,
                                                            loValue,
                                                            loTypeValue,
                                                            loReturnMessage);
														
            meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotEditElement() -> " + loReturnMessage);
        }
        else
        {
			// Get and display the name of the computer.
			TCHAR infoBuf[INFO_BUFFER_SIZE];
			DWORD  bufCharCount = INFO_BUFFER_SIZE;
			GetComputerName(infoBuf,&bufCharCount);			
			
            meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotEditElement() -> update canceled");
        }


    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotEditElement -> " + QString(e.what()));
    }	
	
}
void clParameterView::slotDeleteElement()
{
    try
    {
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList = meParameterView.tvwObjects->selectedItems();
        clClassObjectTreeItem *loClassObjectTreeItem = NULL;
        QTreeWidgetItem *loTreeWidgetItem = NULL;

        int loRows = loList.count();
        for (int i = 0; i < loRows; i++)
        {
            loTreeWidgetItem = loList.takeAt(i);
        }

        loClassObjectTreeItem = (clClassObjectTreeItem*) loTreeWidgetItem->parent();
        QString loReturnMessage;

        meIceClientServer->deleteIntoTableDatabase(loClassObjectTreeItem->getClassName(),loClassObjectTreeItem->meUUID,loReturnMessage);
		
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotDeleteElement() -> " + loReturnMessage);

    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotDeleteElement -> " + QString(e.what()));
    }
}

void clParameterView::slotNewElement()
{
    try
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);	
		
		
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList = meParameterView.tvwObjects->selectedItems();
        clClassObjectTreeItem *loClassObjectTreeItem = NULL;

        int loRows = loList.count();
        for (int i = 0; i < loRows; i++)
        {
			
            loClassObjectTreeItem = (clClassObjectTreeItem*) loList.takeAt(i);
			//cout << string(loClassObjectTreeItem->text(i).toUtf8()) << endl;
			loClassObjectTreeItem->setClassName(loClassObjectTreeItem->text(0));
        }

        //cout << string(loClassObjectTreeItem->getName().toUtf8()) << endl;
        if (loClassObjectTreeItem == NULL) return;
		
		
		
		
		//*********************************
        //* Getting the table information *
        //*********************************
        vector<std::string> loPropertyName;
		vector<std::string> loAlias;
		vector<std::string> loType;
		vector<std::string> loExtra;
		vector<std::string> loReference;
        QString loMessage;
		meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotNewElement()-> Table name=" + loClassObjectTreeItem->getClassName());		
		meIceClientServer->getAllPropertiesFromTable(loClassObjectTreeItem->getClassName(),loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotNewElement()-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		for (int i = 0; i < loPropertyName.size(); i++)
		{
			meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotNewElement()-> Propertie name [" + QString(loPropertyName[i].c_str()) + "]");
			clDatabaseColumn test(								QString(loPropertyName[i].c_str()),
																QString(loAlias[i].c_str()),
																QString(loType[i].c_str()),
																QString(loExtra[i].c_str()),
																QString(loReference[i].c_str()));
																
			loDatabaseColumn.push_back(test);
		}
		/////////////////////////////////////
		
        clNewObjectUI *loNewObjectUI;
        loNewObjectUI = new clNewObjectUI(meIceClientServer,meIceClientLogging,loDatabaseColumn);
		
        if (loNewObjectUI->exec() == 0)
		{
		

            QString loReturnMessage;

            vector<std::string> loColumns;
            vector<std::string> loValue;
            vector<std::string> loTypeValue;


            for (int j = 1; j < loDatabaseColumn.size();j++)
            {
                if(loDatabaseColumn[j].getName().toUpper().compare("PKEY") == 0)
                {

                }
                else
                {
                    loColumns.push_back(string(loDatabaseColumn[j].getName().toUtf8()));
                    loValue.push_back(string(loNewObjectUI->meValues[j]->toUtf8()));
                    loTypeValue.push_back(string(loDatabaseColumn[j].getType().toUtf8()));
                    //cout << "----------" << endl;
                    //cout << string(loDatabaseColumn[j].getType().toUtf8()) << endl;
                    //cout << "----------" << endl;
                }
            }
            meIceClientServer->insertIntoTableDatabase(    loClassObjectTreeItem->getClassName(),
                                                            loColumns,
                                                            loValue,
                                                            loTypeValue,
                                                            loReturnMessage);
														
            meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotNewElement() -> " + loReturnMessage);
        }
        else
        {
			// Get and display the name of the computer.
			TCHAR infoBuf[INFO_BUFFER_SIZE];
			DWORD  bufCharCount = INFO_BUFFER_SIZE;
			GetComputerName(infoBuf,&bufCharCount);			
			
            meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotNewElement() -> insert canceled");
        }


    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotTreeClassItemMenu -> " + QString(e.what()));
    }
}
/********************************************
* User interface funcitons
************************************************/
bool clParameterView::createClassesInTreeView(vector<std::string> paTables)
{
    try
    {
        removeElementsFromTreeView();
        meParameterView.tvwObjects->setColumnCount(2);

        QStringList headers;
        headers << tr("Classes") << tr("Objects");
        meParameterView.tvwObjects->setHeaderLabels(headers);

        for (int i = 0; i < paTables.size();i++)
        {
            QString loTableName = QString(paTables[i].c_str());
            if (int j = loTableName.indexOf("a_") == 0)
            {
				// Get and display the name of the computer.
				TCHAR infoBuf[INFO_BUFFER_SIZE];
				DWORD  bufCharCount = INFO_BUFFER_SIZE;
				GetComputerName(infoBuf,&bufCharCount);				
				
                meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::createClassesInTreeView -> Not added table [" + loTableName + "]");
            }
            else
            {

                /*****************************************
                * Logic to get the info
                *******************************************/
                 vector<std::string> loPropertyName;
                 vector<std::string> loAlias;
                 vector<std::string> loType;
                 vector<std::string> loExtra;
                 vector<std::string> loReference;
                 QString loReturnMessageObject;


                 meIceClientServer->getAllPropertiesFromTable(  loTableName,
                                                                loPropertyName,
                                                                loAlias,
                                                                loType,
                                                                loExtra,
                                                                loReference,
                                                                loReturnMessageObject);
																
																
				// Get and display the name of the computer.
				TCHAR infoBuf[INFO_BUFFER_SIZE];
				DWORD  bufCharCount = INFO_BUFFER_SIZE;
				GetComputerName(infoBuf,&bufCharCount);
																
                meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + loReturnMessageObject);

                vector<clDatabaseColumn> loDatabaseColumnArr;
                for (int i=0; i< loPropertyName.size(); i++)
                {
                    clDatabaseColumn loDatabaseColumn(  QString(loPropertyName[i].c_str()),
                                                        QString(loAlias[i].c_str()),
                                                        QString(loType[i].c_str()),
                                                        QString(loExtra[i].c_str()),
                                                        QString(loReference[i].c_str()));
                    loDatabaseColumnArr.push_back(loDatabaseColumn);
                }


                vector<std::string> loMethodName;
                vector<std::string> loAliasName;
                vector<std::string> loSource;
                vector<std::string> loSourceName;
                vector<std::string> loReturnType;
                QString loReturnMessageMethod;

                meIceClientServer->getAllMethodsFromTable(      loTableName,
                                                                loMethodName,
                                                                loAliasName,
                                                                loSource,
                                                                loSourceName,
                                                                loReturnType,
                                                                loReturnMessageMethod);
				
                meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + loReturnMessageMethod);

                vector<clDatabaseAction> loDatabaseActionArr;
                for (int i=0; i < loMethodName.size(); i++)
                {
                    clDatabaseAction loDatabaseAction(  QString(loMethodName[i].c_str()),
                                                        QString(loAliasName[i].c_str()),
                                                        QString(loSource[i].c_str()),
                                                        QString(loSourceName[i].c_str()),
                                                        QString(loReturnType[i].c_str()));
                    loDatabaseActionArr.push_back(loDatabaseAction);
                }

                /*****************************************
                * End logic
                *******************************************/



                clClassObjectTreeItem *newItem;

                newItem = new clClassObjectTreeItem(meParameterView.tvwObjects, meParameterView.tvwObjects->currentItem());
                newItem->setDatabaseActions(loDatabaseActionArr);
                newItem->setDatabaseColumns(loDatabaseColumnArr);
                newItem->setClassName(loTableName);

                const QIcon Icon(".\\ICONS\\" + getRelatedIcon(loTableName));
                newItem->setIcon(0,Icon);
                newItem->setText(0,loTableName);
				
                meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::createClassesInTreeView -> Added table [" + loTableName + "]");
            }
        }


        return true;
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::createClassesInTreeView -> " + QString(e.what()));
        return false;
    }
}
QString clParameterView::getRelatedIcon(QString paClassName)
{
	if(meClass.size() > 0)
	{
		for (int i = 0; i < meClass.size(); i++)
		{
			if (paClassName.toUpper().compare(meClass.at(i)) == 0)
			{
					return meIcon.at(i);
			}
		}
		
	}
	return QString("");
}

bool clParameterView::removeElementsFromTreeView()
{
    try
    {
        QList<QTreeWidgetItem *> paColumnItems;

        for (int i = 0; i < meParameterView.tvwObjects->topLevelItemCount(); ++i)
        {
            paColumnItems << meParameterView.tvwObjects->topLevelItem(i);
        }
        qDeleteAll(paColumnItems.begin(),paColumnItems.end());
        return true;
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::removeElementsFromTreeView -> " + QString(e.what()));
        return false;
    }
}

bool clParameterView::addElementsToQTreeWidget(QTreeWidgetItem * paItem, vector<std::string> paReturnIds, vector<std::string> paReturnNames)
{
    try
    {
        paItem->takeChildren();
		clClassObjectTreeItem *loItem = (clClassObjectTreeItem*)paItem;

        for (int i = 0; i<paReturnIds.size();i++)
        {
            clClassObjectTreeItem *newItem;
            newItem = new clClassObjectTreeItem();
			newItem->setClassName(paItem->text(0));
			newItem->meUUID = QString(paReturnIds[i].c_str());
			newItem->meNameDisplay = QString(paReturnNames[i].c_str());
			newItem->setDatabaseActions(loItem->getDatabaseActions());
			newItem->setDatabaseColumns(loItem->getDatabaseColumns());
			
			const QIcon Icon(".\\ICONS\\" + getRelatedIcon(paItem->text(0)));
			newItem->setIcon(0,Icon);
			newItem->setText(0,QString(paReturnNames[i].c_str()));
            paItem->addChild(newItem);
        }
        return true;
    }
    catch(exception &e)
    {
		
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::addElementsToQTreeWidget -> " + QString(e.what()));
        return false;
    }
}
bool clParameterView::addElementToQTreeWidgetSub(QTreeWidgetItem * paParent, QString paID, QString paTable)
{
		// Get and display the name of the computer.
	TCHAR infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	GetComputerName(infoBuf,&bufCharCount);
    try
    {
		/*****************************************
		* Getting the table information for the object
		*******************************************/
		 vector<std::string> loPropertyName;
		 vector<std::string> loAlias;
		 vector<std::string> loType;
		 vector<std::string> loExtra;
		 vector<std::string> loReference;
		 QString loReturnMessageObject;


		 meIceClientServer->getAllPropertiesFromTable(  paTable,
														loPropertyName,
														loAlias,
														loType,
														loExtra,
														loReference,
														loReturnMessageObject);
														
		meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::addElementToQTreeWidgetSub -> " + loReturnMessageObject);

		vector<clDatabaseColumn> loDatabaseColumnArr;
		for (int i=0; i< loPropertyName.size(); i++)
		{
			clDatabaseColumn loDatabaseColumn(  QString(loPropertyName[i].c_str()),
												QString(loAlias[i].c_str()),
												QString(loType[i].c_str()),
												QString(loExtra[i].c_str()),
												QString(loReference[i].c_str()));
			loDatabaseColumnArr.push_back(loDatabaseColumn);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		
		
		
		clClassObjectTreeItem *loItem = (clClassObjectTreeItem*)paParent;

		clClassObjectTreeItem *newItem;
		newItem = new clClassObjectTreeItem();
		newItem->setClassName(paTable);
		newItem->meUUID = paID;
		
		vector<std::string> loPropertyNames;
		vector<std::string> loReturnValues;
		QString loReturnMessageGetById;
		loPropertyNames.push_back("name");	
		meIceClientServer->getFromTableDatabaseById(paTable,
													paID,
													loPropertyNames,
													loReturnValues,
													loReturnMessageGetById);
		meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::addElementToQTreeWidgetSub -> " + loReturnMessageGetById);
		
		for (int j = 0; j < loPropertyNames.size(); j++)
		{
			QString loReturnValue = loReturnValues.at(j).c_str();
			if(QString(loPropertyNames.at(j).c_str()).toLower().compare("name") == 0)
			{
				newItem->meNameDisplay = loReturnValue; 
			}
		}
		//newItem->setDatabaseActions(loItem->getDatabaseActions());
		newItem->setDatabaseColumns(loDatabaseColumnArr);
		const QIcon Icon(".\\ICONS\\" + getRelatedIcon(paTable));
		newItem->setIcon(0,Icon);
		newItem->setText(0,newItem->meNameDisplay);
		paParent->addChild(newItem);
       
        return true;
    }
    catch(exception &e)
    {
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::addElementsToQTreeWidget -> " + QString(e.what()));
        return false;
    }
}
bool clParameterView::addElementsToQTreeWidgetSub(QTreeWidgetItem * paParent, QString paID, QString paTable)
{
	// Get and display the name of the computer.
	TCHAR infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	GetComputerName(infoBuf,&bufCharCount);	
    try
    {
		clClassObjectTreeItem *loItem = (clClassObjectTreeItem*)paParent;

		QStringList loElements = paID.remove("}").remove("{").split(",", QString::SkipEmptyParts);


		clClassObjectTreeItem *infoItem;
        infoItem = new clClassObjectTreeItem();
		infoItem->meIsInfo = true;
		infoItem->setText(0,paTable);
        paParent->addChild(infoItem);

        for (int i = 0; i< loElements.size();i++)
        {
			
			
			/*****************************************
			* Getting the table information for the object
			*******************************************/
			 vector<std::string> loPropertyName;
			 vector<std::string> loAlias;
			 vector<std::string> loType;
			 vector<std::string> loExtra;
			 vector<std::string> loReference;
			 QString loReturnMessageObject;


			 meIceClientServer->getAllPropertiesFromTable(  paTable,
															loPropertyName,
															loAlias,
															loType,
															loExtra,
															loReference,
															loReturnMessageObject);
															
			meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::addElementsToQTreeWidgetSub -> " + loReturnMessageObject);

			vector<clDatabaseColumn> loDatabaseColumnArr;
			for (int j=0; j< loPropertyName.size(); j++)
			{
				clDatabaseColumn loDatabaseColumn(  QString(loPropertyName[j].c_str()),
													QString(loAlias[j].c_str()),
													QString(loType[j].c_str()),
													QString(loExtra[j].c_str()),
													QString(loReference[j].c_str()));
				loDatabaseColumnArr.push_back(loDatabaseColumn);
			}
			
            clClassObjectTreeItem *newItem;
            newItem = new clClassObjectTreeItem();
			newItem->setClassName(paTable);
			newItem->meUUID = loElements.at(i);
			
			vector<std::string> loPropertyNames;
			vector<std::string> loReturnValues;
			QString loReturnMessageGetById;
			loPropertyNames.push_back("name");

			bool loGetID = false;
			loGetID = meIceClientServer->getFromTableDatabaseById(paTable,
														QString(loElements.at(i)),
														loPropertyNames,
														loReturnValues,
														loReturnMessageGetById);
			meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::addElementsToQTreeWidgetSub -> " + loReturnMessageGetById);
			
			if (!loGetID) return false;
			
			
			
			
			
			for (int j = 0; j < loReturnValues.size(); j++)
			{
				QString loReturnValue = loReturnValues.at(j).c_str();
				if(QString(loPropertyNames.at(j).c_str()).toLower().compare("name") == 0)
				{
					newItem->meNameDisplay = loReturnValue;
					break;
				}
			}
			//newItem->setDatabaseActions(loItem->getDatabaseActions());
			newItem->setDatabaseColumns(loDatabaseColumnArr);
			
			const QIcon Icon(".\\ICONS\\" + getRelatedIcon(paTable));
			newItem->setIcon(0,Icon);
			newItem->setText(0,newItem->meNameDisplay);
			
			//Check if an element is Queryed
			if (loReturnValues.size() >= 1)
			{
				infoItem->addChild(newItem);
			}
            
        }
        return true;
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::addElementsToQTreeWidget -> " + QString(e.what()));
        return false;
    }
}
bool clParameterView::fillListViewWithProperties(vector<std::string> paPropertyName, vector<std::string> paPropertyValue, clClassObjectTreeItem *paConvertedItem, clClassObjectTreeItem * paObject)
{
    try
    {
        //Remove previous items form the list
        int loItems = meParameterView.lstProperties->count();

        while (loItems > 0)
        {
            meParameterView.lstProperties->takeItem(0);
            loItems = meParameterView.lstProperties->count();
        }

        //Add new items to the list
        vector<clDatabaseColumn> loDatabaseColumn = paObject->getDatabaseColumns();

        for (int i = 0; i < paPropertyName.size(); i++)
        {
            for (int j=0; j < loDatabaseColumn.size(); j++)
            {
                //Get the right property
                QString loPropertyName = QString(paPropertyName[i].c_str());
                //cout << paPropertyName[i] << endl;
                if (loPropertyName.toUpper().compare(loDatabaseColumn[j].getName().toUpper()) == 0)
                {

                    clObjectListItem *loNewItem;

                    const QIcon Icon(".\\ICONS\\prop.ico");
                    loNewItem = new clObjectListItem(Icon,QString(paPropertyValue[i].c_str()));
					loNewItem->setIcon(Icon);
                    loNewItem->setName(QString(paPropertyName[i].c_str()));
                    loNewItem->setAlias(loDatabaseColumn[j].getAlias());
                    loNewItem->setType(loDatabaseColumn[j].getType());
                    loNewItem->setExtra(loDatabaseColumn[j].getExtra());
                    loNewItem->setReference(loDatabaseColumn[j].getReference());
                    loNewItem->setValue(QString(paPropertyValue[i].c_str()));
                    loNewItem->setTableName(paObject->getClassName());
                    loNewItem->setId(paObject->meUUID);

                    meParameterView.lstProperties->addItem(loNewItem);


                }
            }
        }

        return true;
    }
    catch(exception &e)
    {
		
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::fillListViewWithProperties -> " + QString(e.what()));
        return false;
    }
}
bool clParameterView::fillListViewWithActions(vector<std::string> paActionIDs, vector <std::string> paActionNames, QString paClassName)
{
			
	// Get and display the name of the computer.
	TCHAR infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	GetComputerName(infoBuf,&bufCharCount);		
	
	try
	{
		removeElementsFromActions();
		
		QStringList loColumnNames;
		loColumnNames << "action_name" << "action_alias" << "action_source" << "action_sourcename" << "action_returntype";
		meParameterView.tvwActions->setHeaderLabels(loColumnNames);		
		
		for (int i = 0; i < paActionIDs.size(); ++i) 
		{
			
			vector<std::string> loPropertyNames;
			vector<std::string> loReturnValues;
			QString loReturnMessageGetById;
			loPropertyNames.push_back("action_name");	
			loPropertyNames.push_back("action_alias");	
			loPropertyNames.push_back("action_source");	
			loPropertyNames.push_back("action_sourcename");	
			loPropertyNames.push_back("action_returntype");	
			
			meIceClientServer->getFromTableDatabaseById(paClassName,
														QString(paActionIDs.at(i).c_str()),
														loPropertyNames,
														loReturnValues,
														loReturnMessageGetById);
			meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::fillListViewWithActions -> " + loReturnMessageGetById);	
			
			
			
				
			clClassObjectTreeItem *newItem;
			newItem = new clClassObjectTreeItem(meParameterView.tvwActions, meParameterView.tvwActions->currentItem());
			newItem->meUUID = QString(paActionIDs.at(i).c_str());
			const QIcon Icon(".\\ICONS\\" + getRelatedIcon(paClassName));
			newItem->setIcon(0,Icon);
			newItem->setText(0,QString(loReturnValues.at(0).c_str()));	
			newItem->setText(1,QString(loReturnValues.at(1).c_str()));	
			newItem->setText(2,QString(loReturnValues.at(2).c_str()));	
			newItem->setText(3,QString(loReturnValues.at(3).c_str()));	
			newItem->setText(4,QString(loReturnValues.at(4).c_str()));
		}			
		

        return true;
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::fillListViewWithActions -> " + QString(e.what()));
        return false;
    }	
	
}
bool clParameterView::removeElementsFromActions()
{
    try
    {
        QList<QTreeWidgetItem *> paColumnItems;

        for (int i = 0; i < meParameterView.tvwActions->topLevelItemCount(); ++i)
        {
            paColumnItems << meParameterView.tvwActions->topLevelItem(i);
        }
        qDeleteAll(paColumnItems.begin(),paColumnItems.end());
        return true;
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::removeElementsFromActions -> " + QString(e.what()));
        return false;
    }
}
