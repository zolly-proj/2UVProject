#define INFO_BUFFER_SIZE 32767
#include "clProductPlanner.h"

clProductPlanner::clProductPlanner(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clClassLoader *paClassLoader, QString paConfigurationFile, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;

    meProductPlanner.setupUi(this);
	
	meClassLoader = paClassLoader;




	readMappingIcons();
	
    connect(meProductPlanner.btnPlan, SIGNAL(clicked()),this,SLOT(slotButtonPlanPressed()));
	
	
	//Fill the userinterfaces
	fillMachineWindow();
	
	fillProductWindow();
}

clProductPlanner::~clProductPlanner()
{
}
bool clProductPlanner::fillMachineWindow()
{
	try
	{
		vector<std::string> loTables;
		loTables.push_back("LIVING_OBJ_MACH");
		if (!createClassesInTreeView(loTables))
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::fillMachineWindow(loTables) Could not create tables in treeview");
		
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::fillMachineWindow() -> " + QString(e.what()));
    }
}
bool clProductPlanner::fillProductWindow()
{
	try
	{
		vector<std::string> loTables;
		loTables.push_back("PRODUCT");
		if (!createProductsInTreeView(loTables))
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::fillWorkpieceWindow(loTables) Could not create tables in treeview");
		
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::fillWorkpieceWindow() -> " + QString(e.what()));
    }
}
void clProductPlanner::readMappingIcons()
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
QString clProductPlanner::getRelatedIcon(QString paClassName)
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
void clProductPlanner::slotButtonPlanPressed()
{
	try
    {
		//Getting all the products
		vector <struct_product> loProducts;
		
		for (int i = 0; i < meProductPlanner.tvwProductView->topLevelItemCount(); i++)
		{
			clClassObjectTreeItem *loClassObjectTreeItem = (clClassObjectTreeItem *)meProductPlanner.tvwProductView->itemAt(i,0);
			
			loProducts.push_back(struct_product());
			loProducts[i].meUUID = loClassObjectTreeItem->meUUID;
			loProducts[i].meName = loClassObjectTreeItem->meNameDisplay;
			loProducts[i].meClassName = loClassObjectTreeItem->getClassName();
			
			clObject loObject = meClassLoader->getObject(loClassObjectTreeItem->getClassName());
			loObject.ObjectId = loClassObjectTreeItem->meUUID;
			
			//Getting the batch object
			vector <clObject> loObjectsBatch;
			if (loObject.getParents(QString("BATCH"),QString("PRODUCTS"),loObjectsBatch))
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the parent batch of the object");
			else
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the parent batch of the object");
			
			//Getting the priority of the batch
			int loPriority = 0;
			if (loObjectsBatch.at(0).get("PRODUCT_PRIORITY",loPriority))
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the priority of the batch");
			else
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the priority of the batch");
			loProducts[i].mePriority = loPriority;
			
			//Getting the worksteps
			vector <clObject> loObjectsWs;
			if (loObjectsBatch.at(0).get("WORKSTEPS",loObjectsWs))
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting worksteps ok");
			else			
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting worksteps nok");
			
			//Getting the current ws
			clObject loCurrentWsObject(meIceClientServer, meIceClientLogging);
			if (loObject.get("CURRENT_WORKSTEP",loCurrentWsObject))
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the current ws of product");
			else
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the current ws of the product");
			
			//Go trough the ws and fille the data
			bool loCheckWs = false;
			for (int j = 0; j < loObjectsWs.size(); j++)
			{
				if (loObjectsWs.at(j).ObjectId.compare(loCurrentWsObject.ObjectId))
					loCheckWs = true;
				//Add ws to structure
				if (loCheckWs)
				{
					loProducts[i].meWsUuid.push_back(loObjectsWs.at(j).ObjectId);
					
					//Getting the technologie
					int loTech = 0;
					if (loObjectsWs.at(j).get("WS_TECH", loTech))
						meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting tech from ws ok");
					else
						meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting tech from ws nok");
					loProducts[i].meTech.push_back(loTech);
					
					//Getting the start date
					QDateTime loStartDate;
					if (loObjectsWs.at(j).get("WS_PLANNED_START", loStartDate))
						meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting start date from ws ok");
					else
						meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting start date from ws nok");
					loProducts[i].meStartTime.push_back(loStartDate);					
					
					//Getting the stop date
					QDateTime loStopDate;
					if (loObjectsWs.at(j).get("WS_PLANNED_STOP", loStopDate))
						meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting stop date from ws ok");
					else
						meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting stop date from ws nok");
					loProducts[i].meStopTime.push_back(loStopDate);

					quint64 loMseconds = loStartDate.msecsTo(loStopDate);
					QTime loTime(0, 0, 0);
					loTime.addMSecs(loMseconds);
					loProducts[i].meTime.push_back(loTime);
					
					//Getting the current ws
					clObject loMachGroup(meIceClientServer, meIceClientLogging);
					if (loObjectsWs.at(j).get("WS_MACH_GROUP",loMachGroup))
						meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the mach group from ws");
					else
						meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get mach group from ws");
					loProducts[i].meMachGroupUuid.push_back(loMachGroup.ObjectId);				
				}
			}
			
		}
		
		
		//Getting all the workplaces
		vector <struct_workplace> loWorkplaces;
		
		for (int i = 0; i < meProductPlanner.tvwProductView->topLevelItemCount(); i++)
		{
			clClassObjectTreeItem *loClassObjectTreeItem = (clClassObjectTreeItem *)meProductPlanner.tvwProductView->itemAt(i,0);
			
			loWorkplaces.push_back(struct_workplace());
			loWorkplaces[i].meUUID = loClassObjectTreeItem->meUUID;
			loWorkplaces[i].meName = loClassObjectTreeItem->meNameDisplay;
			loWorkplaces[i].meClassName = loClassObjectTreeItem->getClassName();
			loWorkplaces[i].meFreeDateTime = QDateTime::currentDateTime();
			
			clObject loObject = meClassLoader->getObject(loClassObjectTreeItem->getClassName());
			loObject.ObjectId = loClassObjectTreeItem->meUUID;
						
			//Getting the technology
			int loTech = 0;
			if (loObject.get("WS_TECH", loTech))
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting tech from workplace ok");
			else
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting tech from workplace nok");
			loWorkplaces[i].meTech = loTech;

			//Getting the mach group
			vector <clObject> loObjectsGroup;
			if (loObject.getParents(QString("MACH_GROUP"),QString("MACH_GROUP_MACHS"),loObjectsGroup))
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the group of the workplace");
			else
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the group of the workplace");
				
				
			for (int k = 0; k < loObjectsGroup.size(); k++)
			{	
				loWorkplaces[i].meMachGroupUUID.push_back(loObjectsGroup.at(k).ObjectId);
				
				//Getting the planning
				clObject loPlanning(meIceClientServer, meIceClientLogging);
				if (loObjectsGroup.at(k).get("MACH_GROUP_PLANNING",loPlanning))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the planning of the machgroup");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the planning form the mach group");
				loWorkplaces[i].mePlanUUID.push_back(loPlanning.ObjectId);
				
				//Getting the name
				QString loPlanningName;
				if (loPlanning.get("NAME",loPlanningName))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting name from planning ok");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting name from planning nok");
				loWorkplaces[i].mePlanName.push_back(loPlanningName);
				
				//Getting the overlap
				int loOverlap;
				if (loPlanning.get("OVERLAP_TIME", loOverlap))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting overlaptime from planning ok");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting overlaptime from planning nok");
				loWorkplaces[i].meOverlapTime.push_back(loOverlap);
				
				//Getting the preprep
				int loPrePrep;
				if (loPlanning.get("PRE_PREP", loPrePrep))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting preprep from planning ok");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting preprep from planning nok");
				loWorkplaces[i].mePrePrep.push_back(loPrePrep);
				
				//Getting the postprep
				int loPostPrep;
				if (loPlanning.get("POST_PREP", loPostPrep))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting postprep from planning ok");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> getting postprep from planning nok");
				loWorkplaces[i].mePostPrep.push_back(loPostPrep);			
				
				//Getting the shift
				clObject loShift(meIceClientServer, meIceClientLogging);
				if (loPlanning.get("PLANNING_SHIFT", loShift))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift of the planning");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift form the planning");
						
				loWorkplaces[i].meShiftUUID.push_back(loShift.ObjectId);
				
				//Getting the name of the shift
				QString loShiftName;
				if (loShift.get("NAME", loShiftName))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the name of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the name form the shift");
				loWorkplaces[i].meShiftName.push_back(loShiftName);
				
				//Getting the start on monday
				QDateTime loMonStart;
				if (loShift.get("SHIFT_START_MONDAY", loMonStart))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift start monday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift start monday form the shift");
				loWorkplaces[i].meStartMonday.push_back(loMonStart);

				//Getting the start on tuesday
				QDateTime loTueStart;
				if (loShift.get("SHIFT_START_TUESDAY", loTueStart))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift start tuesday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift start tuesday form the shift");
				loWorkplaces[i].meStartTues.push_back(loTueStart);			
				
				//Getting the start on wednesday
				QDateTime loWedStart;
				if (loShift.get("SHIFT_START_WEDNESDAY", loWedStart))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift start wednesday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift start wednesday form the shift");
				loWorkplaces[i].meStartWedn.push_back(loWedStart);			

				//Getting the start on wednesday
				QDateTime loThurStart;
				if (loShift.get("SHIFT_START_THURSDAY", loThurStart))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift start thursday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift start thursday form the shift");
				loWorkplaces[i].meStartThur.push_back(loThurStart);	

				//Getting the start on friday
				QDateTime loFriStart;
				if (loShift.get("SHIFT_START_FRIDAY", loFriStart))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift start friday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift start friday form the shift");
				loWorkplaces[i].meStartFrid.push_back(loFriStart);

				//Getting the start on saturday
				QDateTime loSatStart;
				if (loShift.get("SHIFT_START_SATURDAY", loSatStart))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift start saturday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift start saturday form the shift");
				loWorkplaces[i].meStartSat.push_back(loSatStart);

				//Getting the start on sunday
				QDateTime loSunStart;
				if (loShift.get("SHIFT_START_SUNDAY", loSunStart))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift start sunday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift start sunday form the shift");
				loWorkplaces[i].meStartSun.push_back(loSunStart);			
				
				//Getting the stop on monday
				QDateTime loMonStop;
				if (loShift.get("SHIFT_START_MONDAY", loMonStop))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift stop monday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift stop monday form the shift");
				loWorkplaces[i].meStopMonday.push_back(loMonStop);

				//Getting the stop on tuesday
				QDateTime loTueStop;
				if (loShift.get("SHIFT_START_TUESDAY", loTueStop))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift stop tuesday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift stop tuesday form the shift");
				loWorkplaces[i].meStopTues.push_back(loTueStop);			
				
				//Getting the stop on wednesday
				QDateTime loWedStop;
				if (loShift.get("SHIFT_START_WEDNESDAY", loWedStop))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift stop wednesday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift stop wednesday form the shift");
				loWorkplaces[i].meStopWedn.push_back(loWedStop);			

				//Getting the stop on wednesday
				QDateTime loThurStop;
				if (loShift.get("SHIFT_START_THURSDAY", loThurStop))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift stop thursday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift stop thursday form the shift");
				loWorkplaces[i].meStopThur.push_back(loThurStop);	

				//Getting the stop on friday
				QDateTime loFriStop;
				if (loShift.get("SHIFT_START_FRIDAY", loFriStop))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift stop friday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift stop friday form the shift");
				loWorkplaces[i].meStopFrid.push_back(loFriStop);

				//Getting the stop on saturday
				QDateTime loSatStop;
				if (loShift.get("SHIFT_START_SATURDAY", loSatStop))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift stop saturday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift stop saturday form the shift");
				loWorkplaces[i].meStopSat.push_back(loSatStop);

				//Getting the stop on sunday
				QDateTime loSunStop;
				if (loShift.get("SHIFT_START_SUNDAY", loSunStop))
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> got the shift stop sunday of the shift");
				else
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed -> could not get the shift stop sunday form the shift");
				loWorkplaces[i].meStopSun.push_back(loSunStop);			
			}
		}
		
		//Sort to the priority
		std::sort(loProducts.begin(), loProducts.end(), &clProductPlanner::compareProductByPriority);
		
		//Or sort to date time ???
		
		
		
		for (int i = 0; i < loProducts.size(); i++)
		{
			//Get the workplaces for this product
			vector <vector<struct_workplace *>> loProductWorkstepsMatchingWorkplaces;		
			//Check all the ws
			for (int j = 0; j < loProducts.at(i).meMachGroupUuid.size(); j++)
			{
				vector <struct_workplace *> loMatchingWorkplaces;
				//Check all the workplaces
				for (int k = 0; k < loWorkplaces.size(); k++)
				{
					//Check all the machine groups
					for (int l = 0; l < loWorkplaces.at(k).meMachGroupUUID.size();l++)
					{
						//If the group compares
						if (loProducts.at(i).meMachGroupUuid.at(j).compare(loWorkplaces.at(k).meMachGroupUUID.at(l)) == 0)
						{
							if (loProducts.at(i).meTech.at(j) == loWorkplaces.at(k).meTech)
								loMatchingWorkplaces.push_back(&loWorkplaces.at(k));
						}
					}
				}
				loProductWorkstepsMatchingWorkplaces.push_back(loMatchingWorkplaces);
			}
			
			//Plan the workplaces for this product
			for (int j = 0; j < loProductWorkstepsMatchingWorkplaces.size(); j++)
			{
				vector <struct_workplace *> loMatchingWorkplaces = loProductWorkstepsMatchingWorkplaces.at(j); 
				std::sort(loMatchingWorkplaces.begin(), loMatchingWorkplaces.end(), &clProductPlanner::compareWorkplaceByDateTime);
				
				
				
				
				
				//Store the workplace one the product
				
				//Store the times on the product
				
				//Add the free date time to the workplace
				//loMatchingWorkplaces.at(0).meFreeDateTime
				
				
				
			}
		}
		
		
		/*
		for (int i = 0; i < loProducts.size(); i++)
		{
			cout << loProducts.at(i).meUUID.toStdString();
		}
		*/
		
		
        //*****************************
        //* Getting the tables *
        //********************************
		/*
        vector<std::string> loTables;
        QString loMessage;

        meIceClientServer->getAllTablesFromDatabase(loTables,loMessage);

        QString loMessageTable = QString("TABLE:");
        for(int i=0; i < loTables.size(); i++)
        {
            loMessageTable = loMessageTable + QString("[") + QString(loTables[i].c_str()) + QString("]");
        }
		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed()" + loMessage);
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed()" + loMessageTable);
		*/
		
		
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::slotButtonPlanPressed() -> " + QString(e.what()));
    }
}
bool clProductPlanner::compareProductByPriority(const struct_product& a, const struct_product& b)
{
    return a.mePriority < b.mePriority;
}

