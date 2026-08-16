#ifndef CLCHANGEPROPERTIETEXT_H
#define CLCHANGEPROPERTIETEXT_H

#include <exception>
#include <string>
#include <iostream>

#include <QtCore/QString>
#include <QtWidgets/QDialog>
#include <QtCore/QStorageInfo>
#include <QtNetwork/QHostInfo>




#include "clIceClientLogging.h"
#include "wdChangePropertieText.h"
#include "clIceClientServer.h"
#include "clObjectListItem.h"


using namespace std;

class clChangePropertieText : public QDialog
{

    Q_OBJECT;

public:

    clChangePropertieText(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clObjectListItem *paObjectListItem, QWidget* paParent = 0, const char* paName = 0);
    ~clChangePropertieText ();

    Ui::wdChangePropertieText meChangePropertieText;
    //QThread meThread;

public slots:
    //Slots Camera setup
    void slotButtonOKPressed();
    void slotButtonCancelPressed();
private:
    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;
    clObjectListItem * meObjectListItem;
};

#endif
