#include "clDatabaseRoutine.h"

clDatabaseRoutine::clDatabaseRoutine(QString paName, QString paAlias, QString paSource, QString paSourceName, QString paReturnType, vector <QString> paObjects)
{
    setName(paName);
    setAlias(paAlias);
    setSource(paSource);
    setSourceName(paSourceName);
    setReturnType(paReturnType);
	setObjects(paObjects);
}

clDatabaseRoutine::~clDatabaseRoutine()
{

}

//Setters
void clDatabaseRoutine::setName(QString paName){ meName= paName;}
void clDatabaseRoutine::setAlias(QString paAlias){ meAlias = paAlias;}
void clDatabaseRoutine::setSource(QString paSource){meSource = paSource;}
void clDatabaseRoutine::setSourceName(QString paSourceName){meSourceName = paSourceName;}
void clDatabaseRoutine::setReturnType(QString paReturnType){meReturnType = paReturnType;}
void clDatabaseRoutine::setObjects(vector <QString> paObjects){meObjects = paObjects;}

//Getters
QString clDatabaseRoutine::getName(){return meName;}
QString clDatabaseRoutine::getAlias(){return meAlias;}
QString clDatabaseRoutine::getSource(){return meSource;}
QString clDatabaseRoutine::getSourceName(){return meSourceName;}
QString clDatabaseRoutine::getReturnType(){return meReturnType;}
vector <QString> clDatabaseRoutine::getObjects(){return meObjects;}
