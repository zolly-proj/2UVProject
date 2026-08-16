#define INFO_BUFFER_SIZE 32767
#include "clWidgetExtendForObject.h"

clWidgetExtendForObject::clWidgetExtendForObject(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <clDatabaseColumn> paDatabaseColumns, clDatabaseColumn paDatabaseColumn, QWidget* paParent, const char* paName): meDatabaseColumn(paDatabaseColumn)
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
clWidgetExtendForObject::~clWidgetExtendForObject()
{
}
void clWidgetExtendForObject::initializeElements()
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
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::initializeElements" + QString(e.what()));
    }
}


 void clWidgetExtendForObject::createGridGroupBox()
 {
    meGridGroupBox = new QGroupBox(tr("Object"));
    QGridLayout *layout = new QGridLayout;
	
	meHBoxLayout[0] = new QHBoxLayout;
	meHBoxLayout[1] = new QHBoxLayout;
	layout->addLayout(meHBoxLayout[0], 0, 0);
    layout->addLayout(meHBoxLayout[1], 0, 1);

	meButtons[0] = new QPushButton(QString("Delete"));
	meButtons[1] = new QPushButton(QString("Add"));

	connect(meButtons[0], SIGNAL(clicked()), this, SLOT(slotButtonDeletePressed()));
    connect(meButtons[1], SIGNAL(clicked()), this, SLOT(slotButtonAddPressed()));
	meHBoxLayout[0]->addWidget(meButtons[0]);
    meHBoxLayout[0]->addWidget(meButtons[1]);
	 
	 meLineEdit = new QLineEdit;
	 layout->addWidget(meLineEdit, 1,0);

     meGridGroupBox->setLayout(layout);
 }


void clWidgetExtendForObject::slotButtonDeletePressed()
{
    try
    {
		meLineEdit->setText(QString(""));
    }
    catch(exception &e)
    {
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::slotButtonDeletePressed() -> " + QString(e.what()));
    }
}

void clWidgetExtendForObject::slotButtonAddPressed()
{	
    try
    {
        //clListWidgetSelectObject::clListWidgetSelectObject(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <clDatabaseColumn> paDatabaseColumns, QWidget* paParent = 0, const char* paName = 0);
		clListWidgetSelectObject *loListWidgetSelectObject = new clListWidgetSelectObject(meIceClientServer, meIceClientLogging, meDatabaseColumns, meDatabaseColumn);
		if (loListWidgetSelectObject->exec() == 0)
        {
            QString loReturnMessage;
			//cout << loListWidgetSelectObject->meValues[0]->toStdString() << endl;
            //meListWidget->addItem(QString(static_cast<char *>(loListWidgetSelectObject->meValues[0]->toStdString())));
			meLineEdit->setText(QString(loListWidgetSelectObject->meValues[0]->toStdString().c_str()));
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clWidgetExtendForObject::slotButtonAddPressed() -> " + loReturnMessage));
        }
        else
        {			
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::slotButtonAddPressed() -> insert canceled");
        }
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::slotButtonCancelPressed() -> " + QString(e.what()));
    }
}
