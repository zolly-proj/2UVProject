#ifndef CLDATABASEROW_H_WVD
#define CLDATABASEROW_H_WVD

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>


using namespace std;


class clDatabaseRow
{

public:
    clDatabaseRow (QString paTableName, vector <std::string> paValues, vector <std::string> paColumnName, vector <std::string> paAlias, vector <std::string> paType, vector <std::string> paExtra, vector <std::string> paReference);
    ~clDatabaseRow ();
public:
    //public database functions
	QString getTableName();
	vector <std::string> getValues();
    vector <std::string> getColumnName();
    vector <std::string> getAlias();
    vector <std::string> getType();
    vector <std::string> getExtra();
    vector <std::string> getReference();

private:
	QString meTableName;
	vector <std::string> meValues;
    vector <std::string> meColumnName;
    vector <std::string> meAlias;
    vector <std::string> meType;
    vector <std::string> meExtra;
    vector <std::string> meReference;

	void setTableName(QString paTableName);
	void setValues(vector <std::string> paValues);
    void setColumnName(vector <std::string> paColumnName);
    void setAlias(vector <std::string> paAlias);
    void setType(vector <std::string> paType);
    void setExtra(vector <std::string> paExtra);
    void setReference(vector <std::string> paReference);
};

#endif
