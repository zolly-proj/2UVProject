#ifndef CLLISTWIDGETSELECTOBJECT_H
#define CLLISTWIDGETSELECTOBJECT_H

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
#include <QtWidgets/QTreeWidget>
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


using namespace std;

class clListWidgetSelectObject : public QDialog
{

    Q_OBJECT;

public:

    clListWidgetSelectObject::clListWidgetSelectObject(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <clDatabaseColumn> paDatabaseColumns, clDatabaseColumn paDatabaseColumn, QWidget* paParent = 0, const char* paName = 0);
    ~clListWidgetSelectObject();

    enum { NumGridRows = 200, NumButtons = 2 };
	
	QTreeWidget *meTreeWidget;
	
    QLabel *meLabels[NumGridRows];
	QDateTimeEdit *meDateTimeEdit[NumGridRows];
	QListWidget *meListWidget[NumGridRows];
    QLineEdit *meLineEdits[NumGridRows];
    QGroupBox *meGridGroupBox;
    QPushButton *meButtons[NumButtons];
    QVBoxLayout *meMainLayout;
    QString *meValues[NumButtons];

public slots:
    //Slots Camera setup
    void slotButtonOKPressed();
    void slotButtonCancelPressed();
private:
    void createGridGroupBox();
    void initializeElements();
	QList<QStandardItem *> prepareRow(const QString &first, const QString &second, const QString &third);

    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;
    vector <clDatabaseColumn> meDatabaseColumns;
	clDatabaseColumn meDatabaseColumn;


};

#endif
