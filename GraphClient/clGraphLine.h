#ifndef CLGRAPHLINE_H_WVD
#define CLGRAPHLINE_H_WVD

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>

#include "clQuery.h"

using namespace std;


class clGraphLine
{

public:
	clGraphLine ();
    clGraphLine (QString paColor, QString paName, vector<clQuery> paQuery_X, vector<clQuery> paQuery_Y);
    ~clGraphLine ();
public:
    //public database functions
    QString getName();
    QString getColor();
	vector <clQuery> getQuery_X();
	vector <clQuery> getQuery_Y();
	
    void setName(QString paName);
    void setColor(QString paColor);
    void setQuery_X(vector <clQuery> paQuery_X);	
	void setQuery_Y(vector <clQuery> paQuery_Y);	
private:
    QString meName;
    QString meColor;
	vector <clQuery> meQuery_X;
	vector <clQuery> meQuery_Y;
};

#endif
