#define INFO_BUFFER_SIZE 32767
#include "clShowQrCodeOfObject.h"
clShowQrCodeOfObject::clShowQrCodeOfObject()
{
    try
    {		
		
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
    }	
    catch(...)
    {
        printf("clShowQrCodeOfObject::clShowQrCodeOfObject() -> error ...");
    }
}
clShowQrCodeOfObject::~clShowQrCodeOfObject ()
{
	try
	{
	}
	catch(...)
	{
		
	}
}
int clShowQrCodeOfObject::GetReturnParameters()
{
    try
    {
		return 0;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
			return -1;
    }	
    catch(...)
    {
		return -1;
    }
}

bool clShowQrCodeOfObject::doMethod(const vector <QString> &paParametersType, const vector <QString> &paParameters)
{
	try
	{
		//QApplication::setQuitOnLastWindowClosed(false);	
		QString loTableName = QString(paParameters.at(0));
		QString loObjectID = QString(paParameters.at(1));
		
		QrCodeGenerator generator;
		QString data = QString(loTableName + "$;$" + loObjectID);
		QImage qrCodeImage = generator.generateQr(data);
		
		meShowQrCodeOfObjectUI = new clShowQrCodeOfObjectUI(meIceClientServer,meIceClientLogging);
		meShowQrCodeOfObjectUI->meLabels[0]->setText(QString("./QrCodes/" + loTableName + "_" + loObjectID + ".png"));
		
		QPixmap * loPixmap = new QPixmap(QPixmap::fromImage(qrCodeImage));
		meShowQrCodeOfObjectUI->meLabels[1]->setPixmap(loPixmap->scaled(150,150));
		meShowQrCodeOfObjectUI->meImage = &qrCodeImage;
		
		meShowQrCodeOfObjectUI->setAttribute(Qt::WA_DeleteOnClose);
		
		if (meShowQrCodeOfObjectUI->exec() == 0)
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("Application closed"));
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("Application closed"));
			
		
		//qrCodeImage.save(QString("./QrCodes/" + loTableName + "_" + loObjectID + ".png"), "PNG", 100);
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clShowQrCodeOfObject::doMethod-> qr code is shown: ./QrCodes/" + loTableName + "_" + loObjectID + ".png"));
		return true;
	}
	catch(const std::exception& ex)
	{
		printf(ex.what());       
		return false;
	}
}
bool clShowQrCodeOfObject::createPluginClass( clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging)	
{
    try
    {
		meIceClientServer = paIceClientServer;
		meIceClientLogging = paIceClientLogging;
		return true;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
			return false;
    }	
    catch(...)
    {
		return false;
    }
}