bool clProductPlanner::compareWorkplaceByDateTime(const struct_workplace* a, const struct_workplace* b)
{
    return a->meFreeDateTime < b->meFreeDateTime;
}




/*
void clProductPlanner::slotTreeClassItemPressed(QTreeWidgetItem *item, int index)
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
			
			meIceClientServer->getFromTableDatabaseGeneral( QString("a_" + loElement->getClassName()),
															QString("0"),
															QString("1000"),
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
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotTreeClassItemPressed -> " + QString(e.what()));
    }
}
*/
//Item Clicked
/*
void clProductPlanner::slotTreeClassItemMenu(QTreeWidgetItem *item, int i)
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
            





            QObject::connect(action, SIGNAL(triggered()),this,SLOT(slotNewElement()));
            menu.exec(QCursor::pos());
        }
        //Element pressed -> get all properties of the element
        else
        {
            
            QMenu loMenu(this);
			QAction *loEdit=loMenu.addAction("EDIT");
            QAction *loDelete=loMenu.addAction("DELETE");
			QAction *loClassProperties=loMenu.addAction("CLASS INFO");

			QObject::connect(loEdit,SIGNAL(triggered()),this,SLOT(slotEditElement()));
            QObject::connect(loDelete,SIGNAL(triggered()),this,SLOT(slotDeleteElement()));
			QObject::connect(loClassProperties,SIGNAL(triggered()),this,SLOT(slotClassInfo()));

			
			QAction *separator01 = new QAction(this);
			separator01->setSeparator(true);
			loMenu.addAction(separator01);
			
			
			
            clClassObjectTreeItem *loConvertedItem;
            loConvertedItem = (clClassObjectTreeItem *) item->parent();

            vector <clDatabaseAction> loDatabaseAction = loConvertedItem->getDatabaseActions();
            QAction *loMethods[200];			
			QSignalMapper* signalMapper = new QSignalMapper(this);
    		
            for (int i = 0; i < loDatabaseAction.size(); i ++)
            {
				if (methodInConfigFile(loConvertedItem->getClassName(),loDatabaseAction[i].getName()))
				{					
					loMethods[i] = loMenu.addAction(loDatabaseAction[i].getName());
					QObject::connect(loMethods[i], SIGNAL(triggered()), signalMapper, SLOT(map()));
					signalMapper->setMapping(loMethods[i], loDatabaseAction[i].getName());
				}
            }
			connect(signalMapper, SIGNAL(mapped(QString)),this, SLOT(slotCallAction(QString)));
			
			
			QAction *separator02 = new QAction(this);
			separator02->setSeparator(true);
			loMenu.addAction(separator02);
			
			
            QAction *loMethods02[500];			
			QSignalMapper* signalMapper02 = new QSignalMapper(this);
    		
            for (int i = 0; i < meClassLoader->meMethodsGeneralNames.size(); i ++)
            {
				if (methodInConfigFile(loConvertedItem->getClassName(),meClassLoader->meMethodsGeneralNames.at(i)))
				{
					loMethods02[i] = loMenu.addAction(meClassLoader->meMethodsGeneralNames.at(i));
					QObject::connect(loMethods02[i], SIGNAL(triggered()), signalMapper02, SLOT(map()));
					signalMapper02->setMapping(loMethods02[i], meClassLoader->meMethodsGeneralNames.at(i));
				}
            }
			connect(signalMapper02, SIGNAL(mapped(QString)),this, SLOT(slotCallMethod(QString)));            
			
			
			QAction *separator03 = new QAction(this);
			separator03->setSeparator(true);
			loMenu.addAction(separator03);			
			
			
			
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
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotTreeClassItemMenu -> " + QString(e.what()));
    }
}

//General method call
void clProductPlanner::slotCallMethod(const QString &paActionName)
{
	//TODO
	// Get and display the name of the computer.
	TCHAR infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	GetComputerName(infoBuf,&bufCharCount);	
	
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
					meClassLoader->meMethodCallGeneral.at(i)->doMethod(loCurrentMethodArgType, loCurrentMethodArgValue);
					break;
				}
			}
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clParameterView::slotCallMethod -> " + QString(e.what()));
    }	
}
void clProductPlanner::slotCallClassMethod(const QString &paActionName)
{
	//TODO
	// Get and display the name of the computer.
	TCHAR infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	GetComputerName(infoBuf,&bufCharCount);	
	
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
			
			
			for (int i = 0; i < meClassLoader->meClassNames.size(); i++)
			{
				if (meClassLoader->meClassNames.at(i).toUpper().compare(loConvertedItem->getClassName().toUpper()) == 0)
				{
					for (int j = 0; j < meClassLoader->meClassMethods.at(i).size(); j++)
					{
						if(paActionName.toUpper().compare(meClassLoader->meClassMethods.at(i).at(j).toUpper()) == 0)
						{
							//TODO create input screen
							//meClassLoader->meObjectCall.at(i)->doMethod();
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
void clObjectViewer::slotEditElement()
{
    try
    {
		//TODO
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);	
		
		
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
		meIceClientServer->getAllPropertiesFromTable(loItem->getClassName(),loPropertyName,loAlias,loType,loExtra,loReference,loMessage);
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotEditElement()-> Amount of properties=" + QString::number(loPropertyName.size()));
														
        
		vector<clDatabaseColumn> loDatabaseColumn;
		vector<std::string> loPropertyRequestedName;
		for (int i = 0; i < loPropertyName.size(); i++)
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
		meIceClientServer->getFromTableDatabaseById(    loItem->getClassName(),
														loItem->meUUID,
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
			
            meIceClientServer->updateIntoTableDatabase(    loItem->getClassName(),
															loItem->meUUID,
                                                            loColumns,
                                                            loValue,
                                                            loTypeValue,
                                                            loReturnMessage);
														
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotEditElement() -> " + loReturnMessage);
        }
        else
        {
			// Get and display the name of the computer.
			TCHAR infoBuf[INFO_BUFFER_SIZE];
			DWORD  bufCharCount = INFO_BUFFER_SIZE;
			GetComputerName(infoBuf,&bufCharCount);			
			
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotEditElement() -> update canceled");
        }


    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::slotEditElement -> " + QString(e.what()));
    }	
	
}
*/
/********************************************
* User interface funcitons
************************************************/
bool clProductPlanner::createProductsInTreeView(vector <std::string> paTables)
{

    try
    {
        removeElementsFromProductView();
        meProductPlanner.tvwProductView->setColumnCount(2);

        QStringList headers;
        headers << tr("Classes") << tr("Objects");
        meProductPlanner.tvwProductView->setHeaderLabels(headers);

        for (int i = 0; i < paTables.size();i++)
        {
            QString loTableName = QString(paTables[i].c_str());

			
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
															
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::createProductsInTreeView -> " + loReturnMessageObject);
			
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





			vector <std::string> loProperties;
			vector <std::string> loValue;
			vector <std::string> loTypeValue;
			vector <std::string> loLogExp;
			vector <std::string> loReturnIds;
			QString loReturnMessage;
			
			//Must have a current workstep
			loProperties.push_back("CURRENT_WORKSTEP");
			loValue.push_back("");
			loTypeValue.push_back("uuid");
			loLogExp.push_back("IS NOT");

			//Must be lower than finished
			loProperties.push_back("PRODUCT_WS_STATE");
			loValue.push_back("30");
			loTypeValue.push_back("int");
			loLogExp.push_back("<");

			//Must be set to free
			loProperties.push_back("PRODUCT_STATE");
			loValue.push_back("10");
			loTypeValue.push_back("int");
			loLogExp.push_back("=");

			QString loStart = QString("0");
			if(!meIceClientServer->getFromTableDatbaseByProperty(	loTableName,
																	loStart,
																	loStart,
																	loProperties,
																	loValue,
																	loTypeValue,
																	loLogExp,
																	loReturnIds,
																	loReturnMessage))
			{
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::createProductsInTreeView -> " + loReturnMessage);
			}
			else
			{
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::createProductsInTreeView -> " + loReturnMessage);
				
				for (int k = 0; k < loReturnIds.size(); k++)
				{
					vector <std::string> loProps;
					vector <std::string> loVal;
					QString loReturnMessageProduct;
					loProps.push_back("NAME");
					

					QString loUUid = QString(loReturnIds.at(k).c_str());
					if(!meIceClientServer->getFromTableDatabaseById(	loTableName,
																		loUUid,
																		loProps,
																		loVal,
																		loReturnMessageProduct))
					{
						meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::createProductsInTreeView -> " + loReturnMessageProduct);
					}
					else
					{
						meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::createProductsInTreeView -> " + loReturnMessageProduct);
						
						for (int m = 0; m < loVal.size(); m++)
						{
							clClassObjectTreeItem *newItem;
							newItem = new clClassObjectTreeItem(meProductPlanner.tvwProductView, meProductPlanner.tvwProductView->currentItem());
							//newItem->setDatabaseActions(loDatabaseActionArr);
							newItem->setDatabaseColumns(loDatabaseColumnArr);
							newItem->setClassName(loTableName);
							newItem->meUUID = QString(loReturnIds.at(k).c_str());
							newItem->meNameDisplay = QString(loVal.at(m).c_str());
							newItem->meIsInfo = false;
							const QIcon Icon(".\\ICONS\\" + getRelatedIcon(loTableName));
							newItem->setIcon(0,Icon);
							newItem->setText(0,QString(loVal.at(m).c_str()));
							
							meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::createProductsInTreeView -> Added ID [" + newItem->meUUID + "]");
						}
					}
				}
			}
        }
		
        return true;
    }
    catch(exception &e)
    {	
		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::createProductsInTreeView -> " + QString(e.what()));
        return false;
    }	
}
bool clProductPlanner::createClassesInTreeView(vector<std::string> paTables)
{
	try
    {
        removeElementsFromMachineView();
        meProductPlanner.tvwMachineView->setColumnCount(2);

        QStringList headers;
        headers << tr("Classes") << tr("Objects");
        meProductPlanner.tvwMachineView->setHeaderLabels(headers);

        for (int i = 0; i < paTables.size();i++)
        {
            QString loTableName = QString(paTables[i].c_str());

			
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
															
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::createClassesInTreeView -> " + loReturnMessageObject);
			
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
			QString loStartT = QString("0");
			if (!meIceClientServer->getFromTableDatabaseGeneral( loTableName, loStartT, loStartT, loReturnId, loReturnName, loReturnMessage))
			{
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::createClassesInTreeView -> " + loReturnMessage);
			}
			else
			{
				for (int k = 0; k < loReturnId.size(); k++)
				{
					clClassObjectTreeItem *newItem;
					newItem = new clClassObjectTreeItem(meProductPlanner.tvwMachineView, meProductPlanner.tvwMachineView->currentItem());
					//newItem->setDatabaseActions(loDatabaseActionArr);
					newItem->setDatabaseColumns(loDatabaseColumnArr);
					newItem->setClassName(loTableName);
					newItem->meUUID = QString(loReturnId.at(k).c_str());
					newItem->meNameDisplay = QString(loReturnName.at(k).c_str());
					newItem->meIsInfo = false;
					const QIcon Icon(".\\ICONS\\" + getRelatedIcon(loTableName));
					newItem->setIcon(0,Icon);
					newItem->setText(0,QString(loReturnName.at(k).c_str()));
					
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::createClassesInTreeView -> Added ID [" + newItem->meUUID + "]");
				}
			}




        }
		

        return true;
    }
    catch(exception &e)
    {	
		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::createClassesInTreeView -> " + QString(e.what()));
        return false;
    }
}


