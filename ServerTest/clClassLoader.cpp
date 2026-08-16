#define INFO_BUFFER_SIZE 32767
#include "clClassLoader.h"

clClassLoader::clClassLoader(clIceClientServer * paIceClientServer, clIceClientLogging *paIceClientLogging)
{
	try
	{
		meIceClientServer = paIceClientServer;
		meIceClientLogging = paIceClientLogging;
		
		initializeLibrarys();
		readClassesFromXmlFile();
		readMethodsFromXmlFileGeneral();
		loadClasses();
		loadMethodsGeneral();

	}
	catch(...)
	{
		cout << "clWorkstationCycles::clWorkstationCycles -> failed" << endl;
	}
}
clClassLoader::~clClassLoader()
{

}
bool clClassLoader::initializeLibrarys()
{
	try
	{

		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::initializeLibrarys -> Librarys initialized");
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::initializeLibrarys -> " + QString(e.what()));
		return false;
    }	
}
bool clClassLoader::readMethodsFromXmlFileGeneral()
{
	try
	{
		QString loMapLib = QString("./CONFIG/libMapGeneral.xml");
        QFile loFile(loMapLib);
        if ( !loFile.open( QIODevice::ReadOnly ) ) 
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clClassLoader::readMethodsFromXmlFileGeneral -> Cound not open file '" + loMapLib + "' ..."));
            return false;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) 
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clClassLoader::readMethodsFromXmlFileGeneral -> Cound not parse file '" + loMapLib + "' ..."));
            loFile.close();
            return false;
        }

       QDomElement loDocElem = loDomDocument.documentElement();
	   
	   
       // Getting the library's
       QDomNode loTablesNode = loDocElem.firstChild();
       while( !loTablesNode.isNull() ) 
	   {
			if(loTablesNode.nodeName() == "lib")
			{
				QDomElement loTablesElem = loTablesNode.toElement();
				if (!loTablesElem.isNull())
				{
					
					QString loName = loTablesElem.attribute("name");
					if (loName.isEmpty() || loName.compare(QString("")) == 0)
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clClassLoader::readMethodsFromXmlFileGeneral -> mapLib tag no name for node ..."));
						return false;
					}
					QString loSource = loTablesElem.attribute("source");
					if (loSource.isEmpty() || loSource.compare(QString("")) == 0)
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clClassLoader::readMethodsFromXmlFileGeneral -> mapLib tag no source for node ..."));
						return false;
					}
					
					QString loPath = loTablesElem.attribute("path");
					if (loPath.isEmpty())
					{
						loPath = QString("");
					}					
					
					meMethodsGeneralNames.push_back(loName);
					meClassDllNamesGeneral.push_back(loSource);
					meClassPathsGeneral.push_back(loPath);
				}
			}	
			loTablesNode = loTablesNode.nextSibling();
	   }
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::readMethodsFromXmlFileGeneral -> mapping file readed");
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::readMethodsFromXmlFileGeneral -> " + QString(e.what()));
		return false;
    }	
}
bool clClassLoader::readClassesFromXmlFile()
{
	try
	{
		QString loMapLib = QString("./CONFIG/libMap.xml");
        QFile loFile(loMapLib);
        if ( !loFile.open( QIODevice::ReadOnly ) ) 
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clClassLoader::readClassesFromXmlFile -> Cound not open file '" + loMapLib + "' ..."));
            return false;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) 
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clClassLoader::readClassesFromXmlFile -> Cound not parse file '" + loMapLib + "' ..."));
            loFile.close();
            return false;
        }

       QDomElement loDocElem = loDomDocument.documentElement();
	   
	   
       // Getting the library's
       QDomNode loTablesNode = loDocElem.firstChild();
       while( !loTablesNode.isNull() ) 
	   {
			if(loTablesNode.nodeName() == "lib")
			{
				QDomElement loTablesElem = loTablesNode.toElement();
				if (!loTablesElem.isNull())
				{
					
					QString loName = loTablesElem.attribute("name");
					if (loName.isEmpty() || loName.compare(QString("")) == 0)
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clClassLoader::readClassesFromXmlFile -> mapLib tag no name for node ..."));
						return false;
					}
					QString loSource = loTablesElem.attribute("source");
					if (loSource.isEmpty() || loSource.compare(QString("")) == 0)
					{
						meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clClassLoader::readClassesFromXmlFile -> mapLib tag no source for node ..."));
						return false;
					}
					
					QString loPath = loTablesElem.attribute("path");
					if (loPath.isEmpty())
					{
						loPath = QString("");
					}					
					
					
					meClassNames.push_back(loName);
					meClassDllNames.push_back(loSource);
					meClassPaths.push_back(loPath);
					
					
					// Getting the methods
					QDomNode loTableNode = loTablesElem.firstChild();
					
					vector <QString> loMethods;
					while(!loTableNode.isNull())
					{
						QDomElement loMethodElem = loTableNode.toElement();
						if(!loMethodElem.isNull())
						{
							QString loMethodName = loMethodElem.attribute("name");
							if (loMethodName.isEmpty() || loMethodName.compare(QString("")) == 0)
							{
								meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clClassLoader::readClassesFromXmlFile -> mapLib tag no name for method ..."));
								return false;
							}
							loMethods.push_back(loMethodName);
						}
						loTableNode = loTableNode.nextSibling();	
					}
					meClassMethods.push_back(loMethods);
				}
			}
			loTablesNode = loTablesNode.nextSibling();
	   }
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::readClassesFromXmlFile -> mapping file readed");
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::readClassesFromXmlFile -> " + QString(e.what()));
		return false;
    }	
}

