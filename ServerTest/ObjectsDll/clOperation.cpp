#define INFO_BUFFER_SIZE 32767
#include "clOperation.h"
clOperation::clOperation()
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
        printf("clOperation::clOperation-> error ...");
    }
}
clOperation::~clOperation ()
{
	try
	{
	}
	catch(...)
	{
		
	}
}
int clOperation::GetReturnParameters()
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
bool clOperation::doMethod(QString paMethodName, const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue, const vector <QString> &paLogExp)
{	
	try
	{	
		if (paMethodName.toUpper().compare(QString("OPENOPERATION")) == 0)
		{
			return openOperation(paParametersType, paParameters, paParametersValue);
		}
		else if (paMethodName.toUpper().compare(QString("CLOSEOPERATION")) == 0)
		{
			return closeOperation(paParametersType, paParameters, paParametersValue);
		}
		else return false;
	}
	catch(...)
	{
		return false;
	}
}
bool clOperation::openOperation(const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue)
{		
	try
	{
		vector<std::string> loPropertyName;
		vector<std::string> loValues;
		vector<std::string> loTypeValues;
		QString loReturnMessageInsert;
		
		QString loTableName = QString(paParametersValue.at(0));
		
		for (int i = 1; i < paParametersValue.size(); i++)
		{
			loPropertyName.push_back(paParameters.at(i).toStdString());
			loValues.push_back(paParametersValue.at(i).toStdString());
			loTypeValues.push_back(paParametersType.at(i).toStdString());
		}

		bool loInsertDB = false;
		
		loInsertDB = meIceClientServer->insertIntoTableDatabase(	loTableName,
																	loPropertyName,
																	loValues,
																	loTypeValues,
																	loReturnMessageInsert);
		if (!loInsertDB)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMach::readTcpData -> " + loReturnMessageInsert);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clLivingObjectMach::readTcpData -> " + loReturnMessageInsert);
		
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clOperation::openOperation -> " + QString(e.what()));
		return false;
	}
}
bool clOperation::closeOperation(const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue)
{
	try
	{
			vector<std::string> loPropertyName;
			vector<std::string> loValues;
			vector<std::string> loTypeValues;
			QString loReturnMessageUpdate;
			
			QString loTableName = paParametersValue.at(0);
			QString loObjectId = paParametersValue.at(1);
			
			for (int i = 2; i < paParametersValue.size(); i++)
			{	
				loPropertyName.push_back(paParameters.at(i).toStdString());
				loValues.push_back(paParametersValue.at(i).toStdString());
				loTypeValues.push_back(paParametersType.at(i).toStdString());
			}
			
			bool loUpdateDb = false;
			
			
			loUpdateDb = meIceClientServer->updateIntoTableDatabase( 	loTableName,
																		loObjectId,
																		loPropertyName,
																		loValues,
																		loTypeValues,
																		loReturnMessageUpdate);	
			if (!loUpdateDb)
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clOperation::closeOperation -> " + loReturnMessageUpdate);		   
				return false;
			}
			else
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clOperation::closeOperation -> " + loReturnMessageUpdate);		
		return true;	
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clOperation::closeOperation -> " + QString(e.what()));
		return false;
	}
}
bool clOperation::createPluginClass( clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging)	
{
    try
    {
        cout << "TESTING" << endl;
		meIceClientServer = paIceClientServer;
        meIceClientLogging = paIceClientLogging;
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clOperation::createPluginClass -> created");

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
bool clOperation::createClassMethodsCall(vector <QString> paClassNames,vector <vector<QString>> paClassMethods,vector <clObjectCall *> paObjectCall)
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
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clOperation::createClassMethodsCall -> " + QString(e.what()));
		return false;
	}
}	
bool clOperation::createGeneralMethodsCall(vector <QString> paMethodsNameList,vector <clMethodCall *>paMethodCallGeneral)
{
	try
	{
		meMethodsNameList = paMethodsNameList;
		meMethodCallGeneral = paMethodCallGeneral;		
		return true;
	}
	catch(exception &e)
	{	
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clOperation::createGeneralMethodsCall -> " + QString(e.what()));
		return false;
	}
}
bool clOperation::createDatabaseColumnsByClassNameList(vector <vector<clDatabaseColumn*>> paDatabaseColumnsByClassNameList)
{	
	try
	{
		meDatabaseColumnsByClassNameList = paDatabaseColumnsByClassNameList;
		return true;
	}
	catch(exception &e)
	{	
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clOperation::createDatabaseColumnsByClassNameList -> " + QString(e.what()));
		return false;
	}	
}

