#define INFO_BUFFER_SIZE 32767
#include "clListWidgetExtendForString.h"

clListWidgetExtendForString::clListWidgetExtendForString(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <clDatabaseColumn> paDatabaseColumns, clDatabaseColumn paDatabaseColumn, QWidget* paParent, const char* paName): meDatabaseColumn(paDatabaseColumn)
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
clListWidgetExtendForString::~clListWidgetExtendForString()
{
}
void clListWidgetExtendForString::initializeElements()
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
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clListWidgetExtendForString::initializeElements" + QString(e.what()));
    }
}


 void clListWidgetExtendForString::createGridGroupBox()
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


void clListWidgetExtendForString::slotButtonDeletePressed()
{
    try
    {
		meListWidget->takeItem(meListWidget->currentRow());
    }
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clListWidgetExtendForString::slotButtonDeletePressed() -> " + QString(e.what()));
    }
}

void clListWidgetExtendForString::slotButtonAddPressed()
{
    try
    {
		clChangePropertieTextArray *loChangePropertieTextArray = new clChangePropertieTextArray(meIceClientServer,meIceClientLogging,meDatabaseColumn);
		
		
		if (loChangePropertieTextArray->exec() == 0)
        {
            QString loReturnMessage;
			meListWidget->addItem(QString(loChangePropertieTextArray->meValue));
														
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clListWidgetExtendForString::slotButtonAddPressed() -> " + loReturnMessage));
        }
        else
        {
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clListWidgetExtendForString::slotButtonAddPressed() -> insert canceled");
        }
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clListWidgetExtendForString::slotButtonCancelPressed() -> " + QString(e.what()));
    }
}
void clListWidgetExtendForString::slotButtonMoveUp()
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
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clListWidgetExtendForObject::slotButtonMoveUp() -> " + QString(e.what()));
    }
}

void clListWidgetExtendForString::slotButtonMoveDown()
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
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clListWidgetExtendForObject::slotButtonMoveDown() -> " + QString(e.what()));
    }
}

