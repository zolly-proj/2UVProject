#ifndef CLEXPORTTOEXCELUI_H
#define CLEXPORTTOEXCELUI_H

#include <exception>
#include <string>
#include <iostream>

#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>





#include "clIceClientLogging.h"
#include "clIceClientServer.h"

using namespace std;

class clExportToExcelUI : public QDialog
{

    Q_OBJECT;

public:

    clExportToExcelUI(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QWidget* paParent = 0, const char* paName = 0);
    ~clExportToExcelUI ();

    enum { NumGridRows = 200, NumButtons = 2 };
    QLabel *meLabels[NumGridRows];
	QComboBox *meComboBox[NumButtons];
    QPushButton *meButtons[NumButtons];
	
	QVBoxLayout *meMainLayout;
	QString meSaveAss;
	
public slots:
    //Slots Camera setup
    void slotButtonSaveAssPressed();
    void slotButtonCancelPressed();
private:
    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;
	
};

#endif
