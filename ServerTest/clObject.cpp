#define INFO_BUFFER_SIZE 32767
#include "clObject.h"

clObject::clObject(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging)
{
	try
	{
		meIceClientLogging = paIceClientLogging;
		meIceClientServer = paIceClientServer;
	}
	catch(...)
	{
		cout << "clWorkstationCycles::clWorkstationCycles -> failed" << endl;
	}
}
clObject::clObject(clObject * paObject)
{
	try
	{
		
		meIceClientLogging = paObject->meIceClientLogging;
		meIceClientServer = paObject->meIceClientServer;

		ClassName = paObject->ClassName;
		ObjectId = paObject->ObjectId;

		meObjectCalls = paObject->meObjectCalls;
		meClassNameList = paObject->meClassNameList;
		meClassMethodsList = paObject->meClassMethodsList;

		meMethodCalls = paObject->meMethodCalls;
		meMethodNamesList = paObject->meMethodNamesList;

		meDatabaseColumnsByClassNameList = paObject->meDatabaseColumnsByClassNameList;
	}
	catch(...)
	{
	}
}
clObject::~clObject()
{
}
bool clObject::get(QString paPropName, QString &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, QString &paValue) -> Classname or id is empty ...");
			return false;
		}
		
		/*****************************************
		* Getting the PROP from table
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back(paPropName.toUpper().toStdString());
		
		QString loClassName = ClassName.toUpper();
		
		if (!meIceClientServer->getFromTableDatabaseById(loClassName,ObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, QString &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, QString &paValue) -> " + loReturnMessage);			
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, QString &paValue) -> no values to return ...");
			return false;					
		}
		
		paValue = QString(loReturnValues.at(0).c_str());
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, QString &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::get(QString paPropName, vector <QString> &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <QString> &paValue) -> Classname or id is empty ...");
			return false;
		}
		/*****************************************
		* Getting the PROP from table
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back(paPropName.toUpper().toStdString());
		
		QString loClassName = ClassName.toUpper();
		
		if (!meIceClientServer->getFromTableDatabaseById(loClassName,ObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <QString> &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <QString> &paValue) -> " + loReturnMessage);			
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <QString> &paValue) -> no values to return ...");
			return false;					
		}
		
		paValue.clear();
		QString loValues = QString(loReturnValues.at(0).c_str());
		loValues = loValues.replace("{","").replace("}","");
		if (loValues.compare(QString("")) != 0)
		{
			QStringList loStringList = loValues.split(",");
			for (int i = 0; i < loStringList.size(); i++)
			{
					paValue.push_back(loStringList.at(i));
			}
		}
		return true;				
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <QString> &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::get(QString paPropName, QDateTime &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, QDateTime &paValue) -> Classname or id is empty ...");
			return false;
		}	
		/*****************************************
		* Getting the PROP from table
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back(paPropName.toUpper().toStdString());
		
		QString loClassName = ClassName.toUpper();
		if (!meIceClientServer->getFromTableDatabaseById(loClassName,ObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, QDateTime &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, QDateTime &paValue) -> " + loReturnMessage);			
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, QDateTime &paValue) -> no values to return ...");
			return false;					
		}
		
		QDateTime loDateTime = QDateTime::fromString(loReturnValues.at(0).c_str(), "yyyy-MM-dd hh:mm:ss.zzz");
		paValue = loDateTime;
		
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, QDateTime &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::get(QString paPropName, vector <QDateTime> &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <QDateTime> &paValue) -> Classname or id is empty ...");
			return false;
		}	
		/*****************************************
		* Getting the PROP from table
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back(paPropName.toUpper().toStdString());
		
		QString loClassName = ClassName.toUpper();
		
		
		if (!meIceClientServer->getFromTableDatabaseById(loClassName,ObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <QDateTime> &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <QDateTime> &paValue) -> " + loReturnMessage);			
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <QDateTime> &paValue) -> no values to return ...");
			return false;					
		}
		
		paValue.clear();
		QString loValues = QString(loReturnValues.at(0).c_str());
		loValues = loValues.replace("{","").replace("}","");
		if (loValues.compare(QString("")) != 0)
		{
			QStringList loStringList = loValues.split(",");
			for (int i = 0; i < loStringList.size(); i++)
			{
				QDateTime loDateTime = QDateTime::fromString(loStringList.at(i), "yyyy-MM-dd hh:mm:ss.zzz");
				paValue.push_back(loDateTime);
			}
		}		
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <QDateTime> &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::get(QString paPropName, float &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, float &paValue) -> Classname or id is empty ...");
			return false;
		}		
		/*****************************************
		* Getting the PROP from table
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back(paPropName.toUpper().toStdString());
		
		QString loClassName = ClassName.toUpper();
		
		
		if (!meIceClientServer->getFromTableDatabaseById(loClassName,ObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, float &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, float &paValue) -> " + loReturnMessage);			
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, float &paValue) -> no values to return ...");
			return false;					
		}
		
		paValue = QString(loReturnValues.at(0).c_str()).toFloat();
		
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, float &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::get(QString paPropName, vector <float> &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <float> &paValue) -> Classname or id is empty ...");
			return false;
		}		
		/*****************************************
		* Getting the PROP from table
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back(paPropName.toUpper().toStdString());
		
		QString loClassName = ClassName.toUpper();
		
		if (!meIceClientServer->getFromTableDatabaseById(loClassName,ObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <float> &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <float> &paValue) -> " + loReturnMessage);			
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <float> &paValue) -> no values to return ...");
			return false;					
		}
		
		paValue.clear();
		QString loValues = QString(loReturnValues.at(0).c_str());
		loValues = loValues.replace("{","").replace("}","");
		if (loValues.compare(QString("")) != 0)
		{
			QStringList loStringList = loValues.split(",");
			for (int i = 0; i < loStringList.size(); i++)
			{
				paValue.push_back(loStringList.at(i).toFloat());
			}
		}		
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <float> &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::get(QString paPropName, int &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, int &paValue) -> Classname or id is empty ...");
			return false;
		}		
		/*****************************************
		* Getting the PROP from table
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back(paPropName.toUpper().toStdString());
		
		QString loClassName = ClassName.toUpper();
		
		
		if (!meIceClientServer->getFromTableDatabaseById(loClassName,ObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, int &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, int &paValue) -> " + loReturnMessage);			
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, int &paValue) -> no values to return ...");
			return false;					
		}
		
		paValue = QString(loReturnValues.at(0).c_str()).toInt();
		
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, int &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::get(QString paPropName, vector <int> &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <int> &paValue) -> Classname or id is empty ...");
			return false;
		}		
		/*****************************************
		* Getting the PROP from table
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back(paPropName.toUpper().toStdString());
		
		QString loClassName = ClassName.toUpper();
		
		
		
		if (!meIceClientServer->getFromTableDatabaseById(loClassName,ObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <int> &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <int> &paValue) -> " + loReturnMessage);			
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <int> &paValue) -> no values to return ...");
			return false;					
		}
		
		paValue.clear();
		QString loValues = QString(loReturnValues.at(0).c_str());
		loValues = loValues.replace("{","").replace("}","");
		if (loValues.compare(QString("")) != 0)
		{
			QStringList loStringList = loValues.split(",");
			for (int i = 0; i < loStringList.size(); i++)
			{
				paValue.push_back(loStringList.at(i).toInt());
			}
		}		
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <int> &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::get(QString paPropName, double &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, double &paValue) -> Classname or id is empty ...");
			return false;
		}		
		/*****************************************
		* Getting the PROP from table
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back(paPropName.toUpper().toStdString());
		
		QString loClassName = ClassName.toUpper();
		
		
		if (!meIceClientServer->getFromTableDatabaseById(loClassName,ObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, double &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, double &paValue) -> " + loReturnMessage);			
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, double &paValue) -> no values to return ...");
			return false;					
		}
		
		paValue = QString(loReturnValues.at(0).c_str()).toDouble();
		
		return true;			
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, double &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::get(QString paPropName, vector <double> &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <double> &paValue) -> Classname or id is empty ...");
			return false;
		}		
		/*****************************************
		* Getting the PROP from table
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back(paPropName.toUpper().toStdString());
		
		QString loClassName = ClassName.toUpper();
		
		
		if (!meIceClientServer->getFromTableDatabaseById(loClassName,ObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <double> &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <double> &paValue) -> " + loReturnMessage);			
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <double> &paValue) -> no values to return ...");
			return false;					
		}
		
		paValue.clear();
		QString loValues = QString(loReturnValues.at(0).c_str());
		loValues = loValues.replace("{","").replace("}","");
		if (loValues.compare(QString("")) != 0)
		{
			QStringList loStringList = loValues.split(",");
			for (int i = 0; i < loStringList.size(); i++)
			{
				paValue.push_back(loStringList.at(i).toDouble());
			}
		}		
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <double> &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::get(QString paPropName, clObject &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, clObject &paValue) -> Classname or id is empty ...");
			return false;
		}		
		/*****************************************
		* Getting the PROP from table
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back(paPropName.toUpper().toStdString());
		
		
		QString loClassName = ClassName.toUpper();
			

		
		if (!meIceClientServer->getFromTableDatabaseById(loClassName,ObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, clObject &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, clObject &paValue) -> " + loReturnMessage);			
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, clObject &paValue) -> no values to return ...");
			return false;					
		}
		
		//Check the references
		vector <QString> loReferences = getReferencesFromTableInformation(ClassName.toUpper(),paPropName.toUpper());
		
		
		/*****************************************
		* Check if object exists
		*******************************************/
		vector <std::string> loPropertiesCheck;
		vector <std::string> loReturnValuesCheck;
		QString loReturnMessageCheck;

		loProperties.push_back("NAME");
		
		for (int i = 0; i < (int) loReferences.size(); i++)
		{	
			loReturnValuesCheck.clear();
			loReturnMessageCheck = QString("");
			
			QString loReferences = loReferences.at(i).toUpper();
			QString loReturnValues = QString(loReturnValues.at(0));
			
			
			if (!meIceClientServer->getFromTableDatabaseById(loReferences,loReturnValues,loPropertiesCheck,loReturnValuesCheck,loReturnMessageCheck))
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, clObject &paValue) -> " + loReturnMessageCheck);
			}
			
			if (loReturnValuesCheck.size() < 1)
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, clObject &paValue) -> no values to return ...");		
			}
			else
			{
				//Create object for this class
				createObject(loReferences.at(i).toUpper(),QString(loReturnValues.at(0)), paValue);
				return true;			
			}
		}
		return false;			
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, clObject &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::get(QString paPropName, vector <clObject> &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, vector <clObject> &paValue) -> Classname or id is empty ...");
			return false;
		}		
		/*****************************************
		* Getting the PROP from table
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loReturnValues;
		QString loReturnMessage;

		loProperties.push_back(paPropName.toUpper().toStdString());
		
		
		QString loClassName = ClassName.toUpper();
		
		
		if (!meIceClientServer->getFromTableDatabaseById(loClassName,ObjectId,loProperties,loReturnValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, <clObject> &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, <clObject> &paValue) -> " + loReturnMessage);			
		
		if (loReturnValues.size() < 1)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, <clObject> &paValue) -> no values to return ...");
			return false;					
		}
		
		QString loReturnString = QString(loReturnValues.at(0).c_str());
		loReturnString = loReturnString.replace("{","").replace("}","");
		if (loReturnString.compare(QString("")))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, <clObject> &paValue) -> no values to return ...");
			return true;
		}
		
		QStringList loReturnStringSplit = loReturnString.split(",");
		
		//Check the references
		vector <QString> loReferences = getReferencesFromTableInformation(ClassName.toUpper(),paPropName.toUpper());
		
		/*****************************************
		* Check if object exists
		*******************************************/
		vector <std::string> loPropertiesCheck;
		vector <std::string> loReturnValuesCheck;
		QString loReturnMessageCheck;

		loProperties.push_back("NAME");
		
		for (int k = 0; k < (int) loReturnStringSplit.size(); k++)
		{
			
				for (int i = 0; i < (int) loReferences.size(); i++)
				{	
					loReturnValuesCheck.clear();
					loReturnMessageCheck = QString("");
					
					QString loRef = loReferences.at(i).toUpper();
					QString loRetString = QString(loReturnStringSplit.at(k));
					
					
					if (!meIceClientServer->getFromTableDatabaseById(loRef,loRetString,loPropertiesCheck,loReturnValuesCheck,loReturnMessageCheck))
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, <clObject> &paValue) -> " + loReturnMessageCheck);
					}
					if (loReturnValuesCheck.size() < 1)
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get(QString paPropName, <clObject> &paValue) -> no values to return ...");		
					}
					else
					{
						//Create object for this class
						clObject loObject(meIceClientServer, meIceClientLogging);
						createObject(loReferences.at(i).toUpper(),loReturnStringSplit.at(k), loObject);
						paValue.push_back(loObject);
						break;
					}
				}
		}
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::get -> " + QString(e.what()));
		return false;
    }
}
bool clObject::getParents(QString paTableName, QString paPropName, vector <clObject> &paValue)
{
	try
	{
		//TODO WVA
		
		
		
		
		/*****************************************
		* Get the table info
		*******************************************/
		 vector<std::string> loPropertyName;
		 vector<std::string> loAlias;
		 vector<std::string> loType;
		 vector<std::string> loExtra;
		 vector<std::string> loReference;
		 QString loReturnMessageObject;


		 if(!meIceClientServer->getAllPropertiesFromTable(  paTableName,
														loPropertyName,
														loAlias,
														loType,
														loExtra,
														loReference,
														loReturnMessageObject))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::getParents -> " + loReturnMessageObject);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::getParents -> " + loReturnMessageObject);
		
		
		int loKind = 0;
		for(int i = 0; i < (int) loPropertyName.size(); i++)
		{
			if(QString(loPropertyName.at(i).c_str()).toUpper().compare(paPropName.toUpper()) == 0)
			{
				if (QString(loType.at(i).c_str()).toUpper().compare(QString("UUID")) == 0)
				{
					QStringList loStringList = QString(loReference.at(i).c_str()).split("$;$");
					for (int j = 0; j < loStringList.size(); j++)
					{
						if (loStringList.at(j).toUpper().compare(ClassName.toUpper()))
						{
							loKind = 1;
							break;
						}
					}
				}
				else if (QString(loType.at(i).c_str()).toUpper().compare(QString("UUID[]")) == 0)
				{
					QStringList loStringList = QString(loReference.at(i).c_str()).split("$;$");
					for (int j = 0; j < loStringList.size(); j++)
					{
						if (loStringList.at(j).toUpper().compare(ClassName.toUpper()))
						{
							//Get the objects
							loKind = 2;
							break;
						}
					}					
				}
			}			
		}
		
		//Property not found or not for this class
		if (loKind == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::getParents -> Wrong property, no parent with this property is supporting this class ...");
			return false;
		}
		
		/*****************************************
		* Get the id's of the objects containing this id
		*******************************************/
		vector <std::string> loProperties;
		vector <std::string> loValues;
		vector <std::string> loTypeValues;
		vector <std::string> loLogExp;
		vector <std::string> loReturnIds;
		QString loReturnMessage;
		
		loProperties.push_back(paPropName.toStdString());
		loValues.push_back(ObjectId.toStdString());
		if (loKind == 2)
		{					
			loTypeValues.push_back(QString("uuid[]").toStdString());
			loLogExp.push_back(QString("= ANY").toStdString());			
		}
		if (loKind == 1)
		{			
			loTypeValues.push_back(QString("uuid").toStdString());
			loLogExp.push_back(QString("=").toStdString());
		}
		
		
		QString loStop = QString("0");
		QString loStart = QString("0");
		
		if (!meIceClientServer->getFromTableDatbaseByProperty(paTableName,loStart,loStop,loProperties,loValues,loTypeValues,loLogExp,loReturnIds,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::getParents() -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::getParents() -> " + loReturnMessage);
		
		for (int k = 0; k < (int) loReturnIds.size(); k++)
		{
			//Create object for this class
			clObject loObject(meIceClientServer, meIceClientLogging);
			createObject(paTableName.toUpper(),QString(loReturnIds.at(k).c_str()), loObject);
			paValue.push_back(loObject);
		}
		
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::getParents -> " + QString(e.what()));
		return false;
    }
}		
bool clObject::set(QString paPropName, QString paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, QString paValue) -> Classname or id is empty ...");
			return false;
		}	
		//Getting the property type
		QString loPropType;		
		if (!getTypeFromTableInformation(ClassName.toUpper(),paPropName, loPropType))
		{ 
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, QString paValue) -> Could not get type for propertie ...");
			return false;
		}
		
		
		vector<std::string> loColumns;
		vector<std::string> loValue;
		vector<std::string> loTypeValue;
		QString loReturnMessage;
		
		loColumns.push_back(paPropName.toUpper().toStdString());
		loValue.push_back(paValue.toStdString());
		loTypeValue.push_back(loPropType.toStdString());


		if (!meIceClientServer->updateIntoTableDatabase(	ClassName,
															ObjectId,
															loColumns,
															loValue,
															loTypeValue,
															loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, QString paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, QString paValue) -> " + loReturnMessage);

		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, QString paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::set(QString paPropName, vector <QString> paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <QString> paValue) -> Classname or id is empty ...");
			return false;
		}				
		//Getting the property type
		QString loPropType;		
		QString loClassName = ClassName.toUpper();
		if (!getTypeFromTableInformation(loClassName,paPropName, loPropType))
		{ 
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <QString> paValue) -> Could not get type for propertie ...");
			return false;
		}
		
		//Create array string
		QString loValue = QString("");
		for (int i = 0; i < (int) paValue.size(); i++)
		{
			if (loValue.compare(QString("")) == 0)
			{
				loValue = loValue + paValue.at(i);
			}
			else
			{
				loValue = loValue + QString("$;$") + paValue.at(i);
			}
		}
		
		vector<std::string> loColumns;
		vector<std::string> loValues;
		vector<std::string> loTypeValue;
		QString loReturnMessage;
		
		loColumns.push_back(paPropName.toUpper().toStdString());
		loValues.push_back(loValue.toStdString());
		loTypeValue.push_back(loPropType.toStdString());


		if (!meIceClientServer->updateIntoTableDatabase(	ClassName,
															ObjectId,
															loColumns,
															loValues,
															loTypeValue,
															loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <QString> paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <QString> paValue) -> " + loReturnMessage);

		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <QString> paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::set(QString paPropName, QDateTime paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, QDateTime paValue) -> Classname or id is empty ...");
			return false;
		}				
		//Getting the property type
		QString loPropType;		
		if (!getTypeFromTableInformation(ClassName.toUpper(),paPropName, loPropType))
		{ 
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, QDateTime paValue) -> Could not get type for propertie ...");
			return false;
		}
		
		
		vector<std::string> loColumns;
		vector<std::string> loValues;
		vector<std::string> loTypeValue;
		QString loReturnMessage;
		
		loColumns.push_back(paPropName.toUpper().toStdString());
		loValues.push_back(QString(paValue.toString("yyyy-MM-dd HH:mm:ss.zzz")).toStdString());
		loTypeValue.push_back(loPropType.toStdString());


		if (!meIceClientServer->updateIntoTableDatabase(	ClassName,
															ObjectId,
															loColumns,
															loValues,
															loTypeValue,
															loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, QDateTime paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, QDateTime paValue) -> " + loReturnMessage);

		return true;	
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, QDateTime paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::set(QString paPropName, vector <QDateTime> paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <QDateTime> paValue) -> Classname or id is empty ...");
			return false;
		}				
		//Getting the property type
		QString loPropType;		
		if (!getTypeFromTableInformation(ClassName.toUpper(),paPropName, loPropType))
		{ 
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <QDateTime> paValue) -> Could not get type for propertie ...");
			return false;
		}
		
		//Create array string
		QString loValue = QString("");
		for (int i = 0; i < (int) paValue.size(); i++)
		{
			if (loValue.compare(QString("")) == 0)
			{
				loValue = loValue + QString(paValue.at(i).toString("yyyy-MM-dd HH:mm:ss.zzz"));
			}
			else
			{
				loValue = loValue + QString("$;$") + QString(paValue.at(i).toString("yyyy-MM-dd HH:mm:ss.zzz"));
			}
		}
		
		vector<std::string> loColumns;
		vector<std::string> loValues;
		vector<std::string> loTypeValue;
		QString loReturnMessage;
		
		loColumns.push_back(paPropName.toUpper().toStdString());
		loValues.push_back(loValue.toStdString());
		loTypeValue.push_back(loPropType.toStdString());


		if (!meIceClientServer->updateIntoTableDatabase(	ClassName,
															ObjectId,
															loColumns,
															loValues,
															loTypeValue,
															loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <QDateTime> paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <QDateTime> paValue) -> " + loReturnMessage);

		return true;	
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <QDateTime> paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::set(QString paPropName, float paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, float paValue) -> Classname or id is empty ...");
			return false;
		}	
		//Getting the property type
		QString loPropType;		
		if (!getTypeFromTableInformation(ClassName.toUpper(),paPropName, loPropType))
		{ 
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, float paValue) -> Could not get type for propertie ...");
			return false;
		}
		
		
		vector<std::string> loColumns;
		vector<std::string> loValue;
		vector<std::string> loTypeValue;
		QString loReturnMessage;
		
		loColumns.push_back(paPropName.toUpper().toStdString());
		loValue.push_back(QString::number(paValue).toStdString());
		loTypeValue.push_back(loPropType.toStdString());


		if (!meIceClientServer->updateIntoTableDatabase(	ClassName,
															ObjectId,
															loColumns,
															loValue,
															loTypeValue,
															loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, float paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, float paValue) -> " + loReturnMessage);

		return true;				
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, float paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::set(QString paPropName, vector <float> paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <float> paValue) -> Classname or id is empty ...");
			return false;
		}		
		//Getting the property type
		QString loPropType;		
		if (!getTypeFromTableInformation(ClassName.toUpper(),paPropName, loPropType))
		{ 
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <float> paValue) -> Could not get type for propertie ...");
			return false;
		}
		//Create array string
		QString loValue = QString("");
		for (int i = 0; i < (int) paValue.size(); i++)
		{
			if (loValue.compare(QString("")) == 0)
			{
				loValue = loValue + QString::number(paValue.at(i));
			}
			else
			{
				loValue = loValue + QString("$;$") + QString::number(paValue.at(i));
			}
		}
		
		vector<std::string> loColumns;
		vector<std::string> loValues;
		vector<std::string> loTypeValue;
		QString loReturnMessage;
		
		loColumns.push_back(paPropName.toUpper().toStdString());
		loValues.push_back(loValue.toStdString());
		loTypeValue.push_back(loPropType.toStdString());


		if (!meIceClientServer->updateIntoTableDatabase(	ClassName,
															ObjectId,
															loColumns,
															loValues,
															loTypeValue,
															loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <float> paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <float> paValue) -> " + loReturnMessage);

		return true;	
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <float> paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::set(QString paPropName, int paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, int paValue) -> Classname or id is empty ...");
			return false;
		}	
		//Getting the property type
		QString loPropType;	
		QString loClassName = ClassName.toUpper();	
		if (!getTypeFromTableInformation(ClassName.toUpper(),paPropName, loPropType))
		{ 
			meIceClientLogging->insertItem("10",QHostInfo::localHostName(),"2UVServerTest.exe","clObject::set(QString paPropName, int paValue) -> Could not get type for propertie ...");
			return false;
		}
		
		
		vector<std::string> loColumns;
		vector<std::string> loValue;
		vector<std::string> loTypeValue;
		QString loReturnMessage;
		
		loColumns.push_back(paPropName.toUpper().toStdString());
		loValue.push_back(QString::number(paValue).toStdString());
		loTypeValue.push_back(loPropType.toStdString());


		if (!meIceClientServer->updateIntoTableDatabase(	ClassName,
															ObjectId,
															loColumns,
															loValue,
															loTypeValue,
															loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, int paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, int paValue) -> " + loReturnMessage);

		return true;				
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, int paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::set(QString paPropName, vector <int> paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <int> paValue) -> Classname or id is empty ...");
			return false;
		}		
		//Getting the property type
		QString loPropType;		
		if (!getTypeFromTableInformation(ClassName.toUpper(),paPropName, loPropType))
		{ 
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <int> paValue) -> Could not get type for propertie ...");
			return false;
		}
		//Create array string
		QString loValue = QString("");
		for (int i = 0; i < (int) paValue.size(); i++)
		{
			if (loValue.compare(QString("")) == 0)
			{
				loValue = loValue + QString::number(paValue.at(i));
			}
			else
			{
				loValue = loValue + QString("$;$") + QString::number(paValue.at(i));
			}
		}
		
		vector<std::string> loColumns;
		vector<std::string> loValues;
		vector<std::string> loTypeValue;
		QString loReturnMessage;
		
		loColumns.push_back(paPropName.toUpper().toStdString());
		loValues.push_back(loValue.toStdString());
		loTypeValue.push_back(loPropType.toStdString());


		if (!meIceClientServer->updateIntoTableDatabase(	ClassName,
															ObjectId,
															loColumns,
															loValues,
															loTypeValue,
															loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <int> paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <int> paValue) -> " + loReturnMessage);

		return true;	
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <int> paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::set(QString paPropName, double paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, double paValue) -> Classname or id is empty ...");
			return false;
		}				
		//Getting the property type
		QString loPropType;		
		if (!getTypeFromTableInformation(ClassName.toUpper(),paPropName, loPropType))
		{ 
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, double paValue) -> Could not get type for propertie ...");
			return false;
		}
		
		
		vector<std::string> loColumns;
		vector<std::string> loValue;
		vector<std::string> loTypeValue;
		QString loReturnMessage;
		
		loColumns.push_back(paPropName.toUpper().toStdString());
		loValue.push_back(QString::number(paValue).toStdString());
		loTypeValue.push_back(loPropType.toStdString());


		if (!meIceClientServer->updateIntoTableDatabase(	ClassName,
															ObjectId,
															loColumns,
															loValue,
															loTypeValue,
															loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, double paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, double paValue) -> " + loReturnMessage);

		return true;			
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, double paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::set(QString paPropName, vector <double> paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <double> paValue) -> Classname or id is empty ...");
			return false;
		}				
		//Getting the property type
		QString loPropType;		
		if (!getTypeFromTableInformation(ClassName.toUpper(),paPropName, loPropType))
		{ 
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <double> paValue) -> Could not get type for propertie ...");
			return false;
		}
		
		//Create array string
		QString loValue = QString("");
		for (int i = 0; i < (int) paValue.size(); i++)
		{
			if (loValue.compare(QString("")) == 0)
			{
				loValue = loValue + QString::number(paValue.at(i));
			}
			else
			{
				loValue = loValue + QString("$;$") + QString::number(paValue.at(i));
			}
		}
		
		vector<std::string> loColumns;
		vector<std::string> loValues;
		vector<std::string> loTypeValue;
		QString loReturnMessage;
		
		loColumns.push_back(paPropName.toUpper().toStdString());
		loValues.push_back(loValue.toStdString());
		loTypeValue.push_back(loPropType.toStdString());


		if (!meIceClientServer->updateIntoTableDatabase(	ClassName,
															ObjectId,
															loColumns,
															loValues,
															loTypeValue,
															loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <double> paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <double> paValue) -> " + loReturnMessage);

		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <double> paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::set(QString paPropName, clObject &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, clObject &paValue) -> Classname or id is empty ...");
			return false;
		}				
		//Getting the property type
		QString loPropType;		
		if (!getTypeFromTableInformation(ClassName.toUpper(),paPropName, loPropType))
		{ 
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, clObject &paValue) -> Could not get type for propertie ...");
			return false;
		}
		
		
		vector<std::string> loColumns;
		vector<std::string> loValue;
		vector<std::string> loTypeValue;
		QString loReturnMessage;
		
		loColumns.push_back(paPropName.toUpper().toStdString());
		loValue.push_back(paValue.ObjectId.toStdString());
		loTypeValue.push_back(loPropType.toStdString());


		if (!meIceClientServer->updateIntoTableDatabase(	ClassName,
															ObjectId,
															loColumns,
															loValue,
															loTypeValue,
															loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, clObject &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, clObject &paValue) -> " + loReturnMessage);

		return true;	
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, clObject &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::set(QString paPropName, vector <clObject> &paValue)
{
	try
	{
		if(ObjectId.compare(QString("")) == 0 || ClassName.compare(QString("")) == 0)
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <clObject> &paValue) -> Classname or id is empty ...");
			return false;
		}				
		//Getting the property type
		QString loPropType;		
		if (!getTypeFromTableInformation(ClassName.toUpper(),paPropName, loPropType))
		{ 
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <clObject> &paValue) -> Could not get type for propertie ...");
			return false;
		}
		
		//Create array string
		QString loValue = QString("");
		for (int i = 0; i < (int) paValue.size(); i++)
		{
			if (loValue.compare(QString("")) == 0)
			{
				loValue = loValue + paValue.at(i).ObjectId;
			}
			else
			{
				loValue = loValue + QString("$;$") + paValue.at(i).ObjectId;
			}
		}
		
		vector<std::string> loColumns;
		vector<std::string> loValues;
		vector<std::string> loTypeValue;
		QString loReturnMessage;
		
		loColumns.push_back(paPropName.toUpper().toStdString());
		loValues.push_back(loValue.toStdString());
		loTypeValue.push_back(loPropType.toStdString());


		if (!meIceClientServer->updateIntoTableDatabase(	ClassName,
															ObjectId,
															loColumns,
															loValues,
															loTypeValue,
															loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <clObject> &paValue) -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <clObject> &paValue) -> " + loReturnMessage);

		return true;			
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::set(QString paPropName, vector <clObject> &paValue) -> " + QString(e.what()));
		return false;
    }
}
bool clObject::deleteObject()
{
	try
	{
		//**********************
		//Delete references
		//***********************
		//Getting the tables
		vector<std::string> loTables;
		QString loMessage;

		if (!meIceClientServer->getAllTablesFromDatabase(loTables,loMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject()" + loMessage);
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject() -> getting tables failed... ");
			return false;
		}


		for(int i=0; i < loTables.size(); i++)
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
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject()-> Getting properties for table name=" + QString(loTables.at(i).c_str()));
				QString loClassNameR = QString(loTables.at(i).c_str());
				if (!meIceClientServer->getAllPropertiesFromTable(loClassNameR,loPropertyName,loAlias,loType,loExtra,loReference,loMessageProp))
				{
					meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject()-> " + loMessageProp);
					meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject() -> getting properties failed ... ");
					return false;
				}
				else
				{
					for (int j=0; j < loType.size(); j++)
					{
						if (QString(loType.at(j).c_str()).toUpper().compare(QString("uuid").toUpper()) == 0)
						{
							QStringList loRefList = QString(loReference.at(j).c_str()).split("$;$",QString::SkipEmptyParts);
							for (int k=0; k < loRefList.size(); k++)
							{
								if (loRefList.at(k).toUpper().compare(ClassName.toUpper()) == 0)
								{
									vector <std::string> loProperties;
									vector <std::string> loValues;
									vector <std::string> loTypeValue;
									vector <std::string> loLogExp;
									vector <std::string> loReturnIds;
									QString loQueryMessage;

									loProperties.push_back(loPropertyName.at(j));
									loValues.push_back(ObjectId.toUtf8().constData());
									loTypeValue.push_back("uuid");
									loLogExp.push_back("=");


									QString loClassNameE = QString(loTables.at(i).c_str());
									QString loStartE = QString("0");

									//Query and delete the entry in the database single property type
									if (!meIceClientServer->getFromTableDatbaseByProperty(	loClassNameE,
																							loStartE,
																							loStartE,
																							loProperties,
																							loValues,
																							loTypeValue,
																							loLogExp,
																							loReturnIds,
																							loQueryMessage))
									{
										meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject()-> " + loQueryMessage);
										meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject() -> getting ID's failed ... ");
										return false;
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


										QString loClassNameP = QString(loTables.at(i).c_str());
										QString loReturnIdP = QString(loReturnIds.at(l).c_str());

										if(!meIceClientServer->updateIntoTableDatabase( loClassNameP,
																						loReturnIdP,
																						loColumns,
																						loColumnValues,
																						loCulumnValuesTypes,
																						loColumnUpdateMessage))
										{
											meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject()-> " + loColumnUpdateMessage);
											meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject() -> updating ID's for with property uuid failed ... ");
											return false;
										}
									}
								}
							}
						}
						else if (QString(loType.at(j).c_str()).toUpper().compare(QString("uuid[]").toUpper()) == 0)
						{
							QStringList loRefList = QString(loReference.at(j).c_str()).split("$;$",QString::SkipEmptyParts);
							for (int k=0; k < loRefList.size(); k++)
							{

								if (loRefList.at(k).toUpper().compare(ClassName.toUpper()) == 0)
								{
									//Query and delete the entry in the database array type
									vector <std::string> loProperties;
									vector <std::string> loValues;
									vector <std::string> loTypeValue;
									vector <std::string> loLogExp;
									vector <std::string> loReturnIds;
									QString loQueryMessage;

									loProperties.push_back(loPropertyName.at(j));
									loValues.push_back(ObjectId.toUtf8().constData());
									loTypeValue.push_back("uuid");
									loLogExp.push_back("= ANY");


									QString loClassNameK = QString(loTables.at(i).c_str());
									QString loStart = QString("0");


									//Query the db and get id's from records containing the deleted object
									if (!meIceClientServer->getFromTableDatbaseByProperty(	loClassNameK,
																							loStart,
																							loStart,
																							loProperties,
																							loValues,
																							loTypeValue,
																							loLogExp,
																							loReturnIds,
																							loQueryMessage))
									{
										meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject()-> " + loQueryMessage);
										meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject() -> getting ID's with property uuid[] failed ... ");
										return false;
									}


									//Update the column for the retrieved id's
									for (int l = 0; l < loReturnIds.size(); l++)
									{
										//Get the value
										vector <std::string> loGetColumn;
										vector <std::string> loGetColumnValue;
										QString loGetColumnMessage;


										loGetColumn.push_back(loPropertyName.at(j));


										QString loClassNameY = QString(loTables.at(i).c_str());
										QString loUuidY = QString(loReturnIds.at(l).c_str());

										if (!meIceClientServer->getFromTableDatabaseById(   loClassNameY,
																							loUuidY,
																							loGetColumn,
																							loGetColumnValue,
																							loGetColumnMessage))
										{
											meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject()-> " + loGetColumnMessage);
											meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject() -> getting values for ID's with property uuid[] failed ... ");
											return false;
										}

										//Set the new value
										QString loGetColumnValueTemp = QString(loGetColumnValue.at(0).c_str());
										QStringList loElements = loGetColumnValueTemp.remove("}").remove("{").split(",", QString::SkipEmptyParts);
										QString loGetColumnValueTempNew = QString("");

										for (int u = 0; u < loElements.size(); u++)
										{
											if (loElements.at(u).compare(ObjectId) == 0)
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
										loColumnValues.push_back(loGetColumnValueTempNew.toUtf8().constData());
										loCulumnValuesTypes.push_back("uuid[]");

										QString loClassNameT = QString(loTables.at(i).c_str());
										QString loReturnIdsT = QString(loReturnIds.at(l).c_str());

										if(!meIceClientServer->updateIntoTableDatabase( loClassNameT,
																						loReturnIdsT,
																						loColumns,
																						loColumnValues,
																						loCulumnValuesTypes,
																						loColumnUpdateMessage))
										{
											meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject()-> " + loColumnUpdateMessage);
											meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject() -> updating ID's for with property uuid failed ... ");
											return false;
										}
									}
								}
							}
						}
					}
				}
			}

		}
		QString loReturnMessage;
		//Delete the table entry
		meIceClientServer->deleteIntoTableDatabase(ClassName,ObjectId,loReturnMessage);
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject() -> " + loReturnMessage);
		return true;
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::deleteObject() -> " + QString(e.what()));
		return false;
	}
}	
bool clObject::insertObject(vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValues)
{
	try
	{
		//Exceptions for insert
		QString loProdValuesForBatch;
		if (ClassName.toUpper().compare(QString("BATCH")) == 0)
		{
			//Get the product number amount and name
			int loProductCount = 0;
			QString loProductName;
			for (int i = 0; i < paColumns.size(); i++)
			{
				if (paColumns.at(i).toUpper().compare(QString("PRODUCT_NUMBER")) == 0)
					loProductCount = paValues.at(i).toInt();
				if (paColumns.at(i).toUpper().compare(QString("NAME")) == 0)
					loProductName = paValues.at(i);
			}
			//Insert the products
			vector <QString> loProdIds;
			if (loProductCount > 0)
			{
				for (int i = 0; i < loProductCount; i++)
				{
					vector <std::string> loProdColumns;
					vector <std::string> loProdValues;
					vector <std::string> loProdValueTypes;
					QString loProdReturnMessage;

					//QString loCreationDate = QString((QDateTime::currentDateTime()).toString("yyyy-MM-dd HH:mm:ss.zzz"));
					QString loCreationDate = QString((QDateTime::currentDateTime()).toString("yyyy-MM-dd HH:mm:ss.zzz"));

					loProdColumns.push_back("NAME");
					loProdValues.push_back(loProductName.toUtf8().constData());
					loProdValueTypes.push_back("text");

					loProdColumns.push_back("CREATED_ON");
					loProdValues.push_back(loCreationDate.toUtf8().constData());
					loProdValueTypes.push_back("timestamp(3)");

					loProdColumns.push_back("PRODUCT_STATE");
					loProdValues.push_back("0");
					loProdValueTypes.push_back("int");

					loProdColumns.push_back("PRODUCT_WS_STATE");
					loProdValues.push_back("0");
					loProdValueTypes.push_back("int");

					loProdColumns.push_back("PRODUCT_INDEX");
					loProdValues.push_back(std::to_string(i+1));
					loProdValueTypes.push_back("int");

					QString loClassNameK = QString("PRODUCT");
					if (!meIceClientServer->insertIntoTableDatabase(loClassNameK,loProdColumns, loProdValues, loProdValueTypes,loProdReturnMessage))
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject() -> ..." + loProdReturnMessage);
						return false;
					}
					else
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject() -> ..." + loProdReturnMessage);

					//Get the id of the product i
					//Get from the table by property


					vector <std::string> loParameters;
					vector <std::string> loParametersValue;
					vector <std::string> loParametersType;
					vector <std::string> loLogExp;
					vector <std::string> loReturnIds;
					QString loParamReturnMessage;


					loParameters.push_back("NAME");
					loParametersValue.push_back(loProductName.toUtf8().constData());
					loParametersType.push_back("text");
					loLogExp.push_back("=");

					loParameters.push_back("CREATED_ON");
					loParametersValue.push_back(loCreationDate.toUtf8().constData());
					loParametersType.push_back("timestamp(3)");
					loLogExp.push_back("=");

					loParameters.push_back("PRODUCT_STATE");
					loParametersValue.push_back("0");
					loParametersType.push_back("int");
					loLogExp.push_back("=");

					loParameters.push_back("PRODUCT_WS_STATE");
					loParametersValue.push_back("0");
					loParametersType.push_back("int");
					loLogExp.push_back("=");

					loParameters.push_back("PRODUCT_INDEX");
					loParametersValue.push_back(std::to_string(i+1));
					loParametersType.push_back("int");
					loLogExp.push_back("=");



					QString loClassNameT = QString("PRODUCT");
					QString loStartT = QString("0");
					QString loStopT = QString("0");


					if (!meIceClientServer->getFromTableDatbaseByProperty(loClassNameT,loStartT,loStopT,loParameters,loParametersValue,loParametersType,loLogExp,loReturnIds,loParamReturnMessage))
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject() -> " + loParamReturnMessage);
						return false;
					}
					else
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject() -> " + loParamReturnMessage);

						if (loReturnIds.size() > 0)
							loProdIds.push_back(loReturnIds.at(0).c_str());
						else
						{
							meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject() -> no product found to add");
							return false;
						}
					}
				}
				for(int i = 0; i < loProdIds.size();i++)
				{
					if (loProdValuesForBatch.compare("") == 0)
						loProdValuesForBatch = loProdValuesForBatch + loProdIds.at(i);
					else
						loProdValuesForBatch = loProdValuesForBatch + QString("$;$") + loProdIds.at(i);
				}

				//Check if PRODUCTS posted
				bool loProductsPosted = false;
				for (int i = 0; i < paColumns.size(); i++)
				{
					if (paColumns.at(i).toUpper().compare(QString("PRODUCTS")) == 0)
					{
						paValues.at(i) = loProdValuesForBatch;
						loProductsPosted = true;
						break;
					}

				}

				//If products not posted add it to the position
				if (!loProductsPosted)
				{
					paColumns.push_back(QString("PRODUCTS"));
					paValues.push_back(loProdValuesForBatch);
					paTypeValues.push_back(QString("uuid"));
				}
			}
		}


		//Transfer the values in right format
		vector <std::string> loColumns;
		vector <std::string> loValues;
		vector <std::string> loTypeValues;
		QString loReturnMessage;

		for (int i = 0; i < paColumns.size();i++)
		{
			loColumns.push_back(paColumns.at(i).toUtf8().constData());
			loValues.push_back(paValues.at(i).toUtf8().constData());
			loTypeValues.push_back(paTypeValues.at(i).toUtf8().constData());
		}



		QString loClassNameTemp = ClassName.toUpper();
		//Insert the values into the column
		if (!meIceClientServer->insertIntoTableDatabase(loClassNameTemp,loColumns, loValues, loTypeValues,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject() -> ..." + loReturnMessage);
			return false;
		}
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject() -> ..." + loReturnMessage);


		//Get the object ID
		vector <std::string> loReturnObjectIds;
		vector <std::string> loLogExpressions;
		QString loGetIdReturnMessage;
		for (int i = 0; i < loColumns.size();i++)
		{
			loLogExpressions.push_back("=");
		}

		QString loClassName = ClassName.toUpper();
		QString loStart = QString("0");
		QString loStop = QString("1000");

		if (!meIceClientServer->getFromTableDatbaseByProperty(loClassName, loStart, loStop,loColumns,loValues,loTypeValues,loLogExpressions,loReturnObjectIds,loGetIdReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject() -> ..." + loGetIdReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject() -> ..." + loGetIdReturnMessage);

		if (loReturnObjectIds.size() > 0)
		{
			if (loReturnObjectIds.size() > 1)
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject() -> problem more than one objects found, taking first one");
				ObjectId = QString(loReturnObjectIds.at(0).c_str());
			}
			else
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject() -> one object found");
				ObjectId = QString(loReturnObjectIds.at(0).c_str());
			}
		}
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject() -> problem no objects found");
			return false;
		}

		return true;
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::insertObject -> " + QString(e.what()));
		return false;
	}
}	
	//???//