bool clClassLoader::loadMethodsGeneral()
{	
	try
	{
		for (int i = 0; i < meMethodsGeneralNames.size(); i++)
		{
			//Read the class dll ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			meMethodCalls.push_back(callMethodDLLGeneral(QString(meClassPathsGeneral.at(i) + meClassDllNamesGeneral.at(i)),i));
		}
		
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::loadMethodsGeneral -> " + QString(e.what()));
		return false;
    }		
}

bool clClassLoader::loadClasses()
{	
	try
	{
		for (int i = 0; i < meClassDllNames.size(); i++)
		{
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::loadClasses -> " + QString(meClassPaths.at(i) + meClassDllNames.at(i)));
			//Read the class dll ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			meObjectCalls.push_back(callObjectDLL(QString(meClassPaths.at(i) + meClassDllNames.at(i)),i));
			
			//Store the database information of the class //////////////////////////////////////////////////////////////////////////////////////
			vector<std::string> loPropertyName;
			vector<std::string> loAlias;
			vector<std::string> loType;
			vector<std::string> loExtra;
			vector<std::string> loReference;
			QString loReturnMessage;
			bool loGetColumnsCycle = false;
			
			loGetColumnsCycle = meIceClientServer->getAllPropertiesFromTable(meClassNames.at(i),loPropertyName,loAlias,loType,loExtra,loReference,loReturnMessage);
			
			if (!loGetColumnsCycle) 
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::loadClasses -> " + loReturnMessage);
				return false;
			}
			
			vector<clDatabaseColumn *> meDatabaseColumns;
			for (int j = 0; j < loPropertyName.size(); j++)
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::loadClasses() -> Propertie name [" + QString(loPropertyName[j].c_str()) + "]");
				meDatabaseColumns.push_back( new clDatabaseColumn	(	QString(loPropertyName[j].c_str()),
																		QString(loAlias[j].c_str()),
																		QString(loType[j].c_str()),
																		QString(loExtra[j].c_str()),
																		QString(loReference[j].c_str())));										
				
			}
			meDatabaseColumnsByClassName.push_back(meDatabaseColumns);
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		
		return true;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::loadClasses -> " + QString(e.what()));
		return false;
    }		
}

