#define INFO_BUFFER_SIZE 32767
#include "clCycleBuilderView.h"

clCycleBuilderView::clCycleBuilderView(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;

    meCycleBuilderView.setupUi(this);
	
	readMappingIcons();


	/*
    connect(meParameterView.btnRefresh, SIGNAL(clicked()),this,SLOT(slotButtonRefreshPressed()));
    connect(meParameterView.btnRead, SIGNAL(clicked()),this,SLOT(slotButtonReadPressed()));
    connect(meParameterView.btnWrite, SIGNAL(clicked()),this,SLOT(slotButtonWritePressed()));
    connect(meParameterView.btnMethod, SIGNAL(clicked()),this,SLOT(slotButtonMethodPressed()));
    connect(meParameterView.tvwObjects,SIGNAL(itemClicked(QTreeWidgetItem*, int)),this,SLOT(slotTreeClassItemPressed(QTreeWidgetItem*,int)));
    connect(meParameterView.lstProperties,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotListPropertieItemPressed(QListWidgetItem*)));
    connect(meParameterView.tvwObjects,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(slotTreeClassItemMenu(QTreeWidgetItem*,int)));
	*/
	connect(meCycleBuilderView.btnRefresh, SIGNAL(clicked()),this,SLOT(slotButtonRefreshPressed()));
	connect(meCycleBuilderView.btnSaveCycle, SIGNAL(clicked()),this,SLOT(slotButtonSaveCycle()));
	connect(meCycleBuilderView.btnCreateRoutineCycle, SIGNAL(clicked()),this,SLOT(slotButtonCreateCycleRoutinePressed()));
	connect(meCycleBuilderView.trwCycles,SIGNAL(itemClicked(QTreeWidgetItem*, int)),this,SLOT(slotTreeClassItemPressedCycles(QTreeWidgetItem*,int)));
	connect(meCycleBuilderView.trwRoutines,SIGNAL(itemClicked(QTreeWidgetItem*, int)),this,SLOT(slotTreeClassItemPressedRoutines(QTreeWidgetItem*,int)));
	connect(meCycleBuilderView.trwCycles,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(slotTreeClassItemMenu(QTreeWidgetItem*,int)));
	connect(meCycleBuilderView.trwRoutines,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(slotTreeClassItemMenu(QTreeWidgetItem*,int)));
	connect(meCycleBuilderView.trwRoutineCycles,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(slotTreeClassItemMenuRoutineCycles(QTreeWidgetItem*,int)));
	
	meGraphicsView = new clExtendGraphicsView(meIceClientServer,meIceClientLogging, this);
	meCycleBuilderView.horizontalLayout->addWidget(meGraphicsView);
	
}

