#define INFO_BUFFER_SIZE 32767
#include "clErrorTable.h"
clErrorTable::clErrorTable()
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
        printf("clErrorTable::clErrorTable-> error ...");
    }
}
clErrorTable::~clErrorTable ()
{
	try
	{
	}
	catch(...)
	{
		
	}
}
int clErrorTable::GetReturnParameters()
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
bool clErrorTable::doMethod(QString paMethodName, const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue, const vector <QString> &paLogExp)
{	
	try
	{	
		if (paMethodName.toUpper().compare(QString("GETERRORNUMBERFROMTEXT")) == 0)
		{
			return getErrorNumberFromText(paParametersType, paParametersValue);
		}
		else if (paMethodName.toUpper().compare(QString("GETTEXTFROMERRORNUMBER")) == 0)
		{
			return getTextFromErrorNumber(paParametersType, paParametersValue);
		}
		else return false;
	}
	catch(...)
	{
		return false;
	}
}
bool clErrorTable::getErrorNumberFromText(const vector <QString> &paParametersType, const vector <QString> &paParametersValue)
{
	try
	{

		//////////////////////////////// GET THE ERROR TABLES ////////////////////////////////////////
		vector<std::string> loPropertyName;
		vector<std::string> loReturnValues;
		QString loReturnMessageGetById;
		loPropertyName.push_back("STANDARD_ERRORS");
		loPropertyName.push_back("CUSTOM_ERRORS");
		
		QString loTableName = QString(paParametersValue.at(0));
		QString loObjectID = QString(paParametersValue.at(1));
		
		QString loError = QString(paParametersValue.at(2));
		
		bool loGetById = false;
		loGetById = meIceClientServer->getFromTableDatabaseById(    loTableName,
																	loObjectID,
																	loPropertyName,
																	loReturnValues,
																	loReturnMessageGetById);
		
		if(!loGetById)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::getErrorNumberFromText -> " + loReturnMessageGetById);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::getErrorNumberFromText -> " + loReturnMessageGetById);
			
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::getErrorNumberFromText -> no result in getting variables");
			return false;
		}
		
		QString loStandardErrorTable = QString(loReturnValues.at(0).c_str());
		QString loCustomErrorTable = QString(loReturnValues.at(1).c_str());
		
		vector <QString> loErrorNumber;
		vector <QString> loErrorText;
		vector <QString> loMethodCall;
		
		initializeErrorSearch(loStandardErrorTable, loErrorNumber, loErrorText, loMethodCall);
		if (findErrorInText(loError, loErrorNumber, loErrorText, loMethodCall)) return true;
		
		loErrorNumber.clear();
		loErrorText.clear();
		loMethodCall.clear();		
		
		initializeErrorSearch(loCustomErrorTable, loErrorNumber, loErrorText, loMethodCall);
		if (findErrorInText(loError, loErrorNumber, loErrorText, loMethodCall))	return true;
		
		return false;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::getErrorNumberFromText -> " + QString(e.what()));
		return false;
	}
}
bool clErrorTable::initializeErrorSearch(QString paErrorTable, vector <QString> &paErrorNumber, vector <QString> &paErrorText, vector <QString> &paMethod)
{
	try
	{
		vector <int> meNumbers;
		vector <QString> meText;
		vector <QString> meMethod;
		
		QStringList loElements = paErrorTable.remove("}").remove("{").split(",", QString::SkipEmptyParts);
		
		for (int k = 0; k < (int) loElements.size(); k++)
		{	
			QStringList loRow = loElements.at(k).split(";");
			if (loRow.size() == 0) meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clErrorTable::getErrorNumberFromText -> wrong entry in error table on place " + QString::number(k)));
			else if(loRow.size() == 1) meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clErrorTable::getErrorNumberFromText -> wrong entry in error table on place " + QString::number(k)));
			else if(loRow.size() == 2)
			{
				paErrorNumber.push_back(loRow.at(0));
				paErrorText.push_back(loRow.at(1));
				paMethod.push_back(QString(""));				
			}
			else if(loRow.size() == 3)
			{
				paErrorNumber.push_back(loRow.at(0));
				paErrorText.push_back(loRow.at(1));
				paMethod.push_back(loRow.at(2));
			}
		}
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::getErrorNumberFromText -> " + QString(e.what()));
		return false;
	}
}
bool clErrorTable::findErrorInText(QString paError, vector <QString> &paErrorNumber, vector <QString> &paErrorText, vector <QString> &paMethod)
{
	try
	{
		for (int i = 0; i < (int) paErrorText.size(); i++)
		{
			if (paError.contains(paErrorText.at(i)))
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clErrorTable::getErrorNumberFromText -> error found [" + paErrorNumber.at(i) + "]"));
				clObjectCall::meReturnParameters.push_back(paErrorNumber.at(i));
				clObjectCall::meReturnParametersType.push_back(QString("text"));
				clObjectCall::meReturnParameters.push_back(paMethod.at(i));
				clObjectCall::meReturnParametersType.push_back(QString("METHODCALL"));
				return true;
			}
		}
		return false;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::getErrorNumberFromText -> " + QString(e.what()));
		return false;
	}	
}
bool clErrorTable::findTextInError(QString paError, vector <QString> &paErrorNumber, vector <QString> &paErrorText, vector <QString> &paMethod)
{
	try
	{
		for (int i = 0; i < (int) paErrorNumber.size(); i++)
		{
			if (paError.compare(paErrorNumber.at(i)) == 0)
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clErrorTable::getErrorNumberFromText -> error found [" + paErrorText.at(i) + "]"));
				clObjectCall::meReturnParameters.push_back(paErrorText.at(i));
				clObjectCall::meReturnParametersType.push_back(QString("text"));
				clObjectCall::meReturnParameters.push_back(paMethod.at(i));
				clObjectCall::meReturnParametersType.push_back(QString("METHODCALL"));
				return true;
			}
		}
		return false;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::getErrorNumberFromText -> " + QString(e.what()));
		return false;
	}	
}

