#define INFO_BUFFER_SIZE 32767
#include "clExportToExcel.h"
clExportToExcel::clExportToExcel()
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
        printf("clDriverFrame::clDriverFrame(vector <QString> paParameters) -> error ...");
    }
}
clExportToExcel::~clExportToExcel ()
{
}
int clExportToExcel::GetReturnParameters()
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

bool clExportToExcel::doMethod(const vector <QString> &paParametersType, const vector <QString> &paParameters)
{	
    try
    {
		//Getting all the tables
		
		
		meExportToExcelUI = new clExportToExcelUI(meIceClientServer,meIceClientLogging);
		meExportToExcelUI->meLabels[0]->setText("Select class ...");
		
		
		meExportToExcelUI->setAttribute(Qt::WA_DeleteOnClose);
		
        //*****************************
        //* Getting the tables *
        //********************************
        vector<std::string> loTables;
        QString loMessage;

        meIceClientServer->getAllTablesFromDatabase(loTables,loMessage);
        for(int i=0; i < loTables.size(); i++)
        {
            meExportToExcelUI->meComboBox[0]->addItem(QString(loTables[i].c_str()));
        }		
		
		
		if (meExportToExcelUI->exec() == 0)
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("Save excel"));
		else
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("Application closed no save"));
		}
		
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
bool clExportToExcel::createPluginClass( clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging)	
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

