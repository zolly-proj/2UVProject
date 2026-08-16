#include "clXMLReader.h"

clXMLReader::clXMLReader(QString paFileName)
{
        setFileName(paFileName);
}

clXMLReader::~clXMLReader()
{

}

/******************************
* Local functions
*******************************/
//Check if the column exists in the table
bool clXMLReader::checkIfColumnAlreadyExists(clDatabaseColumn paDatabaseColumn, vector <clDatabaseColumn> paDatabaseColumns)
{
    try
    {
        for (int i = 0; i < paDatabaseColumns.size(); i++)
        {
            clDatabaseColumn loDatabaseColumn = paDatabaseColumns[i];
            if (paDatabaseColumn.getName().compare(QString(loDatabaseColumn.getName())) == 0)
            {
                return true;
            }
        }
        return false;
    }
    catch(...)
    {
        return true;
    }
}
//Check if action exists in the vector
bool clXMLReader::checkIfRoutineAlreadyExists(clDatabaseRoutine paDatabaseRoutine, vector <clDatabaseRoutine> paDatabaseRoutines)
{
    try
    {
        for (int i = 0; i < paDatabaseRoutines.size(); i++)
        {
            clDatabaseRoutine loDatabaseRoutine = paDatabaseRoutines[i];
            if (paDatabaseRoutine.getName().compare(QString(loDatabaseRoutine.getName())) == 0)
            {
				vector <QString> loNewObjects = paDatabaseRoutine.getObjects();
				vector <QString> loExistingObjects = paDatabaseRoutines[i].getObjects();
				if ((int)loNewObjects.size() == (int)loExistingObjects.size())
				{
					for (int j = 0; j < loNewObjects.size(); j++)
					{
						if (QString(loNewObjects[j]) != QString(loExistingObjects[j]))
						{
							return false;
						}
					}
					return true;
				}
				else
				{
					return false;
				}
            }
        }
        return false;
    }
    catch(...)
    {
        return true;
    }
}
//Check if table name exists in the vector
bool clXMLReader::checkIfTableAlreadyExists(clDatabaseTable paDatabaseTable, vector <clDatabaseTable> paDatabaseTables)
{
    try
    {
        //printf("%d tetten",paDatabaseTables.size());
        for (int i = 0; i < paDatabaseTables.size(); i++)
        {
            clDatabaseTable loDatabaseTable = paDatabaseTables[i];
            if (paDatabaseTable.getTableName().compare(QString(loDatabaseTable.getTableName())) == 0)
            {
                return true;
            }
        }
        return false;
    }
    catch(...)
    {
        return true;
    }
}
/******************************
* XML Reading function
*******************************/
bool clXMLReader::getAttributeFromConfigFile(QString paAttribute, QString & paValue, QString & paReturnMessage)
{
    try
    {
        QString loFileName = QString(getFileName());

        QFile loFile(loFileName);
        if ( !loFile.open( QIODevice::ReadOnly ) ) {
            QString loTemp("clXMLReader::getAttributeFromConfigFile(QString paAttribute, QString * paValue, QString * paReturnMessage) -> Cound not open file '" + paAttribute + "' ...");

            paReturnMessage = loTemp;
            return false;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) {
            QString loTemp("clXMLReader::getAttributeFromConfigFile(QString paAttribute, QString * paValue, QString * paReturnMessage) -> Cound not parse file '" + paAttribute + "' ...");
            paReturnMessage = loTemp;
            loFile.close();
            return false;
        }

        // create the tree view out of the DOM
       QDomElement loDocElem = loDomDocument.documentElement();

       QDomNode loDomNode = loDocElem.firstChild();
       while( !loDomNode.isNull() ) {

          if(loDomNode.nodeName() == paAttribute)
          {
              QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
              if( !loDocElemChild.isNull() )// the node was really an element.
              {
                  if (loDocElemChild.hasAttribute(QString("value")))
                  {
                        //QString loValue = loDocElemChild.attribute(paAttribute);
                        //paValue = &loValue;
                        paValue = loDocElemChild.attribute("value");
                        QString loTemp("Getting attribute '" + paAttribute + "' with value '" + paValue + "'");
                        paReturnMessage = loTemp;
                        return true;
                        break;
                  }
              }
          }
          loDomNode = loDomNode.nextSibling();
       }

        QString loTempLast("clXMLReader::getAttributeFromConfigFile(QString paAttribute, QString * paValue, QString * paReturnMessage) -> No attribute '" + paAttribute + "' found ...");
        paReturnMessage = loTempLast;
        return true;
    }
    catch(...)
    {
        QString loTemp("clXMLReader::getAttributeFromConfigFile(QString paAttribute, QString * paValue, QString * paReturnMessage) -> error ...");
        paReturnMessage = loTemp;
        return false;
    }
}
//Reading the database configuration
bool clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTables, vector<clDatabaseRoutine> & paDatabaseRoutines, QString &paReturnMessage)
{
    try
    {
        QFile loFile(paFileName);
        if ( !loFile.open( QIODevice::ReadOnly ) ) 
		{
            QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> Cound not open file '" + paFileName + "' ...");
            paReturnMessage = loTemp;
            return false;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) 
		{
            QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> Cound not parse file '" + paFileName + "' ...");
            paReturnMessage = loTemp;
            loFile.close();
            return false;
        }

       QDomElement loDocElem = loDomDocument.documentElement();
	   
	   
        // Get element TABLES
       QDomNode loTablesNode = loDocElem.firstChild();
       while( !loTablesNode.isNull() ) 
	   {
			if(loTablesNode.nodeName() == "ROUTINES")
			{
				QDomElement loTablesElem = loTablesNode.toElement();
				if (!loTablesElem.isNull())
				{
					//Get elemnt ROUTINE
					int j = 1;
					QDomNode loTableNode = loTablesElem.firstChild();

					//For each ROUTINE
					while(!loTableNode.isNull())
					{
						//Read the ROUTINE
						QDomElement loRoutineElem = loTableNode.toElement();
						if(!loRoutineElem.isNull())
						{
							/////////////////////////////
							//Inster create ROUTINE
							/////////////////////////////
							QString loRoutineAlias = loRoutineElem.attribute("alias");
							if (loRoutineAlias.isEmpty() || loRoutineAlias.compare(QString("")) == 0)
							{
								QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> routine tag [" +  QString("%1").arg(j) + "]  no alias for node ...");
								paReturnMessage = loTemp;
								return false;
							}

							QString loRoutineSource = loRoutineElem.attribute("source");
							if (loRoutineSource.isEmpty() || loRoutineSource.compare(QString("")) == 0)
							{
								QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> routine tag [" +  QString("%1").arg(j) + "]  no source for node ...");
								paReturnMessage = loTemp;
								return false;
							}

							QString loRoutineSourceName = loRoutineElem.attribute("sourcename");
							if (loRoutineSourceName.isEmpty() || loRoutineSourceName.compare(QString("")) == 0)
							{
								QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> routine tag [" +  QString("%1").arg(j) + "]  no sourcename for node ...");
								paReturnMessage = loTemp;
								return false;
							}						

							QString loRoutineReturnType = loRoutineElem.attribute("methodname");
							if (loRoutineReturnType.isEmpty() || loRoutineReturnType.compare(QString("")) == 0)
							{
								QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> routine tag [" +  QString("%1").arg(j) + "]  no methodName for node  ...");
								paReturnMessage = loTemp;
								return false;
							}
						
							QString loRoutineName = loRoutineElem.attribute("name");
							if (loRoutineName.isEmpty() || loRoutineName.compare(QString("")) == 0)
							{
								QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> routine tag [" +  QString("%1").arg(j) + "] has wrong or no name  ...");
								paReturnMessage = loTemp;
								return false;
							}
							else
							{
								//Getting the objects nodes
								QDomNode loPartNode = loRoutineElem.firstChild();
								int loNodeCount;
								loNodeCount = 1;
							
								vector <QString> loRoutineObjects;
							
								while(!loPartNode.isNull())
								{

									if (loPartNode.nodeName().compare(QString("OBJECT")) == 0)
									{
										QDomElement loPartElement = loPartNode.toElement();
										if (!loPartElement.isNull())
										{
                                            /////////////////////////////
                                            //Inster create column
                                            /////////////////////////////
                                            QString loRoutineObject = loPartElement.attribute("name");
                                            if (loRoutineObject.isEmpty() || loRoutineObject.compare(QString("")) == 0)
                                            {
                                                QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> object tag [" +  QString("%1").arg(j) + "]  no name for node [" + QString("%1").arg(loNodeCount) + "] ...");
                                                paReturnMessage = loTemp;
                                                return false;
                                            }
											loRoutineObjects.push_back(loRoutineObject);
										}
                                    }
                                    else
                                    {
                                        QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> routine name [" + QString(loRoutineName) + "] has error in node to element from node[" + QString("%1").arg(loNodeCount) + "]  ...");
                                        paReturnMessage = loTemp;
                                        return false;
                                    }
									
									loNodeCount = loNodeCount + 1;
									loPartNode = loPartNode.nextSibling();
                                }
								
								//Create the class and add it to the vector
                                clDatabaseRoutine loDatabaseRoutine(loRoutineName,loRoutineAlias,loRoutineSource,loRoutineSourceName,loRoutineReturnType, loRoutineObjects);

                                //Check if the column exists
                                if (checkIfRoutineAlreadyExists(loDatabaseRoutine,paDatabaseRoutines))
                                {
                                    QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> routine tag [" + loRoutineName + "] definition already exists ...");
                                    paReturnMessage = loTemp;
                                    return false;
                                }
								
                                paDatabaseRoutines.push_back(loDatabaseRoutine);								
								

                            }							
							
						}
						j = j + 1;
						loTableNode = loTableNode.nextSibling();					
					}
										
				}
			} 
			if(loTablesNode.nodeName() == "TABLES")
			{
				QDomElement loTablesElem = loTablesNode.toElement();
				if (!loTablesElem.isNull())
				{
					//Get elemnt TABLE
					int i = 1;
					QDomNode loTableNode = loTablesElem.firstChild();

					//For each TABLE
					while(!loTableNode.isNull())
					{
						//Local vars of table
						QString loDatabaseName = QString("");
						vector <clDatabaseColumn> loDatabaseColumns;
						vector <clDatabaseAction> loDatabaseActions;


						//Read the TABLE
						QDomElement loTableElem = loTableNode.toElement();
						if(!loTableElem.isNull())
						{
							//Getting the table name
							QString loTableName = loTableElem.attribute("name");

							if (loTableName.isEmpty() || loTableName.compare(QString("")) == 0)
							{
								QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> table tag [" +  QString("%1").arg(i) + "] has wrong or no name  ...");
								paReturnMessage = loTemp;
								return false;
							}
							else
							{

								//Getting table nodes
								QDomNode loPartNode = loTableElem.firstChild();
								int loNodeCount;
								loNodeCount = 1;
								while(!loPartNode.isNull())
								{
									//COLUMN NODE



									/*
									QByteArray loBa1 = loPartNode.nodeName().toLatin1();
									char *loArgument1 = loBa1.data();
									printf(" this is the test '%s'",loArgument1);
									*/


									if (loPartNode.nodeName().compare(QString("COLUMN")) == 0)
									{
										QDomElement loPartElement = loPartNode.toElement();
										if (!loPartElement.isNull())
										{
												/////////////////////////////
												//Inster create column
												/////////////////////////////
												QString loColumnName = loPartElement.attribute("name");
												if (loColumnName.isEmpty() || loColumnName.compare(QString("")) == 0)
												{
													QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> table tag [" +  QString("%1").arg(i) + "]  no name for node [" + QString("%1").arg(loNodeCount) + "] ...");
													paReturnMessage = loTemp;
													return false;
												}


												QString loColumnAlias = loPartElement.attribute("alias");
												if (loColumnAlias.isEmpty() || loColumnAlias.compare(QString("")) == 0)
												{
													QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> table tag [" +  QString("%1").arg(i) + "]  no alias for node [" + QString("%1").arg(loNodeCount) + "] ...");
													paReturnMessage = loTemp;
													return false;
												}

												QString loColumnType = loPartElement.attribute("type");
												if (loColumnType.isEmpty() || loColumnType.compare(QString("")) == 0)
												{
													QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> table tag [" +  QString("%1").arg(i) + "]  no type for node [" + QString("%1").arg(loNodeCount) + "] ...");
													paReturnMessage = loTemp;
													return false;
												}

												QString loColumnExtra = loPartElement.attribute("extra");

												QString loColumnReference = loPartElement.attribute("reference");
												if (loColumnReference.isEmpty() || loColumnReference.compare(QString("")) == 0)
												{
													QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> table tag [" +  QString("%1").arg(i) + "]  no reference for node [" + QString("%1").arg(loNodeCount) + "] ...");
													paReturnMessage = loTemp;
													return false;
												}
												//Create the class and add it to the vector
												clDatabaseColumn loDatabaseColumn(loColumnName,loColumnAlias,loColumnType,loColumnExtra,loColumnReference);

												//Check if the column exists
												if (checkIfColumnAlreadyExists(loDatabaseColumn,loDatabaseColumns))
												{
													QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> table tag [" + loTableName + "] nr. [" +  QString("%1").arg(i) + "]  for node [" + loColumnName + "] nr. [" + QString("%1").arg(loNodeCount) + "] definition already exists ...");
													paReturnMessage = loTemp;
													return false;
												}

												loDatabaseColumns.push_back(loDatabaseColumn);
										}
										else
										{
											QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> table name [" + QString(loTableName) + "] has error in node to element from node[" + QString("%1").arg(loNodeCount) + "]  ...");
											paReturnMessage = loTemp;
											return false;
										}
									}
									else
									{
										QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> table name [" + QString(loTableName) + "] has wrong node name [" + QString("%1").arg(loNodeCount) + "]  ...");
										paReturnMessage = loTemp;
										return false;
									}


									loNodeCount = loNodeCount + 1;
									loPartNode = loPartNode.nextSibling();
								}
							}

							clDatabaseTable loDatabaseTable(loTableName,loDatabaseColumns,loDatabaseActions);

							if(checkIfTableAlreadyExists(loDatabaseTable, paDatabaseTables))
							{
								QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> table [" + loTableName + "] tag [" +  QString("%1").arg(i) + "]  definition already exists ...");
								paReturnMessage = loTemp;
								return false;
							}
							
							paDatabaseTables.push_back(loDatabaseTable);
						}
						i = i + 1;
						loTableNode = loTableNode.nextSibling();						
                    }
                }
            }
            loTablesNode = loTablesNode.nextSibling();
        }

        QString loTempLast("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> readed database structure ...");
        paReturnMessage = loTempLast;
        return true;
		
	}
    catch(...)
    {
        QString loTemp("clXMLReader::readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, QString &paReturnMessage) -> error ...");
        paReturnMessage = loTemp;
        return false;
    }
}

/******************************
* Getters and setters
*******************************/
void clXMLReader::setFileName(QString paFileName){meFileName = paFileName;}
QString clXMLReader::getFileName(){return meFileName;}

