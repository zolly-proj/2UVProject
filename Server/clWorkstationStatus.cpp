#include "clWorkstationStatus.h"

clWorkstationStatus::clWorkstationStatus(QString paWorkstationName, QString paPercentageMemUsage, QString paPercentageDiskUsage, QString paPercentageCpuUsage, QDateTime paDateTime, QString paReturnType)
{
    setWorkstationName(paWorkstationName);
    setPercentageMemUsage(paPercentageMemUsage);
    setPercentageDiskUsage(paPercentageDiskUsage);
    setPercentageCpuUsage(paPercentageCpuUsage);
	setTimeStamp(paDateTime);
    setReturnType(paReturnType);
}
clWorkstationStatus::~clWorkstationStatus()
{

}

//Setters
void clWorkstationStatus::setWorkstationName(QString paWorkstationName){ meWorkstationName= paWorkstationName;}
void clWorkstationStatus::setPercentageMemUsage(QString paPercentageMemUsage){ mePercentageMemUsage = paPercentageMemUsage;}
void clWorkstationStatus::setPercentageDiskUsage(QString paPercentageDiskUsage){mePercentageDiskUsage = paPercentageDiskUsage;}
void clWorkstationStatus::setPercentageCpuUsage(QString paPercentageCpuUsage){mePercentageCpuUsage = paPercentageCpuUsage;}
void clWorkstationStatus::setTimeStamp(QDateTime paTimeStamp){meTimeStamp = paTimeStamp;}
void clWorkstationStatus::setReturnType(QString paReturnType){meReturnType = paReturnType;}

//Getters
QString clWorkstationStatus::getWorkstationName(){return meWorkstationName;}
QString clWorkstationStatus::getPercentageMemUsage(){return mePercentageMemUsage;}
QString clWorkstationStatus::getPercentageDiskUsage(){return mePercentageDiskUsage;}
QString clWorkstationStatus::getPercentageCpuUsage(){return mePercentageCpuUsage;}
QDateTime clWorkstationStatus::getTimeStamp(){return meTimeStamp;}
QString clWorkstationStatus::getReturnType(){return meReturnType;}
