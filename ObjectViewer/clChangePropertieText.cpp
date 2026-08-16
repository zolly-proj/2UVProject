#define INFO_BUFFER_SIZE 32767
#include "clChangePropertieText.h"

clChangePropertieText::clChangePropertieText(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clObjectListItem *paObjectListItem, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;
    meObjectListItem = paObjectListItem;

    meChangePropertieText.setupUi(this);

    meChangePropertieText.txtValue->setText(meObjectListItem->getValue());
    meChangePropertieText.lblPropertieName->setText(meObjectListItem->getName());

    connect(meChangePropertieText.btnOK, SIGNAL(clicked()),this,SLOT(slotButtonOKPressed()));
    connect(meChangePropertieText.btnCancel, SIGNAL(clicked()),this,SLOT(slotButtonCancelPressed()));
}
clChangePropertieText::~clChangePropertieText()
{
}

void clChangePropertieText::slotButtonOKPressed()
{
    try
    {
        meObjectListItem->setValue(meChangePropertieText.txtValue->text());
        this->done(0);
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clChangePropertieText::slotButtonOKPressed() -> " + QString(e.what()));
    }
}

void clChangePropertieText::slotButtonCancelPressed()
{
    try
    {
        this->done(0);
    }
    catch(exception &e)
    {
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clChangePropertieText::slotButtonCancelPressed() -> " + QString(e.what()));
    }
}


