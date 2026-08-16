#ifndef CLCOMBOBOXEXTEND_H
#define CLCOMBOBOXEXTEND_H

#include <exception>
#include <string>
#include <iostream>

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>
#include <QtWidgets/QComboBox>
#include <QtNetwork/QHostInfo>






#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clDatabaseColumn.h"
#include "clChangePropertieTextArray.h"


using namespace std;

class clComboBoxExtend : public QComboBox
{

    Q_OBJECT;

public:
    clComboBoxExtend(clIceClientLogging * paIceClientLogging, clDatabaseColumn paDatabaseColumn, QWidget* paParent = 0);
    ~clComboBoxExtend ();
	
	QString getValue();
	bool setValue(QString paValue);
private:
    void initializeElements();
    clIceClientLogging * meIceClientLogging;
	clDatabaseColumn meDatabaseColumn;
};

#endif