bool clObject::queryObjects(vector <QString> paProperties, vector <QString> paValue, vector <QString> paValueType, vector <QString> paLogExp, vector <clObject> &paObjects)
{
	try
	{
		
		
		/*****************************************
		* Getting the CYCLE_ROUTINE objects for this cycle
		*******************************************/
		/*
		QString loTableName("CYCLE_ROUTINE");
		vector <std::string> loProperties;
		vector <std::string> loValues;
		vector <std::string> loTypeValues;
		vector <std::string> loLogExp;
		vector <std::string> loReturnIds;
		QString loReturnMessage;
		
		loProperties.push_back(QString("CYCLE").toStdString());
		loValues.push_back(meObjectId.toStdString());
		loTypeValues.push_back(QString("uuid").toStdString());
		loLogExp.push_back(QString("=").toStdString());
		
		if (!meIceClientServer->getFromTableDatbaseByProperty(loTableName,QString("0"),QString("0"),loProperties,loValues,loTypeValues,loLogExp,loReturnIds,loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycles::getWorkstationCycles() -> " + loReturnMessage);
			return false;
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWorkstationCycles::getWorkstationCycles() -> " + loReturnMessage);
		
		*/
		
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::queryObjects -> " + QString(e.what()));
		return false;
    }
}
bool clObject::queryAll(int paIndexFrom,int paIndexTo, vector <clObject> &paObjects)
{
	try
	{
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::queryAll -> " + QString(e.what()));
		return false;
    }
}

