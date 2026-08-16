#include "clServer_impl.h"
bool clServer_impl::Ping(const Ice::Current& paCurrent)
{
    return true;
}
void clServer_impl::Shutdown(const Ice::Current& paCurrent)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {
        paCurrent.adapter->getCommunicator()->shutdown ();
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
    }
}
bool clServer_impl::insertIntoTableDatabase(const std::string& paTableName,
                                            const UVServerAppServer::tyStringSequence& paColumns,
                                            const UVServerAppServer::tyStringSequence& paValue,
                                            const UVServerAppServer::tyStringSequence& paTypeValue,
                                            std::string& paReturnMessage,
                                            const Ice::Current& paCurrent)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {
        vector<std::string> loColumns;
        vector<std::string> loValue;
        vector<std::string> loTypeValue;

        copy(paColumns.begin(), paColumns.end(), back_inserter(loColumns));
        copy(paValue.begin(), paValue.end(), back_inserter(loValue));
        copy(paTypeValue.begin(), paTypeValue.end(), back_inserter(loTypeValue));

        vector<QString> loColumnsQString;
        vector<QString> loValueQString;
        vector<QString> loTypeValueQString;

        QString loReturnMessage;

        for (int i = 0; i < loColumns.size(); i++)
        {
            loColumnsQString.push_back(QString(loColumns[i].c_str()));
            //cout << loColumns[i] << endl;
            loValueQString.push_back(QString(loValue[i].c_str()));
            //cout << loValue[i] << endl;
            loTypeValueQString.push_back(QString(loTypeValue[i].c_str()));
            //cout << loTypeValue[i] << endl;
        }

        cout << "test" << endl;
        if(meDatabaseClass->insertIntoTableDatabase(QString(paTableName.c_str()), loColumnsQString,loValueQString,loTypeValueQString,loReturnMessage))
        {
            paReturnMessage = string(loReturnMessage.toUtf8());
            return true;
        }
        else
        {
            paReturnMessage = string(loReturnMessage.toUtf8());
            return false;
        }
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }
}

bool clServer_impl::updateIntoTableDatabase(    const std::string& paTableName,
                                                const std::string& paId,
                                                const UVServerAppServer::tyStringSequence& paColumns,
                                                const UVServerAppServer::tyStringSequence& paValue,
                                                const UVServerAppServer::tyStringSequence& paTypeValue,
                                                std::string& paReturnMessage,
                                                const Ice::Current& paCurrent)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {
        vector<std::string> loColumns;
        vector<std::string> loValue;
        vector<std::string> loTypeValue;

        copy(paColumns.begin(), paColumns.end(), back_inserter(loColumns));
        copy(paValue.begin(), paValue.end(), back_inserter(loValue));
        copy(paTypeValue.begin(), paTypeValue.end(), back_inserter(loTypeValue));

        vector<QString> loColumnsQString;
        vector<QString> loValueQString;
        vector<QString> loTypeValueQString;

        QString loReturnMessage;

        for (int i = 0; i < loColumns.size(); i++)
        {
            loColumnsQString.push_back(QString(loColumns[i].c_str()));
            loValueQString.push_back(QString(loValue[i].c_str()));
            loTypeValueQString.push_back(QString(loTypeValue[i].c_str()));
        }

        if(meDatabaseClass->updateIntoTableDatabase(QString(paTableName.c_str()),QString(paId.c_str()),loColumnsQString,loValueQString,loTypeValueQString,loReturnMessage))
        {
            paReturnMessage = string(loReturnMessage.toUtf8());
            return true;
        }
        else
        {
            paReturnMessage = string(loReturnMessage.toUtf8());
            return false;
        }
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }
}

bool clServer_impl::deleteIntoTableDatabase(    const std::string& paTableName,
                                                const std::string& paId,
                                                std::string& paReturnMessage,
                                                const Ice::Current& paCurrent)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {
        QString loReturnMessage;
        if (meDatabaseClass->deleteIntoTableDatabase(QString(paTableName.c_str()),QString(paId.c_str()), loReturnMessage))
        {
            paReturnMessage = string(loReturnMessage.toUtf8());
            return true;
        }
        else
        {
            paReturnMessage = string(loReturnMessage.toUtf8());
            return false;
        }

        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }
}

