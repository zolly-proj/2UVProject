#include "clDatabaseEditorWidget.h"
//Constructors
clDatabaseEditorWidget::clDatabaseEditorWidget(char * paIniFile, QWidget *paParent, const char *paName)
{



    meDatabaseEditor.setupUi(this);
	
    createActions();
    createMenus();
    createXMLReaderClass(paIniFile);
    createDatabaseClass();

    //Initialize the parameters
    meDatabaseConnection = NULL;
    setProgressBar(100,0);
}
//Destructors
clDatabaseEditorWidget::~clDatabaseEditorWidget()
{
    //Clean up the memory
    delete meDatabaseClass;
    delete meXMLReader;
}
/****************************
* UI ACTIONS
****************************/
//File save as
QString clDatabaseEditorWidget::saveAs()
{
    QString loFileName;
    try
    {
        loFileName = QFileDialog::getSaveFileName(this);
        return loFileName;
    }
    catch(...)
    {
        QString loErrorMessage("clDatabaseEditorWidget::saveAs() -> error ...");
        appendTextToLog(loErrorMessage);
        return loFileName;
    }
}

//Creating the actions
void clDatabaseEditorWidget::createActions()
{
    meActOpenDb = new QAction(tr("&Open DB"), this);
    meActOpenDb->setShortcuts(QKeySequence::New);
    meActOpenDb->setStatusTip(tr("Open the database"));
    connect(meActOpenDb, SIGNAL(triggered()), this, SLOT(openDB()));

    meActCloseDB = new QAction(tr("&Close DB..."), this);
    meActCloseDB->setShortcuts(QKeySequence::Open);
    meActCloseDB->setStatusTip(tr("Closing the database"));
    connect(meActCloseDB, SIGNAL(triggered()), this, SLOT(closeDB()));

    meUpdateDatabase = new QAction(tr("&Update DB"), this);
    meUpdateDatabase->setShortcuts(QKeySequence::Save);
    meUpdateDatabase->setStatusTip(tr("Update the database"));
    connect(meUpdateDatabase, SIGNAL(triggered()), this, SLOT(updateDB()));

    meBackupDatabase = new QAction(tr("&Backup DB..."), this);
    meBackupDatabase->setShortcuts(QKeySequence::Print);
    meBackupDatabase->setStatusTip(tr("Backup the database"));
    connect(meBackupDatabase, SIGNAL(triggered()), this, SLOT(backupDB()));

    meExitApplication = new QAction(tr("&Exit"), this);
    meExitApplication->setStatusTip(tr("Exit"));
    connect(meExitApplication, SIGNAL(triggered()), this, SLOT(exitApplication()));

    meSaveAss = new QAction(tr("&SaveAs"), this);
    meSaveAss->setStatusTip(tr("Save ass ..."));
    connect(meSaveAss, SIGNAL(triggered()), this, SLOT(saveLogging()));



}
//Creating the menu
void clDatabaseEditorWidget::createMenus()
{

    fileMenu = menuBar()->addMenu(tr("&File"));
    editMenu = menuBar()->addMenu(tr("&Edit"));
    helpMenu = menuBar()->addMenu(tr("&Help"));
    formatMenu = editMenu->addMenu(tr("&Format"));


    fileMenu->addAction(meActOpenDb);
    fileMenu->addAction(meActCloseDB);
    fileMenu->addAction(meUpdateDatabase);
    fileMenu->addAction(meBackupDatabase);
    fileMenu->addAction(meExitApplication);
    fileMenu->addAction(meSaveAss);

}
//Append text line to QTextEdit
void clDatabaseEditorWidget::appendTextToLog(const QString paString)
{
    try
    {
       meDatabaseEditor.txtInfo->append(QDateTime::currentDateTime ().toString() + ": " + paString);
    }
    catch(...)
    {
        printf("clDatabaseEditorWidget::appendTextToLog -> error ... ");
    }
}
//Void set progress bar
void clDatabaseEditorWidget::setProgressBar(int paTotal, int paNumber)
{
        try
        {
        meDatabaseEditor.pgbProgress->reset();
        meDatabaseEditor.pgbProgress->setMaximum(paTotal);
        meDatabaseEditor.pgbProgress->setMinimum (0);
        meDatabaseEditor.pgbProgress->setValue (paNumber);

        }
        catch(...)
        {
            printf("clDatabaseEditorWidget::setProgressBar -> error ... ");
        }
}

