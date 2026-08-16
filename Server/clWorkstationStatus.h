#ifndef CLWORSTATIONSTATUS_H_WVD_16122023
#define CLWORSTATIONSTATUS_H_WVD_16122023

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>
#include <QtCore/QDateTime>


using namespace std;


class clWorkstationStatus
{

public:
    clWorkstationStatus (QString paWorkstationName, QString paPercentageMemUsage, QString paPercentageDiskUsage, QString paPercentageCpuUsage, QDateTime paDateTime, QString paReturnType);
    ~clWorkstationStatus ();
public:
    //public database functions
    QString getWorkstationName();
	QString getPercentageMemUsage();
    QString getPercentageDiskUsage();
    QString getPercentageCpuUsage();
	QDateTime getTimeStamp();
    QString getReturnType();
private:
    QString meWorkstationName;
    QString mePercentageMemUsage;
    QString mePercentageDiskUsage;
    QString mePercentageCpuUsage;
	QDateTime meTimeStamp;
    QString meReturnType;

    void setWorkstationName(QString paName);
    void setPercentageMemUsage(QString paAlias);
    void setPercentageDiskUsage(QString paSource);
    void setPercentageCpuUsage(QString paSourceName);
	void setTimeStamp(QDateTime paTimeStamp);
    void setReturnType(QString paReturnType);
};
#endif