vector<QString> clObject::getReferencesFromTableInformation(QString paClassName,QString paPropertyName)
{
	vector<QString> loRef;
	try
	{
		for (int i = 0; i < (int) meClassNameList.size(); i++)
		{
			if (meClassNameList.at(i).toUpper().compare(paClassName.toUpper()) == 0)
			{
				vector <clDatabaseColumn *> loDatabaseColumns = meDatabaseColumnsByClassNameList.at(i);
				for (int j = 0; j < (int) loDatabaseColumns.size(); j++)
				{
					if (loDatabaseColumns.at(j)->getName().toUpper().compare(paPropertyName.toUpper()) == 0)
					{
						QString loReferences = loDatabaseColumns.at(j)->getReference();
						QStringList loStringList = loReferences.split("$;$");
						for (int k = 0; k < loStringList.size(); k++)
						{
							loRef.push_back(loStringList.at(k));
						}
						return loRef;
					}
				}
			}
		}
		return loRef;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::getReferencesFromTableInformation -> " + QString(e.what()));
		return loRef;
    }
}
bool clObject::getTypeFromTableInformation(QString paClassName,QString paPropertyName, QString &paPropertyType)
{
	vector<QString> loRef;
	try
	{
		for (int i = 0; i < (int) meClassNameList.size(); i++)
		{
			if (meClassNameList.at(i).toUpper().compare(paClassName.toUpper()) == 0)
			{
				vector <clDatabaseColumn *> loDatabaseColumns = meDatabaseColumnsByClassNameList.at(i);
				for (int j = 0; j < (int) loDatabaseColumns.size(); j++)
				{
					if (loDatabaseColumns.at(j)->getName().toUpper().compare(paPropertyName.toUpper()) == 0)
					{
						paPropertyType = loDatabaseColumns.at(j)->getType();
						return true;
					}
				}
			}
		}
		return false;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::getTypeFromTableInformation -> " + QString(e.what()));
		return false;
    }
}

bool clObject::doMethod(QString paMethodName, const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue, const vector <QString> &paLogExp)
{
	try
	{
		//TODO
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::doMethod -> " + QString(e.what()));
		return false;
	}
}

int clObject::getParameters()
{
	try
	{
		//TODO
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::getParameters() -> " + QString(e.what()));
		return -1;
	}
}

bool clObject::createObject(QString paClassName, QString paObjectId, clObject &paObject)
{
	try
	{
		paObject.ObjectId = paObjectId;
		paObject.ClassName = ClassName.toUpper();

		paObject.meObjectCalls = meObjectCalls;
		paObject.meClassNameList = meClassNameList;
		paObject.meClassMethodsList = meClassMethodsList;

		paObject.meMethodCalls = meMethodCalls;
		paObject.meMethodNamesList =  meMethodNamesList;

		paObject.meDatabaseColumnsByClassNameList = meDatabaseColumnsByClassNameList;

		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObject::createObject -> " + QString(e.what()));
		return false;
    }	
}