bool clServer_impl::getFromTableDatabaseGeneral(const std::string& paTableName,
                                                const std::string& paStartValue,
                                                const std::string& paMaxValue,
                                                UVServerAppServer::tyStringSequence& paReturnId,
                                                UVServerAppServer::tyStringSequence& paReturnName,												
                                                std::string& paMessage,
                                                const Ice::Current& paCurrent)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {

        QString loTableName = QString(paTableName.c_str());
        QString loStartValue = QString(paStartValue.c_str());
        QString loMaxValue = QString(paMaxValue.c_str());
        vector <QString> loReturnIds;
        vector <std::string> loReturnIdsHelp;
		vector <QString> loReturnNames;
		vector <std::string> loReturnNamesHelp;
        QString loMessage;


        if (meDatabaseClass->getFromTableDatabaseGeneral( loTableName, loStartValue, loMaxValue, loReturnIds, loReturnNames, loMessage))
        {
            for (int i=0; i<loReturnIds.size(); i++)
            {
                loReturnIdsHelp.push_back(string(loReturnIds[i].toUtf8()));
            }
            for (int i=0; i<loReturnNames.size(); i++)
            {
                loReturnNamesHelp.push_back(string(loReturnNames[i].toUtf8()));
            }
            copy(loReturnIdsHelp.begin(), loReturnIdsHelp.end(), back_inserter(paReturnId));
			copy(loReturnNamesHelp.begin(), loReturnNamesHelp.end(), back_inserter(paReturnName));
            paMessage = string(loMessage.toUtf8());
            return true;
        }
        else
        {
            paMessage = string(loMessage.toUtf8());
            return false;
        }
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }
}

bool clServer_impl::getFromTableDatabaseGeneralOrdered(const std::string& paTableName,
                                                const std::string& paStartValue,
                                                const std::string& paMaxValue,
												const std::string& paOrderBy,
                                                UVServerAppServer::tyStringSequence& paReturnId,
                                                UVServerAppServer::tyStringSequence& paReturnName,												
                                                std::string& paMessage,
                                                const Ice::Current& paCurrent)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {

        QString loTableName = QString(paTableName.c_str());
        QString loStartValue = QString(paStartValue.c_str());
        QString loMaxValue = QString(paMaxValue.c_str());
		QString loOrderBy = QString(paOrderBy.c_str());
        vector <QString> loReturnIds;
        vector <std::string> loReturnIdsHelp;
		vector <QString> loReturnNames;
		vector <std::string> loReturnNamesHelp;
        QString loMessage;


        if (meDatabaseClass->getFromTableDatabaseGeneralOrdered( loTableName, loStartValue, loMaxValue, loOrderBy, loReturnIds, loReturnNames, loMessage))
        {
            for (int i=0; i<loReturnIds.size(); i++)
            {
                loReturnIdsHelp.push_back(string(loReturnIds[i].toUtf8()));
            }
            for (int i=0; i<loReturnNames.size(); i++)
            {
                loReturnNamesHelp.push_back(string(loReturnNames[i].toUtf8()));
            }
            copy(loReturnIdsHelp.begin(), loReturnIdsHelp.end(), back_inserter(paReturnId));
			copy(loReturnNamesHelp.begin(), loReturnNamesHelp.end(), back_inserter(paReturnName));
            paMessage = string(loMessage.toUtf8());
            return true;
        }
        else
        {
            paMessage = string(loMessage.toUtf8());
            return false;
        }
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }
}