/****************************
* UI slots
****************************/
//Opening the database
void clDatabaseEditorWidget::openDB()
{
        try
        {
            QString loMessage;
            if (meDatabaseClass->openDatabase(loMessage))
            {
                appendTextToLog(loMessage);
            }
            else
            {
                appendTextToLog(loMessage);
                QString loErrorMessage("clDatabaseEditorWidget::openDB() -> error ...");
                appendTextToLog(loErrorMessage);
            }
        }
        catch(...)
        {
            QString loErrorMessage("clDatabaseEditorWidget::openDB() -> error ...");
            appendTextToLog(loErrorMessage);
        }
}
//Closeing the database
void clDatabaseEditorWidget::closeDB()
{
        try
        {
            QString loMessage;
            if (meDatabaseClass->closeDatabase(loMessage))
            {
                appendTextToLog(loMessage);
            }
            else
            {
                appendTextToLog(loMessage);
                QString loErrorMessage("clDatabaseEditorWidget::closeDB() -> error ...");
                appendTextToLog(loErrorMessage);
            }
        }
        catch(...)
        {
            QString loErrorMessage("clDatabaseEditorWidget::closeDB() -> error ...");
            appendTextToLog(loErrorMessage);
        }
}
//Update the database from XML
void clDatabaseEditorWidget::updateDB()
{
    try
    {
        //initialize loLocal vars
        vector <clDatabaseTable> loDatabaseTables;
		vector <clDatabaseRoutine> loDatabaseRoutines;
		vector <clDatabaseAction> loDatabaseActions;
        QString loErrorMessage;
        QString loDatabaseConfigFileName;

        //Get file path
        if (meXMLReader->getAttributeFromConfigFile("DATABASE_CONFIG_PATH",loDatabaseConfigFileName,loErrorMessage))
        {
			appendTextToLog(loErrorMessage);
			
            if (meXMLReader->readDatabaseConfiguration(loDatabaseConfigFileName,loDatabaseTables,loDatabaseRoutines,loErrorMessage))
            {
                appendTextToLog(QString("OK configuration file readed ") + QString("[%1] tables").arg(loDatabaseTables.size()) + QString("[%1] routines").arg(loDatabaseRoutines.size()));



				//Add the routine tables
				for (int i = 0; i < (int) loDatabaseRoutines.size();i++)
				{
					
					//Fill up the columns
					vector <clDatabaseColumn> loDatabaseColumns;
					
					clDatabaseColumn loDatabaseColumnPkey(QString("PKEY"),QString("Primary key"),QString("uuid"),QString(""), QString("no"));
					loDatabaseColumns.push_back(loDatabaseColumnPkey);
					
					clDatabaseColumn loDatabaseColumnName(QString("NAME"),QString(loDatabaseRoutines[i].getName()),QString("varchar(255)"),QString(""), QString("no"));
					loDatabaseColumns.push_back(loDatabaseColumnName);
					
					clDatabaseColumn loDatabaseColumnSourceType(QString("SOURCE_TYPE"),QString(loDatabaseRoutines[i].getSource()),QString("varchar(255)"),QString(""), QString("no"));
					loDatabaseColumns.push_back(loDatabaseColumnSourceType);
					
					clDatabaseColumn loDatabaseColumnSourceFile(QString("SOURCE_FILE"),QString(loDatabaseRoutines[i].getSourceName()),QString("varchar(255)"),QString(""), QString("no"));
					loDatabaseColumns.push_back(loDatabaseColumnSourceFile);
					
					clDatabaseColumn loDatabaseColumnReturnType(QString("METHOD_NAME"),QString(loDatabaseRoutines[i].getReturnType()),QString("varchar(255)"),QString(""), QString("no"));
					loDatabaseColumns.push_back(loDatabaseColumnReturnType);
					
					//clDatabaseColumn loDatabaseColumnWorkstation(QString("WORKSTATION_NAME"),QString("WORKSTATION_NAME"),QString("uuid"),QString(""), QString("WORKSTATION"));
					//loDatabaseColumns.push_back(loDatabaseColumnWorkstation);
					
					//clDatabaseColumn loDatabaseColumnCycle(QString("CYCLE_NAME"),QString("CYCLE_NAME"),QString("uuid"),QString(""), QString("CYCLE"));
					//loDatabaseColumns.push_back(loDatabaseColumnCycle);
					
					vector <QString> loRoutineObjects = loDatabaseRoutines[i].getObjects();
					for (int j = 0; j < (int) loRoutineObjects.size(); j++)
					{
							clDatabaseColumn loDatabaseColumnObject(QString(loRoutineObjects[j]),QString(loRoutineObjects[j]),QString("uuid"),QString(""),QString(loRoutineObjects[j]));
							loDatabaseColumns.push_back(loDatabaseColumnObject);							
					}					
					
					//Fill up the table
					clDatabaseTable loDatabaseTable(loDatabaseRoutines[i].getName(), loDatabaseColumns, loDatabaseActions);
					loDatabaseTables.push_back(loDatabaseTable);
				}

                //UPDATE THE DATABASE
                if (updateDatabase(loDatabaseTables,loErrorMessage))
                {
                    appendTextToLog(QString("OK update database ..."));
                }
                else
                {
                    appendTextToLog(loErrorMessage);
                    QString loMessage("clDatabaseEditorWidget::updateDB() -> error updating database ...");
                    appendTextToLog(loMessage);
                    return;
                }
            }
            else
            {
                appendTextToLog(loErrorMessage);
                QString loMessage("clDatabaseEditorWidget::updateDB() -> error reading '" + loDatabaseConfigFileName + "'...");
                appendTextToLog(loMessage);
                return;
            }
        }
        else
        {
            appendTextToLog(loErrorMessage);
            QString loMessage("clDatabaseEditorWidget::updateDB() -> error getting var 'DATABASE_CONFIG_PATH'...");
            appendTextToLog(loMessage);
            return;
        }
    }
    catch(...)
    {
        QString loErrorMessage("clDatabaseEditorWidget::updateDB() -> error ...");
        appendTextToLog(loErrorMessage);
    }
}
//Create backup of the database
void clDatabaseEditorWidget::backupDB()
{
    try
    {

        QString loSaveFileAs = saveAs();
        if (!loSaveFileAs.isEmpty())
        {
            appendTextToLog("Saving backup on: '" + loSaveFileAs + "'");
            //Backup the database

            if (backupDatabase(loSaveFileAs,meDatabaseClass->getDataseName(),meDatabaseClass->getUserName()))
            {
                QString loInfoMessage("Backup successfull ...");
                appendTextToLog(loInfoMessage);
            }
            else
            {
                QString loInfoMessage("Backup not succesfull ...");
                appendTextToLog(loInfoMessage);
            }
        }


    }
    catch(...)
    {
        QString loErrorMessage("clDatabaseEditorWidget::saveAs() -> error ...");
        appendTextToLog(loErrorMessage);
    }
}
//Exit the application
void clDatabaseEditorWidget::exitApplication()
{
        try
        {
            closeDB();
            this->close();
        }
        catch(...)
        {
            QString loErrorMessage("clDatabaseEditorWidget::exitApplication() -> error ...");
            appendTextToLog(loErrorMessage);
        }
}

