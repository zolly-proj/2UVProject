#define INFO_BUFFER_SIZE 32767
#include "clParameterView.h"

clParameterView::clParameterView(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clClassLoader *paClassLoader, QMutex * paLock, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;

    meParameterView.setupUi(this);
	
	meClassLoader = paClassLoader;
	
	meLock = paLock;
	
	readMappingIcons();



    connect(meParameterView.btnRefresh, SIGNAL(clicked()),this,SLOT(slotButtonRefreshPressed()));
    connect(meParameterView.btnRead, SIGNAL(clicked()),this,SLOT(slotButtonReadPressed()));
    connect(meParameterView.btnWrite, SIGNAL(clicked()),this,SLOT(slotButtonWritePressed()));
    connect(meParameterView.btnMethod, SIGNAL(clicked()),this,SLOT(slotButtonMethodPressed()));
    connect(meParameterView.tvwObjects,SIGNAL(itemClicked(QTreeWidgetItem*, int)),this,SLOT(slotTreeClassItemPressed(QTreeWidgetItem*,int)));
    connect(meParameterView.lstProperties,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotListPropertieItemPressed(QListWidgetItem*)));
    connect(meParameterView.tvwObjects,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(slotTreeClassItemMenu(QTreeWidgetItem*,int)));
	

	
	
	

	
	//meGraphWidget = new GraphWidget;
	//meParameterView.horizontalLayout->addWidget(meGraphWidget);
	
}

