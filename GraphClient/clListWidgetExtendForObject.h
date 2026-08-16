#ifndef CLLISTWIDGETEXTENDFOROBJECT_H
#define CLLISTWIDGETEXTENDFOROBJECT_H

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
#include <QtXml/QDomElement>
#include <QtNetwork/QHostInfo>





#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clDatabaseColumn.h"
#include "clListWidgetSelectObject.h"


using namespace std;

class clListWidgetExtendForObject : public QWidget
{

    Q_OBJECT;

public:

    clListWidgetExtendForObject(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <clDatabaseColumn> paDatabaseColumns, clDatabaseColumn paDatabaseColumn, QWidget* paParent = 0, const char* paName = 0);
    ~clListWidgetExtendForObject ();

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