bool clServer_impl::getFromTableDatbaseByProperty(  const std::string& paTableName,
                                                    const std::string& paStartValue,
                                                    const std::string& paMaxValue,
                                                    const UVServerAppServer::tyStringSequence& paProperties,
                                                    const UVServerAppServer::tyStringSequence& paValue,
                                                    const UVServerAppServer::tyStringSequence& paTypeValue,
                                                    const UVServerAppServer::tyStringSequence& paLogExp,
                                                    UVServerAppServer::tyStringSequence& paReturnId,
                                                    std::string& paReturnMessage,
                                                    const Ice::Current& paCurrent)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {
        vector<std::string> loProperties;
        vector<QString> loPropertiesHelp;
		vector<std::string> loValues;
		vector<QString> loValuesHelp;
		vector<std::string> loTypeValue;
		vector<QString> loTypeValueHelp;
		vector<std::string> loLogExp;
		vector<QString> loLogExpHelp;
		
        vector<QString> loReturnIds;
        vector<std::string> loReturnIdsHelp;
        QString loReturnMessage;

        copy(paProperties.begin(), paProperties.end(), back_inserter(loProperties));
        for (int i = 0; i < loProperties.size(); i++)
        {
            loPropertiesHelp.push_back(QString(loProperties[i].c_str()));
        }

        copy(paValue.begin(), paValue.end(), back_inserter(loValues));
        for (int i = 0; i < loValues.size(); i++)
        {
            loValuesHelp.push_back(QString(loValues[i].c_str()));
        }
		
        copy(paTypeValue.begin(), paTypeValue.end(), back_inserter(loTypeValue));
        for (int i = 0; i < loTypeValue.size(); i++)
        {
            loTypeValueHelp.push_back(QString(loTypeValue[i].c_str()));
        }
		
		cout << "size " << paLogExp.size() << endl;
        copy(paLogExp.begin(), paLogExp.end(), back_inserter(loLogExp));
        for (int i = 0; i < loLogExp.size(); i++)
        {
            loLogExpHelp.push_back(QString(loLogExp[i].c_str()));
        }		



		//bool getFromTableDatabaseByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue,vector <QString> paTypeValue, vector <QString> paLogExp, vector <QString> &paReturnId, QString &paMessage);
        if(meDatabaseClass->getFromTableDatabaseByProperty(QString(paTableName.c_str()),QString(paStartValue.c_str()),QString(paMaxValue.c_str()),loPropertiesHelp,loValuesHelp,loTypeValueHelp,loLogExpHelp,loReturnIds,loReturnMessage))
        {

            for(int j = 0; j < loReturnIds.size(); j ++)
            {
                loReturnIdsHelp.push_back(string(loReturnIds[j].toUtf8()));
            }
            copy(loReturnIdsHelp.begin(),loReturnIdsHelp.end(),back_inserter(paReturnId));
            paReturnMessage = string(loReturnMessage.toUtf8());
            return true;
        }
        else
        {
            paReturnMessage = string(loReturnMessage.toUtf8());
            return false;
        }		
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }
}

bool clServer_impl::getFromTableDatbaseByPropertyOrdered(  const std::string& paTableName,
                                                    const std::string& paStartValue,
                                                    const std::string& paMaxValue,
                                                    const UVServerAppServer::tyStringSequence& paProperties,
                                                    const UVServerAppServer::tyStringSequence& paValue,
                                                    const UVServerAppServer::tyStringSequence& paTypeValue,
                                                    const UVServerAppServer::tyStringSequence& paLogExp,
													const std::string& paOrderBy,
                                                    UVServerAppServer::tyStringSequence& paReturnId,
                                                    std::string& paReturnMessage,
                                                    const Ice::Current& paCurrent)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {
        vector<std::string> loProperties;
        vector<QString> loPropertiesHelp;
		vector<std::string> loValues;
		vector<QString> loValuesHelp;
		vector<std::string> loTypeValue;
		vector<QString> loTypeValueHelp;
		vector<std::string> loLogExp;
		vector<QString> loLogExpHelp;
		
        vector<QString> loReturnIds;
        vector<std::string> loReturnIdsHelp;
        QString loReturnMessage;

        copy(paProperties.begin(), paProperties.end(), back_inserter(loProperties));
        for (int i = 0; i < loProperties.size(); i++)
        {
            loPropertiesHelp.push_back(QString(loProperties[i].c_str()));
        }

        copy(paValue.begin(), paValue.end(), back_inserter(loValues));
        for (int i = 0; i < loValues.size(); i++)
        {
            loValuesHelp.push_back(QString(loValues[i].c_str()));
        }
		
        copy(paTypeValue.begin(), paTypeValue.end(), back_inserter(loTypeValue));
        for (int i = 0; i < loTypeValue.size(); i++)
        {
            loTypeValueHelp.push_back(QString(loTypeValue[i].c_str()));
        }
		
		cout << "size " << paLogExp.size() << endl;
        copy(paLogExp.begin(), paLogExp.end(), back_inserter(loLogExp));
        for (int i = 0; i < loLogExp.size(); i++)
        {
            loLogExpHelp.push_back(QString(loLogExp[i].c_str()));
        }		



		//bool getFromTableDatabaseByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue,vector <QString> paTypeValue, vector <QString> paLogExp, vector <QString> &paReturnId, QString &paMessage);
        if(meDatabaseClass->getFromTableDatabaseByPropertyOrdered(QString(paTableName.c_str()),QString(paStartValue.c_str()),QString(paMaxValue.c_str()),loPropertiesHelp,loValuesHelp,loTypeValueHelp,loLogExpHelp,QString(paOrderBy.c_str()), loReturnIds,loReturnMessage))
        {

            for(int j = 0; j < loReturnIds.size(); j ++)
            {
                loReturnIdsHelp.push_back(string(loReturnIds[j].toUtf8()));
            }
            copy(loReturnIdsHelp.begin(),loReturnIdsHelp.end(),back_inserter(paReturnId));
            paReturnMessage = string(loReturnMessage.toUtf8());
            return true;
        }
        else
        {
            paReturnMessage = string(loReturnMessage.toUtf8());
            return false;
        }		
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }
}

