#ifndef CLSETPRIORITYANDWORKSTEPUI_H
#define CLSETPRIORITYANDWORKSTEPUI_H

#include <exception>
#include <string>
#include <iostream>

#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollArea>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileDialog>
#include <QtCore/QTextStream>




#include "clDatabaseColumn.h"
#include "clComboBoxExtend.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clObjectCall.h"
#include "clMethodCall.h"
#include "clObject.h"
#include "clClassObjectTreeItem.h"


using namespace std;

class clProductSetPriorityAndWorkstepUI : public QDialog
{

    Q_OBJECT;

public:

    clProductSetPriorityAndWorkstepUI(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <vector<clDatabaseColumn *>> paDatabaseColumnsByClassNameList, QWidget* paParent = 0, const char* paName = 0);
    ~clProductSetPriorityAndWorkstepUI ();
	
	bool setContentOfUI(clObject *paProduct, clObject *paBatch, vector <clObject*> paWorksteps);
	
	
    enum { NumGridRows = 200, NumButtons = 2 };
    QLabel *meLabels[NumGridRows];
	QLineEdit *meLineEdits[NumGridRows];
	QTreeWidget *meTreeWidget;
	clComboBoxExtend *meComboBox[NumButtons];
    QPushButton *meButtons[NumButtons];
	
	QVBoxLayout *meMainLayout;
	QString meSaveAss;
	
	QWidget *viewport;
	QScrollArea *scrollArea;
	QGridLayout *layout;	
	
public slots:
    //Slots Camera setup
    void slotButtonSaveAssPressed();
    void slotButtonCancelPressed();
private:
    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;
	vector <vector<clDatabaseColumn *>> meDatabaseColumnsByClassNameList;
	
	clDatabaseColumn * getDatabaseColumn(QString paColumnName);
	
	clObject * meObjectProduct;
	clObject * meObjectBatch;
	vector <clObject*> meObjectsWs;
};

#endif
