#ifndef CLCLASSINFO_H
#define CLCLASSINFO_H

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
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QScrollArea>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QTextStream>
#include <QtXml/QDomElement>
#include <QtNetwork/QHostInfo>

#include "clDatabaseColumn.h"
#include "clClassObjectTreeItem.h"


using namespace std;

class clClassInfo : public QDialog
{

    Q_OBJECT;

public:

    clClassInfo::clClassInfo(QString paTableName, vector <clDatabaseColumn> paDatabaseColumns, QWidget* paParent = 0, const char* paName = 0);
    ~clClassInfo();

    enum { NumGridRows = 50, NumButtons = 2 };
    QLabel *meLabels[NumGridRows];
	QTreeWidget *meTreeWidget;
	QPushButton *meButtons[NumButtons];
	
	QWidget *viewport;
	QScrollArea *scrollArea;
	QGridLayout *layout;
	QVBoxLayout *meMainLayout;	
	
public slots:
    //Slots Camera setup
    void slotButtonOKPressed();
private:
    void createGridGroupBox();
    void initializeElements();
	QString getRelatedIcon(QString paClassName);
	void readMappingIcons();
	
    vector <clDatabaseColumn> meDatabaseColumns;
	QString meTableName;
	vector <QString> meIcon;
	vector <QString> meClass;

};

#endif
