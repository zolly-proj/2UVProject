#define INFO_BUFFER_SIZE 32767
#include "clListWidgetExtendForObject.h"

clListWidgetExtendForObject::clListWidgetExtendForObject(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <clDatabaseColumn> paDatabaseColumns, clDatabaseColumn paDatabaseColumn, QWidget* paParent, const char* paName): meDatabaseColumn(paDatabaseColumn)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;
    meDatabaseColumns = paDatabaseColumns;
	

    initializeElements();
    createGridGroupBox();

    meMainLayout = new QVBoxLayout();
    meMainLayout->addWidget(meGridGroupBox);
    setLayout(meMainLayout);


}
clListWidgetExtendForObject::~clListWidgetExtendForObject()
{
}
void clListWidgetExtendForObject::initializeElements()
{
    try
    {

        //for(int i = 0; i < NumGridRows; i++)
        //{
        //    meLineEdits[i] = NULL;
        //}

    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);			
		
		
        meIceClientLogging->insertItem("10"QString(QHostInfo::localHostName()),"2UVServerTest.exe","clListWidgetExtendForObject::initializeElements" + QString(e.what()));
    }
}


 void clListWidgetExtendForObject::createGridGroupBox()
 {
    meGridGroupBox = new QGroupBox(tr("Objects"));
    QGridLayout *layout = new QGridLayout;
	
	meHBoxLayout[0] = new QHBoxLayout;
	meHBoxLayout[1] = new QHBoxLayout;
	layout->addLayout(meHBoxLayout[0], 0, 0);
    layout->addLayout(meHBoxLayout[1], 0, 1);
	
	meButtons[0] = new QPushButton(QString("Up"));
	meButtons[1] = new QPushButton(QString("Down"));
	meButtons[2] = new QPushButton(QString("Delete"));
	meButtons[3] = new QPushButton(QString("Add"));

	connect(meButtons[0], SIGNAL(clicked()), this, SLOT(slotButtonMoveUp()));
    connect(meButtons[1], SIGNAL(clicked()), this, SLOT(slotButtonMoveDown()));	 
	connect(meButtons[2], SIGNAL(clicked()), this, SLOT(slotButtonDeletePressed()));
    connect(meButtons[3], SIGNAL(clicked()), this, SLOT(slotButtonAddPressed()));
	meHBoxLayout[0]->addWidget(meButtons[0]);
    meHBoxLayout[0]->addWidget(meButtons[1]);
    meHBoxLayout[1]->addWidget(meButtons[2]);
    meHBoxLayout[1]->addWidget(meButtons[3]);
	 
	 meListWidget = new QListWidget;
	 meListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	 layout->addWidget(meListWidget, 1,0);

     meGridGroupBox->setLayout(layout);
 }


void clListWidgetExtendForObject::slotButtonDeletePressed()
{
    try
    {
		meListWidget->takeItem(meListWidget->currentRow());
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);			
		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clListWidgetExtendForObject::slotButtonDeletePressed() -> " + QString(e.what()));
    }
}

void clListWidgetExtendForObject::slotButtonAddPressed()
{
    try
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);	
		
		
        //clListWidgetSelectObject::clListWidgetSelectObject(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <clDatabaseColumn> paDatabaseColumns, QWidget* paParent = 0, const char* paName = 0);
		clListWidgetSelectObject *loListWidgetSelectObject = new clListWidgetSelectObject(meIceClientServer, meIceClientLogging, meDatabaseColumns, meDatabaseColumn);
		if (loListWidgetSelectObject->exec() == 0)
        {
            QString loReturnMessage;
			//cout << loListWidgetSelectObject->meValues[0]->toStdString() << endl;
            //meListWidget->addItem(QString(static_cast<char *>(loListWidgetSelectObject->meValues[0]->toStdString())));
			meListWidget->addItem(QString(loListWidgetSelectObject->meValues[0]->toStdString().c_str()));
														
            meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe",QString("clListWidgetExtendForObject::slotButtonAddPressed() -> " + loReturnMessage));
        }
        else
        {
			// Get and display the name of the computer.
			TCHAR infoBuf[INFO_BUFFER_SIZE];
			DWORD  bufCharCount = INFO_BUFFER_SIZE;
			GetComputerName(infoBuf,&bufCharCount);			
			
            meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clListWidgetExtendForObject::slotButtonAddPressed() -> insert canceled");
        }
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);			
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clListWidgetExtendForObject::slotButtonCancelPressed() -> " + QString(e.what()));
    }
}
void clListWidgetExtendForObject::slotButtonMoveUp()
{
    try
    {
		int loRow;
		loRow = meListWidget->currentRow();
		if (loRow != 0)
		{
			QListWidgetItem *loListWidgetItemTemp = meListWidget->takeItem(loRow);
			meListWidget->insertItem(loRow - 1, loListWidgetItemTemp);
		}		
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);			
		
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clListWidgetExtendForObject::slotButtonMoveUp() -> " + QString(e.what()));
    }
}

void clListWidgetExtendForObject::slotButtonMoveDown()
{
    try
    {
		int loRow;
		loRow = meListWidget->currentRow();
		if (meListWidget->count() > 1)
		{
			QListWidgetItem *loListWidgetItemTemp = meListWidget->takeItem(loRow);
			meListWidget->insertItem(loRow + 1, loListWidgetItemTemp);
		}
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);			
		
        meIceClientLogging->insertItem("10",QString(infoBuf),"2UVServerTest.exe","clListWidgetExtendForObject::slotButtonMoveDown() -> " + QString(e.what()));
    }
}

