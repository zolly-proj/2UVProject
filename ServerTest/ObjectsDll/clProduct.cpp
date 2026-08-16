#define INFO_BUFFER_SIZE 32767
#include "clProduct.h"
clProduct::clProduct()
{
    try
    {
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
    }	
    catch(...)
    {
        printf("clProduct::clProduct-> error ...");
    }
}
clProduct::~clProduct ()
{
	try
	{
	}
	catch(...)
	{
		
	}
}
int clProduct::GetReturnParameters()
{
    try
    {
		return 0;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
			return -1;
    }	
    catch(...)
    {
		return -1;
    }
}
bool clProduct::doMethod(QString paMethodName, const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue, const vector <QString> &paLogExp)
{
	try
	{	
		if (paMethodName.toUpper().compare(QString("SETPRIORITYANDWORKSTEP")) == 0)
		{
			return setPriorityAndWorkstep(paParametersType, paParameters, paParametersValue);
		}
		if (paMethodName.toUpper().compare(QString("SETNEXTWORKSTEP")) == 0)
		{
			return setNextWorkstep(paParametersType, paParameters, paParametersValue);
		}
		if (paMethodName.toUpper().compare(QString("SETPREVIOUSWORKSTEP")) == 0)
		{
			return setPreviousWorkstep(paParametersType, paParameters, paParametersValue);
		}		
		else return false;
	}
	catch(...)
	{
		return false;
	}
}
bool clProduct::setPriorityAndWorkstep(const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue)
{
	try
	{
		//Getting the product object
		/*
		  	QString ClassName;
			QString ObjectId;	
			vector <clObjectCall *> meObjectCalls;
			vector <QString> meClassNameList;
			vector <vector<QString>> meClassMethodsList;

			vector <clMethodCall *> meMethodCalls;
			vector <QString> meMethodNamesList;
			
			vector <vector<clDatabaseColumn *>> meDatabaseColumnsByClassNameList;
		*/
		
		//Getting the product object
		clObject loObject(meIceClientServer,meIceClientLogging);
		
		loObject.meObjectCalls = meObjectCall;
		loObject.meClassNameList = meClassNames;
		loObject.meClassMethodsList = meClassMethods;
		
		loObject.meMethodNamesList = meMethodsNameList;
		loObject.meMethodCalls = meMethodCallGeneral;
		loObject.meDatabaseColumnsByClassNameList = meDatabaseColumnsByClassNameList;
			
		for (int i = 0; i < paParametersType.size(); i++)
		{
			if (paParametersType.at(i).toUpper().compare(QString("CLASS")) == 0)
				loObject.ClassName = paParametersValue.at(i).toUpper();
			if (paParametersType.at(i).toUpper().compare(QString("UUID")) == 0)
				loObject.ObjectId = paParametersValue.at(i);
		}
		
		if (loObject.ObjectId.compare(QString("")) == 0 || loObject.ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> wrong parameters in call of method");
			return false;
		}
		
		//Getting the batch object
		vector <clObject> loObjectsBatch;
		if (loObject.getParents(QString("BATCH"),QString("PRODUCTS"),loObjectsBatch))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> got the parent batch of the object");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> could not get the parent batch of the object");
			return false;
		}
		
		//Check if only one batch
		if (loObjectsBatch.size() == 1)
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> one batch found");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> multiple or none batch found");
			return false;
		}
		
		//Getting the worksteps
		vector <clObject> loObjectsWs;
		if (loObjectsBatch.at(0).get("WORKSTEPS",loObjectsWs))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting worksteps ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting worksteps nok");
			return false;
		}
		
		
		
		vector <clObject *> loObjectsWsPointer;
		
		for (int i = 0 ; i < loObjectsWs.size(); i++)
			loObjectsWsPointer.push_back(&loObjectsWs.at(i));
			
		//Start the UI with the parameters
		clProductSetPriorityAndWorkstepUI *loProductSetPriorityAndWorkstep = new clProductSetPriorityAndWorkstepUI(meIceClientServer,meIceClientLogging, meDatabaseColumnsByClassNameList);
		loProductSetPriorityAndWorkstep->setContentOfUI(&loObject,&loObjectsBatch.at(0),loObjectsWsPointer);
		if (loProductSetPriorityAndWorkstep->exec() == 0)
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clProduct::setPriorityAndWorkstep -> save changes"));
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clProduct::setPriorityAndWorkstep -> canceled"));
		}
		
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> " + QString(e.what()));
		return false;
	}
}
bool clProduct::setNextWorkstep(const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue)
{
	try
	{
		/*
		  	QString ClassName;
			QString ObjectId;	
			vector <clObjectCall *> meObjectCalls;
			vector <QString> meClassNameList;
			vector <vector<QString>> meClassMethodsList;

			vector <clMethodCall *> meMethodCalls;
			vector <QString> meMethodNamesList;
			
			vector <vector<clDatabaseColumn *>> meDatabaseColumnsByClassNameList;
		*/
		
		//Getting the product object
		clObject loObject(meIceClientServer,meIceClientLogging);
		
		loObject.meObjectCalls = meObjectCall;
		loObject.meClassNameList = meClassNames;
		loObject.meClassMethodsList = meClassMethods;
		
		loObject.meMethodNamesList = meMethodsNameList;
		loObject.meMethodCalls = meMethodCallGeneral;
		loObject.meDatabaseColumnsByClassNameList = meDatabaseColumnsByClassNameList;
	
		for (int i = 0; i < paParametersType.size(); i++)
		{
			if (paParametersType.at(i).toUpper().compare(QString("CLASS")) == 0)
				loObject.ClassName = paParametersValue.at(i).toUpper();
			if (paParametersType.at(i).toUpper().compare(QString("UUID")) == 0)
				loObject.ObjectId = paParametersValue.at(i);
		}
		
		if (loObject.ObjectId.compare(QString("")) == 0 || loObject.ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> wrong parameters in call of method");
			return false;
		}
		
		//Getting the batch object
		vector <clObject> loObjectsBatch;
		if (loObject.getParents(QString("BATCH"),QString("PRODUCTS"),loObjectsBatch))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> got the parent batch of the object");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> could not get the parent batch of the object");
			return false;
		}
		
		//Check if only one batch
		if (loObjectsBatch.size() == 1)
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> one batch found");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> multiple or none batch found");
			return false;
		}
		
		//Getting the worksteps
		vector <clObject> loObjectsWs;
		if (loObjectsBatch.at(0).get("WORKSTEPS",loObjectsWs))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> getting worksteps ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> getting worksteps nok");
			return false;
		}

		int loProdState;
		if (loObject.get("PRODUCT_STATE",loProdState))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> getting product state ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> getting product state nok");
			return false;
		}		

		int loProdWsState;
		if (loObject.get("PRODUCT_WS_STATE",loProdWsState))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> getting product ws state ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> getting product ws state nok");
			return false;
		}		
		
		clObject loPreviousWs(meIceClientServer, meIceClientLogging);
		if (loObject.get("PREVIOUS_WORKSTEP", loPreviousWs))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> getting product previous ws ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> getting product previous ws nok");
			return false;
		}	
		
		clObject loCurrentWs(meIceClientServer, meIceClientLogging);
		if (loObject.get("CURRENT_WORKSTEP", loCurrentWs))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> getting product current ws ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> getting product current ws nok");
			return false;
		}			
		
		clObject loNextWs(meIceClientServer, meIceClientLogging);
		if (loObject.get("NEXT_WORKSTEP", loNextWs))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> getting product next ws ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> getting product next ws nok");
			return false;
		}		
		
		if (loNextWs.ObjectId.compare(QString("")) == 0)
		{
			//No next ws
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> no next ws");
			//Set to finisched
			loObject.set("PRODUCT_WS_STATE", 30);
			
		}
		else
		{
			//if loProdState = Free and loProdWsStae = Done
			if (loProdState == 10 && loProdWsState == 20)
			{
				//Next ws found set next ws
				loObject.set("CURRENT_WORKSTEP", loNextWs);
				loObject.set("PREVIOUS_WORKSTEP",loCurrentWs);
				//Set to started
				loObject.set("PRODUCT_WS_STATE", 10);
				for (int i = 0; i < loObjectsWs.size(); i++)
				{
					if (loObjectsWs.at(i).ObjectId.compare(loNextWs.ObjectId) == 0)
					{
						if (i < loObjectsWs.size() - 1)
						{
							//Set the next ws to the following ws
							loObject.set("NEXT_WORKSTEP", loObjectsWs.at(i+1));
						}
						else if (i == loObjectsWs.size() - 1)
						{
							clObject loNewNextWs(meIceClientServer, meIceClientLogging);
							//Set the next ws to empty because las ws
							loObject.set("NEXT_WORKSTEP", loNewNextWs);
						}
					}
				}
			}
		}
		
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setNextWorkstep -> " + QString(e.what()));
		return false;
	}
}
bool clProduct::setPreviousWorkstep(const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue)
{
	try
	{
		/*
		  	QString ClassName;
			QString ObjectId;	
			vector <clObjectCall *> meObjectCalls;
			vector <QString> meClassNameList;
			vector <vector<QString>> meClassMethodsList;

			vector <clMethodCall *> meMethodCalls;
			vector <QString> meMethodNamesList;
			
			vector <vector<clDatabaseColumn *>> meDatabaseColumnsByClassNameList;
		*/
		
		//Getting the product object
		clObject loObject(meIceClientServer,meIceClientLogging);
		
		loObject.meObjectCalls = meObjectCall;
		loObject.meClassNameList = meClassNames;
		loObject.meClassMethodsList = meClassMethods;
		
		loObject.meMethodNamesList = meMethodsNameList;
		loObject.meMethodCalls = meMethodCallGeneral;
		loObject.meDatabaseColumnsByClassNameList = meDatabaseColumnsByClassNameList;
		
		for (int i = 0; i < paParametersType.size(); i++)
		{
			if (paParametersType.at(i).toUpper().compare(QString("CLASS")) == 0)
				loObject.ClassName = paParametersValue.at(i).toUpper();
			if (paParametersType.at(i).toUpper().compare(QString("UUID")) == 0)
				loObject.ObjectId = paParametersValue.at(i);
		}
		
		if (loObject.ObjectId.compare(QString("")) == 0 || loObject.ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> wrong parameters in call of method");
			return false;
		}
		
		//Getting the batch object
		vector <clObject> loObjectsBatch;
		if (loObject.getParents(QString("BATCH"),QString("PRODUCTS"),loObjectsBatch))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> got the parent batch of the object");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> could not get the parent batch of the object");
			return false;
		}
		
		//Check if only one batch
		if (loObjectsBatch.size() == 1)
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> one batch found");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> multiple or none batch found");
			return false;
		}
		
		//Getting the worksteps
		vector <clObject> loObjectsWs;
		if (loObjectsBatch.at(0).get("WORKSTEPS",loObjectsWs))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting worksteps ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting worksteps nok");
			return false;
		}

		int loProdState;
		if (loObject.get("PRODUCT_STATE",loProdState))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting product state ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting product state nok");
			return false;
		}		

		int loProdWsState;
		if (loObject.get("PRODUCT_WS_STATE",loProdWsState))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting product ws state ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting product ws state nok");
			return false;
		}		
		
		clObject loPreviousWs(meIceClientServer, meIceClientLogging);
		if (loObject.get("PREVIOUS_WORKSTEP", loPreviousWs))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting product previous ws ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting product previous ws nok");
			return false;
		}	
		
		clObject loCurrentWs(meIceClientServer, meIceClientLogging);
		if (loObject.get("CURRENT_WORKSTEP", loCurrentWs))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting product current ws ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting product current ws nok");
			return false;
		}			
		
		clObject loNextWs(meIceClientServer, meIceClientLogging);
		if (loObject.get("NEXT_WORKSTEP", loNextWs))
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting product next ws ok");
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> getting product next ws nok");
			return false;
		}		
		
		if (loPreviousWs.ObjectId.compare(QString("")) == 0)
		{
			//No next ws
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::setPriorityAndWorkstep -> no next ws");
			//Set to finisched
			loObject.set("PRODUCT_WS_STATE", 10);
			
		}
		else
		{
			//if loProdState = Free and loProdWsStae = Done
			if (loProdState == 10)
			{
				//Next ws found set next ws
				loObject.set("CURRENT_WORKSTEP", loPreviousWs);
				loObject.set("NEXT_WORKSTEP",loCurrentWs);
				//Set to started
				loObject.set("PRODUCT_WS_STATE", 10);
				for (int i = 0; i < loObjectsWs.size(); i++)
				{
					if (loObjectsWs.at(i).ObjectId.compare(loPreviousWs.ObjectId) == 0)
					{
						if (i > 0)
						{
							//Set the previous ws to the precending ws
							clObject loNewNextWs(meIceClientServer, meIceClientLogging);
							loObject.set("PREVIOUS_WORKSTEP", loObjectsWs.at(i-1));
						}
						else if (i == 0)
						{
							clObject loNewPreviousWs(meIceClientServer, meIceClientLogging);
							//Set the next ws to empty because first ws
							loObject.set("PREVIOUS_WORKSTEP", loNewPreviousWs);
						}
					}
				}
			}
		}
		
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clOperation::openOperation -> " + QString(e.what()));
		return false;
	}
}
bool clProduct::createPluginClass( clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging)	
{
    try
    {
		meIceClientServer = paIceClientServer;
		meIceClientLogging = paIceClientLogging;
		return true;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
			return false;
    }	
    catch(...)
    {
		return false;
    }
}
bool clProduct::createClassMethodsCall(vector <QString> paClassNames,vector <vector<QString>> paClassMethods,vector <clObjectCall *> paObjectCall)
{
	try
	{
	//Loaded dll's to call the methods
		meClassNames = paClassNames;
		meClassMethods = paClassMethods;
		meObjectCall = paObjectCall;
		return true;
	}
	catch(exception &e)
	{	
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::createClassMethodsCall -> " + QString(e.what()));
		return false;
	}
}	
bool clProduct::createGeneralMethodsCall(vector <QString> paMethodsNameList,vector <clMethodCall *>paMethodCallGeneral)
{
	try
	{
		meMethodsNameList = paMethodsNameList;
		meMethodCallGeneral = paMethodCallGeneral;		
		return true;
	}
	catch(exception &e)
	{	
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::createGeneralMethodsCall -> " + QString(e.what()));
		return false;
	}
}
bool clProduct::createDatabaseColumnsByClassNameList(vector <vector<clDatabaseColumn*>> paDatabaseColumnsByClassNameList)
{	
	try
	{
		meDatabaseColumnsByClassNameList = paDatabaseColumnsByClassNameList;
		return true;
	}
	catch(exception &e)
	{	
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clProduct::createDatabaseColumnsByClassNameList -> " + QString(e.what()));
		return false;
	}	
}
