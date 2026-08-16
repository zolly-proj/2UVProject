#define INFO_BUFFER_SIZE 32767
#include "clChangePropertieTextArray.h"

clChangePropertieTextArray::clChangePropertieTextArray(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clDatabaseColumn paDatabaseColumn, QWidget* paParent, const char* paName): meDatabaseColumn(paDatabaseColumn)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;
    
    meChangePropertieText.setupUi(this);


	meDatabaseColumn = paDatabaseColumn;

    meChangePropertieText.txtValue->setText(QString(""));
    meChangePropertieText.lblPropertieName->setText(meDatabaseColumn.getName());

    connect(meChangePropertieText.btnOK, SIGNAL(clicked()),this,SLOT(slotButtonOKPressed()));
    connect(meChangePropertieText.btnCancel, SIGNAL(clicked()),this,SLOT(slotButtonCancelPressed()));
}
clChangePropertieTextArray::~clChangePropertieTextArray()
{
}

void clChangePropertieTextArray::slotButtonOKPressed()
{
    try
    {
		meValue = meChangePropertieText.txtValue->text();
        this->done(0);
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clChangePropertieText::slotButtonOKPressed() -> " + QString(e.what()));
    }
}

void clChangePropertieTextArray::slotButtonCancelPressed()
{
    try
    {
        this->done(1);
    }
    catch(exception &e)
    {
		// Get and display the name of the computer.
		TCHAR infoBuf[INFO_BUFFER_SIZE];
		DWORD  bufCharCount = INFO_BUFFER_SIZE;
		GetComputerName(infoBuf,&bufCharCount);		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clChangePropertieText::slotButtonCancelPressed() -> " + QString(e.what()));
    }
}
