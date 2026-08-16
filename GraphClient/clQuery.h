#ifndef CLQUERY_H_WVD
#define CLQUERY_H_WVD

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>


using namespace std;


class clQuery
{

public:
    clQuery (QString paClass, QString paProperty, QString paType, int paIndex);
    ~clQuery ();
public:
    //public database functions
    QString getClass();
    QString getProperty();
    QString getType();
    int getIndex();
private:
    QString meClass;
    QString meProperty;
    QString meType;
    int meIndex;


    void setClass(QString paClass);
    void setProperty(QString paProperty);
    void setType(QString paType);
    void setIndex(int paIndex);
};

#endif