clCycleBuilderView::~clCycleBuilderView()
{
}
void clCycleBuilderView::readMappingIcons()
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
QString clCycleBuilderView::getRelatedIcon(QString paClassName)
{
	if(meClass.size() > 0)
	{
		for (int i = 0; i < (int) meClass.size(); i++)
		{
			if (paClassName.toUpper().compare(meClass.at(i)) == 0)
			{
					return meIcon.at(i);
			}
		}
		
	}
	return QString("");
}
void clCycleBuilderView::slotButtonRefreshPressed()
{
    try
    {
        //*****************************
        //* Getting the tables *
        //********************************
        vector<std::string> loTables;
        QString loMessage;
		QString loTable;
        meIceClientServer->getAllTablesFromDatabase(loTables,loMessage);

        for(int i=0; i < (int) loTables.size(); i++)
        {
			
            loTable = QString(loTables[i].c_str());
			if (!loTable.contains("RTI_", Qt::CaseInsensitive) ||(loTable.indexOf(QString("a_")) == 0))
			{
				loTables.erase(loTables.begin() + i);
				i = i - 1;
			}
        }
		loTables.push_back("cycles");
        //***********************************
        //* Fill listview with tables
        //**************************************
        if (!createClassesInTreeView(loTables))
        {		
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","createClassesInTreeView(loTables) Could not create tables in treeview");
        }
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotButtonRefreshPressed() -> " + QString(e.what()));
    }
}
void clCycleBuilderView::slotButtonCreateCycleRoutinePressed()
{
		
    try
    {
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList;
		//Get the routine
		loList = meCycleBuilderView.trwRoutines->selectedItems();
		
        QTreeWidgetItem *loItemRoutine = NULL;
        int loRowsRoutine = loList.count();
        for (int i = 0; i < loRowsRoutine; i++)
        {
			
            loItemRoutine = loList.takeAt(i);
			break;
        }
		//Get the cycle
		loList = meCycleBuilderView.trwCycles->selectedItems();
		
        QTreeWidgetItem *loItemCycle = NULL;
        int loRowsCycle = loList.count();
        for (int i = 0; i < loRowsCycle; i++)
        {
			
            loItemCycle = loList.takeAt(i);
			break;
        }
		//Check if elements are not null
		if(loItemCycle == NULL)return;
		if (loItemRoutine == NULL)return;
		//Check if not top element
		if (loItemRoutine->parent() == NULL)return;
		if (loItemCycle->parent() == NULL)return;
		
		clClassObjectTreeItem *loItemRout = (clClassObjectTreeItem*)loItemRoutine;
		clClassObjectTreeItem *loItemCyc = (clClassObjectTreeItem*)loItemCycle;
		
		
		//Message box for creating object
		QMessageBox msgBox;
		msgBox.setText("Create a Routine Cycle");
		msgBox.setInformativeText("Do you want to save " + loItemCyc->meNameDisplay + "_" + loItemRout->meNameDisplay);
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		int ret = msgBox.exec();
		
		switch (ret) {
		  case QMessageBox::Save:
			  // Save was clicked
			  break;
		  case QMessageBox::Cancel:
			  // Cancel was clicked
			  return;
		  default:
			  // should never be reached
			  return;
		}
		
		//Getting the columns from the table CYCLE_ROUTINE
		/*****************************************
		* Logic to get the info
		*******************************************/
		vector<std::string> loPropertyName;
		vector<std::string> loAlias;
		vector<std::string> loType;
		vector<std::string> loExtra;
		vector<std::string> loReference;
		QString loReturnMessageObject;


		meIceClientServer->getAllPropertiesFromTable(  	QString("CYCLE_ROUTINE"),
														loPropertyName,
														loAlias,
														loType,
														loExtra,
														loReference,
														loReturnMessageObject);

		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotButtonCreateCycleRoutinePressed-> Amount of properties=" + QString::number(loPropertyName.size()));
		
		vector<clDatabaseColumn> loDatabaseColumn;
		for (int i = 0; i < (int) loPropertyName.size(); i++)
		{
			clDatabaseColumn loTemp(			QString(loPropertyName[i].c_str()),
												QString(loAlias[i].c_str()),
												QString(loType[i].c_str()),
												QString(loExtra[i].c_str()),
												QString(loReference[i].c_str()));
																
			loDatabaseColumn.push_back(loTemp);
		}
		//Create
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
				
				if(loDatabaseColumn[j].getName().toUpper().compare("NAME") == 0)
				{
					loColumns.push_back(string(loDatabaseColumn[j].getName().toUtf8()));
					loValue.push_back(string(QString(loItemCyc->meNameDisplay + "_" + loItemRout->meNameDisplay).toUtf8()));
					loTypeValue.push_back(string(loDatabaseColumn[j].getType().toUtf8()));					
				}				
				else if(loDatabaseColumn[j].getName().toUpper().compare("ROUTINE") == 0)
				{
					loColumns.push_back(string(loDatabaseColumn[j].getName().toUtf8()));
					loValue.push_back(string(QString(loItemRout->meUUID).toUtf8()));
					loTypeValue.push_back(string(loDatabaseColumn[j].getType().toUtf8()));					
				}
				else if(loDatabaseColumn[j].getName().toUpper().compare("ROUTINE_CLASS") == 0)
				{
					loColumns.push_back(string(loDatabaseColumn[j].getName().toUtf8()));
					loValue.push_back(string(QString(loItemRout->getClassName()).toUtf8()));
					loTypeValue.push_back(string(loDatabaseColumn[j].getType().toUtf8()));					
				}
				else if(loDatabaseColumn[j].getName().toUpper().compare("CYCLE") == 0)
				{
					loColumns.push_back(string(loDatabaseColumn[j].getName().toUtf8()));
					loValue.push_back(string(QString(loItemCyc->meUUID).toUtf8()));
					loTypeValue.push_back(string(loDatabaseColumn[j].getType().toUtf8()));					
				}						

				//cout << "----------" << endl;
				//cout << string(loDatabaseColumn[j].getType().toUtf8()) << endl;
				//cout << "----------" << endl;
			}
		}
		bool loInsert = false;
		loInsert = meIceClientServer->insertIntoTableDatabase(    	QString("CYCLE_ROUTINE"),
																	loColumns,
																	loValue,
																	loTypeValue,
																	loReturnMessage);
		
		if (!loInsert)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotButtonCreateCycleRoutinePressed() -> " + loReturnMessage);
			return;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotButtonCreateCycleRoutinePressed() -> " + loReturnMessage);
			
		//Refresh the view
		vector <clDatabaseRow> loDatabaseRows = createClassesInTreeViewCycleRoutine(QString("CYCLE_ROUTINE"),loItemCyc->meUUID);
		
    }
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotButtonCreateCycleRoutinePressed -> " + QString(e.what()));
    }	
}
void clCycleBuilderView::slotButtonSaveCycle()
{
	try
	{
		clClassObjectTreeItem *loClassObjectTreeItem = (clClassObjectTreeItem*) meCycleBuilderView.trwCycles->selectedItems()[0];
		vector <clDatabaseRow> loDatabaseRows = createClassesInTreeViewCycleRoutine(QString("CYCLE_ROUTINE"),loClassObjectTreeItem->meUUID);
		vector <clDatabaseRow> loNewDatabaseRows = meGraphicsView->safeCycle(loDatabaseRows);
		
		for(int i = 0; i < (int) loNewDatabaseRows.size(); i++)
		{
			clDatabaseRow loDatabaseRow = loNewDatabaseRows.at(i);
			
			vector <std::string> loDatabaseProperties = loDatabaseRow.getColumnName();
			vector <std::string> loDatabaseValues = loDatabaseRow.getValues();
			vector <std::string> loDatabaseType = loDatabaseRow.getType();
			vector <std::string> loDatabaseAlias = loDatabaseRow.getAlias();
			vector <std::string> loDatabaseExtra = loDatabaseRow.getExtra();
			vector <std::string> loDatabaseReference = loDatabaseRow.getReference();
			QString loReturnMessage;
						
			QString meUUID = QString("");
			for (int j=0; j < (int) loDatabaseProperties.size(); j++)
			{
				if (QString(loDatabaseProperties.at(j).c_str()).toUpper().compare("PKEY") == 0)
				{
					meUUID = QString(loDatabaseValues.at(j).c_str());
					break;
				}
			}
			
			bool loUpdate = false;
			
			QString loClass = QString("CYCLE_ROUTINE");
			
			
			loUpdate = meIceClientServer->updateIntoTableDatabase( 		loClass,
																		meUUID,
																		loDatabaseProperties,
																		loDatabaseValues,
																		loDatabaseType,
																		loReturnMessage);
			if(!loUpdate)
			{
				meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotButtonSaveCycle -> " + loReturnMessage);
				return;
			}
			else
				meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotButtonSaveCycle -> " + loReturnMessage);
		}
		
		
		
		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotButtonSaveCycle -> " + QString(e.what()));
    }
}
vector <clDatabaseRow> clCycleBuilderView::createClassesInTreeViewCycleRoutine(QString paTable, QString paCycleID)
{
	
	vector <clDatabaseRow> loDatabaseRows;
    try
    {
		
		//Delete elements from routine cycles
        QList<QTreeWidgetItem *> paColumnItemsRoutineCycles;
        for (int i = 0; i < meCycleBuilderView.trwRoutineCycles->topLevelItemCount(); ++i)
            paColumnItemsRoutineCycles << meCycleBuilderView.trwRoutineCycles->topLevelItem(i);
        qDeleteAll(paColumnItemsRoutineCycles.begin(),paColumnItemsRoutineCycles.end());	
		
		
		/*****************************************
		* Getting the properties
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
														
		meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotTreeClassItemPressed -> " + loReturnMessageObject);

		vector<clDatabaseColumn> loDatabaseColumnArr;
		for (int i=0; i< (int) loPropertyName.size(); i++)
		{
			clDatabaseColumn loDatabaseColumn(  QString(loPropertyName[i].c_str()),
												QString(loAlias[i].c_str()),
												QString(loType[i].c_str()),
												QString(loExtra[i].c_str()),
												QString(loReference[i].c_str()));
			loDatabaseColumnArr.push_back(loDatabaseColumn);
		}
		/*****************************************
		* End logic
		*******************************************/
		
		/***********************************************
		* Getting all objects 
		************************************************/
		vector<std::string> loProperties;
		vector<std::string> loValues;
		vector<std::string> loTypeValue;
		vector<std::string> loLogExp;
		vector<std::string> loReturnIds;
		QString loReturnMessageGetByProperty;
		
		loProperties.push_back("CYCLE");
		loValues.push_back(string(paCycleID.toUtf8()));
		loTypeValue.push_back("uuid");
		loLogExp.push_back("=");
		
		QString loClass = QString("CYCLE_ROUTINE");
		QString loStart = QString("0");
		QString loStop = QString("0");
		

		bool loGetByProperty = false;
		loGetByProperty = meIceClientServer->getFromTableDatbaseByProperty(	loClass,
																				loStart,
																				loStop,
																				loProperties,
																				loValues,
																				loTypeValue,
																				loLogExp,
																				loReturnIds,
																				loReturnMessageGetByProperty);
																				
		if(!loGetByProperty)
		{
			meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::createClassesInTreeViewCycleRoutine -> " + loReturnMessageGetByProperty);
			return loDatabaseRows;
		}
		else
			meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::createClassesInTreeViewCycleRoutine -> " + loReturnMessageGetByProperty);

		/***********************************************
		* End Getting all objects 
		************************************************/
		
		if (loReturnIds.size() < 1) return loDatabaseRows;
		
		for (int i = 0; i < (int) loReturnIds.size(); i++)
		{
			/*
			      bool getFromTableDatabaseById(    string paTableName,
                                        string paId,
                                        tyStringSequence paProperties,
                                        out tyStringSequence paReturnValue,
                                        out string paReturnMessage);*/
            vector<std::string> loPropertyNames;
            vector<std::string> loReturnValues;
            QString loReturnMessageGetById;
			            
			for (int i = 0; i < (int) loDatabaseColumnArr.size(); i++)
            {
                //loPropertyName.push_back(string(loConvertedItem->getDatabaseColumns()[i].getName().toUtf8()));
				loPropertyNames.push_back(string(loDatabaseColumnArr[i].getName().toUtf8()));
            }
			
			bool loGetById = false;
			QString loClass = QString("CYCLE_ROUTINE");
			QString loUUID = QString(loReturnIds[i].c_str());
            loGetById = meIceClientServer->getFromTableDatabaseById(    loClass,
																		loUUID,
																		loPropertyNames,
																		loReturnValues,
																		loReturnMessageGetById);
			if (!loGetById)
			{
				meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::createClassesInTreeViewCycleRoutine -> " + loReturnMessageGetById);
				return loDatabaseRows;
			}
			else
				meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::createClassesInTreeViewCycleRoutine -> " + loReturnMessageGetById);
			
			if (loReturnValues.size() < 1)return loDatabaseRows;
			
			
			//Add item to list
			clClassObjectTreeItem *newItem;
			newItem = new clClassObjectTreeItem(meCycleBuilderView.trwRoutineCycles, meCycleBuilderView.trwRoutineCycles->currentItem());
			//newItem->setDatabaseActions(loDatabaseActionArr);
			newItem->setDatabaseColumns(loDatabaseColumnArr);
			newItem->setClassName(QString("CYCLE_ROUTINE"));
			newItem->meUUID = QString(loReturnIds[i].c_str());
			const QIcon Icon(".\\ICONS\\" + getRelatedIcon(QString("CYCLE_ROUTINE")));
			newItem->setIcon(0,Icon);
			for (int i = 0; i < (int) loPropertyNames.size(); i ++)
			{
				if (QString(loPropertyNames[i].c_str()).toUpper().compare("NAME") == 0)
				{
					newItem->setText(0,QString(loReturnValues[i].c_str()));
					newItem->meNameDisplay = QString(loReturnValues[i].c_str());
				}
			}
			
			loDatabaseRows.push_back(clDatabaseRow(QString("CYCLE_ROUTINE"),loReturnValues,loPropertyNames, loAlias, loType, loExtra, loReference));		
		}
		return loDatabaseRows;
		//Do stuff
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::createClassesInTreeViewCycleRoutine -> " + QString(e.what()));
        return loDatabaseRows;
    }
}
bool clCycleBuilderView::createClassesInTreeView(vector<std::string> paTables)
{
		
    try
    {
		//Set layout listviews
        removeElementsFromTreeView();
        meCycleBuilderView.trwCycles->setColumnCount(2);
		meCycleBuilderView.trwRoutines->setColumnCount(2);
		meCycleBuilderView.trwRoutineCycles->setColumnCount(2);

        QStringList headers;
        headers << tr("Classes") << tr("Objects");
        meCycleBuilderView.trwCycles->setHeaderLabels(headers);
		meCycleBuilderView.trwRoutines->setHeaderLabels(headers);
		meCycleBuilderView.trwRoutineCycles->setHeaderLabels(headers);

		//For each table
        for (int i = 0; i < (int) paTables.size();i++)
        {
            QString loTableName = QString(paTables[i].c_str());
      

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
																
                meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotTreeClassItemPressed -> " + loReturnMessageObject);

                vector<clDatabaseColumn> loDatabaseColumnArr;
                for (int i=0; i< (int) loPropertyName.size(); i++)
                {
                    clDatabaseColumn loDatabaseColumn(  QString(loPropertyName[i].c_str()),
                                                        QString(loAlias[i].c_str()),
                                                        QString(loType[i].c_str()),
                                                        QString(loExtra[i].c_str()),
                                                        QString(loReference[i].c_str()));
                    loDatabaseColumnArr.push_back(loDatabaseColumn);
                }
                /*****************************************
                * End logic
                *******************************************/



                clClassObjectTreeItem *newItem;
				if (loTableName.toUpper().compare("CYCLES") == 0)
				{
					newItem = new clClassObjectTreeItem(meCycleBuilderView.trwCycles, meCycleBuilderView.trwCycles->currentItem());
					//newItem->setDatabaseActions(loDatabaseActionArr);
					newItem->setDatabaseColumns(loDatabaseColumnArr);
					newItem->setClassName(loTableName);

					const QIcon Icon(".\\ICONS\\" + getRelatedIcon(loTableName));
					newItem->setIcon(0,Icon);
					newItem->setText(0,loTableName);
					
					meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::createClassesInTreeView -> Added table [" + loTableName + "]");
				}
				else
				{
					newItem = new clClassObjectTreeItem(meCycleBuilderView.trwRoutines, meCycleBuilderView.trwRoutines->currentItem());
					//newItem->setDatabaseActions(loDatabaseActionArr);
					newItem->setDatabaseColumns(loDatabaseColumnArr);
					newItem->setClassName(loTableName);

					const QIcon Icon(".\\ICONS\\" + getRelatedIcon(loTableName));
					newItem->setIcon(0,Icon);
					newItem->setText(0,loTableName);
					
					meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::createClassesInTreeView -> Added table [" + loTableName + "]");
				}
        }
        return true;
    }
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::createClassesInTreeView -> " + QString(e.what()));
        return false;
    }
}
bool clCycleBuilderView::removeElementsFromTreeView()
{
	
    try
    {
		//Delete elements from cycles
        QList<QTreeWidgetItem *> paColumnItemsCycles;
        for (int i = 0; i < meCycleBuilderView.trwCycles->topLevelItemCount(); ++i)
            paColumnItemsCycles << meCycleBuilderView.trwCycles->topLevelItem(i);
        qDeleteAll(paColumnItemsCycles.begin(),paColumnItemsCycles.end());
		//Delete elements from routines
        QList<QTreeWidgetItem *> paColumnItemsRoutines;
        for (int i = 0; i < meCycleBuilderView.trwRoutines->topLevelItemCount(); ++i)
            paColumnItemsRoutines << meCycleBuilderView.trwRoutines->topLevelItem(i);
        qDeleteAll(paColumnItemsRoutines.begin(),paColumnItemsRoutines.end());
		//Delete elements from routine cycles
        QList<QTreeWidgetItem *> paColumnItemsRoutineCycles;
        for (int i = 0; i < meCycleBuilderView.trwRoutineCycles->topLevelItemCount(); ++i)
            paColumnItemsRoutineCycles << meCycleBuilderView.trwRoutineCycles->topLevelItem(i);
        qDeleteAll(paColumnItemsRoutineCycles.begin(),paColumnItemsRoutineCycles.end());		
		
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::removeElementsFromTreeView -> " + QString(e.what()));
        return false;
    }
}
void clCycleBuilderView::slotTreeClassItemPressedCycles(QTreeWidgetItem *item, int index)
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

			QString loClass = item->text(0);
			QString loStart = QString("0");
			QString loStop = QString("1000");

            meIceClientServer->getFromTableDatabaseGeneral( loClass,
                                                            loStart,
                                                            loStop,
                                                            loReturnIds,
															loReturnNames,
                                                            loReturnMessage);
			
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotTreeClassItemPressedCycles -> " + loReturnMessage);

            addElementsToQTreeWidget(item, loReturnIds, loReturnNames);

        }
		else
		{
			vector <clDatabaseRow> loDatabaseRows = createClassesInTreeViewCycleRoutine(QString("CYCLE_ROUTINE"),loClassObjectTreeItem->meUUID);
			
			
			meGraphicsView->initializeGraphView(loDatabaseRows);
		}
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotTreeClassItemPressedCycles -> " + QString(e.what()));
    }
}
void clCycleBuilderView::slotTreeClassItemPressedRoutines(QTreeWidgetItem *item, int index)
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

			QString loClass = item->text(0);
			QString loStop = QString("0");
			QString loStart = QString("1000");


            meIceClientServer->getFromTableDatabaseGeneral( loClass,
                                                            loStop,
                                                            loStart,
                                                            loReturnIds,
															loReturnNames,
                                                            loReturnMessage);
			
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotTreeClassItemPressedRoutines -> " + loReturnMessage);

            addElementsToQTreeWidget(item, loReturnIds, loReturnNames);
        }
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBulder.exe","clCycleBuilderView::slotTreeClassItemPressedRoutines -> " + QString(e.what()));
    }
}
bool clCycleBuilderView::addElementsToQTreeWidget(QTreeWidgetItem * paItem, vector<std::string> paReturnIds, vector<std::string> paReturnNames)
{
    try
    {
        paItem->takeChildren();
		clClassObjectTreeItem *loItem = (clClassObjectTreeItem*)paItem;

        for (int i = 0; i< (int) paReturnIds.size();i++)
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
		
		
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::addElementsToQTreeWidget -> " + QString(e.what()));
        return false;
    }
}
//Item Clicked
void clCycleBuilderView::slotTreeClassItemMenu(QTreeWidgetItem *item, int i)
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



			if (loConvertedItemParent->getClassName().toUpper().compare(QString("CYCLES")) == 0)
				QObject::connect(action, SIGNAL(triggered()),this,SLOT(slotNewElementCycles()));
			else
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

            clClassObjectTreeItem *loConvertedItem;
            loConvertedItem = (clClassObjectTreeItem *) item->parent();
			
			/*
            vector <clDatabaseAction> loDatabaseAction = loConvertedItem->getDatabaseActions();
            QAction *loMethods[100];
            for (int i = 0; i < loDatabaseAction.size(); i ++)
            {
                loMethods[i] = loMenu.addAction(loDatabaseAction[i].getName());
            }
			*/
			if (loConvertedItem->getClassName().toUpper().compare(QString("CYCLES")) == 0)
			{
				QObject::connect(loEdit,SIGNAL(triggered()),this,SLOT(slotEditElementCycles()));
				QObject::connect(loDelete,SIGNAL(triggered()),this,SLOT(slotDeleteElementCycles()));					
			}
			else
			{			
				QObject::connect(loEdit,SIGNAL(triggered()),this,SLOT(slotEditElement()));
				QObject::connect(loDelete,SIGNAL(triggered()),this,SLOT(slotDeleteElement()));
			}
            loMenu.exec(QCursor::pos());
        }
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotTreeClassItemMenu -> " + QString(e.what()));
    }
}
void clCycleBuilderView::slotTreeClassItemMenuRoutineCycles(QTreeWidgetItem *item, int i)
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
            QAction *action=menu.addAction("DELETE");
			QObject::connect(action, SIGNAL(triggered()),this,SLOT(slotDeleteElementRoutineCycles()));
            menu.exec(QCursor::pos());

        }
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotTreeClassItemMenu -> " + QString(e.what()));
    }
}
void clCycleBuilderView::slotEditElement()
{
	
    try
    {	
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList;

		loList = meCycleBuilderView.trwRoutines->selectedItems();

		
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
		meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement()-> Table name=" + loItem->getClassName());		
		meIceClientServer->getAllPropertiesFromTable(loItem->getClassName(),loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement()-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		for (int i = 0; i < loPropertyName.size(); i++)
		{
			meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement()-> Propertie name [" + QString(loPropertyName[i].c_str()) + "]");
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
		QString loClassName = loItem->getClassName();
		QString loUUID = loItem->meUUID;
		meIceClientServer->getFromTableDatabaseById(    loClassName,
														loUUID,
														loPropertyName,
														loReturnValues,
														loReturnMessageGetById);
														
		meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement -> " + loReturnMessageGetById);
		
		
		
		
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
			
			
	
			
			
			
			cout << "before update" << endl;
			
			QString loClass = loItem->getClassName();
			QString loUUID = loItem->meUUID;
			
            meIceClientServer->updateIntoTableDatabase(    loClass,
															loUUID,
                                                            loColumns,
                                                            loValue,
                                                            loTypeValue,
                                                            loReturnMessage);
														
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement() -> " + loReturnMessage);
        }
        else
        {		
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement() -> update canceled");
        }


    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement -> " + QString(e.what()));
    }	
	
}
void clCycleBuilderView::slotDeleteElement()
{	
    try
    {
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList;
		
		loList = meCycleBuilderView.trwRoutines->selectedItems();
		
		
        clClassObjectTreeItem *loClassObjectTreeItem = NULL;
        QTreeWidgetItem *loTreeWidgetItem = NULL;

        int loRows = loList.count();
        for (int i = 0; i < loRows; i++)
        {
            loTreeWidgetItem = loList.takeAt(i);
        }

        loClassObjectTreeItem = (clClassObjectTreeItem*) loTreeWidgetItem;
        QString loReturnMessage;
		
		QString loClassName = loClassObjectTreeItem->getClassName();
		QString loUUID = loClassObjectTreeItem->meUUID;
        meIceClientServer->deleteIntoTableDatabase(loClassName,loUUID,loReturnMessage);
		
	
		
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotDeleteElement() -> " + loReturnMessage);

    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotDeleteElement -> " + QString(e.what()));
    }
}
void clCycleBuilderView::slotDeleteElementRoutineCycles()
{
	
    try
    {
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList;
		
		loList = meCycleBuilderView.trwRoutineCycles->selectedItems();
		
		
        clClassObjectTreeItem *loClassObjectTreeItem = NULL;
        QTreeWidgetItem *loTreeWidgetItem = NULL;

        int loRows = loList.count();
        for (int i = 0; i < loRows; i++)
        {
            loTreeWidgetItem = loList.takeAt(i);
        }

        loClassObjectTreeItem = (clClassObjectTreeItem*) loTreeWidgetItem;
        QString loReturnMessage;

		QString loClassName = loClassObjectTreeItem->getClassName();
		QString loUUID = loClassObjectTreeItem->meUUID;
        meIceClientServer->deleteIntoTableDatabase(loClassName,loUUID,loReturnMessage);
		
	
		
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotDeleteElementRoutineCycles() -> " + loReturnMessage);
		
        //Get the UUID of the cycle
		loList = meCycleBuilderView.trwCycles->selectedItems();
        clClassObjectTreeItem *loClassObjectTreeItemCycle = NULL;

        loRows = loList.count();
        for (int i = 0; i < loRows; i++)
        {
            loClassObjectTreeItemCycle = (clClassObjectTreeItem*) loList.takeAt(i);
        }
		
        if (loClassObjectTreeItemCycle == NULL) return;		
		
		createClassesInTreeViewCycleRoutine(QString("CYCLE_ROUTINE"),loClassObjectTreeItemCycle->meUUID);
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotDeleteElementRoutineCycles -> " + QString(e.what()));
    }
}
void clCycleBuilderView::slotNewElement()
{
    try
    {	
		
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList;
		
		loList = meCycleBuilderView.trwRoutines->selectedItems();
		
			
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
		meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotNewElement()-> Table name=" + loClassObjectTreeItem->getClassName());		
		QString loClassName = loClassObjectTreeItem->getClassName();
		meIceClientServer->getAllPropertiesFromTable(loClassName,loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotNewElement()-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		for (int i = 0; i < (int) loPropertyName.size(); i++)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotNewElement()-> Propertie name [" + QString(loPropertyName[i].c_str()) + "]");
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


            for (int j = 1; j < (int) loDatabaseColumn.size();j++)
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
														
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotNewElement() -> " + loReturnMessage);
        }
        else
        {		
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotNewElement() -> insert canceled");
        }


    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotTreeClassItemMenu -> " + QString(e.what()));
    }
}
///////////////////////////////////////////////////////////////////
void clCycleBuilderView::slotEditElementCycles()
{
	
    try
    {	
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList;

		loList = meCycleBuilderView.trwCycles->selectedItems();
		
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
		meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement()-> Table name=" + loItem->getClassName());		
		
		QString loClassName = loItem->getClassName();
		meIceClientServer->getAllPropertiesFromTable(loClassName,loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement()-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		for (int i = 0; i < (int) loPropertyName.size(); i++)
		{
			meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement()-> Propertie name [" + QString(loPropertyName[i].c_str()) + "]");
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
		QString loClass = loItem->getClassName();
		QString loUUID = loItem->meUUID;
		meIceClientServer->getFromTableDatabaseById(    loClass,
														loUUID,
														loPropertyName,
														loReturnValues,
														loReturnMessageGetById);
														
		meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement -> " + loReturnMessageGetById);
		
		
		
		
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
			
			
	
			
			
			
			cout << "before update" << endl;
			QString loClassName = loItem->getClassName();
			QString loUUIDex = loItem->meUUID;
            meIceClientServer->updateIntoTableDatabase(    loClassName,
															loUUIDex,
                                                            loColumns,
                                                            loValue,
                                                            loTypeValue,
                                                            loReturnMessage);
														
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement() -> " + loReturnMessage);
        }
        else
        {		
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement() -> update canceled");
        }


    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotEditElement -> " + QString(e.what()));
    }	
	
}
void clCycleBuilderView::slotDeleteElementCycles()
{
	
    try
    {
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList;

		loList = meCycleBuilderView.trwCycles->selectedItems();
		
        clClassObjectTreeItem *loClassObjectTreeItem = NULL;
        QTreeWidgetItem *loTreeWidgetItem = NULL;

        int loRows = loList.count();
        for (int i = 0; i < (int) loRows; i++)
        {
            loTreeWidgetItem = loList.takeAt(i);
        }

        loClassObjectTreeItem = (clClassObjectTreeItem*) loTreeWidgetItem;
        QString loReturnMessage;

		QString loClassName = loClassObjectTreeItem->getClassName();
		QString loUUID = loClassObjectTreeItem->meUUID;

        meIceClientServer->deleteIntoTableDatabase(loClassName,loUUID,loReturnMessage);
		
	
		
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotDeleteElement() -> " + loReturnMessage);

    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotDeleteElement -> " + QString(e.what()));
    }
}
void clCycleBuilderView::slotNewElementCycles()
{
    try
    {	
		
        //Class is pressed so get the objects
        QList<QTreeWidgetItem*> loList;
		
		loList = meCycleBuilderView.trwCycles->selectedItems();

			
        clClassObjectTreeItem *loClassObjectTreeItem = NULL;

        int loRows = loList.count();
        for (int i = 0; i < (int) loRows; i++)
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
		meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotNewElement()-> Table name=" + loClassObjectTreeItem->getClassName());		
		QString loClassName = loClassObjectTreeItem->getClassName();
		meIceClientServer->getAllPropertiesFromTable(loClassName,loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotNewElement()-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		for (int i = 0; i < (int) loPropertyName.size(); i++)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotNewElement()-> Propertie name [" + QString(loPropertyName[i].c_str()) + "]");
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


            for (int j = 1; j < (int) loDatabaseColumn.size();j++)
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
														
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotNewElement() -> " + loReturnMessage);
        }
        else
        {		
            meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotNewElement() -> insert canceled");
        }


    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("30",QString(QHostInfo::localHostName()),"2UVCycleBuilder.exe","clCycleBuilderView::slotTreeClassItemMenu -> " + QString(e.what()));
    }
}