//Exit the application
void clDatabaseEditorWidget::saveLogging()
{
        try
        {
            QString loFilePath = saveAs();
            QString loContent = meDatabaseEditor.txtInfo->toPlainText();
            ofstream loOutfile;

            loFilePath.replace(QString("\\"),QString("\\\\"));
            QByteArray loBa = loFilePath.toLatin1();
            char *loString = loBa.data();

            loOutfile.open(loString);

            QByteArray loBa1 = loContent.toLatin1();
            char *loStringContent = loBa1.data();
            loOutfile<<loStringContent;
            loOutfile.close();



        }
        catch(...)
        {
            QString loErrorMessage("clDatabaseEditorWidget::saveLogging() -> error ...");
            appendTextToLog(loErrorMessage);
        }
}

/****************************
* Database actions
****************************/
//Backup the database
bool clDatabaseEditorWidget::backupDatabase(QString paFileName, QString paDatbaseName, QString paUserName)
{
    try
    {
		appendTextToLog("backupDatabase enter");
        QByteArray loBa1 = paFileName.toLatin1();
     
        QByteArray loBa2 = paDatbaseName.toLatin1();
     
        QByteArray loBa3 = paUserName.toLatin1();
     
		char * loArg01 = NULL;
		char * loArg02 = NULL;
		char * loArg03 = NULL;
		char * loArg04 = NULL;
		char * loArg05 = NULL;
		char * loArg06 = NULL;
		char * loArg07 = NULL;
		char * loArg08 = NULL;
		char * loArg09 = NULL;
		char * loArg10 = NULL;
		QByteArray byteArray[10];
		
	
		for(int i = 0; i < 3; i++)
		{
			switch(i)
			{
				case 0:
					loArg01 = loBa1.data();
					break;
				case 1:
					loArg02 = loBa2.data();
					break;
				case 2:
					loArg03 = loBa3.data();
					break;					
				default:
					break;
			}
		}
		QString loTemp = QString(paFileName);	
        if (!performScript(QString(loTemp.replace(".py","")).toUtf8().data(),"createDatabaseBackup","integer",loArg01, loArg02, loArg03, loArg04, loArg05, loArg06, loArg07, loArg08, loArg09, loArg10))
			return false;
		
		cout << endl << "Do something with the return value" << endl;

		appendTextToLog("backupDatabase exit");
   		
		return true;		
		
		
    }
    catch(...)
    {
        QString loErrorMessage("clDatabaseEditorWidget::backupDatabase(QString paFileName) -> error ...");
        appendTextToLog(loErrorMessage);
        return false;
    }
}
//Perform a script
bool clDatabaseEditorWidget::performScript(char * paScriptName, char * paScriptClass, char * paScriptMethod, char *paArg01, char *paArg02, char *paArg03, char *paArg04, char *paArg05, char *paArg06, char *paArg07, char *paArg08, char *paArg09, char *paArg10)
{
        try
        {
	        PyObject *pName, *pModule, *pDict, *pClass, *pInstance, *pValue;
    		/*
			PyErr_Clear();
			*/
			if (!Py_IsInitialized())
			{ 
				Py_Initialize();
				PyRun_SimpleString("import sys");
			}			
			
			
            pName = PyUnicode_FromString(paScriptName);
            pModule = PyImport_Import(pName);
            pDict = PyModule_GetDict(pModule);
    		
            pClass = PyDict_GetItemString(pDict, paScriptClass);
			
	
            // Create an instance of the class

            if (PyCallable_Check(pClass))
            {
                pInstance = PyObject_CallObject(pClass, NULL);
            }
	
            switch (3)
            {
                    case 0:
                        pValue = PyObject_CallMethod(pInstance, paScriptMethod, NULL);
                        break;
                    case 1:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(s)", paArg01);
                        break;
                    case 2:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ss)", paArg01, paArg02);
                        break;
                    case 3:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sss)", paArg01, paArg02, paArg03);
                        break;
                    case 4:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssss)", paArg01, paArg02, paArg03,paArg04);
                        break;
                    case 5:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssss)", paArg01, paArg02, paArg03,paArg04,paArg05);
                        break;
                    case 6:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06);
                        break;
                    case 7:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07);
                        break;
                    case 8:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08);
                        break;
                    case 9:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(sssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08,paArg09);
                        break;
                    case 10:
                        pValue = PyObject_CallMethod(pInstance,
                        paScriptMethod, "(ssssssssss)", paArg01, paArg02, paArg03,paArg04,paArg05,paArg06,paArg07,paArg08,paArg09,paArg10);
                        break;
                    default:
                        pValue = PyObject_CallMethod(pInstance, paScriptMethod, NULL);
                        break;
            }

            if (pValue != NULL)
            {
				meResultFromFunction = PyFloat_AsDouble(pValue);
                printf("Return of call : %d\n", (int) meResultFromFunction);
				if (pValue != NULL)Py_DECREF(pValue);
				/*
                if (pInstance != NULL)Py_DECREF(pInstance);
				if (pClass != NULL)Py_DECREF(pClass);
				if (pDict != NULL)Py_DECREF(pDict);
				if (pModule != NULL)Py_DECREF(pModule);
				if (pName != NULL)Py_DECREF(pName);
				*/
            }
            else
            {
                PyErr_Print();
				meResultFromFunction = -1;
				printf("Return of call : %d\n", (int) meResultFromFunction);			
				return false;
            }
			
			PyErr_Clear();
			
			//if (pInstance != NULL)Py_DECREF(pInstance);
			//PyErr_Clear();
			
            return true;
        }
        catch(exception &e)
        {
            return false;
        }
}
/*************************************
* Local actions
**************************************/
//Create the database class
//bool clDatabaseEditorWidget::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader & paXMLReader)
bool clDatabaseEditorWidget::createDatabaseClass()
{
    try
    {
        QString loValue;
        QString  loMessage;

        QString loUserName, loPassword, loDatabaseName, loHostAdress, loPort, loDatabaseTech;

        //Getting the user name
        if (meXMLReader->getAttributeFromConfigFile("USERNAME",loValue,loMessage))
        {
            loUserName = loValue;
			appendTextToLog(loMessage);
        }
        else
        {
            appendTextToLog(loMessage);
            QString loErrorMessage("clDatabaseEditorWidget::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error getting var 'USERNAME'...");
            appendTextToLog(loErrorMessage);
            return false;
        }

        //Getting the password
        if (meXMLReader->getAttributeFromConfigFile("PASSWORD",loValue,loMessage))
        {
            loPassword = loValue;
			appendTextToLog(loMessage);
        }
        else
        {
            appendTextToLog(loMessage);
            QString loErrorMessage("clDatabaseEditorWidget::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error getting var 'PASSWORD'...");
            appendTextToLog(loErrorMessage);
            return false;
        }

        //Getting the DatabaseName
        if (meXMLReader->getAttributeFromConfigFile("DATABASENAME",loValue,loMessage))
        {
            loDatabaseName = loValue;
			appendTextToLog(loMessage);
        }
        else
        {
            appendTextToLog(loMessage);
            QString loErrorMessage("clDatabaseEditorWidget::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error getting var 'DATABASENAME'...");
            appendTextToLog(loErrorMessage);
            return false;
        }

        //Getting the host adress
        if (meXMLReader->getAttributeFromConfigFile("HOSTADRESS",loValue,loMessage))
        {
            loHostAdress = loValue;
			appendTextToLog(loMessage);
        }
        else
        {
            appendTextToLog(loMessage);
            QString loErrorMessage("clDatabaseEditorWidget::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error getting var 'HOSTADRESS'...");
            appendTextToLog(loErrorMessage);
            return false;
        }

        //Getting the port
        if (meXMLReader->getAttributeFromConfigFile("PORT",loValue,loMessage))
        {
            loPort = loValue;
			appendTextToLog(loMessage);
        }
        else
        {
            appendTextToLog(loMessage);
            QString loErrorMessage("clDatabaseEditorWidget::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error getting var 'PORT'...");
            appendTextToLog(loErrorMessage);
            return false;
        }

        //Getting the tech
        if (meXMLReader->getAttributeFromConfigFile("DATABASETECH",loValue,loMessage))
        {
            loDatabaseTech = loValue;
			appendTextToLog(loMessage);
        }
        else
        {
            appendTextToLog(loMessage);
            QString loErrorMessage("clDatabaseEditorWidget::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error getting var 'DATABASETECH'...");
            appendTextToLog(loErrorMessage);
            return false;
        }
        appendTextToLog("Config XML file is readed correctly ...");


        meDatabaseClass = new clDatabaseClass(loUserName,loPassword,loDatabaseName,loHostAdress,loPort,loDatabaseTech);
		appendTextToLog("DatatbaseClass created ...");
        return true;
    }
    catch(...)
    {
        QString loErrorMessage("clDatabaseEditorWidget::createDatabaseClass(clDatabaseClass * paDatbaseClass, clXMLReader * paXMLReader) -> error ...");
        appendTextToLog(loErrorMessage);
        return false;
    }
}