bool clProductPlanner::removeElementsFromMachineView()
{
	try
    {
        QList<QTreeWidgetItem *> paColumnItems;

        for (int i = 0; i < meProductPlanner.tvwMachineView->topLevelItemCount(); ++i)
        {
            paColumnItems << meProductPlanner.tvwMachineView->topLevelItem(i);
        }
        qDeleteAll(paColumnItems.begin(),paColumnItems.end());
        return true;
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::removeElementsFromMachineView -> " + QString(e.what()));
        return false;
    }
}
bool clProductPlanner::removeElementsFromProductView()
{
	try
    {
        QList<QTreeWidgetItem *> paColumnItems;

        for (int i = 0; i < meProductPlanner.tvwProductView->topLevelItemCount(); ++i)
        {
            paColumnItems << meProductPlanner.tvwProductView->topLevelItem(i);
        }
        qDeleteAll(paColumnItems.begin(),paColumnItems.end());
        return true;
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::removeElementsFromProductView -> " + QString(e.what()));
        return false;
    }
}
/*
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
		
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::addElementsToQTreeWidget -> " + QString(e.what()));
        return false;
    }
}

bool clObjectViewer::addElementToQTreeWidgetSub(QTreeWidgetItem * paParent, QString paID, QString paTable)
{
		// Get and display the name of the computer.
	TCHAR infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
	GetComputerName(infoBuf,&bufCharCount);
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
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::addElementsToQTreeWidgetSub -> " + loReturnMessageGetById);
			
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
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::addElementsToQTreeWidget -> " + QString(e.what()));
        return false;
    }
}
*/
