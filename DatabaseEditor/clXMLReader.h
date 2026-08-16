#ifndef CLXMLREADER_H_WVD_17052011
#define CLXMLREADER_H_WVD_17052011

#include <vector>

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>

//Include the XML structure files
#include "clDatabaseTable.h"
#include "clDatabaseColumn.h"
#include "clDatabaseAction.h"
#include "clDatabaseRoutine.h"

using namespace std;


class clXMLReader
{

public:
    clXMLReader (QString paFileName);
    ~clXMLReader ();
public:
    QString getFileName();
    bool getAttributeFromConfigFile(QString paAttribute, QString & paValue, QString & paReturnMessage);
    bool readDatabaseConfiguration(QString paFileName, vector<clDatabaseTable> & paDatabaseTable, vector <clDatabaseRoutine> & paDatabaseRoutines, QString &paReturnMessage);

private:
    //Local functions
    bool checkIfColumnAlreadyExists(clDatabaseColumn paDatabaseColumn, vector <clDatabaseColumn> paDatabaseColumns);
    bool checkIfRoutineAlreadyExists(clDatabaseRoutine paDatabaseRoutine, vector <clDatabaseRoutine> paDatabaseRoutines);
    bool checkIfTableAlreadyExists(clDatabaseTable paDatabaseTable, vector <clDatabaseTable> paDatabaseTables);
    //Private local functions
    void setFileName(QString paFileName);

    //Private local variables
    QString meFileName;
};

#endif
