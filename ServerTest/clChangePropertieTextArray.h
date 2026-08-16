#ifndef CLCHANGEPROPERTIETEXTARRAY_H
#define CLCHANGEPROPERTIETEXTARRAY_H

#include <exception>
#include <string>
#include <iostream>

#include <QtCore/QString>
#include <QtWidgets/QDialog>
#include <QtNetwork/QHostInfo>




#include "clIceClientLogging.h"
#include "wdChangePropertieText.h"
#include "clIceClientServer.h"
#include "clDatabaseColumn.h"


using namespace std;

class clChangePropertieTextArray : public QDialog
{

    Q_OBJECT;

public:

    clChangePropertieTextArray(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clDatabaseColumn paDatabaseColumn, QWidget* paParent = 0, const char* paName = 0);
    ~clChangePropertieTextArray ();

    Ui::wdChangePropertieText meChangePropertieText;
    //QThread meThread;
	QString meValue;
public slots:
    //Slots Camera setup
    void slotButtonOKPressed();
    void slotButtonCancelPressed();
private:
    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;
	clDatabaseColumn meDatabaseColumn;

    
};

#endif