bool clServer_impl::getFromTableDatbaseByPropertyRepresentProp(    const std::string& paTableName,
                                                            const std::string& paStartValue,
                                                            const std::string& paMaxValue,
                                                            const UVServerAppServer::tyStringSequence& paProperties,
                                                            const UVServerAppServer::tyStringSequence& paValue,
                                                            const UVServerAppServer::tyStringSequence& paTypeValue,
                                                            const UVServerAppServer::tyStringSequence& paLogExp,
                                                            UVServerAppServer::tyStringSequence& paPropertiesReturn,
                                                            UVServerAppServer::tyStringSequence& paValuesReturn,
                                                            std::string& paReturnMessage,
                                                            const Ice::Current&)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {
        vector<std::string> loProperties;
        vector<QString> loPropertiesHelp;
        vector<std::string> loValues;
        vector<QString> loValuesHelp;
        vector<std::string> loTypeValue;
        vector<QString> loTypeValueHelp;
        vector<std::string> loLogExp;
        vector<QString> loLogExpHelp;


        vector<std::string> loValuesReturn;
        vector<QString> loValuesReturnHelp;

        vector<std::string> loPropertiesReturn;
        vector<QString> loPropertiesReturnHelp;

        QString loReturnMessage;

        copy(paProperties.begin(), paProperties.end(), back_inserter(loProperties));
        for (int i = 0; i < loProperties.size(); i++)
        {
            loPropertiesHelp.push_back(QString(loProperties[i].c_str()));
        }

        copy(paValue.begin(), paValue.end(), back_inserter(loValues));
        for (int i = 0; i < loValues.size(); i++)
        {
            loValuesHelp.push_back(QString(loValues[i].c_str()));
        }

        copy(paTypeValue.begin(), paTypeValue.end(), back_inserter(loTypeValue));
        for (int i = 0; i < loTypeValue.size(); i++)
        {
            loTypeValueHelp.push_back(QString(loTypeValue[i].c_str()));
        }

        cout << "size " << paLogExp.size() << endl;
        copy(paLogExp.begin(), paLogExp.end(), back_inserter(loLogExp));
        for (int i = 0; i < loLogExp.size(); i++)
        {
            loLogExpHelp.push_back(QString(loLogExp[i].c_str()));
        }



        //bool getFromTableDatabaseByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue,vector <QString> paTypeValue, vector <QString> paLogExp, vector <QString> &paReturnId, QString &paMessage);
        if(meDatabaseClass->getFromTableDatbaseByPropertyRepresentProp(QString(paTableName.c_str()),QString(paStartValue.c_str()),QString(paMaxValue.c_str()),loPropertiesHelp,loValuesHelp,loTypeValueHelp,loLogExpHelp,loPropertiesReturnHelp,loValuesReturnHelp,loReturnMessage))
        {

            for(int j = 0; j < loPropertiesReturnHelp.size(); j ++)
            {
                loPropertiesReturn.push_back(string(loPropertiesReturnHelp[j].toUtf8()));
                loValuesReturn.push_back(string(loValuesReturnHelp[j].toUtf8()));
            }
            copy(loPropertiesReturn.begin(),loPropertiesReturn.end(),back_inserter(paPropertiesReturn));
            copy(loValuesReturn.begin(),loValuesReturn.end(),back_inserter(paValuesReturn));

            paReturnMessage = string(loReturnMessage.toUtf8());
            return true;
        }
        else
        {
            paReturnMessage = string(loReturnMessage.toUtf8());
            return false;
        }
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }
}

