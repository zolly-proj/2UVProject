#include "clDatabaseRow.h"

clDatabaseRow::clDatabaseRow(QString paTableName, vector <std::string> paValues, vector <std::string> paColumnName, vector <std::string> paAlias, vector <std::string> paType, vector <std::string> paExtra, vector <std::string> paReference)
							: meValues(paValues), meColumnName(paColumnName), meAlias(paAlias),meType(paType), meExtra(paExtra), meReference(paReference)
{
    setTableName(paTableName);
}

clDatabaseRow::~clDatabaseRow()
{

}

//Getters
QString clDatabaseRow::getTableName(){return meTableName;}
vector <std::string> clDatabaseRow::getValues(){return meValues;}
vector <std::string> clDatabaseRow::getColumnName(){return meColumnName;}
vector <std::string> clDatabaseRow::getAlias(){return meAlias;}
vector <std::string> clDatabaseRow::getType(){return meType;}
vector <std::string> clDatabaseRow::getExtra(){return meExtra;}
vector <std::string> clDatabaseRow::getReference(){return meReference;}
//Setters

void clDatabaseRow::setTableName(QString paTableName){meTableName = paTableName;}
void clDatabaseRow::setValues(vector <std::string> paValues){}
void clDatabaseRow::setColumnName(vector <std::string> paColumnName){}
void clDatabaseRow::setAlias(vector <std::string> paAlias){}
void clDatabaseRow::setType(vector <std::string> paType){}
void clDatabaseRow::setExtra(vector <std::string> paExtra){}
void clDatabaseRow::setReference(vector <std::string> paReference){}
