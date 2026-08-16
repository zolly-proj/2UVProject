#ifndef CLEDITOBJECTUI_H
#define CLEDITOBJECTUI_H

#include <exception>
#include <string>
#include <iostream>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QScrollArea>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QSignalMapper>
#include <QtNetwork/QHostInfo>
#include <QtCore/QStorageInfo>





#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clDatabaseColumn.h"
#include "clListWidgetExtendForObject.h"
#include "clWidgetExtendForObject.h"
#include "clListWidgetExtendForString.h"
#include "clComboBoxExtend.h"


using namespace std;

class clEditObjectUI : public QDialog
{

    Q_OBJECT;

public:

    clEditObjectUI(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <clDatabaseColumn> paDatabaseColumns, vector <std::string> paValues, QWidget* paParent = 0, const char* paName = 0);
    ~clEditObjectUI ();

    enum { NumGridRows = 200, NumButtons = 2 };
    QLabel *meLabels[NumGridRows];
	QDateTimeEdit *meDateTimeEdit[NumGridRows];
	clListWidgetExtendForObject *meListWidgetExtendObject[NumGridRows];
	clWidgetExtendForObject *meWidgetExtendObject[NumGridRows];
	clListWidgetExtendForString *meListWidgetExtendString[NumGridRows];
    QLineEdit *meLineEdits[NumGridRows];
	clComboBoxExtend *meComboBox[NumGridRows];

    QGroupBox *meGridGroupBox;
    QPushButton *meButtons[NumButtons];
    QVBoxLayout *meMainLayout;
    QString *meValues[NumGridRows];
	
	QWidget *viewport;
	QScrollArea *scrollArea;
	QGridLayout *layout;	
public slots:
    //Slots Camera setup
    void slotButtonOKPressed();
    void slotButtonCancelPressed();
private:
    void createGridGroupBox(vector <std::string> paValues);
    void initializeElements();

    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;
    vector <clDatabaseColumn> meDatabaseColumns;


};

#endif