bool clServer_impl::getFromTableDatabaseById(   const std::string& paTableName,
                                                const std::string& paId,
                                                const UVServerAppServer::tyStringSequence& paProperties,
                                                UVServerAppServer::tyStringSequence& paReturnValue,
                                                std::string& paReturnMessage,
                                                const Ice::Current& paCurrent)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {
        vector<std::string> loProperties;
        vector<QString> loPropertiesHelp;
        vector<QString> loReturnValues;
        vector<std::string> loReturnValuesHelp;
        QString loReturnMessage;

        copy(paProperties.begin(), paProperties.end(), back_inserter(loProperties));


        for (int i = 0; i < loProperties.size(); i++)
        {
            loPropertiesHelp.push_back(QString(loProperties[i].c_str()));
        }


        if(meDatabaseClass->getFromTableDatabaseById(QString(paTableName.c_str()),QString(paId.c_str()),loPropertiesHelp,loReturnValues,loReturnMessage))
        {

            for(int j = 0; j < loReturnValues.size(); j ++)
            {
                loReturnValuesHelp.push_back(string(loReturnValues[j].toUtf8()));
            }
            copy(loReturnValuesHelp.begin(),loReturnValuesHelp.end(),back_inserter(paReturnValue));
            paReturnMessage = string(loReturnMessage.toUtf8());
            return true;
        }
        else
        {
            paReturnMessage = string(loReturnMessage.toUtf8());
            return false;
        }
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }
}

bool clServer_impl::getAllTablesFromDatabase(  UVServerAppServer::tyStringSequence& paTables,
                                            std::string& paReturnMessage,
                                            const Ice::Current& paCurrent)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {
        vector <clDatabaseTable> loDatabaseTables;
        vector <std::string> loDatabaseTableNames;
        QString loMessage;
		
        if (meDatabaseClass->getDatabaseTables(loDatabaseTables, loMessage))
        {
			//Store the tables into the global object
			meDatabaseTable = loDatabaseTables;
			//--------------------------------------------
            for (int i=0; i<loDatabaseTables.size(); i++)
            {
                QString loDatabaseTableName = ((clDatabaseTable)loDatabaseTables[i]).getTableName();
                loDatabaseTableNames.push_back(string(loDatabaseTableName.toUtf8()));
            }

            copy(loDatabaseTableNames.begin(), loDatabaseTableNames.end(), back_inserter(paTables));
            paReturnMessage = string(loMessage.toUtf8());
            return true;
        }
        else
        {
            paReturnMessage = string(loMessage.toUtf8());
            return false;
        }
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }
}

bool clServer_impl::getAllMethodsFromTable(    const std::string& paTable,
                                            UVServerAppServer::tyStringSequence& paMethodName,
                                            UVServerAppServer::tyStringSequence& paAlias,
                                            UVServerAppServer::tyStringSequence& paSource,
                                            UVServerAppServer::tyStringSequence& paSourceName,
                                            UVServerAppServer::tyStringSequence& paReturnType,
                                            std::string& paReturnMessage,
                                            const Ice::Current& paCurrent)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {
        vector<std::string> loMethodName;
        vector<std::string> loAlias;
        vector<std::string> loSource;
        vector<std::string> loSourceName;
        vector<std::string> loReturnType;

        for (int i = 0; i < meDatabaseTable.size(); i++)
        {
            if (meDatabaseTable[i].getTableName().toUpper().compare(QString(paTable.c_str()).toUpper()) == 0)
            {
                vector <clDatabaseAction> loDatabaseActions;
                loDatabaseActions = meDatabaseTable[i].getDatabaseActions();
                for(int j = 0; j < loDatabaseActions.size(); j++)
                {
                    loMethodName.push_back(string(loDatabaseActions[j].getName().toUtf8()));
                    loAlias.push_back(string(loDatabaseActions[j].getAlias().toUtf8()));
                    loSource.push_back(string(loDatabaseActions[j].getSource().toUtf8()));
                    loSourceName.push_back(string(loDatabaseActions[j].getSourceName().toUtf8()));
                    loReturnType.push_back(string(loDatabaseActions[j].getReturnType().toUtf8()));
                }
            }
        }

        copy(loMethodName.begin(), loMethodName.end(), back_inserter(paMethodName));
        copy(loAlias.begin(), loAlias.end(), back_inserter(paAlias));
        copy(loSource.begin(), loSource.end(), back_inserter(paSource));
        copy(loSourceName.begin(), loSourceName.end(), back_inserter(paSourceName));
        copy(loReturnType.begin(), loReturnType.end(), back_inserter(paReturnType));
        paReturnMessage = "clServer_impl::getAllMethodsFromTable -> succes";
        return true;
    }
    catch(exception &e)
    {
        paReturnMessage = "clServer_impl::getAllMethodsFromTable -> error";
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }
}