//Create the XML reader class
//bool clDatabaseEditorWidget::createXMLReaderClass(char * paFileName, clXMLReader & paXMLReader)
bool clDatabaseEditorWidget::createXMLReaderClass(char * paFileName)
{
    try
    {
        //padatbaseClass = clDatabaseClass(paFileName);
        QString loFileName(paFileName);
        meXMLReader = new clXMLReader(loFileName);
        return true;
    }
    catch(...)
    {
        QString loErrorMessage("clDatabaseEditorWidget::createXMLReaderClass(char * paFilename, clXMLReader * paXMLReader) -> error ...");
        appendTextToLog(loErrorMessage);
        return false;
    }
}
//Update the database (run over structure of tables and create update or delete the tables and columns)
bool clDatabaseEditorWidget::updateDatabase(vector <clDatabaseTable> paDatabaseTable, QString & paMessage)
{
    try
    {

        for (int i = 0; i < (int) paDatabaseTable.size(); i++)
        {
            setProgressBar((int) paDatabaseTable.size(),i + 1);
            //////////////////////////////////
            //Checking and adding the table //
            //////////////////////////////////
            clDatabaseTable loDatabaseTable = paDatabaseTable[i];
            QString loMessage("Start processing Table[" + loDatabaseTable.getTableName() + "] nr. [" + QString("%1").arg(i) + "]");
            appendTextToLog(loMessage);

            bool loTableExist = false;
            QString loExistMessage;
            if (meDatabaseClass->checkDatabaseTableExist(loDatabaseTable.getTableName(),loTableExist,loExistMessage))
            {
                if (!loTableExist)
                {
                    appendTextToLog("Table [" + loDatabaseTable.getTableName() + "] NOT exists -> create ...");

                    //Create the normal table
                    QString loTableCreationMessage;
                    if (!meDatabaseClass->createDatabaseTable(loDatabaseTable.getTableName(),loTableCreationMessage))
                    {
                        appendTextToLog(loTableCreationMessage);
                        return false;
                    }
                    appendTextToLog(loTableCreationMessage);
                }
                else
                {
                    appendTextToLog("Table [" + loDatabaseTable.getTableName() + "] ALREADY exists ...");
                }
            }
            else
            {
                appendTextToLog(loExistMessage);
                return false;
            }
            //!!!!!!!!!!!!!!!!!!! At this point the table must exist

            ///////////////////////////////////
            //Cheking and adding the columns //
            ///////////////////////////////////

            QString loGetColumnsMessage;
            //Get the columns of the table in the xml file
            vector <clDatabaseColumn> loDatabaseColumns = loDatabaseTable.getDatabaseColumns();
            //meDatabaseClass->getDatabaseColumns(loDatabaseTable.getTableName(), loDatabaseColumns, loGetColumnsMessage);

            for (int j= 0; j < (int) loDatabaseColumns.size(); j++)
            {
                clDatabaseColumn loDatabaseColumn = loDatabaseColumns[j];
                QString loColumnMessage("Start processing Column[" + loDatabaseColumn.getName() + "]");
                appendTextToLog(loColumnMessage);

                bool loColumnExists = false;
                bool loTypeMatch = false;
                QString loColumnExistsMessage;

                //TODO check columns
                meDatabaseClass->checkDatabaseColumnExist(loDatabaseTable.getTableName(), loDatabaseColumn.getName(), loDatabaseColumn.getType(),loDatabaseColumn.getExtra(), loColumnExists, loTypeMatch, loColumnExistsMessage);
                appendTextToLog(loColumnExistsMessage);
                if (loColumnExists && loTypeMatch)
                {
                    QString loTypeMessage("Column [" + loDatabaseColumn.getName() + "] with type [" + loDatabaseColumn.getType() + "] exists with same type in database ...");
                    appendTextToLog(loTypeMessage);
                }
                else if (loColumnExists && !loTypeMatch)
                {
                    QString loTypeMessage("Column [" + loDatabaseColumn.getName() + "] with type [" + loDatabaseColumn.getType() + "] exists with other type in database ...");
                    appendTextToLog(loTypeMessage);
                    if (!meDatabaseClass->updateDatabaseColumn(loDatabaseTable.getTableName(), loDatabaseColumn.getName(), loDatabaseColumn.getType(), loDatabaseColumn.getExtra(), loDatabaseColumn.getReference(),loTypeMessage))
                    {
                        appendTextToLog(loTypeMessage);
                        return false;
                    }
                    else
                    {
                        appendTextToLog(loTypeMessage);
                    }

                }
                else if (!loColumnExists && !loTypeMatch)
                {
                    QString loTypeMessage("Column [" + loDatabaseColumn.getName() + "] with type [" + loDatabaseColumn.getType() + "] does not exists -> to create ...");
                    appendTextToLog(loTypeMessage);
                    if (!meDatabaseClass->createDatabaseColumn(loDatabaseTable.getTableName(), loDatabaseColumn.getName(), loDatabaseColumn.getType(), loDatabaseColumn.getExtra(), loDatabaseColumn.getReference(),loTypeMessage))
                    {
                        appendTextToLog(loTypeMessage);
                        return false;
                    }
                    else
                    {
                        appendTextToLog(loTypeMessage);
                    }

                }

            }

            ////////////////////////////////////
            //checking and adding the actions //
            ////////////////////////////////////
            vector <clDatabaseAction> loDatabaseActions = loDatabaseTable.getDatabaseActions();
            for (int z = 0; z < (int) loDatabaseActions.size(); z++)
            {
                clDatabaseAction loDatabaseAction = loDatabaseActions[z];
                QString loActionMessage("Start processing Action[" + loDatabaseAction.getName() + "]");
                appendTextToLog(loActionMessage);

                vector <QString> loProperties;
                vector <QString> loTypeValue;
                vector <QString> loValue;
                vector <QString> loLogExp;
                vector <QString> loIds;
                loProperties.push_back(QString("ACTION_NAME"));
                loValue.push_back(QString(loDatabaseAction.getName()));
                loLogExp.push_back(QString("="));
                loTypeValue.push_back(QString("TEXT"));


                QString loGetActionMessage;
                if (!meDatabaseClass->getFromTableDatabaseByProperty("a_" + loDatabaseTable.getTableName(), QString("0"), QString("100"), loProperties, loValue, loTypeValue, loLogExp, loIds, loGetActionMessage))
                {
                    appendTextToLog(loGetActionMessage);
                    return false;
                }
                else
                {
                    if(loIds.size() == 0)
                    {
                        vector <QString> loInsertProperties;
                        vector <QString> loInsertValues;
                        vector <QString> loInsertTypeValue;

                        loInsertProperties.push_back(QString("ACTION_NAME"));
                        loInsertProperties.push_back(QString("ACTION_ALIAS"));
                        loInsertProperties.push_back(QString("ACTION_SOURCE"));
                        loInsertProperties.push_back(QString("ACTION_SOURCENAME"));
                        loInsertProperties.push_back(QString("ACTION_METHODNAME"));

                        loInsertValues.push_back(QString(loDatabaseAction.getName()));
                        loInsertValues.push_back(QString(loDatabaseAction.getAlias()));
                        loInsertValues.push_back(QString(loDatabaseAction.getSource()));
                        loInsertValues.push_back(QString(loDatabaseAction.getSourceName()));
                        loInsertValues.push_back(QString(loDatabaseAction.getReturnType()));

                        loInsertTypeValue.push_back(QString("TEXT"));
                        loInsertTypeValue.push_back(QString("TEXT"));
                        loInsertTypeValue.push_back(QString("TEXT"));
                        loInsertTypeValue.push_back(QString("TEXT"));
                        loInsertTypeValue.push_back(QString("TEXT"));

                        QString loInsertMessage;

                        if(!meDatabaseClass->insertIntoTableDatabase(QString("a_" + loDatabaseTable.getTableName()),loInsertProperties,loInsertValues,loInsertTypeValue,loInsertMessage))
                        {
                            appendTextToLog(loInsertMessage);
                            return false;
                        }
                        else
                        {
                            appendTextToLog(loInsertMessage);
                        }
                    }
                    else
                    {
                        appendTextToLog("Action name [" + QString(loDatabaseAction.getName()) + "] exists in database -> check if update needed ...");

                        vector <QString> loUpdateProperties;
                        vector <QString> loUpdateValues;
                        vector <QString> loUpdateTypeValue;

                        loUpdateProperties.push_back(QString("ACTION_NAME"));
                        loUpdateProperties.push_back(QString("ACTION_ALIAS"));
                        loUpdateProperties.push_back(QString("ACTION_SOURCE"));
                        loUpdateProperties.push_back(QString("ACTION_SOURCENAME"));
                        loUpdateProperties.push_back(QString("ACTION_METHODNAME"));

                        loUpdateValues.push_back(QString(loDatabaseAction.getName()));
                        loUpdateValues.push_back(QString(loDatabaseAction.getAlias()));
                        loUpdateValues.push_back(QString(loDatabaseAction.getSource()));
                        loUpdateValues.push_back(QString(loDatabaseAction.getSourceName()));
                        loUpdateValues.push_back(QString(loDatabaseAction.getReturnType()));

                        loUpdateTypeValue.push_back(QString("TEXT"));
                        loUpdateTypeValue.push_back(QString("TEXT"));
                        loUpdateTypeValue.push_back(QString("TEXT"));
                        loUpdateTypeValue.push_back(QString("TEXT"));
                        loUpdateTypeValue.push_back(QString("TEXT"));

                        QString loUpdateMessage;
                        //bool updateIntoTableDatabase(QString paTableName,QString paId, vector <QString> paColumns, vector <QString> paValues, vector<QString> paTypeValue, QString &paMessage);
                        if (!meDatabaseClass->updateIntoTableDatabase(QString("a_" + loDatabaseTable.getTableName()),loIds[0],loUpdateProperties,loUpdateValues,loUpdateTypeValue,loUpdateMessage))
                        {
                            appendTextToLog(loUpdateMessage);
                            return false;
                        }
                        else
                        {
                            appendTextToLog(loUpdateMessage);
                        }
                    }
                }
                //TODO check actoins
                //Add actions
                //meDatabaseClass->addA
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////
        // Getting all the tables and check wether a column or table should be dropped
        //////////////////////////////////////////////////////////////////////////////////////

        QString loTableMessage;
        vector<clDatabaseTable> loDatabaseTables;
        if (!meDatabaseClass->getDatabaseTables(loDatabaseTables,loTableMessage))
        {
            appendTextToLog(loTableMessage);
            return false;
        }
        else
        {
            appendTextToLog(loTableMessage);
            /////////////////////////////////////////////////////////////////////////////////
            // Loop over the Tables readed and check if the Table exists in the database //
            /////////////////////////////////////////////////////////////////////////////////
            for (int j=0; j < (int) loDatabaseTables.size(); j++)
            {
                    setProgressBar((int) loDatabaseTables.size(),j + 1);
                    clDatabaseTable loDatabaseTable = loDatabaseTables[j];
                    appendTextToLog(QString("Check if the table [" + loDatabaseTable.getTableName() + "] still in XML file"));
                    //////////////////////////////////////////////////////////////////////
                    // Loop over the tables in xml and delete what is not there anymore //
                    //////////////////////////////////////////////////////////////////////
                    bool loExists = false;
                    bool loExistsAction = false;
                    int loMemActions = 0;
                    for (int k=0; k < (int) paDatabaseTable.size(); k++)
                    {
                        clDatabaseTable loDatabaseTableXML = paDatabaseTable[k];

                        if ( QString(loDatabaseTableXML.getTableName()).toUpper().compare(loDatabaseTable.getTableName().toUpper()) == 0)
                        {
                            loExists = true;
                            loMemActions = k;
                        }
                        else if( QString("a_" + loDatabaseTableXML.getTableName()).toUpper().compare(loDatabaseTable.getTableName().toUpper()) == 0)
                        {
                            loExistsAction = true;

                        }
                    }

                    //////////////////////////////////////////////////////////////////
                    // The table does not exists remove the table from the database //
                    //////////////////////////////////////////////////////////////////
                    if (!loExists && !loExistsAction)
                    {
                        appendTextToLog("Drop table [" + QString(loDatabaseTable.getTableName()) + "] -> not anymore in xml");
                        QString loDropTableMessage;

                        if (!meDatabaseClass->dropDatabaseTable(loDatabaseTable.getTableName(),loDropTableMessage))
                        {
                            appendTextToLog(loDropTableMessage);
                            return false;
                        }
                        else
                        {
                            appendTextToLog(loDropTableMessage);
                        }
                    }
                    else
                    {
                        appendTextToLog("Don't drop table [" + QString(loDatabaseTable.getTableName()) + "] -> still in XML");
                    }


                    ///////////////////////////////////////////////////////////////////
                    // The table does exists, check columns and actions on the table //
                    ///////////////////////////////////////////////////////////////////
                    if (loExists)
                    {
                        ///////////////////////////////////////////////
                        // Delete actions who are not anymore in XML //
                        ///////////////////////////////////////////////
                        vector <clDatabaseAction> loDatabaseActionsXML = paDatabaseTable[loMemActions].getDatabaseActions();

                        QString loGetActionsMessage;
                        vector <QString> loIds;
                        if (!meDatabaseClass->getFromTableDatabaseGeneral(QString("a_" + loDatabaseTable.getTableName()),QString("0"),QString("0"),loIds,loGetActionsMessage))
                        {
                            appendTextToLog(loGetActionsMessage);
                            return false;
                        }
                        else
                        {
                            for (int z = 0; z < (int) loIds.size(); z++)
                            {
                                vector <QString> loProp;
                                vector <QString> loVal;


                                loProp.push_back("ACTION_NAME");
                                loProp.push_back("ACTION_SOURCE");


                                QString loGetPropMessage;
                                if (!meDatabaseClass->getFromTableDatabaseById(QString("a_" + loDatabaseTable.getTableName()),loIds[z], loProp,loVal,loGetPropMessage))
                                {
                                    appendTextToLog(loGetPropMessage);
                                    return false;
                                }
                                else
                                {
                                    bool loExistActionXML = false;
                                    for(int l = 0; l < (int) loDatabaseActionsXML.size();l++)
                                    {
                                        if (loDatabaseActionsXML[l].getName().toLower().compare(loVal[0].toLower()) == 0)
                                        {
                                            loExistActionXML = true;
                                            break;
                                        }
                                    }
                                    if (!loExistActionXML)
                                    {
                                        //Delete the actoin
                                        appendTextToLog(QString("Action [" + loVal[0] + "] not exists in xml -> deleting ..."));
                                        QString loDeleteMessage;
										/*
                                        if (!meDatabaseClass->deleteIntoTableDatabase(QString("a_" + loDatabaseTable.getTableName()),loIds[z], loDeleteMessage))
                                        {
                                            appendTextToLog(loDeleteMessage);
                                            return false;
                                        }
                                        else
                                        {
                                            appendTextToLog(loDeleteMessage);
                                            appendTextToLog(QString("Action [" + loVal[0] + "] not exists in xml -> deleted ..."));
                                        }
										*/
                                    }
                                    else
                                    {
                                        appendTextToLog(QString("Action [" + loVal[0] + "] exists in xml -> not deleting ..."));
                                    }
                                }
                            }
                        }





                        ///////////////////////////////////////////////
                        // Delete columns who are not anymore in XML //
                        ///////////////////////////////////////////////
                        vector <clDatabaseColumn> loDatabaseColumns;
                        QString loGetColumnsMess;
                        if (meDatabaseClass->getDatabaseColumns(loDatabaseTable.getTableName(),loDatabaseColumns,loGetColumnsMess))
                        {
                            appendTextToLog("Get Columns OK");
                            appendTextToLog(loGetColumnsMess);
                        }
                        else
                        {
                            appendTextToLog("Get Columns NOT OK");
                            appendTextToLog(loGetColumnsMess);
                            return false;
                        }

                        
                        for (int t = 0; t < (int) loDatabaseColumns.size(); t++)
                        {
                            for (int l = 0; l < (int) paDatabaseTable.size(); l++)
                            {
                                clDatabaseTable loDatabaseTableXML = paDatabaseTable[l];
                                if ( QString(loDatabaseTableXML.getTableName()).toUpper().compare(loDatabaseTable.getTableName().toUpper()) == 0)
                                {
                                    vector <clDatabaseColumn> loDatabaseXMLColumns;
                                    loDatabaseXMLColumns = loDatabaseTableXML.getDatabaseColumns();


                                    bool loColumnExists= false;
                                    for(int z = 0; z < (int) loDatabaseXMLColumns.size(); z++)
                                    {
                                        clDatabaseColumn loDatabaseColumn = loDatabaseXMLColumns[z];
                                        if (QString(loDatabaseColumn.getName()).toUpper().compare(loDatabaseColumns[t].getName().toUpper()) == 0)
                                        {
                                            loColumnExists = true;
                                        }
                                    }
                                    //loColumnExists
                                    if (loColumnExists)
                                    {
                                        appendTextToLog("Leave column '" + QString(loDatabaseColumns[t].getName()).toUpper() + "'");
                                    }
                                    else
                                    {
                                        appendTextToLog("Drop column '" + QString(loDatabaseColumns[t].getName()).toUpper() + "'");
                                        QString loDropColumnMessage;
                                        if (meDatabaseClass->dropDatabaseColumn(loDatabaseTable.getTableName(), QString(loDatabaseColumns[t].getName()).toUpper(), loDropColumnMessage))
                                        {
                                            appendTextToLog(loDropColumnMessage);
                                        }
                                        else
                                        {
                                            appendTextToLog(loDropColumnMessage);
                                            return false;
                                        }
                                    }
                                }
                            }
                        }
                    }
            }
        }
        return true;
    }
    catch(...)
    {
        QString loErrorMessage("clDatabaseEditorWidget::updateDatabase(vector <clDatabaseTable> paDatabaseTable,QString & paErrorMessage) -> error ...");
        paMessage = loErrorMessage;
        return false;
    }
}
/*********************************
* Test code
***********************************/
/*
QString loTestMessage;
                bool loTestExists;

                if(meDatabaseClass->checkDatabaseTableExist("persoon",loTestExists,loTestMessage))
                {
                    appendTextToLog(loTestMessage);
                }
                else
                {
                    appendTextToLog(loTestMessage);
                }

                if(meDatabaseClass->checkDatabaseTableExist("atable",loTestExists,loTestMessage))
                {
                    appendTextToLog(loTestMessage);
                }
                else
                {
                    appendTextToLog(loTestMessage);
                }
*/
/*
                //TESTING CODE
                QString loTestMessage;
                bool loTestExists;
                bool loTestRightType;

                if(meDatabaseClass->checkDatabaseColumnExist("t_people","did","integer","",loTestExists,loTestRightType,loTestMessage))
                {
                    appendTextToLog(loTestMessage + QString(loTestRightType));
                }
                else
                {
                    appendTextToLog(loTestMessage);
                }
                if(meDatabaseClass->checkDatabaseColumnExist("t_people","test","test","",loTestExists,loTestRightType,loTestMessage))
                {
                    appendTextToLog(loTestMessage + QString(loTestRightType));
                }
                else
                {
                    appendTextToLog(loTestMessage);
                }
*/
