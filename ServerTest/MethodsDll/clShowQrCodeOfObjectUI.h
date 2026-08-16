#ifndef CLSHOWQRCODEOFOBJECTUI_H
#define CLSHOWQRCODEOFOBJECTUI_H

#include <exception>
#include <string>
#include <iostream>

#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileDialog>
#include <QtNetwork/QHostInfo>





#include "clIceClientLogging.h"
#include "clIceClientServer.h"

using namespace std;

class clShowQrCodeOfObjectUI : public QDialog
{

    Q_OBJECT;

public:

    clShowQrCodeOfObjectUI(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QWidget* paParent = 0, const char* paName = 0);
    ~clShowQrCodeOfObjectUI ();

    enum { NumGridRows = 200, NumButtons = 2 };
    QLabel *meLabels[NumGridRows];
    QPushButton *meButtons[NumButtons];
	
	QVBoxLayout *meMainLayout;
	QImage *meImage;
	
public slots:
    //Slots Camera setup
    void slotButtonSaveAssPressed();
    void slotButtonCancelPressed();
private:
    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;
	
};

#endif