clParameterView::~clParameterView()
{
}
void clParameterView::readMappingIcons()
{
	try
	{
		QFile inputFile("./ICONS/mapping.txt");
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
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotButtonRefreshPressed()" + loMessage);
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotButtonRefreshPressed()" + loMessageTable);


        //***********************************
        //* Fill listview with tables
        //**************************************
        if (!createClassesInTreeView(loTables))
        {
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createClassesInTreeView(loTables) Could not create tables in treeview");
        }
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotButtonRefreshPressed() -> " + QString(e.what()));
    }
}
void clParameterView::slotButtonReadPressed()
{
    try
    {
    }
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotButtonReadPressed() -> " + QString(e.what()));
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
														

        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotButtonWritePressed() -> " + loReturnMessage);

    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotButtonWritePressed() -> " + QString(e.what()));
    }
}
void clParameterView::slotButtonMethodPressed()
{
    try
    {

    }
    catch(exception &e)
    {

        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotButtonMethodPressed() -> " + QString(e.what()));
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
			QString loStart = QString("0");
			QString loStop = QString("0");
			QString loClassName = item->text(0);
            meIceClientServer->getFromTableDatabaseGeneral( loClassName,
                                                            loStart,
                                                            loStop,
                                                            loReturnIds,
															loReturnNames,
                                                            loReturnMessage);

            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + loReturnMessage);
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

			QString loClassName = loElement->getClassName();
			QString loUUID = loElement->meUUID;

            loGetById = meIceClientServer->getFromTableDatabaseById(    loClassName,
																		loUUID,
																		loPropertyName,
																		loReturnValues,
																		loReturnMessageGetById);

            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + loReturnMessageGetById);
			
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
			

			QString loClassNameTemp = QString("a_" + loElement->getClassName());
			QString loStart = QString("0");
			QString loStop = QString("1000");
			meIceClientServer->getFromTableDatabaseGeneral( loClassNameTemp,
															loStart,
															loStop,
															loReturnIdActions,
															loReturnNameActions,
															loReturnMessageActions);
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + loReturnMessageActions);
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
                        //for (int t = 0; t < loClasses.size();t++)
                        //{
							if (loDatabaseColumn.getType().toLower().compare(QString("uuid[]")) == 0)
							{
								for (int y = 0; y < loPropertyName.size(); y++)
								{
									if (QString(loPropertyName.at(y).c_str()).toLower().compare(loDatabaseColumn.getName().toLower()) == 0)
									{

                                        if (QString(loReturnValues.at(y).c_str()).trimmed().compare("") != 0)
                                        {
                                            QStringList loUUids = QString(loReturnValues.at(y).c_str()).split("$;$");
                                            if (loUUids.size() > 0)
                                            {
                                                for(int k = 0; k < loUUids.size();k++)
                                                {
                                                    //Add object as treeitem
                                                    if (loUUids.at(k).trimmed().compare("") != 0)
                                                        addElementsToQTreeWidgetSub(loElement, loUUids.at(k),loClasses.at(0));
                                                }
                                            }
                                        }

									}
								}
                            }
                        //}
					}
                    else if (loClasses.size() == 1)
					{
                            if(loDatabaseColumn.getType().toLower().compare(QString("uuid")) == 0)
							{
								for (int y = 0; y < loPropertyName.size(); y++)
								{
									if (QString(loPropertyName.at(y).c_str()).toLower().compare(loDatabaseColumn.getName().toLower()) == 0)
									{
										//Add object as treeitem
                                        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> testing" + QString(loReturnValues.at(y).c_str()));
                                        if (QString(loReturnValues.at(y).c_str()).trimmed().compare("") != 0)
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
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + QString(e.what()));
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
		

        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotListPropertieItemPressed -> " + QString(e.what()));
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
            /* General actions */
            QMenu loMenu(this);
			QAction *loEdit=loMenu.addAction("EDIT");
            QAction *loDelete=loMenu.addAction("DELETE");
			QAction *loClassProperties=loMenu.addAction("CLASS INFO");

			QObject::connect(loEdit,SIGNAL(triggered()),this,SLOT(slotEditElement()));
            QObject::connect(loDelete,SIGNAL(triggered()),this,SLOT(slotDeleteElement()));
			QObject::connect(loClassProperties,SIGNAL(triggered()),this,SLOT(slotClassInfo()));

			/* separator */
			QAction *separator01 = new QAction(this);
			separator01->setSeparator(true);
			loMenu.addAction(separator01);
			
			
			/*Python methods */
            clClassObjectTreeItem *loConvertedItem;
            loConvertedItem = (clClassObjectTreeItem *) item->parent();

            vector <clDatabaseAction> loDatabaseAction = loConvertedItem->getDatabaseActions();
            QAction *loMethods[200];			
			QSignalMapper* signalMapper = new QSignalMapper(this);
    		
            for (int i = 0; i < loDatabaseAction.size(); i ++)
            {
                loMethods[i] = loMenu.addAction(loDatabaseAction[i].getName());
				QObject::connect(loMethods[i], SIGNAL(triggered()), signalMapper, SLOT(map()));
				signalMapper->setMapping(loMethods[i], loDatabaseAction[i].getName());
            }
			connect(signalMapper, SIGNAL(mapped(QString)),this, SLOT(slotCallAction(QString)));
			
			/* separator */			
			QAction *separator02 = new QAction(this);
			separator02->setSeparator(true);
			loMenu.addAction(separator02);
			
			/*General methods */
            QAction *loMethods02[500];			
			QSignalMapper* signalMapper02 = new QSignalMapper(this);
    		
            for (int i = 0; i < meClassLoader->meMethodsGeneralNames.size(); i ++)
            {
                loMethods02[i] = loMenu.addAction(meClassLoader->meMethodsGeneralNames.at(i));
				QObject::connect(loMethods02[i], SIGNAL(triggered()), signalMapper02, SLOT(map()));
				signalMapper02->setMapping(loMethods02[i], meClassLoader->meMethodsGeneralNames.at(i));
            }
			connect(signalMapper02, SIGNAL(mapped(QString)),this, SLOT(slotCallMethod(QString)));            
			
			/* separator */			
			QAction *separator03 = new QAction(this);
			separator03->setSeparator(true);
			loMenu.addAction(separator03);			
			
			
			/*Class methods */
			//Getting the class name
            clClassObjectTreeItem *loConvertedItemClassMethod;
            loConvertedItemClassMethod = (clClassObjectTreeItem *) item;
			
			QAction *loMethods03[500];			
			QSignalMapper* signalMapper03 = new QSignalMapper(this);
			
			for (int i = 0; i < meClassLoader->meClassNames.size(); i++)
			{
				if (meClassLoader->meClassNames.at(i).toUpper().compare(loConvertedItemClassMethod->getClassName().toUpper()) == 0)
				{
					for (int j = 0; j < meClassLoader->meClassMethods.at(i).size(); j++)
					{
						loMethods03[i] = loMenu.addAction(meClassLoader->meClassMethods.at(i).at(j));
						QObject::connect(loMethods03[i], SIGNAL(triggered()), signalMapper03, SLOT(map()));
						signalMapper03->setMapping(loMethods03[i], meClassLoader->meClassMethods.at(i).at(j));
					}
					connect(signalMapper03, SIGNAL(mapped(QString)),this, SLOT(slotCallClassMethod(QString)));            
					break;
				}
			}
			
			
			loMenu.exec(QCursor::pos());
        }
    }
    catch(exception &e)
    {

        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotTreeClassItemMenu -> " + QString(e.what()));
    }
}
void clParameterView::slotCallAction(const QString &paActionName)
{
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
			
			vector <clDatabaseAction> loDatabaseActions = loConvertedItem->getDatabaseActions();
			
			if (loDatabaseActions.size() < 1) return;
			
			for (int i = 0; i < loDatabaseActions.size(); i++)
			{
				clDatabaseAction loDatabaseAction = loDatabaseActions.at(i);
				if (loDatabaseAction.getName().toUpper().compare(paActionName.toUpper()) == 0)
				{
					if (loDatabaseAction.getSource().toUpper().compare(QString("DLL")) == 0)
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotCallAction -> Source DLL called ...");
						//Call dll action
						callRoutineDLL(loConvertedItem->meUUID,loDatabaseAction.getSourceName(), loConvertedItem->getClassName());
					}
					else if (loDatabaseAction.getSource().toUpper().compare(QString("PYTHON")) == 0)
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotCallAction -> Source python called ...");
						//Call Python action
						//TODO implement WVA if debug
						callRoutinePythonNoDebug(loConvertedItem->meUUID, loDatabaseAction.getSourceName(), loConvertedItem->getClassName(), loDatabaseAction.getReturnType());
					}
					else
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotCallAction -> Source for method not supported ...");
					}
					break;
				}
			}
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotCallAction -> " + QString(e.what()));
    }	
}
//General method call
void clParameterView::slotCallMethod(const QString &paActionName)
{
	
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
			
			
			//Get the method
			for (int i = 0; i < meClassLoader->meMethodsGeneralNames.size(); i++)
			{
				if (meClassLoader->meMethodsGeneralNames.at(i).toUpper().compare(paActionName.toUpper()) == 0)
				{
					vector <QString> loCurrentMethodArgType;
					loCurrentMethodArgType.push_back(QString("CLASS"));
					loCurrentMethodArgType.push_back(QString("uuid"));
					vector <QString> loCurrentMethodArgValue;
					loCurrentMethodArgValue.push_back(loConvertedItem->getClassName());
					loCurrentMethodArgValue.push_back(loConvertedItem->meUUID);
					meClassLoader->meMethodCalls.at(i)->doMethod(loCurrentMethodArgType, loCurrentMethodArgValue);
					break;
				}
			}
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotCallMethod -> " + QString(e.what()));
    }	
}
void clParameterView::slotCallClassMethod(const QString &paActionName)
{
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
			
			
			for (int i = 0; i < meClassLoader->meClassNames.size(); i++)
			{
				if (meClassLoader->meClassNames.at(i).toUpper().compare(loConvertedItem->getClassName().toUpper()) == 0)
				{
					for (int j = 0; j < meClassLoader->meClassMethods.at(i).size(); j++)
					{
						if(paActionName.toUpper().compare(meClassLoader->meClassMethods.at(i).at(j).toUpper()) == 0)
						{
							//TODO create input screen
							//bool doMethod(QString paMethodName, const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue, const vector <QString> &paLogExp) override;
							vector <QString> loParametersType;
							vector <QString> loParameters;
							vector <QString> loParametersValue;
							vector <QString> loLogExp;
							
							loParametersType.push_back("CLASS");
							loParameters.push_back(loConvertedItem->getClassName());
							loParametersValue.push_back(loConvertedItem->getClassName());
							
							loParametersType.push_back("UUID");
							loParameters.push_back("PKEY");
							loParametersValue.push_back(loConvertedItem->meUUID);							
							
							meClassLoader->meObjectCalls.at(i)->doMethod(paActionName, loParametersType, loParameters, loParametersValue, loLogExp);
							break;	
						}
					}
					break;
				}
			}			
			
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotCallClassMethod -> " + QString(e.what()));
    }	
}
void clParameterView::slotClassInfo()
{

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
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotClassInfo -> " + QString(e.what()));
    }			
			
			
		
	
	
}
void clParameterView::slotEditElement()
{
    try
    {		
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
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotEditElement()-> Table name=" + loItem->getClassName());
		meIceClientServer->getAllPropertiesFromTable(loItem->getClassName(),loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotEditElement()-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		for (int i = 0; i < loPropertyName.size(); i++)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotEditElement()-> Propertie name [" + QString(loPropertyName[i].c_str()) + "]");
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

		QString loClassNameTemp = loItem->getClassName();
		QString loUUIDTemp = loItem->meUUID;
		meIceClientServer->getFromTableDatabaseById(    loClassNameTemp,
														loUUIDTemp,
														loPropertyName,
														loReturnValues,
														loReturnMessageGetById);
														
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotEditElement -> " + loReturnMessageGetById);
		
		
		
		
		/////////////////////////////////////
		
        clEditObjectUI *loEditObjectUI;
        loEditObjectUI = new clEditObjectUI(meIceClientServer,meIceClientLogging,loDatabaseColumn, loReturnValues);
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
                    loValue.push_back(string(loEditObjectUI->meValues[j]->toUtf8().constData()));
                    loTypeValue.push_back(string(loDatabaseColumn[j].getType().toUtf8()));
                }
            }

            QString loClassNameTemp = loItem->getClassName();
			QString loUUIDTemp = loItem->meUUID;
            meIceClientServer->updateIntoTableDatabase(    loClassNameTemp,
															loUUIDTemp,
                                                            loColumns,
                                                            loValue,
                                                            loTypeValue,
                                                            loReturnMessage);
														
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotEditElement() -> " + loReturnMessage);
        }
        else
        {
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotEditElement() -> update canceled");
        }
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotEditElement -> " + QString(e.what()));
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

		//Get the class object
        loClassObjectTreeItem = (clClassObjectTreeItem*) loTreeWidgetItem->parent();		
		
		//Delete the object
		clObject loObject = meClassLoader->getObject(loClassObjectTreeItem->getClassName().toUpper());
		loObject.ObjectId = ((clClassObjectTreeItem*)loTreeWidgetItem)->meUUID;
		loObject.deleteObject();
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotDeleteElement -> " + QString(e.what()));
    }
}