/*********************************************************************
* Object calls
**********************************************************************/
/*********************************************************************
* Object calls
**********************************************************************/
clMethodCall * clClassLoader::callMethodDLLGeneral(QString paCurrentMethodSourceFile, int paLibNumber)
{
	try
	{
		/*
		*   typedef bool (*createPluginClass_func_ptr)(clIceClientServer * paIceClientServer,clIceClientLogging  * paIceClientLogging);
		*   typedef bool (*doMethod_func_ptr)(QString paMethodName, const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue, const vector <QString> &paLogExp);
		*   typedef int (*GetReturnParameters_func_ptr)();
		*/
		clMethodCall * loMethodCall;
		CreateModule loCreateModule;


		////////////////////////Loading the library///////////////////////////////////////////////////////////////////
		string loLibraryName = string(paCurrentMethodSourceFile.toStdString());
		void * loLibraryLib = dlopen(loLibraryName.c_str(), RTLD_NOW);
		meLibraryLib.push_back(loLibraryLib);

		if (!loLibraryLib) {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::callObjectDLL -> Library [" + paCurrentMethodSourceFile + "] not found");
			return NULL;
		}
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::callObjectDLL -> Library [" + paCurrentMethodSourceFile + "] found");


			dlerror(); /* Clear any existing error */

			loCreateModule = (CreateModule)dlsym(loLibraryLib, "CreateModule");
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createPluginClass loaded");


			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createPluginClass after stocking");
			loMethodCall = loCreateModule();

			if (loMethodCall->createPluginClass(meIceClientServer, meIceClientLogging))
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createPluginClass called");
			}
			else
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createPluginClass not called");
			}
		}
		return loMethodCall;
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::callObjectDLL() -> " + QString(e.what()));
		return NULL;
	}
}
/*
clObjectCall clClassLoader::setClassName(dlloader::DLLoader& dlloader,QString paClassName)
{
	std::shared_ptr<clObjectCall> loObjectCall = dlloader.DLGetInstance();
	loObjectCall->ClassName = paClassName;
	return loObjectCall;
}
*/
clObjectCall * clClassLoader::callObjectDLL(QString paCurrentMethodSourceFile, int paLibNumber)
{
	try
	{
        /*
        typedef bool (*createPluginClass_func_ptr)(clIceClientServer * paIceClientServer,clIceClientLogging  * paIceClientLogging);
        typedef bool (*doMethod_func_ptr)(QString paMethodName, const vector <QString> &paParametersType, const vector <QString> &paParameters, const vector <QString> &paParametersValue, const vector <QString> &paLogExp);
        typedef int (*GetReturnParameters_func_ptr)();
        */
		clObjectCall * loObjectCall;
        CreateModuleObject loCreateModuleObject;


		////////////////////////Loading the library///////////////////////////////////////////////////////////////////
		string loLibraryName = string(paCurrentMethodSourceFile.toStdString());
        void * loLibraryLib = dlopen(loLibraryName.c_str(), RTLD_NOW);
		meLibraryLib.push_back(loLibraryLib);

		if (!loLibraryLib) {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::callObjectDLL -> Library [" + paCurrentMethodSourceFile + "] not found");
			return NULL;
		}
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::callObjectDLL -> Library [" + paCurrentMethodSourceFile + "] found");


			dlerror(); /* Clear any existing error */

            loCreateModuleObject = (CreateModuleObject)dlsym(loLibraryLib, "CreateModuleObject");
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createPluginClass loaded");

            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createPluginClass after stocking");
			loObjectCall = loCreateModuleObject();

            if (loObjectCall->createPluginClass(meIceClientServer, meIceClientLogging))
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createPluginClass called");
			}
			else
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","createPluginClass not called");
			}
		}
		return loObjectCall;
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::callObjectDLL() -> " + QString(e.what()));
		return NULL;
    }
}
bool clClassLoader::cleanObjectDLL(int paLibNumber)
{
	try
	{
		////////////////////////////////////////////////////////// Free the library handle ////////////////////////////////////////////////////////////////////////
		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::cleanObjectDLL() -> " + QString(e.what()));
		return NULL;
    }
}
bool clClassLoader::cleanObjectDLLGeneral(int paLibNumber)
{
	try
	{
		////////////////////////////////////////////////////////// Free the library handle /////////////////////////////////////////////////////////////////////////

		return true;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::meLibraryLibGeneral() -> " + QString(e.what()));
		return NULL;
    }
}
clObject clClassLoader::getObject(QString meClassName)
{
	clObject loObject(meIceClientServer, meIceClientLogging);
	try
	{
		////////////////////////////////////////////////////////// Free the library handle /////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < meClassNames.size(); i++)
		{
			if (meClassDllNames.at(i).toUpper().compare(meClassName.toUpper()) == 0)
			{

				loObject.ClassName = meClassName.toUpper();
			}
		}

        loObject.meObjectCalls = meObjectCalls;
        loObject.meClassNameList = meClassNames;
        loObject.meClassMethodsList = meClassMethods;

        loObject.meMethodCalls = meMethodCalls;
        loObject.meMethodNamesList =  meMethodsGeneralNames;

        loObject.meDatabaseColumnsByClassNameList = meDatabaseColumnsByClassName;

		return loObject;		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clClassLoader::getObject() -> " + QString(e.what()));
		return loObject;
    }	
}
