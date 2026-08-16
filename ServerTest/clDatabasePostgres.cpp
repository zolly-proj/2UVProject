#include "clDatabasePostgres.h"

clDatabasePostgres::clDatabasePostgres(QString paUserName, QString paPassword, QString paDatabaseName, QString paHostAdress, QString paPort, vector<clDatabaseTable> paXMLDatabaseTable, vector<clDatabaseRoutine> paXMLDatabaseRoutine)
{
    //Initialize the parameters
    setUserName(paUserName);
    setPassword(paPassword);
    setDatabaseName(paDatabaseName);
    setHostAdress(paHostAdress);
    setPort(paPort);
	
	meXMLDatabaseTable = paXMLDatabaseTable;
	meXMLDatabaseRoutine = paXMLDatabaseRoutine;
	
	
	
	
	
	
	
	
    meDatabaseConnection = NULL;
}

clDatabasePostgres::~clDatabasePostgres()
{

}
/******************************
* Database operations
********************************/
//Open the database
bool clDatabasePostgres::openDatabase(QString & paMessage)
{
    try
    {
        return ConnectDB(paMessage);
    }
    catch(...)
    {
        QString loErrorMessage("clDatabasePostgres::openDatabase(QString & paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
//Close the database
bool clDatabasePostgres::closeDatabase(QString & paMessage)
{
    try
    {
        return CloseConn(paMessage);
    }
    catch(...)
    {
        QString loErrorMessage("clDatabasePostgres::closeDatabase(QString & paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
//Check column exist
bool clDatabasePostgres::checkDatabaseColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExists, bool &paTypeMatch, QString &paMessage)
{
    try
    {
        return checkColumnExist(paTableName,paCollName,paType,paExtra,paExists,paTypeMatch,paMessage);
    }
    catch(...)
    {
        QString loErrorMessage("clDatabasePostgres::checkDatabaseColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExists, bool &paTypeMatch, QString &paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
//Check if the database table exists
bool clDatabasePostgres::checkDatabaseTableExist(QString paTableName, bool &paExist, QString &paMessage)
{
    try
    {
        return checkTableExist(paTableName,paExist,paMessage);
    }
    catch(...)
    {
        QString loTemp("bool clDatabaseClass::checkDatabaseTableExist(QString paTableName, bool &paExist, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}

//Create database table
bool clDatabasePostgres::createDatabaseTable(QString paTableName, QString &paMessage)
{
    try
    {
        return createTable(paTableName,paMessage);
    }
    catch(...)
    {
        QString loTemp("bool clDatabasePostgres::createDatabaseTable(QString paTableName, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//create database column
bool clDatabasePostgres::createDatabaseColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, QString paReference, QString &paMessage)
{
    try
    {
        return createColumn(paTableName,paCollName,paType,paExtra,paReference,paMessage);
    }
    catch(...)
    {
        QString loTemp("clDatabasePostgres::createDatabaseColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, bool paReference, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Update a table column
bool clDatabasePostgres::updateDatabaseColumn(QString paTableName, QString paCollName, QString paType, QString paExtra, QString paReference, QString &paMessage)
{
    try
    {
        return updateColumn( paTableName, paCollName, paType, paExtra, paReference, paMessage);
    }
    catch(...)
    {
        QString loTemp("clDatabasePostgres::updateDatbasaseColumn(QString paTableName, QString paCollName, QString paType, QSTrin paExtra, QString paReference, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Drop a table column
bool clDatabasePostgres::dropDatabaseColumn(QString paTableName, QString paCollName, QString &paMessage)
{
    try
    {
        return dropColumn( paTableName, paCollName, paMessage);
    }
    catch(...)
    {
        QString loTemp("clDatabasePostgres::dropDatabaseColumn(QString paTableName, QString paCollName, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Drop a table
bool clDatabasePostgres::dropDatabaseTable(QString paTableName, QString &paMessage)
{
    try
    {
        return dropTable(paTableName, paMessage);
    }
    catch(...)
    {
        QString loTemp("clDatabasePostgres::dropDatabaseTable(QString paTableName, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Get columns
bool clDatabasePostgres::getDatabaseColumns(QString paTableName,vector <clDatabaseColumn> &paDatabaseColumns, QString &paMessage)
{
    try
    {
        return getColumns(paTableName, paDatabaseColumns, paMessage);
    }
    catch(...)
    {
        QString loTemp("clDatabasePostgres::getDatabaseColumns(QString paTableName, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Get the tables
bool clDatabasePostgres::getDatabaseTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage)
{
    try
    {
        return getTables(paDatabaseTables,paMessage);
    }
    catch(...)
    {
        QString loTemp("clDatabasePostgres::bool getDatabaseTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Insert into tableDatabase
bool clDatabasePostgres::insertIntoTableDatabase(QString paTableName,vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paMessage)
{
    try
    {
        return insertIntoTable( paTableName, paColumns, paValues, paTypeValue, paMessage);
    }
    catch(...)
    {
        QString loTemp("bool clDatabasePostgres::insertIntoTableDatabase(QString paTableName,vector <QString> paColumns, vector <QString> paValues, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//delete from table database
bool clDatabasePostgres::deleteIntoTableDatabase(QString paTableName,QString paId, QString &paMessage)
{
    try
    {
        return deleteIntoTable( paTableName, paId,  paMessage);
    }
    catch(...)
    {
        QString loTemp("bool clDatabasePostgres::deleteIntoTableDatabase(QString paTableName,QString paId, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//update into table database
bool clDatabasePostgres::updateIntoTableDatabase(QString paTableName, QString paId, vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paMessage)
{
    try
    {
        return updateIntoTable( paTableName, paId, paColumns, paValues, paTypeValue, paMessage);
    }
    catch(...)
    {
        QString loTemp("bool clDatabasePostgres::updateIntoTableDatabase(QString paTableName, QString paId, vector <QString> paColumns, vector <QString> paValues, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}

//get into table database
bool clDatabasePostgres::getFromTableDatabaseGeneral(QString paTableName,QString paStartValue, QString paMaxValue, vector <QString> &paReturnId, vector <QString> &paReturnName, QString &paMessage)
{
    try
    {
        return getFromTableGeneral( paTableName, paStartValue, paMaxValue, paReturnId, paReturnName, paMessage);
    }
    catch(...)
    {
        QString loTemp("bool clDatabasePostgres::getFromTableDatabaseGeneral(QString paTableName,QString paStartValue, QString paMaxValue, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}


bool clDatabasePostgres::getFromTableDatabaseGeneralOrdered(QString paTableName,QString paStartValue, QString paMaxValue, QString paOrderBy, vector <QString> &paReturnId, vector <QString> &paReturnName, QString &paMessage)
{
    try
    {
        return getFromTableGeneralOrdered( paTableName, paStartValue, paMaxValue, paOrderBy, paReturnId, paReturnName, paMessage);
    }
    catch(...)
    {
        QString loTemp("bool clDatabasePostgres::getFromTableDatabaseGeneralOrdered(QString paTableName,QString paStartValue, QString paMaxValue, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//get by property into table database
bool clDatabasePostgres::getFromTableDatabaseByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue,vector <QString> paLogExp, vector <QString> &paReturnId,QString &paMessage)
{
    try
    {
        return getFromTableByProperty( paTableName, paStartValue, paMaxValue, paProperties, paValue, paTypeValue, paLogExp, paReturnId, paMessage);
    }
    catch(...)
    {
        QString loTemp("bool clDatabasePostgres::getFromTableDatabaseByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue,vector <QString> paLogExp, vector <QString> &paReturnId,QString &paMessage); -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//get by property into table database
bool clDatabasePostgres::getFromTableDatabaseByPropertyOrdered(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue,vector <QString> paLogExp, QString paOrderBy, vector <QString> &paReturnId,QString &paMessage)
{
    try
    {
        return getFromTableByPropertyOrdered( paTableName, paStartValue, paMaxValue, paProperties, paValue, paTypeValue, paLogExp, paOrderBy, paReturnId, paMessage);
    }
    catch(...)
    {
        QString loTemp("bool clDatabasePostgres::getFromTableDatabaseByPropertyOrdered(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue,vector <QString> paLogExp, QString paOrderBy vector <QString> &paReturnId,QString &paMessage); -> error ...");
        paMessage = loTemp;
        return false;
    }
}




//get by id into table database
bool clDatabasePostgres::getFromTableDatabaseById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue, QString &paMessage)
{
    try
    {
        return getFromTableById( paTableName, paId, paProperties, paReturnValue, paMessage);
    }
    catch(...)
    {
        QString loTemp("bool clDatabasePostgres::getFromTableDatabaseById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue, QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}







/******************************
* Getters and setters
*******************************/
void clDatabasePostgres::setUserName(QString paUserName){meUserName = paUserName;}
QString clDatabasePostgres::getUserName(){return meUserName;}

void clDatabasePostgres::setPassword(QString paPassword){mePassword = paPassword;}
QString clDatabasePostgres::getPassword(){return mePassword;}

void clDatabasePostgres::setDatabaseName(QString paDatabaseName){meDatabaseName = paDatabaseName;}
QString clDatabasePostgres::getDataseName(){return meDatabaseName;}

void clDatabasePostgres::setHostAdress(QString paHostAdress){meHostAdress = paHostAdress;}
QString clDatabasePostgres::getHostAdress(){return meHostAdress;}

void clDatabasePostgres::setPort(QString paPort){mePort = paPort;}
QString clDatabasePostgres::getPort(){return mePort;}

/**********************************
* Local functions
**********************************/
//Connect to the database
bool clDatabasePostgres::ConnectDB(QString &paMessage)
{
    try
    {
        //Build the connection string
        QString loConnectionString = QString("user=" + getUserName() + " password=" + getPassword() + " dbname=" + getDataseName() + " hostaddr=" + getHostAdress() + " port=" + mePort);
        QByteArray loBa = loConnectionString.toLatin1();
        char *loConnectionStringTemp = loBa.data();

        // Make a connection to the database
        meDatabaseConnection = PQconnectdb(loConnectionStringTemp);

        // Check to see that the backend connection was successfully made
        if (PQstatus(meDatabaseConnection) != CONNECTION_OK)
        {
            QString loTemp("clDatabasePostgres::ConnectDB(QString &paMessage) -> Could not connect to db with connection string '" + loConnectionString + "' ...");
            paMessage = loTemp;
            return false;
        }

        QString loOK("clDatabasePostgres::ConnectDB(QString &paMessage) -> Connection to database ok ...");
        paMessage = loOK;
        return true;
    }
    catch(...)
    {
        QString loTemp("clDatabasePostgres::ConnectDB(QString &paMessage) -> error ...");
        paMessage = loTemp;
        return false;
    }
}
//Disconnect from the database
bool clDatabasePostgres::CloseConn(QString &paMessage)
{
    try
    {
        if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
        {
            PQfinish(meDatabaseConnection);
            QString loTemp("clDatabasePostgres::CloseConn(QString &paMessage) -> Logged out from database ...");
            paMessage = loTemp;
            return true;
        }
        else
        {
            QString loTemp("clDatabasePostgres::CloseConn(QString &paMessage) -> db was not connected ...");
            paMessage = loTemp;
            return true;
        }

    }
    catch(...)
    {
        QString loErrorMessage("clDatabasePostgres::CloseConn(QString &paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }

}
//Check if table exists
bool clDatabasePostgres::checkTableExist(QString paTableName, bool &paExist, QString &paMessage)
{
    try
    {
        paExist = false;

        if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
        {
            int loFieldsCount;
            // Start a transaction block
            PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
            // Check if transaction block is ok
            if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
            {
                PQclear(loResult);
                QString loErrorMessage("clDatabasePostgres::checkTableExist(QString paTableName, bool &paExist, QString &paMessage) -> 'BEGIN' command failed ...");
                paMessage = loErrorMessage;
                return false;
            }

            // Clear result
            PQclear(loResult);

            // Fetch rows from employee table
            loResult = PQexec(meDatabaseConnection, "SELECT * FROM pg_tables WHERE schemaname='public';");
            if (PQresultStatus(loResult) != PGRES_TUPLES_OK)
            {
                PQclear(loResult);
                QString loErrorMessage("clDatabasePostgres::checkTableExist(QString paTableName, bool &paExist, QString &paMessage) -> \"SELECT * FROM pg_tables WHERE schemaname='public';\" command failed ...");
                paMessage = loErrorMessage;
                return false;
            }

            // Get the field name
            loFieldsCount = PQnfields(loResult);
            for (int i = 0; i < loFieldsCount; i++)
            {
                //Get column name
                if (QString(PQfname(loResult, i)).toUpper().compare(QString("tablename").toUpper()) == 0)
                {

                    //Get table names
                    for (int j = 0; j < PQntuples(loResult); j++)
                    {
                        if (QString(PQgetvalue(loResult, j, i)).toUpper().compare(paTableName.toUpper()) == 0)
                        {
                            paExist = true;
                            PQclear(loResult);
                            QString loTemp("clDatabasePostgres::checkTableExist -> Table '" + paTableName + "' exists ...");
                            paMessage = loTemp;
                            PQexec(meDatabaseConnection, "END;");
                            return true;
                        }
                    }
                }
            }

            //Table not found
            PQclear(loResult);

            //Error message
            QString loTemp("clDatabasePostgres::checkTableExist(QString paTableName, bool &paExist, QString &paMessage) -> Table name '" + paTableName + "' does not exist ...");
            paMessage = loTemp;
            //Ending the transaction
            PQexec(meDatabaseConnection, "END;");
            return true;
        }
        else
        {
            QString loTemp("clDatabasePostgres::checkTableExist(QString paTableName, bool &paExist, QString &paMessage) -> db was not connected ...");
            paMessage = loTemp;
            paExist = false;
            return true;
        }

    }
    catch(...)
    {
        QString loErrorMessage("clDatabasePostgres::checkTableExist(QString paTableName, bool &paExist, QString &paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
//Check if the column exists
bool clDatabasePostgres::checkColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExist, bool &paTypeMatch, QString &paMessage)
{
        try
        {
            paExist = false;
            paTypeMatch = false;

            if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {


                int loFieldsCount;
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    QString loErrorMessage("clDatabasePostgres::checkColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExist, bool &paTypeMatch, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);


                QString loQuery = QString("SELECT * FROM information_schema.columns WHERE table_schema='" + QString("public") + "' AND table_name='"  + paTableName.toLower() + "' AND column_name='" + paCollName.toLower() + "';");
                QByteArray loBa = loQuery.toLatin1();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table

                loResult = PQexec(meDatabaseConnection, loQueryToChar);
                if (PQresultStatus(loResult) != PGRES_TUPLES_OK)
                {
                    PQclear(loResult);
                    QString loErrorMessage("clDatabasePostgres::checkColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExist, bool &paTypeMatch, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }


                loFieldsCount = PQnfields(loResult);

                //For each row
                for (int i = 0; i < PQntuples(loResult); i++)
                {
                    QString loTableName;
                    QString loColumnName;
                    QString loType;
                    QString loUdt_Name;
                    QString loLength_char;

                    //For each column
                    for (int j = 0; j < loFieldsCount; j++)
                    {
                        if (QString(PQfname(loResult, j)).toLower().compare(QString("table_name")) == 0)
                        {
                            loTableName = QString(PQgetvalue(loResult, i, j)).toLower();
                        }
                        if (QString(PQfname(loResult, j)).toLower().compare(QString("column_name")) == 0)
                        {
                            loColumnName = QString(PQgetvalue(loResult, i, j)).toLower();
                        }
                        if (QString(PQfname(loResult, j)).toLower().compare(QString("data_type")) == 0)
                        {
                            loType = QString(PQgetvalue(loResult, i, j)).toLower();
                        }
                        if (QString(PQfname(loResult, j)).toLower().compare(QString("udt_name")) == 0)
                        {
                            loUdt_Name = QString(PQgetvalue(loResult, i, j)).toLower();
                        }
                        if (QString(PQfname(loResult, j)).toLower().compare(QString("character_maximum_length")) == 0)
                        {
                            loLength_char = QString(PQgetvalue(loResult, i, j)).toLower();
                        }
                    }
                    /*
                    QString loTest(loTableName + "," + loColumnName + "," + loType + "," + loUdt_Name);
                    QByteArray loBa1 = loTest.toLatin1();
                    char *loQueryToChar1 = loBa1.data();
                    printf("%s",loQueryToChar1);
                    */


                    if (    paTableName.toLower().compare(loTableName.toLower()) == 0
                            && paCollName.toLower().compare(loColumnName.toLower()) == 0)
                    {
                        paExist = true;
                        QString loTypeMessage;
                        paTypeMatch = convertTypeName(loType,loUdt_Name,loLength_char,paType,loTypeMessage);
                    }



                    //Message
                    QString loTemp("clDatabasePostgres::checkColumnExist -> Column '" + paCollName + "' in table '" + paTableName + "' exists ...");
                    paMessage = loTemp;
                    //Clear result
                    PQclear(loResult);
                    //Ending the transaction
                    PQexec(meDatabaseConnection, "END;");
                    return true;


                }

                //Table not found
                PQclear(loResult);

                //Error message
                QString loTemp("clDatabasePostgres::checkColumnExist -> Column '" + paCollName + "' in table '" + paTableName + "' does not exist ...");
                paMessage = loTemp;
                //Ending the transaction
                PQexec(meDatabaseConnection, "END;");
                return true;
            }
            else
            {
                QString loTemp("clDatabasePostgres::checkColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExist, bool &paTypeMatch, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                paExist = false;
                return true;
            }
        }
        catch(...)
        {
            QString loErrorMessage("clDatabasePostgres::checkColumnExist(QString paTableName, QString paCollName, QString paType,QString paExtra, bool &paExists, bool &paTypeMatch, QString &paMessage) -> error ...");
            paMessage = loErrorMessage;
            return false;
        }
}
//Create the table
bool clDatabasePostgres::createTable(QString paTableName, QString &paMessage)
{
    try
    {
        //DO THE SHIT
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::createTable(QString paTableName, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);


                //Creating the table with properties

                //QString loQuery = QString("CREATE TABLE " + QString(paTableName).toLower() + " (pid uuid NOT NULL DEFAULT uuid_generate_v1(), CONSTRAINT " + QString(paTableName).toLower() + "_pkey PRIMARY KEY (pid)) WITH ( OIDS=FALSE); ALTER TABLE " + QString(paTableName).toLower() + " OWNER TO postgres;");
                QString loQuery = QString("CREATE TABLE " + QString(paTableName.toLower()) + " (PKEY uuid PRIMARY KEY DEFAULT uuid_generate_v1());");
                QByteArray loBa = loQuery.toLatin1();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);

                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::createTable(QString paTableName, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }
                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Start the second query
                PQexec(meDatabaseConnection, "BEGIN;");

                //Creating the table with methods
                QString loQuery2 = QString("CREATE TABLE " + QString("a_" + paTableName.toLower()) + " (PKEY uuid PRIMARY KEY DEFAULT uuid_generate_v1(), ACTION_NAME varchar(64), ACTION_ALIAS varchar(64), ACTION_SOURCE varchar(64), ACTION_SOURCENAME varchar(64), ACTION_METHODNAME varchar(64));");
                QByteArray loBa2 = loQuery2.toLatin1();
                char *loQueryToChar2 = loBa2.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar2);

                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::createTable(QString paTableName, QString &paMessage) -> \"" + loQuery2 + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                //Clear result
                PQclear(loResult);
                //Ending the transaction
                PQexec(meDatabaseConnection, "END;");

                QString loMessage(loQuery2 + "->OK");
                return true;

            }
            else
            {
                QString loTemp("clDatabasePostgres::createTable(QString paTableName, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
    }
    catch(...)
    {
        QString loErrorMessage("clDatabasePostgres::createTable(QString paTableName, QString &paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
//ALTER TABLE distributors ADD COLUMN address varchar(30);
// For the time now we use no paExtra
// For the time paReference only using type uuid
bool clDatabasePostgres::createColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, QString paReference, QString &paMessage)
{
    try
    {
        //DO THE SHIT
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::createColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, bool paReference, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);


                //Creating the table with properties

                //QString loQuery = QString("CREATE TABLE " + QString(paTableName).toLower() + " (pid uuid NOT NULL DEFAULT uuid_generate_v1(), CONSTRAINT " + QString(paTableName).toLower() + "_pkey PRIMARY KEY (pid)) WITH ( OIDS=FALSE); ALTER TABLE " + QString(paTableName).toLower() + " OWNER TO postgres;");
                QString loQuery = QString("ALTER TABLE " + QString(paTableName) + " ADD COLUMN " + QString(paCollName) + " " + QString(paType) + ";");
                QByteArray loBa = loQuery.toLatin1();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);

                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::createColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, bool paReference, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }
                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + "->OK");
                paMessage = loMessage;
                return true;

            }
            else
            {
                QString loTemp("clDatabasePostgres::createColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, bool paReference, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
    }
    catch(...)
    {
        QString loErrorMessage("clDatabasePostgres::createColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, bool paReference, QString &paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
//Dropping a column in the database
bool clDatabasePostgres::dropColumn(QString paTableName, QString paCollName, QString &paMessage)
{
    try
    {
        //DO THE SHIT
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::dropColumn(QString paTableName, QString paCollName, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);

                //Query
                QString loQuery = QString("ALTER TABLE " + QString(paTableName) + " DROP COLUMN " + QString(paCollName) + " RESTRICT;");
                QByteArray loBa = loQuery.toLatin1();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);

                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::dropColumn(QString paTableName, QString paCollName, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }
                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + "->OK");
                paMessage = loMessage;
                return true;

            }
            else
            {
                QString loTemp("clDatabasePostgres::dropColumn(QString paTableName, QString paCollName, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
    }
    catch(...)
    {
        QString loErrorMessage("clDatabasePostgres::dropColumn(QString paTableName, QString paCollName, QString &paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
/*
//Drop columns
bool clDatabasePostgres::dropColumn(QString paTableName,QString paColumnName,QString &paMessage)
{
    try
    {
        //DO THE SHIT
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::dropColumn(paTableName,paColumnName,paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);

                //ALTER TABLE distributors
                //ALTER COLUMN address TYPE varchar(80),
                //ALTER COLUMN name TYPE varchar(100);
                //QString loQuery = QString("CREATE TABLE " + QString(paTableName).toLower() + " (pid uuid NOT NULL DEFAULT uuid_generate_v1(), CONSTRAINT " + QString(paTableName).toLower() + "_pkey PRIMARY KEY (pid)) WITH ( OIDS=FALSE); ALTER TABLE " + QString(paTableName).toLower() + " OWNER TO postgres;");
                QString loQuery = QString("ALTER TABLE " + QString(paTableName) + " DROP COLUMN " + QString(paColumnName) + ";");
                QByteArray loBa = loQuery.toLatin1();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);

                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::dropColumn(paTableName,paColumnName,paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }
                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage("Column '" + QString(paColumnName) + "' dropped ...");
                paMessage = loMessage;
                return true;

            }
            else
            {
                QString loTemp("clDatabasePostgres::dropColumn(paTableName,paColumnName,paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
        return true;
    }
    catch(...)
    {
        QString loErrorMessage("clDatabasePostgres::dropColumn(paTableName,paColumnName,paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
*/
//Update the column alter table
bool clDatabasePostgres::updateColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, QString paReference, QString &paMessage)
{
    try
    {
        //DO THE SHIT
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::updateColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, QString paReference, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);

                //ALTER TABLE distributors
                //ALTER COLUMN address TYPE varchar(80),
                //ALTER COLUMN name TYPE varchar(100);
                //QString loQuery = QString("CREATE TABLE " + QString(paTableName).toLower() + " (pid uuid NOT NULL DEFAULT uuid_generate_v1(), CONSTRAINT " + QString(paTableName).toLower() + "_pkey PRIMARY KEY (pid)) WITH ( OIDS=FALSE); ALTER TABLE " + QString(paTableName).toLower() + " OWNER TO postgres;");
                QString loQuery = QString("ALTER TABLE " + QString(paTableName) + " ALTER COLUMN " + QString(paCollName) + " TYPE " + QString(paType) + ";");
                QByteArray loBa = loQuery.toLatin1();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);

                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::updateColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, QString paReference, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }
                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + "->OK");
                paMessage = loMessage;
                return true;

            }
            else
            {
                QString loTemp("clDatabasePostgres::updateColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, QString paReference, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
    }
    catch(...)
    {
        QString loErrorMessage("clDatabasePostgres::updateColumn(QString paTableName, QString paCollName, QString paType,QString paExtra, QString paReference, QString &paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
///////////////////////////////////////////////////////
// Drop table of the database
//////////////////////////////////////////////////////
bool clDatabasePostgres::dropTable(QString paTableName, QString &paMessage)
{
    try
    {
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::dropTable(QString paTableName, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);

                //ALTER TABLE distributors
                //ALTER COLUMN address TYPE varchar(80),
                //ALTER COLUMN name TYPE varchar(100);
                //QString loQuery = QString("CREATE TABLE " + QString(paTableName).toLower() + " (pid uuid NOT NULL DEFAULT uuid_generate_v1(), CONSTRAINT " + QString(paTableName).toLower() + "_pkey PRIMARY KEY (pid)) WITH ( OIDS=FALSE); ALTER TABLE " + QString(paTableName).toLower() + " OWNER TO postgres;");
                QString loQuery = QString("DROP TABLE " + QString(paTableName) + ";");
                QByteArray loBa = loQuery.toLatin1();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);

                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::dropTable(QString paTableName, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }
                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + "-> OK");
                paMessage = loMessage;
                return true;

            }
            else
            {
                QString loTemp("clDatabasePostgres::dropTable(QString paTableName, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
    }
    catch(...)
    {
        QString loErrorMessage("clDatabasePostgres::dropTable(QString paTableName, QString &paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
//Getting the tables
bool clDatabasePostgres::getTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage)
{
    try
    {
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::getTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);

                QString loQuery = QString("SELECT * FROM pg_tables WHERE schemaname='public';");
                QByteArray loBa = loQuery.toLatin1();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);


                if (PQresultStatus(loResult) != PGRES_TUPLES_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::getTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }


                int loFieldsCount = PQnfields(loResult);

                for (int i = 0; i < PQntuples(loResult); i++)
                {
                    QString loTableName;
                    QString loColumnName;
                    QString loType;
                    QString loUdt_Name;

                    //For each column
                    for (int j = 0; j < loFieldsCount; j++)
                    {
                        if (QString(PQfname(loResult, j)).toLower().compare(QString("tablename")) == 0)
                        {
                            loTableName = QString(PQgetvalue(loResult, i, j)).toLower();

                            vector <clDatabaseColumn> loDatabaseColumns;
							vector <clDatabaseAction> loDatabaseActions;
							
							//Fill the vector with the db columns
							QString loColumnsMessage;
							getColumns(loTableName, loDatabaseColumns,loColumnsMessage);
							
							
							//Fill the vector with the actions
							
							/////////////////////////////////////
							
                            clDatabaseTable loDatabaseTable(loTableName,loDatabaseColumns,loDatabaseActions);
                            paDatabaseTables.push_back(loDatabaseTable);
                        }
                    }
                }

                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + " -> OK");
                paMessage = loMessage;
                return true;

            }
            else
            {
                QString loTemp("bool clDatabasePostgres::getTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
    }
    catch(...)
    {
        QString loErrorMessage("bool clDatabasePostgres::getTables( vector<clDatabaseTable> &paDatabaseTables, QString &paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
bool clDatabasePostgres::getColumns(QString paTableName, vector <clDatabaseColumn> &paDatabaseColumns, QString &paMessage)
{
    try
    {
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::getColumns(QString paTableName, vector <clDatabaseColumn> &paDatabaseColumns, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);

                QString loQuery = QString("SELECT attname, attnum FROM pg_attribute, pg_class WHERE attrelid = pg_class.oid AND relname = '" + paTableName + "' AND attnum > 0 ;");
                QByteArray loBa = loQuery.toLatin1();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);

                if (PQresultStatus(loResult) != PGRES_TUPLES_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("clDatabasePostgres::getColumns(QString paTableName, vector <clDatabaseColumn> &paDatabaseColumns, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }


                int loFieldsCount = PQnfields(loResult);

                for (int i = 0; i < PQntuples(loResult); i++)
                {

                    QString loColumnName;

                    //For each column
                    for (int j = 0; j < loFieldsCount; j++)
                    {
                        if (QString(PQfname(loResult, j)).toLower().compare(QString("attname")) == 0)
                        {

                            loColumnName = QString(PQgetvalue(loResult, i, j)).toLower();
							if (loColumnName.contains("pg.dropped", Qt::CaseInsensitive))
							{
								//TODO nothing cause of dropped column
							}
							else
							{
								//Set the other properties
								for (int ll = 0; ll < meXMLDatabaseTable.size(); ll++)
								{
									if (meXMLDatabaseTable[ll].getTableName().toUpper().compare(paTableName.toUpper()) == 0)
									{
										vector <clDatabaseColumn> loDatabaseColumns = meXMLDatabaseTable[ll].getDatabaseColumns();
										for (int kk = 0; kk < loDatabaseColumns.size(); kk++)
										{
											if (loDatabaseColumns[kk].getName().toUpper().compare(loColumnName.toUpper()) == 0)
											{
												
												clDatabaseColumn loDatabaseColumn(loColumnName, loDatabaseColumns[kk].getAlias(), loDatabaseColumns[kk].getType(), loDatabaseColumns[kk].getExtra(), loDatabaseColumns[kk].getReference());
												paDatabaseColumns.push_back(loDatabaseColumn);												
											}
											
										}
									}
								}
								//In case of routine
								for (int ll = 0; ll < meXMLDatabaseRoutine.size();ll++)
								{
									if (meXMLDatabaseRoutine[ll].getName().toUpper().compare(paTableName.toUpper()) == 0)
									{
									
										//Fill up the columns
										vector <clDatabaseColumn> loDatabaseColumns;
										
										
										if (loColumnName.toUpper().compare(QString("PKEY")) == 0)
										{
											clDatabaseColumn loDatabaseColumnPkey(QString("PKEY"),QString("Primary key"),QString("uuid"),QString(""), QString("no"));
											paDatabaseColumns.push_back(loDatabaseColumnPkey);
										}
										else if (loColumnName.toUpper().compare(QString("NAME")) == 0)
										{
											clDatabaseColumn loDatabaseColumnName(QString("NAME"),QString(meXMLDatabaseRoutine[ll].getName()),QString("varchar(255)"),QString(""), QString("no"));
											paDatabaseColumns.push_back(loDatabaseColumnName);
										}
										else if (loColumnName.toUpper().compare(QString("SOURCE_TYPE")) == 0)
										{
											clDatabaseColumn loDatabaseColumnSourceType(QString("SOURCE_TYPE"),QString(meXMLDatabaseRoutine[ll].getSource()),QString("varchar(255)"),QString(""), QString("no"));
											paDatabaseColumns.push_back(loDatabaseColumnSourceType);
										}
										else if (loColumnName.toUpper().compare(QString("SOURCE_FILE")) == 0)
										{
											clDatabaseColumn loDatabaseColumnSourceFile(QString("SOURCE_FILE"),QString(meXMLDatabaseRoutine[ll].getSourceName()),QString("varchar(255)"),QString(""), QString("no"));
											paDatabaseColumns.push_back(loDatabaseColumnSourceFile);
										}
										else if (loColumnName.toUpper().compare(QString("METHOD_NAME")) == 0)
										{
											clDatabaseColumn loDatabaseColumnReturnType(QString("METHOD_NAME"),QString(meXMLDatabaseRoutine[ll].getReturnType()),QString("varchar(255)"),QString(""), QString("no"));
											paDatabaseColumns.push_back(loDatabaseColumnReturnType);
										}
										
										//clDatabaseColumn loDatabaseColumnWorkstation(QString("WORKSTATION_NAME"),QString("WORKSTATION_NAME"),QString("uuid"),QString(""), QString("WORKSTATION"));
										//loDatabaseColumns.push_back(loDatabaseColumnWorkstation);
										
										//clDatabaseColumn loDatabaseColumnCycle(QString("CYCLE_NAME"),QString("CYCLE_NAME"),QString("uuid"),QString(""), QString("CYCLE"));
										//loDatabaseColumns.push_back(loDatabaseColumnCycle);
										
										vector <QString> loRoutineObjects = meXMLDatabaseRoutine[ll].getObjects();
										for (int lll = 0; lll < loRoutineObjects.size(); lll++)
										{
												if (loColumnName.toUpper().compare(QString(loRoutineObjects[lll])) == 0)
												{
													clDatabaseColumn loDatabaseColumnObject(QString(loRoutineObjects[lll]),QString(loRoutineObjects[lll]),QString("uuid"),QString(""),QString(loRoutineObjects[lll]));
													paDatabaseColumns.push_back(loDatabaseColumnObject);							
												}
										}
									}
								}
								//In case of method
								if (paTableName.indexOf("a_") == 0)
								{	
										//Fill up the columns
										vector <clDatabaseColumn> loDatabaseColumns;
										
										
										if (loColumnName.toUpper().compare(QString("PKEY")) == 0)
										{
											clDatabaseColumn loDatabaseColumnPkey(QString("PKEY"),QString("Primary key"),QString("uuid"),QString(""), QString("no"));
											paDatabaseColumns.push_back(loDatabaseColumnPkey);
										}
										else if (loColumnName.toUpper().compare(QString("ACTION_NAME")) == 0)
										{
											clDatabaseColumn loDatabaseColumnName(QString("ACTION_NAME"),QString("Action name"),QString("varchar(255)"),QString(""), QString("no"));
											paDatabaseColumns.push_back(loDatabaseColumnName);
										}
										else if (loColumnName.toUpper().compare(QString("ACTION_ALIAS")) == 0)
										{
											clDatabaseColumn loDatabaseColumnSourceType(QString("ACTION_ALIAS"),QString("Action alias"),QString("varchar(255)"),QString(""), QString("no"));
											paDatabaseColumns.push_back(loDatabaseColumnSourceType);
										}
										else if (loColumnName.toUpper().compare(QString("ACTION_SOURCE")) == 0)
										{
											clDatabaseColumn loDatabaseColumnSourceFile(QString("ACTION_SOURCE"),QString("Action source"),QString("varchar(255)"),QString(""), QString("no"));
											paDatabaseColumns.push_back(loDatabaseColumnSourceFile);
										}
										else if (loColumnName.toUpper().compare(QString("ACTION_SOURCENAME")) == 0)
										{
											clDatabaseColumn loDatabaseColumnSourceFile(QString("ACTION_SOURCENAME"),QString("Action sourcename"),QString("varchar(255)"),QString(""), QString("no"));
											paDatabaseColumns.push_back(loDatabaseColumnSourceFile);
										}										
										else if (loColumnName.toUpper().compare(QString("ACTION_METHODNAME")) == 0)
										{
											clDatabaseColumn loDatabaseColumnReturnType(QString("ACTION_METHODNAME"),QString("Method name"),QString("varchar(255)"),QString(""), QString("no"));
											paDatabaseColumns.push_back(loDatabaseColumnReturnType);
										}
								}								
								
							};
                        }
                    }
                }

                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + "-> OK");
                paMessage = loMessage;
                return true;

            }
            else
            {
                QString loTemp("clDatabasePostgres::getColumns(QString paTableName, vector <clDatabaseColumn> &paDatabaseColumns, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
    }
    catch(...)
    {
        QString loErrorMessage("clDatabasePostgres::getColumns(QString paTableName, vector <clDatabaseColumn> &paDatabaseColumns, QString &paMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
//Insert into a table data
bool clDatabasePostgres::insertIntoTable(QString paTableName,vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paMessage)
{
    try
    {

           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::insertIntoTable(QString paTableName,vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);


                //Creating the strings values
                QString loColumns;
                QString loValues;

                if(!getColumnsString(paColumns, loColumns))
                {
                    QString loErrorMessage("bool clDatabasePostgres::getColumnsString -> Error in building properties ...");
                    paMessage = loErrorMessage;
                    return false;
                }
                if(!getValuesString( paValues, paTypeValue, loValues))
                {
                    QString loErrorMessage("bool clDatabasePostgres::getValuesString -> Error in building values ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                //Query
                QString loQuery = QString("INSERT INTO " + QString(paTableName) + " (" + loColumns + ") VALUES (" + loValues + ");");
                QByteArray loBa = loQuery.toUtf8().constData();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);

                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::insertIntoTable(QString paTableName,vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }
                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + "-> OK");
                paMessage = loMessage;
                return true;

            }
            else
            {
                QString loTemp("bool clDatabasePostgres::insertIntoTable(QString paTableName,vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
        return true;
    }
    catch(...)
    {
            QString loErrorMessage("bool clDatabasePostgres::insertIntoTable(QString paTableName,vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paMessage) -> error ...");
            paMessage = loErrorMessage;
            return false;
    }
}
//Update properties in a table
bool clDatabasePostgres::updateIntoTable(QString paTableName,QString paId, vector <QString> paColumns, vector <QString> paValues,vector <QString> paTypeValue, QString &paMessage)
{
    try
    {
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::updateIntoTable(QString paTableName,QString paId, vector <QString> paColumns, vector <QString> paValues,vector <QString> paTypeValue, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);


                //Creating the strings values
                QString loValueColumnsString;

                if(!getValueColumnsString( paColumns, paValues, paTypeValue, loValueColumnsString))
                {
                    QString loErrorMessage("bool clDatabasePostgres::getValueColumnsString -> Error in building string ...");
                    paMessage = loErrorMessage;
                    return false;
                }


                //Query
                QString loQuery = QString("UPDATE " + QString(paTableName) + " SET " + loValueColumnsString + " WHERE pkey = '" + paId + "';");
                //QByteArray loBa = loQuery.toLatin1();
				QByteArray loBa = loQuery.toUtf8().constData();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);

                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::updateIntoTable(QString paTableName,QString paId, vector <QString> paColumns, vector <QString> paValues,vector <QString> paTypeValue, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }
                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + "-> OK");
                paMessage = loMessage;
                return true;

            }
            else
            {
                QString loTemp("bool clDatabasePostgres::updateIntoTable(QString paTableName,QString paId, vector <QString> paColumns, vector <QString> paValues,vector <QString> paTypeValue, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
        return true;
    }
    catch(...)
    {
            QString loErrorMessage("bool clDatabasePostgres::updateIntoTable(QString paTableName,QString paId, vector <QString> paColumns, vector <QString> paValues,vector <QString> paTypeValue, QString &paMessage) -> error ...");
            paMessage = loErrorMessage;
            return false;
    }
}
//Delete from a table
bool clDatabasePostgres::deleteIntoTable(QString paTableName,QString paId, QString &paMessage)
{
    try
    {
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::deleteIntoTable(QString paTableName,QString paId, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);


                //Query
                QString loQuery = QString("DELETE FROM " + QString(paTableName) + " WHERE pkey='" + paId + "';");
                QByteArray loBa = loQuery.toUtf8().constData();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);

                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::deleteIntoTable(QString paTableName,QString paId, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }
                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + "-> OK");
                paMessage = loMessage;
                return true;

            }
            else
            {
                QString loTemp("bool clDatabasePostgres::deleteIntoTable(QString paTableName,QString paId, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
        return true;
    }
    catch(...)
    {
            QString loErrorMessage("bool clDatabasePostgres::deleteIntoTable(QString paTableName,QString paId, QString &paMessage) -> error ...");
            paMessage = loErrorMessage;
            return false;
    }
}
//Get data from the table in general
bool clDatabasePostgres::getFromTableGeneral(QString paTableName,QString paStartValue, QString paMaxValue,vector <QString> &paReturnId,vector <QString> &paReturnName, QString &paMessage)
{
    try
    {
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::getFromTableGeneral(QString paTableName,QString paStartValue, QString paMaxValue,vector <QString> &paReturnId, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);

                /*
                SELECT select_list
                FROM table_expression
                [LIMIT { number | ALL }] [OFFSET number]
                */
				
				QString loQuery;

				if (paTableName.toUpper().indexOf("A_") == 0)
					loQuery = QString("SELECT pkey,action_name FROM " + paTableName + " LIMIT "+ ((paMaxValue.compare(QString("0")) == 0) ? QString("ALL") : QString(paMaxValue)) + " OFFSET " + QString(paStartValue) + ";");
				else
					loQuery = QString("SELECT pkey,name FROM " + paTableName + " LIMIT "+ ((paMaxValue.compare(QString("0")) == 0) ? QString("ALL") : QString(paMaxValue)) + " OFFSET " + QString(paStartValue) + ";");

				
                QByteArray loBa = loQuery.toUtf8().constData();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);


                if (PQresultStatus(loResult) != PGRES_TUPLES_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::getFromTableGeneral(QString paTableName,QString paStartValue, QString paMaxValue,vector <QString> &paReturnId, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }


                int loFieldsCount = PQnfields(loResult);

                for (int i = 0; i < PQntuples(loResult); i++)
                {
                    for (int j = 0; j < loFieldsCount; j++)
                    {
                        if (QString(PQfname(loResult, j)).toLower().compare(QString("pkey")) == 0)
                        {
                           paReturnId.push_back(QString(PQgetvalue(loResult, i, j)));
                        }
						else if (QString(PQfname(loResult, j)).toLower().compare(QString("name")) == 0)
                        {
                           paReturnName.push_back(QString(PQgetvalue(loResult, i, j)));
                        }						
						else if (QString(PQfname(loResult, j)).toLower().compare(QString("action_name")) == 0)
                        {
                           paReturnName.push_back(QString(PQgetvalue(loResult, i, j)));
                        }							
                    }
                }

                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + "->OK");
                paMessage = loMessage;
                return true;
            }
            else
            {
                QString loTemp("bool clDatabasePostgres::getFromTableGeneral(QString paTableName,QString paStartValue, QString paMaxValue,vector <QString> &paReturnId, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
        return true;
    }
    catch(...)
    {
            QString loErrorMessage("bool clDatabasePostgres::getFromTableGeneral(QString paTableName,QString paStartValue, QString paMaxValue,vector <QString> &paReturnId, QString &paMessage) -> error ...");
            paMessage = loErrorMessage;
            return false;
    }
}
//Get data from the table in general
bool clDatabasePostgres::getFromTableGeneralOrdered(QString paTableName,QString paStartValue, QString paMaxValue, QString paOrderBy, vector <QString> &paReturnId,vector <QString> &paReturnName, QString &paMessage)
{
    try
    {
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::getFromTableGeneralOrdered(QString paTableName,QString paStartValue, QString paMaxValue,vector <QString> &paReturnId, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);

                /*
                SELECT select_list
                FROM table_expression
                [LIMIT { number | ALL }] [OFFSET number]
                */
				
				QString loQuery;

				if (paTableName.toUpper().indexOf("A_") == 0)
					loQuery = QString("SELECT pkey,action_name FROM " + paTableName + " ORDER BY " + paOrderBy + " LIMIT " + ((paMaxValue.compare(QString("0")) == 0) ? QString("ALL") : QString(paMaxValue)) + " OFFSET " + QString(paStartValue) + ";");
				else
					loQuery = QString("SELECT pkey,name FROM " + paTableName + " ORDER BY " + paOrderBy + " LIMIT "+ ((paMaxValue.compare(QString("0")) == 0) ? QString("ALL") : QString(paMaxValue)) + " OFFSET " + QString(paStartValue) + ";");

				
                QByteArray loBa = loQuery.toUtf8().constData();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);


                if (PQresultStatus(loResult) != PGRES_TUPLES_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::getFromTableGeneralOrdered(QString paTableName,QString paStartValue, QString paMaxValue,vector <QString> &paReturnId, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }


                int loFieldsCount = PQnfields(loResult);

                for (int i = 0; i < PQntuples(loResult); i++)
                {
                    for (int j = 0; j < loFieldsCount; j++)
                    {
                        if (QString(PQfname(loResult, j)).toLower().compare(QString("pkey")) == 0)
                        {
                           paReturnId.push_back(QString(PQgetvalue(loResult, i, j)));
                        }
						else if (QString(PQfname(loResult, j)).toLower().compare(QString("name")) == 0)
                        {
                           paReturnName.push_back(QString(PQgetvalue(loResult, i, j)));
                        }						
						else if (QString(PQfname(loResult, j)).toLower().compare(QString("action_name")) == 0)
                        {
                           paReturnName.push_back(QString(PQgetvalue(loResult, i, j)));
                        }							
                    }
                }

                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + "->OK");
                paMessage = loMessage;
                return true;
            }
            else
            {
                QString loTemp("bool clDatabasePostgres::getFromTableGeneralOrdered(QString paTableName,QString paStartValue, QString paMaxValue,vector <QString> &paReturnId, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
        return true;
    }
    catch(...)
    {
            QString loErrorMessage("bool clDatabasePostgres::getFromTableGeneralOrdered(QString paTableName,QString paStartValue, QString paMaxValue,vector <QString> &paReturnId, QString &paMessage) -> error ...");
            paMessage = loErrorMessage;
            return false;
    }
}
//Get data from tables trough properties
bool clDatabasePostgres::getFromTableByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue, vector <QString> paLogExp, vector <QString> &paReturnId,QString &paMessage)
{
    try
    {
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
						cout << "test1" << endl;
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::getFromTableByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue, vector <QString> paLogExp, vector <QString> &paReturnId,QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);

                QString loConditions;
                if (!buildConditions(paProperties,paValue,paTypeValue,paLogExp,loConditions))
                {
							
                    QString loErrorMessage("bool clDatabasePostgres::buildConditions -> Error in building conditions ...");
                    paMessage = loErrorMessage;
                    return false;
                }

		
                /*
                SELECT select_list
                FROM table_expression
                [LIMIT { number | ALL }] [OFFSET number]
                */

                QString loQuery = QString("SELECT pkey FROM " + paTableName + loConditions + " LIMIT "+ ((paMaxValue.compare(QString("0")) == 0) ? QString("ALL") : QString(paMaxValue)) + " OFFSET " + QString(paStartValue) + ";");
                QByteArray loBa = loQuery.toUtf8().constData();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);


                if (PQresultStatus(loResult) != PGRES_TUPLES_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::getFromTableByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue, vector <QString> paLogExp, vector <QString> &paReturnId,QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }


                int loFieldsCount = PQnfields(loResult);

                for (int i = 0; i < PQntuples(loResult); i++)
                {
                    QString loId;

                    //For each column
                    for (int j = 0; j < loFieldsCount; j++)
                    {
                        if (QString(PQfname(loResult, j)).toLower().compare(QString("pkey")) == 0)
                        {
                           paReturnId.push_back(QString(PQgetvalue(loResult, i, j)));
                           break;
                        }
                    }
                }

                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + "->OK");
                paMessage = loMessage;
                return true;
            }
            else
            {
                QString loTemp("bool clDatabasePostgres::getFromTableByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue, vector <QString> paLogExp, vector <QString> &paReturnId,QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
        return true;
    }
    catch(...)
    {
            QString loErrorMessage("bool clDatabasePostgres::getFromTableByProperty(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue, vector <QString> paLogExp, vector <QString> &paReturnId, &paMessage) -> error ...");
            paMessage = loErrorMessage;
            return false;
    }
}
//Get data from tables trough properties ordered by
bool clDatabasePostgres::getFromTableByPropertyOrdered(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue, vector <QString> paLogExp, QString paOrderBy, vector <QString> &paReturnId,QString &paMessage)
{
    try
    {
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
						cout << "test1" << endl;
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::getFromTableByPropertyOrdered(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue, vector <QString> paLogExp, QString paOrderBy, vector <QString> &paReturnId,QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);

                QString loConditions;
                if (!buildConditions(paProperties,paValue,paTypeValue,paLogExp,loConditions))
                {
							
                    QString loErrorMessage("bool clDatabasePostgres::buildConditions -> Error in building conditions ...");
                    paMessage = loErrorMessage;
                    return false;
                }

		
                /*
                SELECT select_list
                FROM table_expression
                [LIMIT { number | ALL }] [OFFSET number]
                */

                QString loQuery = QString("SELECT pkey FROM " + paTableName + loConditions + " ORDER BY " + QString(paOrderBy) + " LIMIT "+ ((paMaxValue.compare(QString("0")) == 0) ? QString("ALL") : QString(paMaxValue)) + " OFFSET " + QString(paStartValue) + ";");
                QByteArray loBa = loQuery.toUtf8().constData();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);


                if (PQresultStatus(loResult) != PGRES_TUPLES_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::getFromTableByPropertyOrdered(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue, vector <QString> paLogExp, QString paOrderBy, vector <QString> &paReturnId,QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }


                int loFieldsCount = PQnfields(loResult);

                for (int i = 0; i < PQntuples(loResult); i++)
                {
                    QString loId;

                    //For each column
                    for (int j = 0; j < loFieldsCount; j++)
                    {
                        if (QString(PQfname(loResult, j)).toLower().compare(QString("pkey")) == 0)
                        {
                           paReturnId.push_back(QString(PQgetvalue(loResult, i, j)));
                           break;
                        }
                    }
                }

                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + "->OK");
                paMessage = loMessage;
                return true;
            }
            else
            {
                QString loTemp("bool clDatabasePostgres::getFromTableByPropertyOrdered(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue, vector <QString> paLogExp, QString paOrderBy, vector <QString> &paReturnId,QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
        return true;
    }
    catch(...)
    {
            QString loErrorMessage("bool clDatabasePostgres::getFromTableByPropertyOrdered(QString paTableName, QString paStartValue, QString paMaxValue,vector <QString> paProperties,vector <QString> paValue, vector <QString> paTypeValue, vector <QString> paLogExp, QString paOrderBy, vector <QString> &paReturnId,QString &paMessage) -> error ...");
            paMessage = loErrorMessage;
            return false;
    }
}
//Get data from tables trough id
bool clDatabasePostgres::getFromTableById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue, QString &paMessage)
{
    try
    {
           if (PQstatus(meDatabaseConnection) == CONNECTION_OK)
            {
                // Start a transaction block
                PGresult *loResult  = PQexec(meDatabaseConnection, "BEGIN");
                // Check if transaction block is ok
                if (PQresultStatus(loResult) != PGRES_COMMAND_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::getFromTableById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue, QString &paMessage) -> 'BEGIN' command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }

                // Clear result
                PQclear(loResult);



                QString loColumns;
                if(!getColumnsString(paTableName, paProperties, loColumns))
                {
                    QString loErrorMessage("bool clDatabasePostgres::getColumnsString -> Error in building properties ...");
                    paMessage = loErrorMessage;
                    return false;
                }


                /*
                SELECT select_list
                FROM table_expression
                [LIMIT { number | ALL }] [OFFSET number]
                */

                QString loQuery = QString("SELECT " + loColumns + " FROM " + paTableName + " WHERE pkey= '" + paId + "';");
                QByteArray loBa = loQuery.toUtf8().constData();
                char *loQueryToChar = loBa.data();
                // Fetch rows from employee table
                loResult = PQexec(meDatabaseConnection, loQueryToChar);


                if (PQresultStatus(loResult) != PGRES_TUPLES_OK)
                {
                    PQclear(loResult);
                    PQexec(meDatabaseConnection, "END;");
                    QString loErrorMessage("bool clDatabasePostgres::getFromTableById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue, QString &paMessage) -> \"" + loQuery + "\" command failed ...");
                    paMessage = loErrorMessage;
                    return false;
                }


                int loFieldsCount = PQnfields(loResult);

                for (int i = 0; i < PQntuples(loResult); i++)
                {
                    //For each column
                    for (int j = 0; j < loFieldsCount; j++)
                    {
                        paReturnValue.push_back(QString(PQgetvalue(loResult, i, j)));
                    }
                }

                //End the first query
                PQexec(meDatabaseConnection, "END;");

                //Clear result
                PQclear(loResult);
                QString loMessage(loQuery + " -> OK");
                paMessage = loMessage;
                return true;
            }
            else
            {
                QString loTemp("bool clDatabasePostgres::getFromTableById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue, QString &paMessage) -> db was not connected ...");
                paMessage = loTemp;
                return false;
            }
        return true;
    }
    catch(...)
    {
            QString loErrorMessage("bool clDatabasePostgres::getFromTableById(QString paTableName, QString paId, vector<QString> paProperties, vector<QString> &paReturnValue, QString &paMessage) -> error ...");
            paMessage = loErrorMessage;
            return false;
    }
}
/*****************************************************
* Local functions
*******************************************************/
//Convert the type name to the right type
bool clDatabasePostgres::convertTypeName( QString paType, QString paUdt_Name, QString paLength_char, QString paXMLType, QString & paMessage)
{
    try
    {


        //Special variables
        if (paType.toLower().compare(QString("array").toLower()) == 0)
        {
            QString loNewRepresent(paUdt_Name + "[]");
            loNewRepresent.replace(QString("_"),QString(""));

            if (loNewRepresent.compare(paXMLType) == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (paType.toLower().compare(QString("character varying").toLower()) == 0)
        {
            QString loNewRepresent("varchar(" + paLength_char + ")");
            if (loNewRepresent.compare(paXMLType) == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (paType.toLower().compare(QString("timestamp without time zone").toLower()) == 0)
        {
            QString loNewRepresent("timestamp");
            if (loNewRepresent.compare(paXMLType) == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        //Normal variables
        else
        {
            if (paType.compare(paXMLType) == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    catch(...)
    {
            QString loErrorMessage("clDatabasePostgres::convertTypeName(QString &paType,QString &paUdt_Name, QString & paMessage) -> error ...");
            paMessage = loErrorMessage;
            return false;
    }
}
//Building the conditions
bool clDatabasePostgres::buildConditions(vector <QString> paProperties,vector <QString> paValue,vector <QString> paTypeValue,vector <QString> paLogExp,QString &paConditions)
{
    try
    {

        if (paProperties.size() > 0)
        {
			
			
            paConditions = QString(paConditions + " WHERE");

            for (int i = 0; i < paProperties.size(); i++)
            {
                QString loLogExp(paLogExp[i]);
                QString loTypeValue(paTypeValue[i]);
                QString loProperty(paProperties[i]);
                QString loValue(paValue[i]);
                //Array split by -> $;$ -> 12.09$;$890$;$89.8
                //Double array split by -> $?$ -> 12.09$;$890$;$89.8$?$12.09$;$890$;$89.8
				
				QString loVariable;
				//Work around for searching in array's
				if (loLogExp.toUpper().compare(QString("= ANY")) == 0)
				{
					if (	loTypeValue.toLower().compare(QString("text[]")) ||
							loTypeValue.toLower().compare(QString("uuid[]")) ||
							loTypeValue.toLower().compare(QString("varchar(255)[]")) ||
							loTypeValue.toLower().compare(QString("varchar(64)[]")) ||
							loTypeValue.toLower().compare(QString("timestamp[]")) ||
							loTypeValue.toLower().compare(QString("timestamp(3)[]")))
					{
						loProperty = QString(" '" + loValue.replace("'","''") + "' ");
						loVariable = QString(loLogExp.toUpper() + "(" + paProperties[i] + ")");
					}
					else
					{
						loProperty = QString(loValue);
						loVariable = QString(loLogExp.toUpper() + "(" + paProperties[i] + ")");
					}
				}
                else if (loTypeValue.toLower().compare(QString("varchar(64)")) == 0)
                {
                    loVariable = QString(" " + loLogExp + " '" + loValue.replace("'","''") + "'");
                }				
                else if (loTypeValue.toLower().compare(QString("varchar(255)")) == 0)
                {
                    loVariable = QString(" " + loLogExp + " '" + loValue.replace("'","''") + "'");
                }					
                else if (loTypeValue.toLower().compare(QString("text")) == 0)
                {
                    loVariable = QString(" " + loLogExp + " '" + loValue.replace("'","''") + "'");
                }
                else if (loTypeValue.toLower().compare(QString("text[]")) == 0)
                {
					QStringList loArrValues = loValue.split("$;$");
					QString loArrValue = QString("[");
					if (loArrValues.size() > 0)
					{
						for (int t = 0; t < loArrValues.size(); t++)
						{
							if (loArrValue.compare(QString("[")) == 0)
								loArrValue = loArrValue + QString("'") + QString(loArrValues.at(t)).replace("'","''") + QString("'");
							else
								loArrValue = loArrValue + QString(",'") + QString(loArrValues.at(t)).replace("'","''") + QString("'");
						}
					}
					loArrValue = loArrValue + QString("]:: as TEXT[]");
					loVariable = QString(" " + loLogExp + " " + loArrValue);
                }
                else if (loTypeValue.toLower().compare(QString("text[][]")) == 0)
                {
                }
				//select * from tool where number01 = 1;				
                else if (loTypeValue.toUpper().compare(QString("INT")) == 0)
                {
                    loVariable = QString(" " + loLogExp + " " + loValue);
                }
				//select * from tool where number06 = '{1,45}'::int[];
                else if (loTypeValue.toUpper().compare(QString("INT[]")) == 0)
                {
					QStringList loArrValues = loValue.split("$;$");
					QString loArrValue = QString("'{");
					if (loArrValues.size() > 0)
					{
						for (int t = 0; t < loArrValues.size(); t++)
						{
							if (loArrValue.compare(QString("'{")) == 0)
								loArrValue = loArrValue + loArrValues.at(t);
							else
								loArrValue = loArrValue + QString(",") + loArrValues.at(t);
						}
					}
					loArrValue = loArrValue + QString("}'::int[]");
					loVariable = QString(" " + loLogExp + " " + loArrValue);					
                }
                else if (loTypeValue.toUpper().compare(QString("INT[][]")) == 0)
                {
                }
				//select * from tool where number01 = 1;				
                else if (loTypeValue.toUpper().compare(QString("INT4")) == 0)
                {
                    loVariable = QString(" " + loLogExp + " " + loValue);
                }
				//select * from tool where number06 = '{1,45}'::int[];
                else if (loTypeValue.toUpper().compare(QString("INT4[]")) == 0)
                {
					QStringList loArrValues = loValue.split("$;$");
					QString loArrValue = QString("'{");
					if (loArrValues.size() > 0)
					{
						for (int t = 0; t < loArrValues.size(); t++)
						{
							if (loArrValue.compare(QString("'{")) == 0)
								loArrValue = loArrValue + loArrValues.at(t);
							else
								loArrValue = loArrValue + QString(",") + loArrValues.at(t);
						}
					}
					loArrValue = loArrValue + QString("}'::int4[]");
					loVariable = QString(" " + loLogExp + " " + loArrValue);					
                }
                else if (loTypeValue.toUpper().compare(QString("INT4[][]")) == 0)
                {
                }
				//select * from tool where number01 = 1;				
                else if (loTypeValue.toUpper().compare(QString("INT8")) == 0)
                {
                    loVariable = QString(" " + loLogExp + " " + loValue);
                }
				//select * from tool where number06 = '{1,45}'::int[];
                else if (loTypeValue.toUpper().compare(QString("INT8[]")) == 0)
                {
					QStringList loArrValues = loValue.split("$;$");
					QString loArrValue = QString("'{");
					if (loArrValues.size() > 0)
					{
						for (int t = 0; t < loArrValues.size(); t++)
						{
							if (loArrValue.compare(QString("'{")) == 0)
								loArrValue = loArrValue + loArrValues.at(t);
							else
								loArrValue = loArrValue + QString(",") + loArrValues.at(t);
						}
					}
					loArrValue = loArrValue + QString("}'::int8[]");
					loVariable = QString(" " + loLogExp + " " + loArrValue);					
                }
                else if (loTypeValue.toUpper().compare(QString("INT8[][]")) == 0)
                {
                }
				//select * from tool where number01 = 1;				
                else if (loTypeValue.toUpper().compare(QString("FLOAT4")) == 0)
                {
                    loVariable = QString(" " + loLogExp + " " + loValue);
                }
				//select * from tool where number06 = '{1,45}'::int[];
                else if (loTypeValue.toUpper().compare(QString("FLOAT4[]")) == 0)
                {
					QStringList loArrValues = loValue.split("$;$");
					QString loArrValue = QString("'{");
					if (loArrValues.size() > 0)
					{
						for (int t = 0; t < loArrValues.size(); t++)
						{
							if (loArrValue.compare(QString("'{")) == 0)
								loArrValue = loArrValue + loArrValues.at(t);
							else
								loArrValue = loArrValue + QString(",") + loArrValues.at(t);
						}
					}
					loArrValue = loArrValue + QString("}'::float4[]");
					loVariable = QString(" " + loLogExp + " " + loArrValue);					
                }
                else if (loTypeValue.toUpper().compare(QString("FLOAT4[][]")) == 0)
                {
                }
				//select * from tool where number01 = 1;				
                else if (loTypeValue.toUpper().compare(QString("FLOAT8")) == 0)
                {
                    loVariable = QString(" " + loLogExp + " " + loValue);
                }
				//select * from tool where number06 = '{1,45}'::int[];
                else if (loTypeValue.toUpper().compare(QString("FLOAT8[]")) == 0)
                {
					QStringList loArrValues = loValue.split("$;$");
					QString loArrValue = QString("'{");
					if (loArrValues.size() > 0)
					{
						for (int t = 0; t < loArrValues.size(); t++)
						{
							if (loArrValue.compare(QString("'{")) == 0)
								loArrValue = loArrValue + loArrValues.at(t);
							else
								loArrValue = loArrValue + QString(",") + loArrValues.at(t);
						}
					}
					loArrValue = loArrValue + QString("}'::float8[]");
					loVariable = QString(" " + loLogExp + " " + loArrValue);					
                }
                else if (loTypeValue.toUpper().compare(QString("FLOAT8[][]")) == 0)
                {
                }
				//select * from opertation where objects = array['b08808c2-7c4d-11ef-a4b0-2cf05d44ba6c','bf2082b2-829a-11ef-9c0e-2cf05d44ba6c']::uuid[];
                else if (loTypeValue.toUpper().compare(QString("UUID[]")) == 0)
                {
					QStringList loArrValues = loValue.split("$;$");
					QString loArrValue = QString("array[");
					if (loArrValues.size() > 0)
					{
						for (int t = 0; t < loArrValues.size(); t++)
						{
							if (loArrValue.compare(QString("array[")) == 0)
								loArrValue = loArrValue + QString("'") + loArrValues.at(t) + QString("'");
							else
								loArrValue = loArrValue + QString(",'") + loArrValues.at(t) + QString("'");
						}
					}
					loArrValue = loArrValue + QString("]::uuid[]");
					loVariable = QString(" " + loLogExp + " " + loArrValue);
                }
				//select * from opertation where pkey = '8e5c83e2-8416-11ef-aad4-2cf05d44ba6c';
                else if (loTypeValue.toUpper().compare(QString("UUID")) == 0)
                {
                    if (loValue.toUpper().compare(QString("")) == 0)
                    {
						if (loLogExp.toUpper().compare(QString("IS NOT")) == 0)
						{
							loVariable = QString(" IS NOT NULL");
						}
						else
						{
							loVariable = QString(" is NULL");
						}
                    }
                    else
                    {
						loVariable = QString(" " + loLogExp + " '" + loValue + "'");
                    }					

                }
				//select * from opertation where datum_start = timestamp '2024-10-06 21:09:04';
                else if (loTypeValue.toUpper().compare(QString("TIMESTAMP")) == 0)
                {
                    if (loValue.toUpper().compare(QString("")) == 0)
                    {
                        loVariable = QString(" is NULL");
                    }
                    else
                    {
						loVariable = QString(" " + loLogExp + " timestamp '" + loValue + "'");
                    }						
					
                }	
                else if (loTypeValue.toUpper().compare(QString("TIMESTAMP(3)")) == 0)
                {
                    if (loValue.toUpper().compare(QString("")) == 0)
                    {
                        loVariable = QString(" is NULL");
                    }
                    else
                    {
						loVariable = QString(" " + loLogExp + " timestamp '" + loValue + "'");
                    }						
					
                }					
				//select * from opertation where datum_start = timestamp '2024-10-06 21:09:04';
                else if (loTypeValue.toUpper().compare(QString("TIMESTAMP[]")) == 0)
                {
					QStringList loArrValues = loValue.split("$;$");
					QString loArrValue = QString("array[");
					if (loArrValues.size() > 0)
					{
						for (int t = 0; t < loArrValues.size(); t++)
						{
							if (loArrValue.compare(QString("array[")) == 0)
								loArrValue = loArrValue + QString("'") + loArrValues.at(t) + QString("'");
							else
								loArrValue = loArrValue + QString(",'") + loArrValues.at(t) + QString("'");
						}
					}
					loArrValue = loArrValue + QString("]::timestamp[]");
					loVariable = QString(" " + loLogExp + " " + loArrValue);
                }					
                else if (loTypeValue.toUpper().compare(QString("TIMESTAMP(3)[]")) == 0)
                {
					QStringList loArrValues = loValue.split("$;$");
					QString loArrValue = QString("array[");
					if (loArrValues.size() > 0)
					{
						for (int t = 0; t < loArrValues.size(); t++)
						{
							if (loArrValue.compare(QString("array[")) == 0)
								loArrValue = loArrValue + QString("'") + loArrValues.at(t) + QString("'");
							else
								loArrValue = loArrValue + QString(",'") + loArrValues.at(t) + QString("'");
						}
					}
					loArrValue = loArrValue + QString("]::timestamp(3)[]");
					loVariable = QString(" " + loLogExp + " " + loArrValue);
                }					
				
                if (i == 0)
                {
                    paConditions = QString(paConditions + QString(" ") + loProperty + loVariable);
                }
                else
                {
                    paConditions = QString(paConditions + QString(" AND ") + loProperty + loVariable);
                }
            }
        }
        else
        {
            paConditions = QString("");
        }
        return true;
    }
    catch(...)
    {
            return false;
    }
}
//insert into employee (name,datum,datumarr) values ('wouter',to_timestamp('2004/11/31 14:30:12', 'YYYY/MM/DD HH24:MI:SS'),ARRAY[to_timestamp('2004/11/31 14:30:12', 'YYYY/MM/DD HH24:MI:12'),to_timestamp('2004/11/31 14:30:13', 'YYYY/MM/DD HH24:MI:13')]);
bool clDatabasePostgres::getColumnsString(QString paTableName, vector <QString> paColumns, QString &paResult)
{
    try
    {
		
		
		//to_char(created_on, 'YYYY/MM/DD HH24:MI:SS.MS') 
		
		
		
        for (int i = 0; i < paColumns.size();i++)
        {
			
			bool loIsDate = false;
			for (int j = 0; j < meXMLDatabaseTable.size(); j++)
			{
				if (meXMLDatabaseTable.at(j).getTableName().toUpper().compare(paTableName.toUpper()) == 0)
				{
					vector <clDatabaseColumn> loDatabaseColumns = meXMLDatabaseTable.at(j).getDatabaseColumns();
					for (int k = 0; k < loDatabaseColumns.size(); k++)
					{
						if (loDatabaseColumns.at(k).getName().toUpper().compare(paColumns[i].toUpper()) == 0 &&
							loDatabaseColumns.at(k).getType().toUpper().compare(QString("TIMESTAMP(3)")) == 0)
						{
							loIsDate = true;
							break;
						}
					}
					if(loIsDate) break;
				}
			}
			
			
			
			
			
			
			
			
			
			
            if (i == 0)
            {
				if (loIsDate)
					paResult = paResult + QString("to_char(") + QString(paColumns[i]) + QString(",'YYYY-MM-DD HH24:MI:SS.MS')");
				else
					paResult = paResult + QString(paColumns[i]);
            }
            else
            {
				if (loIsDate)
					paResult = paResult + QString(",") + QString("to_char(") + QString(paColumns[i]) + QString(",'YYYY-MM-DD HH24:MI:SS.MS')");
				else
					paResult = paResult + QString(",") + QString(paColumns[i]);
            }
        }
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool clDatabasePostgres::getColumnsString(vector <QString> paColumns, QString &paResult)
{
    try
    {
		
		
		//to_char(created_on, 'YYYY/MM/DD HH24:MI:SS.MS') 
		
		
		
        for (int i = 0; i < paColumns.size();i++)
        {			
            if (i == 0)
				paResult = paResult + QString(paColumns[i]);
            else
				paResult = paResult + QString(",") + QString(paColumns[i]);
            
        }
        return true;
    }
    catch(...)
    {
        return false;
    }
}


bool clDatabasePostgres::getValueColumnsString(vector <QString> paColumns, vector <QString> paValues, vector <QString> paTypeValue, QString &paResult)
{
    try
    {
        for (int i = 0 ; i < paColumns.size(); i ++)
        {

            QString loTypeValue = paTypeValue[i];
            QString loValue = paValues[i];
            QString loPropName = paColumns[i];
			//cout << "[" << loPropName.toStdString() << "][" << loValue.toStdString() << "][" << loTypeValue.toStdString() << "]" << endl;
            /*
            cout << "---------" << endl;
            cout << string(loTypeValue.toUtf8()) << endl;
            cout << string(loValue.toUtf8()) << endl;
            cout << string(loPropName.toUtf8()) << endl;
            */
            //INSERT INTO employee (NAME,CUSTOMERS,CHEF,DATUM,DATUMARR) VALUES ('wouter',ARRAY[]::uuid[],'', to_timestamp('', 'YYYY/MM/DD HH24:MI:SS'),ARRAY[]::timestamp[]);

                QString loVariable;

				if (loTypeValue.toUpper().compare(QString("uuid").toUpper()) == 0)
                {
                    if (loValue.toUpper().compare(QString("")) == 0)
                    {
                        loVariable = QString("NULL");
                    }
                    else
                    {
                        loVariable = QString("CAST ('" + loValue + "' AS uuid)");
                    }

                }
                else if (loTypeValue.toUpper().compare(QString("uuid[]").toUpper()) == 0)
                {

                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);
                            cout << loElements.count() << endl;
                            loVariable = QString(loVariable + "ARRAY[");

                                for (int j = 0; j < loElements.size(); j++)
                                {
                                    if (j == 0)
                                    {
                                        loVariable = QString(  loVariable + "CAST('" + loElements.at(j) + "' AS uuid)");
                                    }
                                    else
                                    {
                                        loVariable = QString( loVariable + ", CAST('" + loElements.at(j) + "' AS uuid)");
                                    }

                                }

                            //End array
                            loVariable = QString(loVariable + "]::uuid[]");

                }
                else if (loTypeValue.toUpper().compare(QString("timestamp").toUpper()) == 0)
                {
                    if (loValue.toUpper().compare(QString("")) == 0)
                    {
                        loVariable = QString("NULL");
                    }
                    else
                    {
                        loVariable = QString(" to_timestamp('" + loValue + "', 'YYYY-MM-DD HH24:MI:SS')");
                    }
                    //to_timestamp('2004/11/31 14:30:12', 'YYYY/MM/DD HH24:MI:SS')
                    
                }
                else if (loTypeValue.toUpper().compare(QString("timestamp(3)").toUpper()) == 0)
                {
                    if (loValue.toUpper().compare(QString("")) == 0)
                    {
                        loVariable = QString("NULL");
                    }
                    else
                    {
                        loVariable = QString(" to_timestamp('" + loValue + "', 'YYYY-MM-DD HH24:MI:SS.MS')");
                    }
                    //to_timestamp('2004/11/31 14:30:12', 'YYYY/MM/DD HH24:MI:SS.MS')
                    
                }				
                else if (loTypeValue.toUpper().compare(QString("timestamp[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + " to_timestamp('" + loElements.at(j) + "', 'YYYY-MM-DD HH24:MI:SS')");
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + " to_timestamp('" + loElements.at(j) + "', 'YYYY-MM-DD HH24:MI:SS')");
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::timestamp[]");

                }
                else if (loTypeValue.toUpper().compare(QString("timestamp(3)[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + " to_timestamp('" + loElements.at(j) + "', 'YYYY-MM-DD HH24:MI:SS.MS')");
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + " to_timestamp('" + loElements.at(j) + "', 'YYYY-MM-DD HH24:MI:SS.MS')");
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::timestamp(3)[]");

                }				
                else if (loTypeValue.toUpper().compare(QString("varchar(255)").toUpper()) == 0)
                {
                    loVariable = QString("'" + loValue.replace("'","''") + "'");
                }
                else if (loTypeValue.toUpper().compare(QString("text").toUpper()) == 0)
                {
                    loVariable = QString("'" + loValue.replace("'","''") + "'");
                }				
                else if (loTypeValue.toUpper().compare(QString("varchar(255)[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + "'" + QString(loElements.at(j)).replace("'","''") + "'");
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + "'" + QString(loElements.at(j)).replace("'","''") + "'");
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::varchar(255)[]");
                }
                else if (loTypeValue.toUpper().compare(QString("text[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + "'" + QString(loElements.at(j)).replace("'","''") + "'");
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + "'" + QString(loElements.at(j)).replace("'","''") + "'");
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::text[]");
                }				
                else if (loTypeValue.toUpper().compare(QString("varchar(64)").toUpper()) == 0)
                {
                    loVariable = QString("'" + loValue.replace("'","''") + "'");
                }
                else if (loTypeValue.toUpper().compare(QString("varchar(64)[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + "'" + QString(loElements.at(j)).replace("'","''") + "'");
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + "'" + QString(loElements.at(j)).replace("'","''") + "'");
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::varchar(64)[]");
                }				
                else if (loTypeValue.toUpper().compare(QString("int").toUpper()) == 0)
                {
                    loVariable = QString(loValue);
                }				
                else if (loTypeValue.toUpper().compare(QString("int[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + loElements.at(j));
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + loElements.at(j));
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::int[]");
                }								
                else if (loTypeValue.toUpper().compare(QString("int4").toUpper()) == 0)
                {
                    loVariable = QString(loValue);
                }				
                else if (loTypeValue.toUpper().compare(QString("int4[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + loElements.at(j));
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + loElements.at(j));
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::int4[]");
                }								
                else if (loTypeValue.toUpper().compare(QString("int8").toUpper()) == 0)
                {
                    loVariable = QString(loValue);
                }	
                else if (loTypeValue.toUpper().compare(QString("int8[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + loElements.at(j));
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + loElements.at(j));
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::int8[]");
                }					
                else if (loTypeValue.toUpper().compare(QString("float8").toUpper()) == 0)
                {
                    loVariable = QString(loValue.replace(",","."));
                }											
                else if (loTypeValue.toUpper().compare(QString("float8[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + QString(loElements.at(j)).replace(",","."));
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + QString(loElements.at(j)).replace(",","."));
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::float8[]");
                }															
                else if (loTypeValue.toUpper().compare(QString("float4").toUpper()) == 0)
                {
                    loVariable = QString(loValue.replace(",","."));
                }															
                else if (loTypeValue.toUpper().compare(QString("float4[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + QString(loElements.at(j)).replace(",","."));
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + QString(loElements.at(j)).replace(",","."));
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::float4[]");
                }																		
                else
                {
                    return false;
                }
				/*
                if (i == 0)
                {
                    paResult = QString(paResult + loVariable);
                }
                else
                {
                    paResult = QString(paResult + "," + loVariable);
                }
				*/
                if (i == 0)
                {
                    paResult = QString(paResult + " " + loPropName + "=" + loVariable);
                }
                else
                {
                    paResult = QString(paResult + " , " + loPropName + "=" + loVariable);
                }

        }


        return true;
    }
    catch(...)
    {
        return false;
    }
}


//Get the values
bool clDatabasePostgres::getValuesString(vector <QString> paValues, vector <QString> paTypeValues, QString &paResult)
{
    try
    {
        for(int i = 0; i < paValues.size(); i++)
        {
                QString loTypeValue = paTypeValues[i];
                QString loValue = paValues[i];



            cout << "---------" << endl;
            cout << string(loTypeValue.toUtf8()) << endl;
            cout << string(loValue.toUtf8()) << endl;
            //cout << string(loPropName.toUtf8()) << endl;



                QString loVariable;

                if (loTypeValue.toUpper().compare(QString("uuid").toUpper()) == 0)
                {
                    if (loValue.toUpper().compare(QString("")) == 0)
                    {
                        loVariable = QString("NULL");
                    }
                    else
                    {
                        loVariable = QString("CAST ('" + loValue + "' AS uuid)");
                    }

                }
                else if (loTypeValue.toUpper().compare(QString("uuid[]").toUpper()) == 0)
                {

                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);
                            cout << loElements.count() << endl;
                            loVariable = QString(loVariable + "ARRAY[");

                                for (int j = 0; j < loElements.size(); j++)
                                {
                                    if (j == 0)
                                    {
                                        loVariable = QString(  loVariable + "CAST('" + loElements.at(j) + "' AS uuid)");
                                    }
                                    else
                                    {
                                        loVariable = QString( loVariable + ", CAST('" + loElements.at(j) + "' AS uuid)");
                                    }

                                }

                            //End array
                            loVariable = QString(loVariable + "]::uuid[]");

                }
                else if (loTypeValue.toUpper().compare(QString("timestamp").toUpper()) == 0)
                {
                    //to_timestamp('2004/11/31 14:30:12', 'YYYY/MM/DD HH24:MI:SS')
                    loVariable = QString(" to_timestamp('" + loValue + "', 'YYYY-MM-DD HH24:MI:SS')");
                }
                else if (loTypeValue.toUpper().compare(QString("timestamp(3)").toUpper()) == 0)
                {
                    //to_timestamp('2004/11/31 14:30:12.789', 'YYYY/MM/DD HH24:MI:SS.MS')
                    loVariable = QString(" to_timestamp('" + loValue + "', 'YYYY-MM-DD HH24:MI:SS.MS')");
                }				
                else if (loTypeValue.toUpper().compare(QString("timestamp[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + " to_timestamp('" + loElements.at(j) + "', 'YYYY-MM-DD HH24:MI:SS')");
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + " to_timestamp('" + loElements.at(j) + "', 'YYYY-MM-DD HH24:MI:SS')");
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::timestamp[]");

                }
                else if (loTypeValue.toUpper().compare(QString("timestamp(3)[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + " to_timestamp('" + loElements.at(j) + "', 'YYYY-MM-DD HH24:MI:SS.MS')");
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + " to_timestamp('" + loElements.at(j) + "', 'YYYY-MM-DD HH24:MI:SS.MS')");
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::timestamp(3)[]");

                }				
                else if (loTypeValue.toUpper().compare(QString("varchar(255)").toUpper()) == 0)
                {
                    loVariable = QString("'" + loValue + "'");
                }
                else if (loTypeValue.toUpper().compare(QString("text").toUpper()) == 0)
                {
                    loVariable = QString("'" + loValue + "'");
                }				
                else if (loTypeValue.toUpper().compare(QString("varchar(255)[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + "'" + loElements.at(j) + "'");
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + "'" + loElements.at(j) + "'");
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::varchar(255)[]");
                }
                else if (loTypeValue.toUpper().compare(QString("text[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + "'" + loElements.at(j) + "'");
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + "'" + loElements.at(j) + "'");
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::text[]");
                }				
                else if (loTypeValue.toUpper().compare(QString("varchar(64)").toUpper()) == 0)
                {
                    loVariable = QString("'" + loValue + "'");
                }
                else if (loTypeValue.toUpper().compare(QString("varchar(64)[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + "'" + loElements.at(j) + "'");
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + "'" + loElements.at(j) + "'");
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::varchar(64)[]");
                }				
                else if (loTypeValue.toUpper().compare(QString("int").toUpper()) == 0)
                {
					if (loValue.compare(QString("")) == 0)
					{
						loVariable = QString("0");
					}
					else
					{
						loVariable = QString(loValue.replace(",","."));
					}
                }				
                else if (loTypeValue.toUpper().compare(QString("int[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + loElements.at(j));
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + loElements.at(j));
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::int[]");
                }								
                else if (loTypeValue.toUpper().compare(QString("int4").toUpper()) == 0)
                {
					if (loValue.compare(QString("")) == 0)
					{
						loVariable = QString("0");
					}
					else
					{
						loVariable = QString(loValue.replace(",","."));
					}
                }				
                else if (loTypeValue.toUpper().compare(QString("int4[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + loElements.at(j));
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + loElements.at(j));
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::int4[]");
                }								
                else if (loTypeValue.toUpper().compare(QString("int8").toUpper()) == 0)
                {
					if (loValue.compare(QString("")) == 0)
					{
						loVariable = QString("0");
					}
					else
					{
						loVariable = QString(loValue.replace(",","."));
					}
                }	
                else if (loTypeValue.toUpper().compare(QString("int8[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + loElements.at(j));
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + loElements.at(j));
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::int8[]");
                }					
                else if (loTypeValue.toUpper().compare(QString("float8").toUpper()) == 0)
                {
					if (loValue.compare(QString("")) == 0)
					{
						loVariable = QString("0");
					}
					else
					{
						loVariable = QString(loValue.replace(",","."));
					}
                }											
                else if (loTypeValue.toUpper().compare(QString("float8[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + QString(loElements.at(j)).replace(",","."));
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + QString(loElements.at(j)).replace(",","."));
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::float8[]");
                }															
                else if (loTypeValue.toUpper().compare(QString("float4").toUpper()) == 0)
                {
					if (loValue.compare(QString("")) == 0)
					{
						loVariable = QString("0");
					}
					else
					{
						loVariable = QString(loValue.replace(",","."));
					}
                }															
                else if (loTypeValue.toUpper().compare(QString("float4[]").toUpper()) == 0)
                {
                        QStringList loElements = loValue.split("$;$", QString::SkipEmptyParts);

                        loVariable = QString(loVariable + "ARRAY[");

                        for (int j = 0; j < loElements.size(); j++)
                        {
                            if (j == 0)
                            {
                                loVariable = QString( loVariable + QString(loElements.at(j)).replace(",","."));
                            }
                            else
                            {
                                loVariable = QString( loVariable + "," + QString(loElements.at(j)).replace(",","."));
                            }

                        }
                        //End array
                        loVariable = QString(loVariable + "]::float4[]");
                }																		
                else
                {
                    return false;
                }

                if (i == 0)
                {
                    paResult = QString(paResult + loVariable);
                }
                else
                {
                    paResult = QString(paResult + "," + loVariable);
                }
        }
        return true;
    }
    catch(...)
    {
        return false;
    }
}
