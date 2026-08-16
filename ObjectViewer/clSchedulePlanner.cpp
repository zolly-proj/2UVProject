#define INFO_BUFFER_SIZE 32767
#include "clSchedulePlanner.h"

clSchedulePlanner::clSchedulePlanner(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clClassLoader *paClassLoader, QString paConfigurationFile, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;

    meSchedulePlanner.setupUi(this);
	
	meClassLoader = paClassLoader;
	
	readMappingIcons();
	
	
	
	//Initialize the dates
	loMinDateTime = QDateTime::fromString("1970-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
	loMaxDateTime = QDateTime::fromString("1970-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
	
	//fill working area
	fillWorkingArea();
	
	//Filling the employees
	fillEmployeeView();
	
}

clSchedulePlanner::~clSchedulePlanner()
{
}
void clSchedulePlanner::slotButtonNewPressed()
{
	try
	{
		
		QString loClassName = meCboClassName->currentText();
		
		
		
		//*********************************
        //* Getting the table information *
        //*********************************
        vector<std::string> loPropertyName;
		vector<std::string> loAlias;
		vector<std::string> loType;
		vector<std::string> loExtra;
		vector<std::string> loReference;
        QString loMessage;
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::slotButtonNewPressed()-> Table name=" + loClassName);
		meIceClientServer->getAllPropertiesFromTable(loClassName,loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::slotButtonNewPressed()-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		for (int i = 0; i < loPropertyName.size(); i++)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::slotButtonNewPressed()-> Propertie name [" + QString(loPropertyName[i].c_str()) + "]");
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
			
			clObject loObject = meClassLoader->getObject(loClassName);
            //Set the class name on the object
            loObject.ClassName = loClassName;
			loObject.insertObject(loColumns,loValue,loTypeValue);										
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::slotButtonNewPressed() -> " + loObject.ObjectId);
 					
			//Filling the employees
			fillEmployeeView();                       
        }
        else
        {
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::slotButtonNewPressed() -> insert canceled");
        }	
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::slotButtonNewPressed() -> " + QString(e.what()));
	}	
}
void clSchedulePlanner::slotButtonRefreshPressed()
{
	fillEmployeeView();
}
bool clSchedulePlanner::fillWorkingArea()
{
	try
	{
		QWidget *loWidget = new QWidget;
		
		QGridLayout *layout = new QGridLayout;
		
		QLabel *loHeader = new QLabel("Working area:");
		QLabel *loNew = new QLabel("New time:");
		QPushButton *loBtnNew = new QPushButton("New");
		connect(loBtnNew, SIGNAL(clicked()), this, SLOT(slotButtonNewPressed()));

		QPushButton *loBtnRefresh = new QPushButton("Refresh");
		connect(loBtnRefresh, SIGNAL(clicked()), this, SLOT(slotButtonRefreshPressed()));

		
		meCboClassName = new QComboBox;		
		QStringList loListOfClassNames = {"EMPLOYEE_WORKING_HOURS","EMPLOYEE_SICK_HOURS","EMPLOYEE_ADV_HOURS","EMPLOYEE_UNDEFINED_HOURS","EMPLOYEE_OVERTIME_HOURS"};
		meCboClassName->addItems(loListOfClassNames);
		
		
		
		
		QDateTime loDateTimeStart = QDateTime::currentDateTime();
		QDateTime loDateTimeStop = QDateTime::currentDateTime();
		loDateTimeStart.setTime(QTime(0,0,0));
		loDateTimeStop.setTime(QTime(24,0,0));
		QDateTime loDateTimeStopTemp = loDateTimeStop.addDays(30);
		QDateTime loDateTimeStartTemp = loDateTimeStart.addDays(-1);
		
		
		QLabel * loStartDate = new QLabel("Start date:");
		meDateTimeEdit.push_back(new QDateTimeEdit(loDateTimeStartTemp));
		meDateTimeEdit.at(0)->setDisplayFormat("yyyy-MM-dd hh:mm:ss.zzz");
		QLabel * loStopDate = new QLabel("Stop date:");
		meDateTimeEdit.push_back(new QDateTimeEdit(loDateTimeStopTemp));
		meDateTimeEdit.at(1)->setDisplayFormat("yyyy-MM-dd hh:mm:ss.zzz");		 
		 
		layout->addWidget(meDateTimeEdit.at(1),0,7);
		layout->addWidget(loStopDate,0,6);
		layout->addWidget(meDateTimeEdit.at(0),0,5);
		layout->addWidget(loStartDate,0,4);
		layout->addWidget(loBtnRefresh,0,3);
		layout->addWidget(loBtnNew,0,2);
		layout->addWidget(meCboClassName,0,1);
		layout->addWidget(loNew,0,0);
		
		
		loWidget->setLayout(layout);
		
		//meSchedulePlanner.gridLayout_timeline->addWidget(loHeader,0,0);	    
		meSchedulePlanner.gridLayout_timeline->addWidget(loWidget,0,0);
		
		return true;
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::fillWorkingArea() -> " + QString(e.what()));
		return false;
	}
}


void clSchedulePlanner::fillEmployeeView()
{
	try
	{
		
		meScene.clear();
        meView.clear();
		meTimeline.clear();
		meGraphicsItems.clear();
		
		//Initialize the dates
		loMinDateTime = QDateTime::fromString("1970-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
		loMaxDateTime = QDateTime::fromString("1970-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
		
		
		
				
		vector<std::string> loTables;
		loTables.push_back("EMPLOYEE");
		if (!createClassesInTreeView(loTables))
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::fillEmployeeView(loTables) Could not create tables in treeview");
        else
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::fillEmployeeView(loTables) Could create tables in treeview");
        return;
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::fillEmployeeView() -> " + QString(e.what()));
        return;
    }
}
bool clSchedulePlanner::removeElementsFromEmployeeView()
{
	try
    {
        QList<QTreeWidgetItem *> paColumnItems;

        for (int i = 0; i < meSchedulePlanner.tvwEmployeeView->topLevelItemCount(); ++i)
        {
            paColumnItems << meSchedulePlanner.tvwEmployeeView->topLevelItem(i);
        }
        qDeleteAll(paColumnItems.begin(),paColumnItems.end());
        return true;
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::removeElementsFromEmployeeView -> " + QString(e.what()));
        return false;
    }
}
bool clSchedulePlanner::createClassesInTreeView(vector<std::string> paTables)
{
	try
    {
        removeElementsFromEmployeeView();
        meSchedulePlanner.tvwEmployeeView->setColumnCount(2);

        QStringList headers;
        headers << tr("Employee") << tr("Hours");
        meSchedulePlanner.tvwEmployeeView->setHeaderLabels(headers);

        for (int i = 0; i < paTables.size();i++)
        {
            QString loTableName = QString(paTables.at(i).c_str());

			
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
															
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::createClassesInTreeView -> " + loReturnMessageObject);
			
			//Store the table information in vector
			vector<clDatabaseColumn> loDatabaseColumnArr;
			for (int l=0; l< loPropertyName.size(); l++)
			{
				clDatabaseColumn loDatabaseColumn(  QString(loPropertyName[l].c_str()),
													QString(loAlias[l].c_str()),
													QString(loType[l].c_str()),
													QString(loExtra[l].c_str()),
													QString(loReference[l].c_str()));
				loDatabaseColumnArr.push_back(loDatabaseColumn);
			}

			vector <std::string> loReturnId;
			vector <std::string> loReturnName;
			QString loReturnMessage;

			QString loStart = QString("0");
			if (!meIceClientServer->getFromTableDatabaseGeneral( loTableName, loStart, loStart, loReturnId, loReturnName, loReturnMessage))
			{
                meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::createClassesInTreeView -> " + loReturnMessage);
			}
			else
			{
				//Init tables
				
				meScene.push_back(new QGraphicsScene());
				meView.push_back(new clGraphicsView(meScene.at(meScene.size()-1)));
				meTimeline.push_back(new clTimeLine(meView.at(meView.size()-1)));
				meTimeline.at(meTimeline.size()-1)->meMinAxes = meDateTimeEdit.at(0)->dateTime().toSecsSinceEpoch();
				meTimeline.at(meTimeline.size()-1)->meMaxAxes = meDateTimeEdit.at(1)->dateTime().toSecsSinceEpoch();
				meTimeline.at(meTimeline.size()-1)->meSpacerInTimeline = meSpacerInTimeline;
				meTimeline.at(meTimeline.size()-1)->meAmountOfEmployees = loReturnId.size();
				meTimeline.at(meTimeline.size()-1)->setAxes();			
					
				meView.at(meView.size()-1)->setInteractive(true);
				meView.at(meView.size()-1)->setMouseTracking(true);
				meView.at(meView.size()-1)->setFocus();
				meSchedulePlanner.gridLayout_timeline->addWidget(meView.at(meView.size()-1),meView.size() + 1,0);						
				clGraphicsView * loGraphicsView = meView.at(meView.size()-1);
				QObject::connect(loGraphicsView,SIGNAL(sendMouseDoubleClickEventSignal(QGraphicsItem*)),this,SLOT(slotItemDoubleClicked(QGraphicsItem*)));
						
				for (int k = 0; k < loReturnId.size(); k++)
				{
					clClassObjectTreeItem *newItem;
					newItem = new clClassObjectTreeItem(meSchedulePlanner.tvwEmployeeView, meSchedulePlanner.tvwEmployeeView->currentItem());
					//newItem->setDatabaseActions(loDatabaseActionArr);
					newItem->setDatabaseColumns(loDatabaseColumnArr);
					newItem->setClassName(loTableName);
					newItem->meUUID = QString(loReturnId.at(k).c_str());
					newItem->meNameDisplay = QString(loReturnName.at(k).c_str());
					newItem->meIsInfo = false;
                    const QIcon Icon("./ICONS/" + getRelatedIcon(loTableName));
					newItem->setIcon(0,Icon);
					newItem->setText(0,QString(loReturnName.at(k).c_str()));
					
                    meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::createClassesInTreeView -> Added ID [" + newItem->meUUID + "]");
                    
        
                    //Fill the time table view
                    fillTimeTableView(newItem->meUUID, newItem->meNameDisplay,k);
				}
			}
        }
		

        return true;
    }
    catch(exception &e)
    {	
		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::createClassesInTreeView -> " + QString(e.what()));
        return false;
    }
}
vector <clObject> clSchedulePlanner::returnObjectsOfTableForId(QString paTableName, QString paId)
{
	vector <clObject> loReturnValues;
	try
	{
		vector <std::string> loProperties;
		vector <std::string> loValue;
		vector <std::string> loTypeValue;
		vector <std::string> loLogExp;
		vector <std::string> loReturnIds;
		QString loReturnMessage;
		
		//Must have a current workstep
		loProperties.push_back("EMPLOYEE");
		loValue.push_back(paId.toStdString());
		loTypeValue.push_back("uuid");
		loLogExp.push_back("=");

		loProperties.push_back("START_SHIFT");
		loValue.push_back(QString(meDateTimeEdit.at(0)->dateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")).toStdString());
		loTypeValue.push_back("timestamp(3)");
		loLogExp.push_back(">");

		loProperties.push_back("STOP_SHIFT");
		loValue.push_back(QString(meDateTimeEdit.at(1)->dateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")).toStdString());
		loTypeValue.push_back("timestamp(3)");
		loLogExp.push_back("<");


		QString loStart = QString("0");
		if(!meIceClientServer->getFromTableDatbaseByProperty(	paTableName,
																loStart,
																loStart,
																loProperties,
																loValue,
																loTypeValue,
																loLogExp,
																loReturnIds,
																loReturnMessage))
		{
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::returnObjectsOfTableForId(QString paId) -> " + loReturnMessage);
		}
		else
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::returnObjectsOfTableForId(QString paId) -> Got HOURS for table " + QString("%1 amount [%2]").arg(paTableName).arg(loReturnIds.size()));
		
		for (int i = 0; i < loReturnIds.size(); i++)
		{
			clObject loObject = meClassLoader->getObject(QString(paTableName));
			loObject.ClassName = paTableName;
			loObject.ObjectId = QString(loReturnIds.at(i).c_str());
			loReturnValues.push_back(loObject);
		}
		
		return loReturnValues;
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::returnObjectsOfTableForId(QString paId) -> " + QString(e.what()));
		return loReturnValues;
	}
}



bool clSchedulePlanner::fillTimeTableView(QString paUUID, QString paName, int k)
{
	try
	{
		//Get the hours
		clObject loObject = meClassLoader->getObject(QString("EMPLOYEE"));
		loObject.ObjectId = paUUID;
	    
	    vector <clObject> loObjectsWorkingHours = returnObjectsOfTableForId(QString("EMPLOYEE_WORKING_HOURS"),loObject.ObjectId);
	    vector <clObject> loObjectsSickHours = returnObjectsOfTableForId(QString("EMPLOYEE_SICK_HOURS"),loObject.ObjectId);
	    vector <clObject> loObjectsAdvHours = returnObjectsOfTableForId(QString("EMPLOYEE_ADV_HOURS"),loObject.ObjectId);
	    vector <clObject> loObjectsUndifinedHours = returnObjectsOfTableForId(QString("EMPLOYEE_UNDEFINED_HOURS"),loObject.ObjectId);
	    vector <clObject> loObjectsOvertimeHours = returnObjectsOfTableForId(QString("EMPLOYEE_OVERTIME_HOURS"),loObject.ObjectId);
	    
	    loMinDateTime = meDateTimeEdit.at(0)->dateTime();
	    loMaxDateTime = meDateTimeEdit.at(1)->dateTime();
		
		//Graphics for the plane line
		QPen pen = QPen(Qt::black,1);
		long loEndOfLine = ((loMaxDateTime.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900) * meSpacerInTimeline;
        QGraphicsItem *item_sep = meScene.at(meScene.size() - 1)->addLine( 0, k * 100 - 5, loEndOfLine, k * 100 - 5, pen);
        
        //Graphics for the plane Add names
        for(int i = 0; i < ((meDateTimeEdit.at(1)->dateTime().toSecsSinceEpoch()-meDateTimeEdit.at(0)->dateTime().toSecsSinceEpoch())/900); i += 16)
        {
			QGraphicsItem *item = meScene.at(meScene.size() - 1)->addText(paName);
			item->setPos(i*meSpacerInTimeline,k * 100);
		}
		
		
		
		int outlineSize = 1;


	    for (int i = 0; i < loObjectsWorkingHours.size(); i++)
	    {
			QDateTime loStop;
			QDateTime loStart;
			loObjectsWorkingHours.at(i).get("START_SHIFT",loStart);
			loObjectsWorkingHours.at(i).get("STOP_SHIFT",loStop);

			QColor red(95,78,42);
			QColor reddimmed(red.red()-80,red.green(),red.blue());
			QPen pen(red,outlineSize);
			pen.setCapStyle(Qt::RoundCap);
			QBrush brush(reddimmed);
			
			long meStartPoint = ((loStart.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900)   * meSpacerInTimeline;
			long meStopPoint = ((loStop.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900) * meSpacerInTimeline;
			
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::fillTimeTableView -> " + QString("Before push back [%1][%2]").arg(meStartPoint).arg(meStopPoint));
			meGraphicsItems.push_back(meTimeline.at(meTimeline.size()-1)->AddItem(QPointF(meStartPoint,k * 100 + 0), QRect(0,0,(meStopPoint - meStartPoint),10), pen, brush));
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::fillTimeTableView -> " + QString("After push back"));
			meGraphicsItems.at(meGraphicsItems.size()-1)->meObject = new clObject(loObjectsWorkingHours.at(i));
		}
	    
	    for (int i = 0; i < loObjectsSickHours.size(); i++)
	    {
			QDateTime loStop;
			QDateTime loStart;
			loObjectsSickHours.at(i).get("START_SHIFT",loStart);
			loObjectsSickHours.at(i).get("STOP_SHIFT",loStop);

			QColor purple(153, 0, 77);
			QColor purpledimmed(153, 0, 77);
			QPen penPurple(purple,outlineSize);
			penPurple.setCapStyle(Qt::RoundCap);
			QBrush brushPurple(purpledimmed);
			
			
			long meStartPoint = ((loStart.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900) * meSpacerInTimeline;
			long meStopPoint = ((loStop.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900) * meSpacerInTimeline;
			meGraphicsItems.push_back(meTimeline.at(meTimeline.size()-1)->AddItem(QPointF(meStartPoint,k * 100 + 20), QRect(0,0,(meStopPoint - meStartPoint),10), penPurple, brushPurple));
			meGraphicsItems.at(meGraphicsItems.size()-1)->meObject = new clObject(loObjectsSickHours.at(i));

		}
			    
	    for (int i = 0; i < loObjectsAdvHours.size(); i++)
	    {
			QDateTime loStop;
			QDateTime loStart;
			loObjectsAdvHours.at(i).get("START_SHIFT",loStart);
			loObjectsAdvHours.at(i).get("STOP_SHIFT",loStop);

			QColor green(51, 204, 51);
			QColor greendimmed(51, 204, 51);

			QPen penGreen(green,outlineSize);
			penGreen.setCapStyle(Qt::RoundCap);
			QBrush brushGreen(greendimmed);
				
			
			long meStartPoint = ((loStart.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900) * meSpacerInTimeline;
			long meStopPoint = ((loStop.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900) * meSpacerInTimeline;
			meGraphicsItems.push_back(meTimeline.at(meTimeline.size()-1)->AddItem(QPointF(meStartPoint, k * 100 + 40), QRect(0,0,(meStopPoint - meStartPoint),10), penGreen, brushGreen));
			meGraphicsItems.at(meGraphicsItems.size()-1)->meObject = new clObject(loObjectsAdvHours.at(i));
		}

	    for (int i = 0; i < loObjectsUndifinedHours.size(); i++)
	    {
			QDateTime loStop;
			QDateTime loStart;
			loObjectsUndifinedHours.at(i).get("START_SHIFT",loStart);
			loObjectsUndifinedHours.at(i).get("STOP_SHIFT",loStop);

			QColor blue(0, 51, 204);
			QColor bluedimmed(0, 51, 204);
			QPen penBlue(blue,outlineSize);
			penBlue.setCapStyle(Qt::RoundCap);
			QBrush brushBlue(bluedimmed);			
			
			
			long meStartPoint = ((loStart.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900) * meSpacerInTimeline;
			long meStopPoint = ((loStop.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900) * meSpacerInTimeline;
			meGraphicsItems.push_back(meTimeline.at(meTimeline.size()-1)->AddItem(QPointF(meStartPoint, k * 100 + 60), QRect(0,0,(meStopPoint - meStartPoint),10), penBlue, brushBlue));
			meGraphicsItems.at(meGraphicsItems.size()-1)->meObject = new clObject(loObjectsUndifinedHours.at(i));
		}
		
	    for (int i = 0; i < loObjectsOvertimeHours.size(); i++)
	    {
			QDateTime loStop;
			QDateTime loStart;
			loObjectsOvertimeHours.at(i).get("START_SHIFT",loStart);
			loObjectsOvertimeHours.at(i).get("STOP_SHIFT",loStop);
			
			QColor orange(255, 102, 0);
			QColor orangedimmed(255, 102, 0);
			QPen penOrange(orange,outlineSize);
			penOrange.setCapStyle(Qt::RoundCap);	
			QBrush brushOrange(orangedimmed);
			
			long meStartPoint = ((loStart.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900) * meSpacerInTimeline;
			long meStopPoint = ((loStop.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900) * meSpacerInTimeline;
			meGraphicsItems.push_back(meTimeline.at(meTimeline.size()-1)->AddItem(QPointF(meStartPoint, k * 100 + 80), QRect(0,0,(meStopPoint - meStartPoint),10), penOrange, brushOrange));
			meGraphicsItems.at(meGraphicsItems.size()-1)->meObject = new clObject(loObjectsOvertimeHours.at(i));
		}
			    
		//meSchedulePlanner.gridLayout_timeline->addWidget(new QLabel(paName),meView.size() + 1,0);	   
		
		return true;
    }
    catch(exception &e)
    {	
		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::fillTimeTableView -> " + QString(e.what()));
        return false;
    }
}

void clSchedulePlanner::slotItemDoubleClicked(QGraphicsItem * paItem)
{
	try
	{
		//clGraphicsItem * loGraphicsItem = meGraphicsItems.at(paIndex);
		for (int i = 0; i < meGraphicsItems.size(); i++)
		{
			QGraphicsItem * loItem = (QGraphicsItem *) meGraphicsItems.at(i)->meItem;
			/*
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe",QString("clSchedulePlanner::showDialogTimeSchedule -> compare [0x%1][0x%2]").arg(reinterpret_cast<quintptr>(loItem), 
                    QT_POINTER_SIZE * 2, 16, QChar('0')).arg(reinterpret_cast<quintptr>(paItem), 
                    QT_POINTER_SIZE * 2, 16, QChar('0')));
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe",QString("clSchedulePlanner::showDialogTimeSchedule -> compare [0x%1][0x%2]").arg(reinterpret_cast<quintptr>(&loItem), 
                    QT_POINTER_SIZE * 2, 16, QChar('0')).arg(reinterpret_cast<quintptr>(&paItem), 
                    QT_POINTER_SIZE * 2, 16, QChar('0')));        
            */
			if (loItem == paItem)
			{
				QMenu menu(this);
				QAction *actionDelete = menu.addAction("DELETE");
				QAction *actionEdit = menu.addAction("EDIT");
			
				meSelectedGraphicsItem = meGraphicsItems.at(i);

				QObject::connect(actionDelete, SIGNAL(triggered()),this,SLOT(slotDeleteObject()));
				QObject::connect(actionEdit, SIGNAL(triggered()),this,SLOT(slotEditObject()));
				menu.exec(QCursor::pos());
					
					
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe",QString("clSchedulePlanner::showDialogTimeSchedule -> object found [0x%1]").arg(reinterpret_cast<quintptr>(paItem), QT_POINTER_SIZE * 2, 16, QChar('0')));
					//Show dialog
					//editObject(meGraphicsItems.at(i));
				break;
			}
		}
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::showDialogTimeSchedule -> " + QString(e.what()));
	}
}
void clSchedulePlanner::slotDeleteObject()
{
	try
	{
		//Delete the object
		clObject * loObject = meSelectedGraphicsItem->meObject;
		loObject->deleteObject();
		
		//Filling the employees
		fillEmployeeView();
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::slotDeleteObject -> " + QString(e.what()));
	}
}		

void clSchedulePlanner::slotEditObject()
{
	try
	{
		
		/////////////////////////////////////
		clObject loObject(meSelectedGraphicsItem->meObject);
		
		//*********************************
        //* Getting the table information *
        //*********************************
        vector<std::string> loPropertyName;
		vector<std::string> loAlias;
		vector<std::string> loType;
		vector<std::string> loExtra;
		vector<std::string> loReference;
        QString loMessage;
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::slotEditObject()-> Table name=" + loObject.ClassName);
		meIceClientServer->getAllPropertiesFromTable(loObject.ClassName,loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::slotEditObject-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		for (int i = 0; i < loPropertyName.size(); i++)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::slotEditObject-> Propertie name [" + QString(loPropertyName[i].c_str()) + "]");
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

		
		
		meIceClientServer->getFromTableDatabaseById(    loObject.ClassName,
														loObject.ObjectId,
														loPropertyName,
														loReturnValues,
														loReturnMessageGetById);
														
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::slotEditObject -> " + loReturnMessageGetById);
		
		
		
		
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

            meIceClientServer->updateIntoTableDatabase(    loObject.ClassName,
															loObject.ObjectId,
                                                            loColumns,
                                                            loValue,
                                                            loTypeValue,
                                                            loReturnMessage);
														
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotEditObject() -> " + loReturnMessage);
            
            
            
            //Modify on timeline
            QGraphicsRectItem * loItem = (QGraphicsRectItem *) meSelectedGraphicsItem->meItem;
            
            
            QDateTime loStart, loStop; 
            loObject.get("START_SHIFT",loStart);
            loObject.get("STOP_SHIFT",loStop);
            
            //Redraw rect
			long meStartPoint = ((loStart.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900) * meSpacerInTimeline;
			long meStopPoint = ((loStop.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch())/900) * meSpacerInTimeline;
			loItem->setRect(0,0,(meStopPoint - meStartPoint),10);
			
			QPointF meCurrentPos = loItem->pos();			
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clParameterView::slotEditObject() -> update canceled [%1][%2][%3][%4][%5][%6]").arg(meStartPoint).arg(meCurrentPos.x()).arg(loStart.toSecsSinceEpoch()).arg(loStop.toSecsSinceEpoch()).arg(loMinDateTime.toSecsSinceEpoch()).arg(loMaxDateTime.toSecsSinceEpoch()));
			
			//SetPos

			loItem->setPos(meStartPoint,meCurrentPos.y());
   
        }
        else
        {
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotEditObject() -> update canceled");
        }		
        
    
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clSchedulePlanner::slotEditObject -> " + QString(e.what()));
	
	}
}

QString clSchedulePlanner::getRelatedIcon(QString paClassName)
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
void clSchedulePlanner::readMappingIcons()
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
void clSchedulePlanner::slotButtonPlanPressed()
{
    try
    {
	}
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clSchedulePlanner::slotPlanPressed -> " + QString(e.what()));
    }
}
void clSchedulePlanner::slotButtonSavePressed()
{
    try
    {
	}
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clSchedulePlanner::slotSavePressed -> " + QString(e.what()));
    }
}
