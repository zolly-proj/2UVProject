#define INFO_BUFFER_SIZE 32767
#include "clObjectViewer.h"

clObjectViewer::clObjectViewer(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clClassLoader *paClassLoader, QString paConfigurationFile, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;

    meObjectView.setupUi(this);
	
	meClassLoader = paClassLoader;

	if (paConfigurationFile.compare(QString("")) != 0)
	{
		meClassExposingFile = paConfigurationFile;
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::clObjectViewer -> " + QString("Config file is [" + meClassExposingFile + "]"));
	}
	else
	{
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::clObjectViewer -> " + QString("No correct config file ..."));
	}
	
	readXMLconfigFile();
	readMappingIcons();
	
    connect(meObjectView.btnRefresh, SIGNAL(clicked()),this,SLOT(slotButtonRefreshPressed()));
    connect(meObjectView.tvwObjects,SIGNAL(itemClicked(QTreeWidgetItem*, int)),this,SLOT(slotTreeClassItemPressed(QTreeWidgetItem*,int)));
    connect(meObjectView.tvwObjects,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(slotTreeClassItemMenu(QTreeWidgetItem*,int)));	
}

clObjectViewer::~clObjectViewer()
{
}
void clObjectViewer::readMappingIcons()
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
bool clObjectViewer::readXMLconfigFile()
{
	try
	{
		QString loClassExposingFile = QString("./CONFIG/" + meClassExposingFile);
        QFile loFile(loClassExposingFile);
        if ( !loFile.open( QIODevice::ReadOnly ) ) 
		{
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe",QString("clObjectViewer::readXMLconfigFile -> Cound not open file '" + loClassExposingFile + "' ..."));
            return false;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) 
		{
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe",QString("clObjectViewer::readXMLconfigFile -> Cound not parse file '" + loClassExposingFile + "' ..."));
            loFile.close();
            return false;
        }

       QDomElement loDocElem = loDomDocument.documentElement();
	   
	   
       // Getting the library's
       QDomNode loTablesNode = loDocElem.firstChild();
       while( !loTablesNode.isNull() ) 
	   {
			if(loTablesNode.nodeName() == "class")
			{
				QDomElement loTablesElem = loTablesNode.toElement();
				if (!loTablesElem.isNull())
				{
					
					QString loName = loTablesElem.attribute("name");
					if (loName.isEmpty() || loName.compare(QString("")) == 0)
					{
						meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe",QString("clObjectViewer::readXMLconfigFile -> mapLib tag no name for node ..."));
						return false;
					}
					
					meClassesConfigured.push_back(loName);
					
					QDomNode loSubNode = loTablesElem.firstChild();
					while( !loSubNode.isNull() ) 
					{
						if(loSubNode.nodeName() == "methods")
						{
							QDomElement loSubElem = loSubNode.toElement();
							QDomNode loMethodsNode = loSubElem.firstChild();
							
							vector <QString> loTempMethodNames;
							while( !loMethodsNode.isNull() ) 
							{	
								QDomElement loMethodsElem = loMethodsNode.toElement();	
								QString loMethodName = loMethodsElem.attribute("name");
								if (loMethodName.isEmpty() || loMethodName.compare(QString("")) == 0)
								{
									meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe",QString("clObjectViewer::readXMLconfigFile -> method tag no name for node ..."));
									return false;
								}
								loTempMethodNames.push_back(loMethodName);							
								loMethodsNode = loMethodsNode.nextSibling();
							}
							meClassMethodsConfigured.push_back(loTempMethodNames);
							
						}
						else if(loSubNode.nodeName() == "props")
						{
							QDomElement loSubElem = loSubNode.toElement();
							QDomNode loPropertiesNode = loSubElem.firstChild();
							
							vector <QString> loTempPropNames;
							while( !loPropertiesNode.isNull() ) 
							{	
								QDomElement loPropElem = loPropertiesNode.toElement();	
								QString loPropertyName = loPropElem.attribute("name");
								if (loPropertyName.isEmpty() || loPropertyName.compare(QString("")) == 0)
								{
									meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe",QString("clObjectViewer::readXMLconfigFile -> prpp tag no name for node ..."));
									return false;
								}
								loTempPropNames.push_back(loPropertyName);							
								loPropertiesNode = loPropertiesNode.nextSibling();
							}
							meClassPropsConfigured.push_back(loTempPropNames);
						}
						loSubNode = loSubNode.nextSibling();
						
					}
				}
			}	
			loTablesNode = loTablesNode.nextSibling();
	   }
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::readXMLconfigFile -> mapping file readed");
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::readXMLconfigFile -> " + QString(e.what()));
		return false;
    }		
}
void clObjectViewer::slotButtonRefreshPressed()
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
        for(int i=0; i < (int) loTables.size(); i++)
        {
            loMessageTable = loMessageTable + QString("[") + QString(loTables[i].c_str()) + QString("]");
        }
		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotButtonRefreshPressed()" + loMessage);
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotButtonRefreshPressed()" + loMessageTable);

		vector<std::string> loFilteredTables;
		for (int i=0; i < (int) loTables.size(); i++)
		{
			if (classInConfig(loTables.at(i)))
				loFilteredTables.push_back(loTables.at(i));
		}
		
        //***********************************
        //* Fill listview with tables
        //**************************************
        if (!createClassesInTreeView(loFilteredTables))
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::createClassesInTreeView(loTables) Could not create tables in treeview");
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotButtonRefreshPressed() -> " + QString(e.what()));
    }
}

