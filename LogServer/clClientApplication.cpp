#include "clClientApplication.h"

clClientApplication::clClientApplication(clIceClient & paIceClient,QWidget* paParent, const char* paName) : meIceClient( paIceClient)
{
    meClientLogger.setupUi(this);

    connect(meClientLogger.btnPing, SIGNAL(clicked()),this,SLOT(slotButtonPingPressed()));
    connect(meClientLogger.btnShutdown, SIGNAL(clicked()),this,SLOT(slotButtonShutdownPressed()));
    connect(meClientLogger.btnSendMessage, SIGNAL(clicked()),this,SLOT(slotButtonSendMessagePressed()));
}

clClientApplication::~clClientApplication()
{
}

void clClientApplication::slotButtonSendMessagePressed()
{
    try
    {
        meIceClient.insertItem(meClientLogger.txtLogNumber->text(),meClientLogger.txtComputerName->text(),QString("LogClient.exe"),meClientLogger.txtMessage->text());
    }
    catch(...)
    {
        printf("clClientApplication::slotButtonSendMessagePressed -> error ...");
    }

}

void clClientApplication::slotButtonPingPressed()
{
        try
        {
            if (meIceClient.Ping())
            {
                printf("clClientApplicaiton::slotButtonPressed -> server returned ping = online");
            }
            else
            {
                printf("clClientApplicaiton::slotButtonPressed -> server returned no ping = offline");
            }
        }
        catch(...)
        {
            printf("clClientApplication::slotButtonPingPressed -> error ...");
        }
}
void clClientApplication::slotButtonShutdownPressed()
{
    try
    {
        meIceClient.Shutdown();
    }
    catch(...)
    {
        printf("clClientApplication::slotButtonShutdownPressed -> error ...");
    }
}

