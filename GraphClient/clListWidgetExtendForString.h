#ifndef CLLISTWIDGETEXTENDFORSTRING_H
#define CLLISTWIDGETEXTENDFORSTRING_H

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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QDateTimeEdit>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/qtalgorithms>
#include <QtCore/QSignalMapper>





#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clDatabaseColumn.h"
#include "clChangePropertieTextArray.h"


using namespace std;

class clListWidgetExtendForString : public QWidget
{

    Q_OBJECT;

public:

    clListWidgetExtendForString::clListWidgetExtendForString(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <clDatabaseColumn> paDatabaseColumns, clDatabaseColumn paDatabaseColumn, QWidget* paParent = 0, const char* paName = 0);
    ~clListWidgetExtendForString ();

    enum { NumGridRows = 200, NumButtons = 4 };
    QLabel *meLabels[NumGridRows];
	QDateTimeEdit *meDateTimeEdit[NumGridRows];
	QListWidget *meListWidget;
    QLineEdit *meLineEdits[NumGridRows];
    QGroupBox *meGridGroupBox;
	QHBoxLayout *meHBoxLayout[NumButtons];
    QPushButton *meButtons[NumButtons];
    QVBoxLayout *meMainLayout;
    QString *meValues[NumButtons];

public slots:
    //Slots Camera setup
    void slotButtonDeletePressed();
    void slotButtonAddPressed();
    void slotButtonMoveUp();
    void slotButtonMoveDown();	
private:
    void createGridGroupBox();
    void initializeElements();

    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;
    vector <clDatabaseColumn> meDatabaseColumns;
	clDatabaseColumn meDatabaseColumn;
};

#endif