bool clObjectViewer::classInConfig(std::string paTableName)
{
	try
	{
		for (int i = 0; i < (int) meClassesConfigured.size(); i++)
		{
			if (meClassesConfigured.at(i).toUpper().compare(QString(paTableName.c_str()).toUpper()) == 0)
				return true;
		}
		return false;
	}
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::classInConfig() -> " + QString(e.what()));
		return false;
    }	
}
bool clObjectViewer::propertyInConfigFile(QString paClassName, QString paPropertyName)
{
	try
	{
		for (int i = 0; i < (int) meClassesConfigured.size(); i++)
		{
			if (meClassesConfigured.at(i).toUpper().compare(paClassName.toUpper()) == 0)
			{
					for (int j = 0; j < (int) meClassPropsConfigured.at(i).size(); j++)
					{
						if (paPropertyName.toUpper().compare(meClassPropsConfigured.at(i).at(j).toUpper()) == 0)
							return true;
					}
			}
		}
		return false;
	}
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::propertyInConfigFile() -> " + QString(e.what()));
		return false;
    }		
}
bool clObjectViewer::methodInConfigFile(QString paClassName, QString paMethodName)
{
	try
	{
		for (int i = 0; i < (int) meClassesConfigured.size(); i++)
		{
			if (meClassesConfigured.at(i).toUpper().compare(paClassName.toUpper()) == 0)
			{
					for (int j = 0; j < (int) meClassMethodsConfigured.at(i).size(); j++)
					{
						if (paMethodName.toUpper().compare(meClassMethodsConfigured.at(i).at(j).toUpper()) == 0)
							return true;
					}
			}
		}
		return false;
	}
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::methodInConfigFile() -> " + QString(e.what()));
		return false;
    }		
}
void clObjectViewer::slotTreeClassItemPressed(QTreeWidgetItem *item, int index)
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

			QString loClassName = item->text(0);
			QString loStart = QString("0");
			QString loStop = QString("1000");

            meIceClientServer->getFromTableDatabaseGeneral( loClassName,
                                                            loStart,
                                                            loStop,
                                                            loReturnIds,
															loReturnNames,
                                                            loReturnMessage);

			
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotTreeClassItemPressed -> " + loReturnMessage);
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
			for (int i = 0; i < (int) loElement->getDatabaseColumns().size(); i++)
            {
                //loPropertyName.push_back(string(loConvertedItem->getDatabaseColumns()[i].getName().toUtf8()));
				loPropertyName.push_back(string(loElement->getDatabaseColumns()[i].getName().toUtf8()));
            }
			
			bool loGetById = false;
			QString loClassById = loElement->getClassName();
			QString loUUIDById = loElement->meUUID;
            loGetById = meIceClientServer->getFromTableDatabaseById(    loClassById,
																		loUUIDById,
																		loPropertyName,
																		loReturnValues,
																		loReturnMessageGetById);
			// Get and display the name of the computer.									
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + loReturnMessageGetById);
			
			if(!loGetById) return;
			if(loReturnValues.size() == 0)
			{
				QMessageBox msgBox;
				msgBox.setText("Item deleted in the mean time ...");
				msgBox.exec();
				return;
			} 
			
            
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /////////////////// Filling the list view with methods ////////////////////////////////////////////////////////////////
            vector <std::string> loReturnIdActions;
			vector <std::string> loReturnNameActions;
			QString loReturnMessageActions;
			
			
			QString loClassNameGeneral = QString("a_" + loElement->getClassName());
			QString loStartGeneral = QString("0");
			QString loStopGeneral = QString("1000");
			
			
			meIceClientServer->getFromTableDatabaseGeneral( loClassNameGeneral,
															loStartGeneral,
															loStopGeneral,
															loReturnIdActions,
															loReturnNameActions,
															loReturnMessageActions);
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + loReturnMessageActions);
			
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////			
			
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////// Add Sub level /////////////////////////////////////////////////////////////////////
			//Remove the childeren bofore refressing
			loElement->takeChildren();
			
			

			vector<clDatabaseColumn> loDatabaseColumns = loElement->getDatabaseColumns();
			
			for (int l = 0; l < (int) loDatabaseColumns.size(); l++)
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
						for (int t = 0; t < (int) loClasses.size();t++)
						{
							if (loDatabaseColumn.getType().toLower().compare(QString("uuid[]")) == 0)
							{
								for (int y = 0; y < (int) loPropertyName.size(); y++)
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
								for (int y = 0; y < (int) loPropertyName.size(); y++)
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
								for (int y = 0; y < (int) loPropertyName.size(); y++)
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
								for (int y = 0; y < (int) loPropertyName.size(); y++)
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
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + QString(e.what()));
    }
}