bool clServer_impl::getAllPropertiesFromTable( const std::string& paTable,
                                            UVServerAppServer::tyStringSequence& paPropertyName,
                                            UVServerAppServer::tyStringSequence& paAlias,
                                            UVServerAppServer::tyStringSequence& paType,
                                            UVServerAppServer::tyStringSequence& paExtra,
                                            UVServerAppServer::tyStringSequence& paReference,
                                            std::string& paReturnMessage,
                                            const Ice::Current& paCurrent)
{
    IceUtil::Mutex::Lock loLock (meGeneralMutex);
    try
    {
        vector<std::string> loPropertyName;
        vector<std::string> loAlias;
        vector<std::string> loType;
        vector<std::string> loExtra;
        vector<std::string> loReference;

        for (int i = 0; i < meDatabaseTable.size(); i++)
        {
            if (meDatabaseTable[i].getTableName().toUpper().compare(QString(paTable.c_str()).toUpper()) == 0)
            {
                vector <clDatabaseColumn> loDatabaseColumns;
                loDatabaseColumns = meDatabaseTable[i].getDatabaseColumns();
                for(int j = 0; j < loDatabaseColumns.size(); j++)
                {
                    loPropertyName.push_back(string(loDatabaseColumns[j].getName().toUtf8()));
                    loAlias.push_back(string(loDatabaseColumns[j].getAlias().toUtf8()));
                    loType.push_back(string(loDatabaseColumns[j].getType().toUtf8()));
                    loExtra.push_back(string(loDatabaseColumns[j].getExtra().toUtf8()));
                    loReference.push_back(string(loDatabaseColumns[j].getReference().toUtf8()));
                }
            }
        }
        copy(loPropertyName.begin(), loPropertyName.end(), back_inserter(paPropertyName));
        copy(loAlias.begin(), loAlias.end(), back_inserter(paAlias));
        copy(loType.begin(), loType.end(), back_inserter(paType));
        copy(loExtra.begin(), loExtra.end(), back_inserter(paExtra));
        copy(loReference.begin(), loReference.end(), back_inserter(paReference));
        paReturnMessage = "clServer_impl::getAllPropertiesFromTable -> succes";
        return true;
    }
    catch(exception &e)
    {
        paReturnMessage = "clServer_impl::getAllPropertiesFromTable -> error";
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }
}

bool clServer_impl::giveWorkstationClientInfo(	const std::string& paWorkstationName,
												const std::string& paPercentageMemUsage,
												const std::string& paPercentageDiskUsage,
												const std::string& paPercentageCpuUsage,
												std::string& paReturnMessage,
												const Ice::Current&)
{
	IceUtil::Mutex::Lock loLock (meGeneralMutex);
	try
    {
		for (int i = 0; i < meWorkstationStatus.size();i++)
		{
			if (meWorkstationStatus[i].getTimeStamp() < ((QDateTime::currentDateTime()).addSecs(-60)))
			{
					meWorkstationStatus.erase(meWorkstationStatus.begin() + i);
					i = i - 1;
			}	
		}
		meWorkstationStatus.push_back(clWorkstationStatus(QString(paWorkstationName.c_str()).toUpper(),QString(paPercentageMemUsage.c_str()).toUpper(),QString(paPercentageDiskUsage.c_str()).toUpper(),QString(paPercentageCpuUsage.c_str()).toUpper(), QDateTime::currentDateTime(),QString(paReturnMessage.c_str()).toUpper()));
		printf("test4");
        return true;
    }
    catch(exception &e)
    {
		printf("test2");
        paReturnMessage = "clServer_impl::giveWorkstationClientInfo -> error";
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }											
}

bool clServer_impl::getServerFileStructure(		std::string& paFileStructure,
												const Ice::Current&)
{
	
	try
	{
		IceUtil::Mutex::Lock loLock (meGeneralMutex);
		paFileStructure = meDatabaseClass->meFileStructure;
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }											
}

bool clServer_impl::getServerAppStructure(		std::string& paAppStructure,
												const Ice::Current&)
{
	try
	{
		IceUtil::Mutex::Lock loLock (meGeneralMutex);
		paAppStructure = meDatabaseClass->meAppStructure;
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("1","2UVLogServer.exe",e.what());
        return false;
    }											
}
