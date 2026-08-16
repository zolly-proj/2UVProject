#define INFO_BUFFER_SIZE 32767
#include "clCreateQrCodeOfObject.h"
clCreateQrCodeOfObject::clCreateQrCodeOfObject()
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
        printf("clPauseForMachine::clContinueForMachine(vector <QString> paParameters) -> error ...");
    }
}
clCreateQrCodeOfObject::~clCreateQrCodeOfObject ()
{
	try
	{
	}
	catch(...)
	{
		
	}
}
int clCreateQrCodeOfObject::GetReturnParameters()
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

bool clCreateQrCodeOfObject::doMethod(const vector <QString> &paParametersType, const vector <QString> &paParameters)
{
	try
	{
			
		QString loTableName = QString(paParameters.at(0));
		QString loObjectID = QString(paParameters.at(1));
		
		
		//Create qr code
		QrCodeGenerator generator;
		QString data = QString(loTableName + "$;$" + loObjectID);
		QImage qrCodeImage = generator.generateQr(data);
		
		//Check if the file exists
		QFileInfo loCheck_file(QString("./QrCodes/" + loTableName + "_" + loObjectID + ".png"));    
		if (loCheck_file.exists() && loCheck_file.isFile()) 
		{ 
			//Delete the file
			QFile loFile (QString("./QrCodes/" + loTableName + "_" + loObjectID + ".png"));
			loFile.remove();
		}
	
		//Save qr code
		qrCodeImage.save(QString("./QrCodes/" + loTableName + "_" + loObjectID + ".png"), "PNG", 100);
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clCreateQrCodeOfObject::doMethod-> qr code created: ./QrCodes/" + loTableName + "_" + loObjectID + ".png"));
		return true;
	}
	catch(...)
	{
		return false;
	}
}
bool clCreateQrCodeOfObject::createPluginClass( clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging)	
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