//Item Clicked
void clObjectViewer::slotTreeClassItemMenu(QTreeWidgetItem *item, int i)
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
    		
            for (int i = 0; i < (int) loDatabaseAction.size(); i ++)
            {
				if (methodInConfigFile(loConvertedItem->getClassName(),loDatabaseAction[i].getName()))
				{					
					loMethods[i] = loMenu.addAction(loDatabaseAction[i].getName());
					QObject::connect(loMethods[i], SIGNAL(triggered()), signalMapper, SLOT(map()));
					signalMapper->setMapping(loMethods[i], loDatabaseAction[i].getName());
				}
            }
			connect(signalMapper, SIGNAL(mapped(QString)),this, SLOT(slotCallAction(QString)));
			
			/* separator */			
			QAction *separator02 = new QAction(this);
			separator02->setSeparator(true);
			loMenu.addAction(separator02);
			
			/*General methods */
            QAction *loMethods02[500];			
			QSignalMapper* signalMapper02 = new QSignalMapper(this);
    		
            for (int i = 0; i < (int) meClassLoader->meMethodsGeneralNames.size(); i ++)
            {
				if (methodInConfigFile(loConvertedItem->getClassName(),meClassLoader->meMethodsGeneralNames.at(i)))
				{
					loMethods02[i] = loMenu.addAction(meClassLoader->meMethodsGeneralNames.at(i));
					QObject::connect(loMethods02[i], SIGNAL(triggered()), signalMapper02, SLOT(map()));
					signalMapper02->setMapping(loMethods02[i], meClassLoader->meMethodsGeneralNames.at(i));
				}
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
			
			for (int i = 0; i < (int) meClassLoader->meClassNames.size(); i++)
			{
				if (meClassLoader->meClassNames.at(i).toUpper().compare(loConvertedItemClassMethod->getClassName().toUpper()) == 0)
				{
					for (int j = 0; j < (int) meClassLoader->meClassMethods.at(i).size(); j++)
					{
						if (methodInConfigFile(loConvertedItem->getClassName(),meClassLoader->meClassMethods.at(i).at(j)))
						{						
							loMethods03[i] = loMenu.addAction(meClassLoader->meClassMethods.at(i).at(j));
							QObject::connect(loMethods03[i], SIGNAL(triggered()), signalMapper03, SLOT(map()));
							signalMapper03->setMapping(loMethods03[i], meClassLoader->meClassMethods.at(i).at(j));
						}
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
void clObjectViewer::slotCallAction(const QString &paActionName)
{
	try
	{
			//Class is pressed so get the objects
			QList<QTreeWidgetItem*> loList = meObjectView.tvwObjects->selectedItems();
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
			
			for (int i = 0; i < (int) loDatabaseActions.size(); i++)
			{
				clDatabaseAction loDatabaseAction = loDatabaseActions.at(i);
				if (loDatabaseAction.getName().toUpper().compare(paActionName.toUpper()) == 0)
				{
					if (loDatabaseAction.getSource().toUpper().compare(QString("DLL")) == 0)
					{
						/*
						meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotCallAction -> Source DLL called ...");
						//Call dll action
						callRoutineDLL(loConvertedItem->meUUID,loDatabaseAction.getSourceName(), loConvertedItem->getClassName());
						*/
					}
					else if (loDatabaseAction.getSource().toUpper().compare(QString("PYTHON")) == 0)
					{
						/*
						meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clParameterView::slotCallAction -> Source python called ...");
						//Call Python action
						//TODO implement WVA if debug
						callRoutinePythonNoDebug(loConvertedItem->meUUID, loDatabaseAction.getSourceName(), loConvertedItem->getClassName(), loDatabaseAction.getReturnType());
						*/
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
void clObjectViewer::slotCallMethod(const QString &paActionName)
{
	try
	{
			//Class is pressed so get the objects
			QList<QTreeWidgetItem*> loList = meObjectView.tvwObjects->selectedItems();
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
			for (int i = 0; i < (int) meClassLoader->meMethodsGeneralNames.size(); i++)
			{
				if (meClassLoader->meMethodsGeneralNames.at(i).toUpper().compare(paActionName.toUpper()) == 0)
				{
					vector <QString> loCurrentMethodArgType;
					loCurrentMethodArgType.push_back(QString("CLASS"));
					loCurrentMethodArgType.push_back(QString("uuid"));


                    vector <QString> lloCurrentMethodParameters;

					vector <QString> loCurrentMethodArgValue;
					loCurrentMethodArgValue.push_back(loConvertedItem->getClassName());
					loCurrentMethodArgValue.push_back(loConvertedItem->meUUID);


					meClassLoader->meMethodCalls.at(i)->doMethod(loCurrentMethodArgType,loCurrentMethodArgValue);
                    //meClassLoader->meMethodCallGeneral.at(i)->doMethod(loCurrentMethodArgType, loCurrentMethodArgValue);
					break;
				}
			}
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotCallMethod -> " + QString(e.what()));
    }	
}
void clObjectViewer::slotCallClassMethod(const QString &paActionName)
{
	try
	{
			//Class is pressed so get the objects
			QList<QTreeWidgetItem*> loList = meObjectView.tvwObjects->selectedItems();
			QTreeWidgetItem *loItem;
			
			int loRows = loList.count();
			for (int i = 0; i < loRows; i++)
			{
				
				loItem = loList.takeAt(i);
				break;
			}



            //TODO set parameters !!!!!!!!!!!!!
            vector<QString> loPropertyNames;
            vector<QString> loReturnValues;
            vector<QString> loPropertyTypes;
			vector<QString> loLogExpr;
			QString loReturnMessageGetById;
			
			clClassObjectTreeItem *loConvertedItem;
			loConvertedItem = (clClassObjectTreeItem *) loItem;

			//Not on property
			if (loConvertedItem->meIsInfo) return;
			
			
			for (int i = 0; i < (int) meClassLoader->meClassNames.size(); i++)
			{
				if (meClassLoader->meClassNames.at(i).toUpper().compare(loConvertedItem->getClassName().toUpper()) == 0)
				{
					for (int j = 0; j < (int) meClassLoader->meClassMethods.at(i).size(); j++)
					{
						if(paActionName.toUpper().compare(meClassLoader->meClassMethods.at(i).at(j).toUpper()) == 0)
						{
							meClassLoader->meObjectCalls.at(i)->doMethod(paActionName.toUpper(),loPropertyTypes,loPropertyNames,loReturnValues,loLogExpr);
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
void clObjectViewer::slotClassInfo()
{
	try
	{
			//Class is pressed so get the objects
			QList<QTreeWidgetItem*> loList = meObjectView.tvwObjects->selectedItems();
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

void clObjectViewer::slotEditElement()
{
    try
    {
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList = meObjectView.tvwObjects->selectedItems();
        QTreeWidgetItem *item;
		
        int loRows = loList.count();
        for (int i = 0; i < loRows; i++)
        {
			
            item = loList.takeAt(i);
			break;
        }

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

        vector<std::string> loPropertyName;
		vector<std::string> loAlias;
		vector<std::string> loType;
		vector<std::string> loExtra;
		vector<std::string> loReference;
        QString loMessage;
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotEditElement()-> Table name=" + loItem->getClassName());
		QString loClassNameProp = loItem->getClassName();		
		meIceClientServer->getAllPropertiesFromTable(loClassNameProp,loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotEditElement()-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		vector<std::string> loPropertyRequestedName;
		for (int i = 0; i < (int) loPropertyName.size(); i++)
		{
			//Filter the properties from config file
			if (propertyInConfigFile(loItem->getClassName(),QString(loPropertyName[i].c_str())) || QString(loPropertyName[i].c_str()).toUpper().compare(QString("pkey").toUpper()) == 0)
			{
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotEditElement()-> Propertie name [" + QString(loPropertyName[i].c_str()) + "]");				
				clDatabaseColumn test(								QString(loPropertyName[i].c_str()),
																	QString(loAlias[i].c_str()),
																	QString(loType[i].c_str()),
																	QString(loExtra[i].c_str()),
																	QString(loReference[i].c_str()));
				loDatabaseColumn.push_back(test);
				loPropertyRequestedName.push_back(loPropertyName[i]);
			}
		}
		
		
		/////////////////////////////////////
		// Getting the information content //
		/////////////////////////////////////
		vector<std::string> loReturnValues;
		QString loReturnMessageGetById;
		
		QString loClassName = loItem->getClassName();
		QString loUUID = loItem->meUUID;
		
		meIceClientServer->getFromTableDatabaseById(    loClassName,
														loUUID,
														loPropertyRequestedName,
														loReturnValues,
														loReturnMessageGetById);
														
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotEditElement -> " + loReturnMessageGetById);
		
		
		
		
		/////////////////////////////////////
		
        clEditObjectUI *loEditObjectUI;
        loEditObjectUI = new clEditObjectUI(meIceClientServer,meIceClientLogging,loDatabaseColumn, loReturnValues);
        if (loEditObjectUI->exec() == 0)
		{
		

            QString loReturnMessage;

            vector<std::string> loColumns;
            vector<std::string> loValue;
            vector<std::string> loTypeValue;


            for (int j = 1; j < (int) loDatabaseColumn.size();j++)
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
			
			
			QString loClassNameUpdateDb = loItem->getClassName();
			QString loUUIDupdateDB = loItem->meUUID;
			
            meIceClientServer->updateIntoTableDatabase(    loClassNameUpdateDb,
															loUUIDupdateDB,
                                                            loColumns,
                                                            loValue,
                                                            loTypeValue,
                                                            loReturnMessage);
														
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotEditElement() -> " + loReturnMessage);
        }
        else
        {
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotEditElement() -> update canceled");
        }


    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotEditElement -> " + QString(e.what()));
    }	
	
}

void clObjectViewer::slotDeleteElement()
{
    try
    {
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList = meObjectView.tvwObjects->selectedItems();
        clClassObjectTreeItem *loClassObjectTreeItem = NULL;
        QTreeWidgetItem *loTreeWidgetItem = NULL;

        int loRows = loList.count();
        for (int i = 0; i < loRows; i++)
        {
            loTreeWidgetItem = loList.takeAt(i);
        }

		//Get the class object
        loClassObjectTreeItem = (clClassObjectTreeItem*) loTreeWidgetItem->parent();
        QString loReturnMessage;

		//**********************
		//Delete references
        //***********************
		//Getting the tables
        vector<std::string> loTables;
        QString loMessage;

		if (!meIceClientServer->getAllTablesFromDatabase(loTables,loMessage))
		{
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement()" + loMessage);
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement() -> getting tables failed... ");
			return;
		}

        
        for(int i=0; i < (int) loTables.size(); i++)
        {
			if (QString(loTables.at(i).c_str()).toUpper().indexOf("A_") == 0)
			{
				//Do not get the methods if it is a method table
			}
			else
			{
				vector<std::string> loPropertyName;
				vector<std::string> loAlias;
				vector<std::string> loType;
				vector<std::string> loExtra;
				vector<std::string> loReference;
				QString loMessageProp;
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement()-> Getting properties for table name=" + QString(loTables.at(i).c_str()));
				if (!meIceClientServer->getAllPropertiesFromTable(QString(loTables.at(i).c_str()),loPropertyName,loAlias,loType,loExtra,loReference,loMessageProp))
				{
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement()-> " + loMessageProp);
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement() -> getting properties failed ... ");
					return;
				}
				else
				{
					for (int j=0; j < (int) loType.size(); j++)
					{
						if (QString(loType.at(j).c_str()).toUpper().compare(QString("uuid").toUpper()) == 0)
						{
							QStringList loRefList = QString(loReference.at(j).c_str()).split("$;$",QString::SkipEmptyParts);
							for (int k=0; k < (int) loRefList.size(); k++)
							{
								if (loRefList.at(k).toUpper().compare(loClassObjectTreeItem->getClassName().toUpper()) == 0)
								{
									vector <std::string> loProperties;
									vector <std::string> loValues;
									vector <std::string> loTypeValue;
									vector <std::string> loLogExp;
									vector <std::string> loReturnIds;
									QString loQueryMessage;
									
									loProperties.push_back(loPropertyName.at(j));
									loValues.push_back(((clClassObjectTreeItem*)loTreeWidgetItem)->meUUID.toStdString());
									loTypeValue.push_back("uuid");
									loLogExp.push_back("=");
									
									//Query and delete the entry in the database single property type
									QString loTableProperty = QString(loTables.at(i).c_str());
									QString loStopProperty = QString("0");
									QString loStartProperty =  QString("0");
									if (!meIceClientServer->getFromTableDatbaseByProperty(	loTableProperty,
																		loStartProperty,
																		loStopProperty,
																		loProperties,
																		loValues,
																		loTypeValue,
																		loLogExp,
																		loReturnIds,
																		loQueryMessage))
									{
										meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement()-> " + loQueryMessage);
										meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement() -> getting ID's failed ... ");																													
										return;
									}
									//Update the column for the rows
									for (int l = 0; l < loReturnIds.size(); l++)
									{
										vector <std::string> loColumns;
										vector <std::string> loColumnValues;
										vector <std::string> loCulumnValuesTypes;
										QString loColumnUpdateMessage;
										
										loColumns.push_back(loPropertyName.at(j));
										loColumnValues.push_back("");
										loCulumnValuesTypes.push_back("uuid");
										
										
										QString loTablesDatabase = QString(loTables.at(i).c_str());
										QString loReturnIdsDatabase = QString(loReturnIds.at(l).c_str());
										
										
										
										if(!meIceClientServer->updateIntoTableDatabase( loTablesDatabase,
																						loReturnIdsDatabase,
																						loColumns,
																						loColumnValues,
																						loCulumnValuesTypes,
																						loColumnUpdateMessage))
										{
											meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement()-> " + loColumnUpdateMessage);
											meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement() -> updating ID's for with property uuid failed ... ");																													
											return;																					
										}									
									}
								}
							}
						}
						else if (QString(loType.at(j).c_str()).toUpper().compare(QString("uuid[]").toUpper()) == 0)
						{
							QStringList loRefList = QString(loReference.at(j).c_str()).split("$;$",QString::SkipEmptyParts);
							for (int k=0; k < (int) loRefList.size(); k++)
							{
								
								if (loRefList.at(k).toUpper().compare(loClassObjectTreeItem->getClassName().toUpper()) == 0)
								{
									//Query and delete the entry in the database array type
									vector <std::string> loProperties;
									vector <std::string> loValues;
									vector <std::string> loTypeValue;
									vector <std::string> loLogExp;
									vector <std::string> loReturnIds;
									QString loQueryMessage;
									
									loProperties.push_back(loPropertyName.at(j));
									loValues.push_back(((clClassObjectTreeItem*)loTreeWidgetItem)->meUUID.toStdString());
									loTypeValue.push_back("uuid");
									loLogExp.push_back("= ANY");




									QString loTablesProperty = QString(loTables.at(i).c_str());
									QString loStopProperty = QString("0");
									QString loStartProperty = QString("0");
									
									//Query the db and get id's from records containing the deleted object
									if (!meIceClientServer->getFromTableDatbaseByProperty(	loTablesProperty,
																							loStopProperty,
																							loStartProperty,
																							loProperties,
																							loValues,
																							loTypeValue,
																							loLogExp,
																							loReturnIds,
																							loQueryMessage))
									{
										meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement()-> " + loQueryMessage);
										meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement() -> getting ID's with property uuid[] failed ... ");																													
										return;
									}
									
									
									//Update the column for the retrieved id's
									for (int l = 0; l < (int) loReturnIds.size(); l++)
									{
										//Get the value
										vector <std::string> loGetColumn;
										vector <std::string> loGetColumnValue;
										QString loGetColumnMessage;
								
										loGetColumn.push_back(loPropertyName.at(j));
								
										QString loTablesId = QString(loTables.at(i).c_str());
										QString loReturnId = QString(loReturnIds.at(l).c_str());
										
										if (!meIceClientServer->getFromTableDatabaseById(   loTablesId,
																							loReturnId,
																							loGetColumn,
																							loGetColumnValue,
																							loGetColumnMessage))
										{
											meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement()-> " + loGetColumnMessage);
											meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement() -> getting values for ID's with property uuid[] failed ... ");
											return;
										}
										cout << "got this far" << endl;
										//Set the new value
										QString loGetColumnValueTemp = QString(loGetColumnValue.at(0).c_str());
										QStringList loElements = loGetColumnValueTemp.remove("}").remove("{").split(",", QString::SkipEmptyParts);
										QString loGetColumnValueTempNew = QString("");
										
										for (int u = 0; u < loElements.size(); u++)
										{
											if (loElements.at(u).compare(((clClassObjectTreeItem*)loTreeWidgetItem)->meUUID) == 0)
											{
												//Do nothing
												cout << "is found" << endl;
											}
											else
											{
												if (loGetColumnValueTempNew.compare(QString("")) == 0)
													loGetColumnValueTempNew = loGetColumnValueTempNew + loElements.at(u);
												else
													loGetColumnValueTempNew = loGetColumnValueTempNew + QString("$;$") + loElements.at(u);
											}
										}
										//Update the value
										vector <std::string> loColumns;
										vector <std::string> loColumnValues;
										vector <std::string> loCulumnValuesTypes;
										QString loColumnUpdateMessage;
										
										loColumns.push_back(loPropertyName.at(j));
										loColumnValues.push_back(loGetColumnValueTempNew.toStdString());
										loCulumnValuesTypes.push_back("uuid[]");										
										
										QString loTablesDatabase = QString(loTables.at(i).c_str());
										QString loReturnIdsDatabase = QString(loReturnIds.at(l).c_str());
										
										
										if(!meIceClientServer->updateIntoTableDatabase( loTablesDatabase,
																						loReturnIdsDatabase,
																						loColumns,
																						loColumnValues,
																						loCulumnValuesTypes,
																						loColumnUpdateMessage))
										{
											meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement()-> " + loColumnUpdateMessage);
											meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement() -> updating ID's for with property uuid failed ... ");																													
											return;																					
										}									
									}									
								}
							}							
						}
					}
				}
			}
			
        }
		
		//Delete the table entry
		QString loClassName = loClassObjectTreeItem->getClassName();
		QString loUUIDName = ((clClassObjectTreeItem*)loTreeWidgetItem)->meUUID;
        meIceClientServer->deleteIntoTableDatabase(loClassName,loUUIDName,loReturnMessage);
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement() -> " + loReturnMessage);

    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotDeleteElement -> " + QString(e.what()));
    }
}


void clObjectViewer::slotNewElement()
{
    try
    {
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList = meObjectView.tvwObjects->selectedItems();
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
		
		
		
		

        vector<std::string> loPropertyName;
		vector<std::string> loAlias;
		vector<std::string> loType;
		vector<std::string> loExtra;
		vector<std::string> loReference;
        QString loMessage;
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotNewElement()-> Table name=" + loClassObjectTreeItem->getClassName());		
		QString loClassNameTable = loClassObjectTreeItem->getClassName();
		meIceClientServer->getAllPropertiesFromTable(loClassNameTable,loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotNewElement()-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		for (int i = 0; i < (int) loPropertyName.size(); i++)
		{
			if (propertyInConfigFile(loClassObjectTreeItem->getClassName(),QString(loPropertyName.at(i).c_str())) || QString(loPropertyName.at(i).c_str()).compare(QString("pkey").toUpper()) == 0)
			{
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotNewElement()-> Propertie name [" + QString(loPropertyName[i].c_str()) + "]");
				clDatabaseColumn test(							QString(loPropertyName.at(i).c_str()),
																QString(loAlias[i].c_str()),
																QString(loType[i].c_str()),
																QString(loExtra[i].c_str()),
																QString(loReference[i].c_str()));					
				loDatabaseColumn.push_back(test);
			}
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


            for (int j = 0; j < (int) loDatabaseColumn.size();j++)
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
            
            QString loClassNameDatabase = loClassObjectTreeItem->getClassName();
            
            
            meIceClientServer->insertIntoTableDatabase(    loClassNameDatabase,
                                                            loColumns,
                                                            loValue,
                                                            loTypeValue,
                                                            loReturnMessage);
														
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotNewElement() -> " + loReturnMessage);
        }
        else
        {
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotNewElement() -> insert canceled");
        }


    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotTreeClassItemMenu -> " + QString(e.what()));
    }
}
/********************************************
* User interface funcitons
************************************************/
bool clObjectViewer::createClassesInTreeView(vector<std::string> paTables)
{
    try
    {
        removeElementsFromTreeView();
        meObjectView.tvwObjects->setColumnCount(2);

        QStringList headers;
        headers << tr("Classes") << tr("Objects");
        meObjectView.tvwObjects->setHeaderLabels(headers);

        for (int i = 0; i < (int) paTables.size();i++)
        {
            QString loTableName = QString(paTables[i].c_str());

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
															
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::createClassesInTreeView -> " + loReturnMessageObject);
			
			//Store the table information in vector
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
				
				
				QString loTableName = QString("a_" + loTableName);
				QString loStartGeneral = QString("0");
				QString loStopGeneral = QString("1000");
				
				
				meIceClientServer->getFromTableDatabaseGeneral( loTableName,
																loStartGeneral,
																loStopGeneral,
																loReturnIdActions,
																loReturnNameActions,
																loReturnMessageActions);
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::createClassesInTreeView -> " + loReturnMessageActions);
				
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

					QString loTableNameID = QString("a_" + loTableName);
					QString loReturnIdActionsID = QString(loReturnIdActions.at(k).c_str());

					meIceClientServer->getFromTableDatabaseById(loTableNameID,
												loReturnIdActionsID,
												loPropertyNames,
												loReturnValues,
												loReturnMessageGetById);
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::createClassesInTreeView -> " + loReturnMessageGetById);
					
					if(loReturnValues.size() > 0)
					{
						QString loMethodName;
						QString loMethodAlias;
						QString loMethodSource;
						QString loMethodSourceName;
						QString loMethodReturnType;							
						for (int kk = 0; kk < (int) loPropertyNames.size(); kk++)
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

			newItem = new clClassObjectTreeItem(meObjectView.tvwObjects, meObjectView.tvwObjects->currentItem());
			newItem->setDatabaseActions(loDatabaseActionArr);
			newItem->setDatabaseColumns(loDatabaseColumnArr);
			newItem->setClassName(loTableName);
			const QIcon Icon(".\\ICONS\\" + getRelatedIcon(loTableName));
			newItem->setIcon(0,Icon);
			newItem->setText(0,loTableName);			
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::createClassesInTreeView -> Added table [" + loTableName + "]");
		
        }
		

        return true;
    }
    catch(exception &e)
    {	
		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::createClassesInTreeView -> " + QString(e.what()));
        return false;
    }
}
QString clObjectViewer::getRelatedIcon(QString paClassName)
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

bool clObjectViewer::removeElementsFromTreeView()
{
    try
    {
        QList<QTreeWidgetItem *> paColumnItems;

        for (int i = 0; i < meObjectView.tvwObjects->topLevelItemCount(); ++i)
        {
            paColumnItems << meObjectView.tvwObjects->topLevelItem(i);
        }
        qDeleteAll(paColumnItems.begin(),paColumnItems.end());
        return true;
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::removeElementsFromTreeView -> " + QString(e.what()));
        return false;
    }
}
bool clObjectViewer::addElementsToQTreeWidget(QTreeWidgetItem * paItem, vector<std::string> paReturnIds, vector<std::string> paReturnNames)
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
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::addElementsToQTreeWidget -> " + QString(e.what()));
        return false;
    }
}

bool clObjectViewer::addElementToQTreeWidgetSub(QTreeWidgetItem * paParent, QString paID, QString paTable)
{
    try
    {
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
														
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::addElementToQTreeWidgetSub -> " + loReturnMessageObject);

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
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::addElementToQTreeWidgetSub -> " + loReturnMessageGetById);
		
		for (int j = 0; j < loPropertyNames.size(); j++)
		{
			QString loReturnValue = loReturnValues.at(j).c_str();
			if(QString(loPropertyNames.at(j).c_str()).toLower().compare("name") == 0)
			{
				newItem->meNameDisplay = loReturnValue; 
			}
		}
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
		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::addElementsToQTreeWidget -> " + QString(e.what()));
        return false;
    }
}


bool clObjectViewer::addElementsToQTreeWidgetSub(QTreeWidgetItem * paParent, QString paID, QString paTable)
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
															
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::addElementsToQTreeWidgetSub -> " + loReturnMessageObject);

			vector<clDatabaseColumn> loDatabaseColumnArr;
			for (int j=0; j< (int) loPropertyName.size(); j++)
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
			
			QString loElementsID = QString(loElements.at(i));
			
			
			
			loGetID = meIceClientServer->getFromTableDatabaseById(paTable,
														loElementsID,
														loPropertyNames,
														loReturnValues,
														loReturnMessageGetById);
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::addElementsToQTreeWidgetSub -> " + loReturnMessageGetById);
			
			if (!loGetID) return false;
			
			
			
			
			
			for (int j = 0; j < (int) loReturnValues.size(); j++)
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
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::addElementsToQTreeWidget -> " + QString(e.what()));
        return false;
    }
}