void clParameterView::slotNewElement()
{
    try
    {
	
		
		
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
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotNewElement()-> Table name=" + loClassObjectTreeItem->getClassName());
		meIceClientServer->getAllPropertiesFromTable(loClassObjectTreeItem->getClassName(),loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotNewElement()-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		for (int i = 0; i < loPropertyName.size(); i++)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotNewElement()-> Propertie name [" + QString(loPropertyName[i].c_str()) + "]");
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
            vector<QString> loColumns;
            vector<QString> loValue;
            vector<QString> loTypeValue;
			
            for (int j = 0; j < loDatabaseColumn.size();j++)
            {
                if(loDatabaseColumn[j].getName().toUpper().compare("PKEY") == 0)
                {
					//DO NOTHING
                }
                else
                {
                    loColumns.push_back(loDatabaseColumn[j].getName());
                    loValue.push_back(QString(string(loNewObjectUI->meValues[j]->toUtf8().constData()).c_str()));
                    loTypeValue.push_back(loDatabaseColumn[j].getType());
                }
            }
			
			clObject loObject = meClassLoader->getObject(loClassObjectTreeItem->getClassName());
            //Set the class name on the object
            loObject.ClassName = loClassObjectTreeItem->getClassName();
			loObject.insertObject(loColumns,loValue,loTypeValue);										
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotNewElement() -> " + loObject.ObjectId);
        }
        else
        {
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotNewElement() -> insert canceled");
        }
		
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotNewElement() -> " + QString(e.what()));
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
			/*
            if (int j = loTableName.indexOf("a_") == 0)
            {
				// Get and display the name of the computer.
				TCHAR infoBuf[INFO_BUFFER_SIZE];
				DWORD  bufCharCount = INFO_BUFFER_SIZE;
				GetComputerName(infoBuf,&bufCharCount);				
				
                meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::createClassesInTreeView -> Not added table [" + loTableName + "]");
            }
            else
            {
			*/
                /*****************************************
                * Get the table info
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
																
																
																
                meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::createClassesInTreeView -> " + loReturnMessageObject);

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


				/***********************************************
				* Get the actions from the database
				**********************************************************/
				vector<clDatabaseAction> loDatabaseActionArr;
				if (loTableName.toUpper().indexOf("A_") == 0)
				{
					//Do not get the methods if it is a method table
				}
				else
				{ 

					vector <std::string> loReturnIdActions;
					vector <std::string> loReturnNameActions;
					QString loReturnMessageActions;
					
					QString loTableNameTemp = QString("a_" + loTableName);
					QString loStart = QString("0");
					QString loStop = QString("1000");


					meIceClientServer->getFromTableDatabaseGeneral( loTableNameTemp,
																	loStart,
																	loStop,
																	loReturnIdActions,
																	loReturnNameActions,
																	loReturnMessageActions);
					meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::createClassesInTreeView -> " + loReturnMessageActions);
					
					for (int k = 0; k < loReturnIdActions.size(); k++)
					{
						vector<std::string> loPropertyNames;
						vector<std::string> loReturnValues;
						QString loReturnMessageGetById;
						loPropertyNames.push_back("PKEY");	
						loPropertyNames.push_back("ACTION_NAME");	
						loPropertyNames.push_back("ACTION_ALIAS");	
						loPropertyNames.push_back("ACTION_SOURCE");	
						loPropertyNames.push_back("ACTION_SOURCENAME");	
						loPropertyNames.push_back("ACTION_METHODNAME");	


						QString loTableNameTemp = QString("a_" + loTableName);
						QString loReturnIdActionsTemp = QString(loReturnIdActions.at(k).c_str());


						meIceClientServer->getFromTableDatabaseById(loTableNameTemp,
													loReturnIdActionsTemp,
													loPropertyNames,
													loReturnValues,
													loReturnMessageGetById);
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::createClassesInTreeView -> " + loReturnMessageGetById);
						
						if(loReturnValues.size() > 0)
						{
							QString loMethodName;
							QString loMethodAlias;
							QString loMethodSource;
							QString loMethodSourceName;
							QString loMethodReturnType;							
							for (int kk = 0; kk < loPropertyNames.size(); kk++)
							{

								if (QString(loPropertyNames.at(kk).c_str()).toUpper().compare(QString("PKEY")) == 0)
								{
									//DO NOTHING
								}
								else if (QString(loPropertyNames.at(kk).c_str()).toUpper().compare(QString("ACTION_NAME")) == 0)
								{
									loMethodName = QString(loReturnValues.at(kk).c_str());
								}
								else if (QString(loPropertyNames.at(kk).c_str()).toUpper().compare(QString("ACTION_ALIAS")) == 0)
								{
									loMethodAlias = QString(loReturnValues.at(kk).c_str());
								}
								else if (QString(loPropertyNames.at(kk).c_str()).toUpper().compare(QString("ACTION_SOURCE")) == 0)
								{
									loMethodSource = QString(loReturnValues.at(kk).c_str());
								}
								else if (QString(loPropertyNames.at(kk).c_str()).toUpper().compare(QString("ACTION_SOURCENAME")) == 0)
								{
									loMethodSourceName = QString(loReturnValues.at(kk).c_str());
								}										
								else if (QString(loPropertyNames.at(kk).c_str()).toUpper().compare(QString("ACTION_RETURNTYPE")) == 0)
								{
									loMethodReturnType = QString(loReturnValues.at(kk).c_str());
								}								
							}
							clDatabaseAction loDatabaseAction(  loMethodName,
																loMethodAlias,
																loMethodSource,
																loMethodSourceName,
																loMethodReturnType);
							loDatabaseActionArr.push_back(loDatabaseAction);													
						}
					}
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
				
                meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::createClassesInTreeView -> Added table [" + loTableName + "]");
			/*
            }
			*/
		
        }
		

        return true;
    }
    catch(exception &e)
    {
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::createClassesInTreeView -> " + QString(e.what()));
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
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::removeElementsFromTreeView -> " + QString(e.what()));
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
		
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::addElementsToQTreeWidget -> " + QString(e.what()));
        return false;
    }
}
bool clParameterView::addElementToQTreeWidgetSub(QTreeWidgetItem * paParent, QString paID, QString paTable)
{
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
														
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::addElementToQTreeWidgetSub -> " + loReturnMessageObject);

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
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::addElementToQTreeWidgetSub -> " + loReturnMessageGetById);
		
		for (int j = 0; j < loPropertyNames.size(); j++)
		{
			QString loReturnValue = loReturnValues.at(j).c_str();
			if(QString(loPropertyNames.at(j).c_str()).toLower().compare("name") == 0)
			{
				newItem->meNameDisplay = loReturnValue; 
			}
		}
        newItem->meUUID = paID;
		newItem->setDatabaseActions(loItem->getDatabaseActions());
		newItem->setDatabaseColumns(loDatabaseColumnArr);
		const QIcon Icon(".\\ICONS\\" + getRelatedIcon(paTable));
		newItem->setIcon(0,Icon);
		newItem->setText(0,newItem->meNameDisplay);
		paParent->addChild(newItem);
       
        return true;
    }
    catch(exception &e)
    {
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::addElementsToQTreeWidget -> " + QString(e.what()));
        return false;
    }
}
bool clParameterView::addElementsToQTreeWidgetSub(QTreeWidgetItem * paParent, QString paID, QString paTable)
{
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
															
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::addElementsToQTreeWidgetSub -> " + loReturnMessageObject);

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

			QString loID = QString(loElements.at(i));


			loGetID = meIceClientServer->getFromTableDatabaseById(paTable,
														loID,
														loPropertyNames,
														loReturnValues,
														loReturnMessageGetById);
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::addElementsToQTreeWidgetSub -> " + loReturnMessageGetById);
			
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
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::addElementsToQTreeWidget -> " + QString(e.what()));
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
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::fillListViewWithProperties -> " + QString(e.what()));
        return false;
    }
}
bool clParameterView::fillListViewWithActions(vector<std::string> paActionIDs, vector <std::string> paActionNames, QString paClassName)
{
	
	try
	{
		removeElementsFromActions();
		
		QStringList loColumnNames;
		loColumnNames << "action_name" << "action_alias" << "action_source" << "action_sourcename" << "action_methodname";
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
			loPropertyNames.push_back("action_methodname");	
			

			QString loActionId = QString(paActionIDs.at(i).c_str());


			meIceClientServer->getFromTableDatabaseById(paClassName,
														loActionId,
														loPropertyNames,
														loReturnValues,
														loReturnMessageGetById);
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::fillListViewWithActions -> " + loReturnMessageGetById);
			
			
			
				
			clClassObjectTreeItem *newItem;
			newItem = new clClassObjectTreeItem(meParameterView.tvwActions, meParameterView.tvwActions->currentItem());
			newItem->meUUID = QString(paActionIDs.at(i).c_str());
			const QIcon Icon("./ICONS/" + getRelatedIcon(paClassName));
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
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::fillListViewWithActions -> " + QString(e.what()));
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
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::removeElementsFromActions -> " + QString(e.what()));
        return false;
    }
}
/*********************************************************************
* Method calls
**********************************************************************/
bool clParameterView::callRoutineDLL(QString paObjectUUID, QString paCurrentMehtodSourceFile, QString paClassName)
{
    try
    {
		clMethodCall * loMethodCall;
		CreateModule loCreateModule;

		std::string loLibraryName = std::string(QString("./Methods/" + paCurrentMehtodSourceFile).toStdString());
		////////////////////////Loading the library///////////////////////////////////////////////////////////////////

		void * loLibraryLib = dlopen(loLibraryName.c_str(), RTLD_NOW);

		if (!loLibraryLib) {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::callObjectDLL -> Library [" + QString(loLibraryName.c_str()) + "] not found");
			return NULL;
		}
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::callObjectDLL -> Library [" + QString(loLibraryName.c_str()) + "] found");


			dlerror(); /* Clear any existing error */

			loCreateModule = (CreateModule)dlsym(loLibraryLib, "CreateModule");
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createPluginClass loaded");


			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createPluginClass after stocking");
			loMethodCall = loCreateModule();

			if (loMethodCall->createPluginClass(meIceClientServer, meIceClientLogging))
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createPluginClass called");


				vector <QString> loCurrentMethodArgType;
				loCurrentMethodArgType.push_back(QString("CLASS"));
				loCurrentMethodArgType.push_back(QString("uuid"));
				vector <QString> loCurrentMethodArgValue;
				loCurrentMethodArgValue.push_back(paClassName);
				loCurrentMethodArgValue.push_back(paObjectUUID);
				loMethodCall->doMethod(loCurrentMethodArgType, loCurrentMethodArgValue);
			}
			else
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createPluginClass not called");
			}

		}
		dlclose(loLibraryLib);
		return true;
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::callObjectDLL() -> " + QString(e.what()));
		return false;
	}
}
bool clParameterView::callRoutinePythonNoDebug(QString paObjectUUID, QString paCurrentMethodSourceFile, QString paClass, QString paMethod)
{
	try
	{
		char * loArg01 = NULL;
		char * loArg02 = NULL;
		char * loArg03 = NULL;
		char * loArg04 = NULL;
		char * loArg05 = NULL;
		char * loArg06 = NULL;
		char * loArg07 = NULL;
		char * loArg08 = NULL;
		char * loArg09 = NULL;
		char * loArg10 = NULL;		
		QByteArray byteArray = paObjectUUID.toUtf8();
		loArg01 = byteArray.data();

		QString loTemp = paCurrentMethodSourceFile;	
        if (!performScript(QString(loTemp.replace(".py","")).toUtf8().data(),paClass.toUpper().toUtf8().data(),paMethod.toUtf8().data(),loArg01,loArg02,loArg03,loArg04,loArg05,loArg06,loArg07,loArg08,loArg09,loArg10,meArgCountScript))
			return false;
		
		cout << endl << "Do something with the return value" << endl;
		
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::callRoutinePythonNoDebug() -> " + QString(e.what()));
		return false;
    }
}
bool clParameterView::callRoutinePythonWithDebug(QString paObjectUUID, QString paCurrentMethodSourceFile, QString paClass, QString paMethod)
{
	try
	{
		char * loArg01 = NULL;
		char * loArg02 = NULL;
		char * loArg03 = NULL;
		char * loArg04 = NULL;
		char * loArg05 = NULL;
		char * loArg06 = NULL;
		char * loArg07 = NULL;
		char * loArg08 = NULL;
		char * loArg09 = NULL;
		char * loArg10 = NULL;		
		QByteArray byteArray = paObjectUUID.toUtf8();
		loArg01 = byteArray.data();

		QString loTemp = paCurrentMethodSourceFile;	
		loTemp.replace(".py","");
        if (!performScriptWithDebug(loTemp.toUtf8().data(),paClass.toUpper().toUtf8().data(),paMethod.toUtf8().data(),loArg01,loArg02,loArg03,loArg04,loArg05,loArg06,loArg07,loArg08,loArg09,loArg10,meArgCountScript))
			return false;
		
		cout << endl << "Do something with the return value" << endl;
		
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::callRoutinePythonWithDebug() -> " + QString(e.what()));
		return false;
    }
}
bool clParameterView::performScript(char * paScriptName, char * paScriptClass, char * paScriptMethod, char *paArg01, char *paArg02, char *paArg03, char *paArg04, char *paArg05, char *paArg06, char *paArg07, char *paArg08, char *paArg09, char *paArg10, int paArgCount)
{
		PyGILState_STATE state;
	
		
        try
        {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::performScript-> initialising python");
			meLock->lock();
			
			state = PyGILState_Ensure();

			PyRun_SimpleString("import sys");
			
			PyObject *pName, *pModule, *pDict, *pClass, *pInstance, *pValue;

			QString loObjectPath = QString("sys.path.append('./SCRIPTS/')");
			//QString loObjectPath = QString("sys.path.append('./" + meObjectId + QString("/')"));
			QByteArray loObjectPathTemp = loObjectPath.toLocal8Bit();
			PyRun_SimpleString(loObjectPathTemp.data());
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::performScript-> path imported -> " + loObjectPath);
			
			loObjectPath = QString("sys.path.append('./SCRIPTS/Lib/')");
			//loObjectPath = QString("sys.path.append('./") + meObjectId + QString("/Lib/')");
			loObjectPathTemp = loObjectPath.toLocal8Bit();			
			PyRun_SimpleString(loObjectPathTemp.data());
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::performScript-> libs imported -> " + loObjectPath);
			
            pName = PyUnicode_FromString(paScriptName);
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::performScript-> python pName created");
            pModule = PyImport_Import(pName);
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::performScript-> python pModule created");
            pDict = PyModule_GetDict(pModule);
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::performScript-> python pDict created");
            pClass = PyDict_GetItemString(pDict, paScriptClass);
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::performScript-> python pClass created");

            // Create an instance of the class

            if (PyCallable_Check(pClass))
            {
                pInstance = PyObject_CallObject(pClass, NULL);
            }
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::performScript-> python pInstance created");

            switch (paArgCount)
            {
                    case 0:
                        pValue = PyObject_CallMethod(pInstance, paScriptMethod, NULL);
                        break;
                    case 1:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(s)", paArg01);
                        break;
                    case 2:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ss)", paArg01, paArg02);
                        break;
                    case 3:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sss)", paArg01, paArg02, paArg03);
                        break;
                    case 4:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssss)", paArg01, paArg02, paArg03,paArg04);
                        break;
                    case 5:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssss)", paArg01, paArg02, paArg03,paArg04,paArg05);
                        break;
                    case 6:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06);
                        break;
                    case 7:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07);
                        break;
                    case 8:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08);
                        break;
                    case 9:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08,paArg09);
                        break;
                    case 10:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08,paArg09,paArg10);
                        break;
                    default:
                        pValue = PyObject_CallMethod(pInstance, paScriptMethod, NULL);
                        break;
            }
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clParameterView::performScript-> python Function [" + QString(paScriptClass) + "] called"));
			int meResult = -1;
            if (pValue != NULL)
            {
				meResult = PyFloat_AsDouble(pValue);
                printf("Return of call : %d\n", (int) meResult);
            }
            else
            {
                PyErr_Print();
				printf("Return of call : %d\n", (int) meResult);
				PyErr_Clear();
				PyRun_SimpleString("import gc");
				PyRun_SimpleString("gc.collect()");				
				
				if (pInstance != NULL) Py_DECREF(pInstance);
				//if (pClass != NULL) Py_DECREF(pClass);
				//if (pDict != NULL) Py_DECREF(pDict);
				if (pModule != NULL) Py_DECREF(pModule);
				if (pName != NULL) Py_DECREF(pName);				
				
				if (!PyGC_IsEnabled()) PyGC_Enable();
				PyGC_Collect();
				PyGILState_Release(state);
				meLock->unlock();		
				
				return false;
            }
			

			
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clParameterView::performScript-> python Objects cleaned"));
			PyErr_Clear();
			PyRun_SimpleString("import gc");
			PyRun_SimpleString("gc.collect()");


			//if (pValue != NULL) Py_DECREF(pValue);
			if (pInstance != NULL) Py_DECREF(pInstance);
			//if (pClass != NULL) Py_DECREF(pClass);
			//if (pDict != NULL) Py_DECREF(pDict);
			if (pModule != NULL) Py_DECREF(pModule);
			if (pName != NULL) Py_DECREF(pName);				
			if (!PyGC_IsEnabled()) PyGC_Enable();
			PyGC_Collect();			
			PyGILState_Release(state);
			meLock->unlock();

            return true;
        }
        catch(exception &e)
        {
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::performScript() -> " + QString(e.what()));
			PyGILState_Release(state);
			meLock->unlock();
            return false;
        }
}