bool clErrorTable::getTextFromErrorNumber(const vector <QString> &paParametersType, const vector <QString> &paParametersValue)
{
	try
	{

		//////////////////////////////// GET THE ERROR TABLES ////////////////////////////////////////
		vector<std::string> loPropertyName;
		vector<std::string> loReturnValues;
		QString loReturnMessageGetById;
		loPropertyName.push_back("STANDARD_ERRORS");
		loPropertyName.push_back("CUSTOM_ERRORS");
		
		QString loTableName = QString(paParametersValue.at(0));
		QString loObjectID = QString(paParametersValue.at(1));
		QString loError = QString(paParametersValue.at(2));
		
		bool loGetById = false;
		loGetById = meIceClientServer->getFromTableDatabaseById(    loTableName,
																	loObjectID,
																	loPropertyName,
																	loReturnValues,
																	loReturnMessageGetById);
		if(!loGetById)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::getTextFromErrorNumber -> " + loReturnMessageGetById);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::getTextFromErrorNumber -> " + loReturnMessageGetById);
			
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::getTextFromErrorNumber -> no result in getting variables");
			return false;
		}
		
		QString loStandardErrorTable = QString(loReturnValues.at(0).c_str());
		QString loCustomErrorTable = QString(loReturnValues.at(1).c_str());
		
		vector <QString> loErrorNumber;
		vector <QString> loErrorText;
		vector <QString> loMethodCall;
		
		initializeErrorSearch(loStandardErrorTable, loErrorNumber, loErrorText, loMethodCall);
		if (findTextInError(loError, loErrorNumber, loErrorText, loMethodCall)) return true;
		
		loErrorNumber.clear();
		loErrorText.clear();
		loMethodCall.clear();		
		
		initializeErrorSearch(loStandardErrorTable, loErrorNumber, loErrorText, loMethodCall);
		if (findTextInError(loError, loErrorNumber, loErrorText, loMethodCall))	return true;
		
		return false;	
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::getTextFromErrorNumber -> " + QString(e.what()));
		return false;
	}
}
bool clErrorTable::createPluginClass( clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging)	
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
bool clErrorTable::createClassMethodsCall(vector <QString> paClassNames,vector <vector<QString>> paClassMethods,vector <clObjectCall *> paObjectCall)
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
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::createClassMethodsCall -> " + QString(e.what()));
		return false;
	}
}	
bool clErrorTable::createGeneralMethodsCall(vector <QString> paMethodsNameList,vector <clMethodCall *>paMethodCallGeneral)
{
	try
	{
		meMethodsNameList = paMethodsNameList;
		meMethodCallGeneral = paMethodCallGeneral;		
		return true;
	}
	catch(exception &e)
	{	
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::createGeneralMethodsCall -> " + QString(e.what()));
		return false;
	}
}
bool clErrorTable::createDatabaseColumnsByClassNameList(vector <vector<clDatabaseColumn*>> paDatabaseColumnsByClassNameList)
{	
	try
	{
		meDatabaseColumnsByClassNameList = paDatabaseColumnsByClassNameList;
		return true;
	}
	catch(exception &e)
	{	
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clErrorTable::createDatabaseColumnsByClassNameList -> " + QString(e.what()));
		return false;
	}	
}