bool clParameterView::performScriptWithDebug(char * paScriptName, char * paScriptClass, char * paScriptMethod, char *paArg01, char *paArg02, char *paArg03, char *paArg04, char *paArg05, char *paArg06, char *paArg07, char *paArg08, char *paArg09, char *paArg10, int paArgCount)
{
		PyGILState_STATE state;		
        try
        {
			/*
            STARTUPINFO startupInfo = { 0 };
            PROCESS_INFORMATION processInformation = { 0 };
            startupInfo.cb = sizeof(startupInfo);
			bool loSucces = false;
			
			
			
			meLock->lock();
			
			
            //loSucces = CreateProcess(NULL,const_cast<char *>(("python -c \"import winpdb;winpdb.main()\" " + QString(meCurrentRoutineSourceFile).toStdString()).c_str()), NULL, NULL, FALSE, 0, NULL,NULL, &startupInfo, &processInformation);
			loSucces = CreateProcess(NULL,const_cast<char *>((".\\THIRD_PARTY\\HapDebuggerBin\\HapDebugger.exe " + QString(".\\" + meObjectId + "\\" + meCurrentRoutineSourceFile).toStdString()).c_str()), NULL, NULL, FALSE, 0, NULL,NULL, &startupInfo, &processInformation);
			
			
			if (!loSucces)
			{
				cout << endl << "No success with the function " << endl;
			}
			else
			{
				WaitForSingleObject( processInformation.hProcess, INFINITE );
				CloseHandle( processInformation.hProcess );
				CloseHandle( processInformation.hThread );
			}

			
			
			state = PyGILState_Ensure();

			PyRun_SimpleString("import sys");
			
			PyObject *pName, *pModule, *pDict, *pClass, *pInstance, *pValue;		
			
			QString loObjectPath = QString("sys.path.append(\"./" + meObjectId + QString("/\")"));
			QByteArray loObjectPathTemp = loObjectPath.toLocal8Bit();
			PyRun_SimpleString(loObjectPathTemp.data());			
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScriptWithDebug-> cycle [" + meObjectId + "] path imported -> " + loObjectPath);
			
			loObjectPath = QString("sys.path.append(\"./") + meObjectId + QString("/Lib/\")");
			loObjectPathTemp = loObjectPath.toLocal8Bit();			
			PyRun_SimpleString(loObjectPathTemp.data());
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycle::performScriptWithDebug-> cycle [" + meObjectId + "] libs imported -> " + loObjectPath);

            pName = PyUnicode_FromString(paScriptName);
            pModule = PyImport_Import(pName);
            pDict = PyModule_GetDict(pModule);
            pClass = PyDict_GetItemString(pDict, paScriptClass);

            // Create an instance of the class

            if (PyCallable_Check(pClass))
            {
                pInstance = PyObject_CallObject(pClass, NULL);
            }

            switch (meCurrentRoutineArgValue.size())
            {
                    case 0:
                        pValue = PyObject_CallMethod(pInstance, paScriptMethod, NULL);
                        break;
                    case 1:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(s)", paArg01);
                        break;
                    case 2:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ss)", paArg01, paArg02);
                        break;
                    case 3:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sss)", paArg01, paArg02, paArg03);
                        break;
                    case 4:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssss)", paArg01, paArg02, paArg03,paArg04);
                        break;
                    case 5:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssss)", paArg01, paArg02, paArg03,paArg04,paArg05);
                        break;
                    case 6:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06);
                        break;
                    case 7:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07);
                        break;
                    case 8:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08);
                        break;
                    case 9:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08,paArg09);
                        break;
                    case 10:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08,paArg09,paArg10);
                        break;
                    default:
                        pValue = PyObject_CallMethod(pInstance, paScriptMethod, NULL);
                        break;
            }
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clWorkstationCycle::performScriptWithDebug-> python Function [" + QString(paScriptClass) + "] called for object [" + meObjectId + "]"));
			
            if (pValue != NULL)
            {
				meResultFromFunction = PyFloat_AsDouble(pValue);
                printf("Return of call : %d\n", (int) meResultFromFunction);
            }
            else
            {
                PyErr_Print();
				meResultFromFunction = -1;
				printf("Return of call : %d\n", (int) meResultFromFunction);
				PyErr_Clear();
				PyRun_SimpleString("import gc");
				PyRun_SimpleString("gc.collect()");				
				
				if (pInstance != NULL) Py_DECREF(pInstance);
				//if (pClass != NULL) Py_DECREF(pClass);
				//if (pDict != NULL) Py_DECREF(pDict);
				if (pModule != NULL) Py_DECREF(pModule);
				if (pName != NULL) Py_DECREF(pName);				
				
				if (!PyGC_IsEnabled()) PyGC_Enable();
				PyGC_Collect();
				PyGILState_Release(state);
				meLock->unlock();		
				
				return false;
            }
			

			
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clWorkstationCycle::performScriptWithDebug-> python Objects cleaned for object [" + meObjectId + "]"));
			PyErr_Clear();
			PyRun_SimpleString("import gc");
			PyRun_SimpleString("gc.collect()");


			//if (pValue != NULL) Py_DECREF(pValue);
			if (pInstance != NULL) Py_DECREF(pInstance);
			//if (pClass != NULL) Py_DECREF(pClass);
			//if (pDict != NULL) Py_DECREF(pDict);
			if (pModule != NULL) Py_DECREF(pModule);
			if (pName != NULL) Py_DECREF(pName);				
			if (!PyGC_IsEnabled()) PyGC_Enable();
			PyGC_Collect();			
			PyGILState_Release(state);
			meLock->unlock();
*/
            return true;
        }
        catch(exception &e)
        {
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::performScriptWithDebug() -> " + QString(e.what()));
			PyGILState_Release(state);
			meLock->unlock();
            return false;
        }
}

