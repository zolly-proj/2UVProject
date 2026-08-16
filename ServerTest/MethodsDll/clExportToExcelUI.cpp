#define INFO_BUFFER_SIZE 32767
#include "clExportToExcelUI.h"

clExportToExcelUI::clExportToExcelUI(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;
    
    meMainLayout = new QVBoxLayout();
	
	meLabels[0] = new QLabel;
	meLabels[1] = new QLabel;

	meButtons[1] = new QPushButton(tr("Close"));
	meButtons[0] = new QPushButton(tr("Save ass"));
	connect(meButtons[0], SIGNAL(clicked()), this, SLOT(slotButtonSaveAssPressed()));
    connect(meButtons[1], SIGNAL(clicked()), this, SLOT(slotButtonCancelPressed()));
	
	meComboBox[0] = new QComboBox;
	
    meMainLayout->addWidget(meLabels[0]);
	meMainLayout->addWidget(meComboBox[0]);
	
	meMainLayout->addWidget(meButtons[0]);
	meMainLayout->addWidget(meButtons[1]);
    setLayout(meMainLayout);
	
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	//QApplication::setQuitOnLastWindowClosed(false);
}
clExportToExcelUI::~clExportToExcelUI()
{
}
void clExportToExcelUI::slotButtonSaveAssPressed()
{
	try
    {
        QString fname = QFileDialog::getSaveFileName(nullptr, "Save excel ass ...", ".", "Excel (*.csv)" );
		
		//Check if the file exists
		QFileInfo loCheck_file(fname);    
		if (loCheck_file.exists() && loCheck_file.isFile()) 
		{ 
			//Delete the file
			QFile loFile (fname);
			loFile.remove();
		}		
		
		meSaveAss = fname;
		
		
		QString loSelectedItem = meComboBox[0]->currentText();
		
		/*****************************************
		* Get the table info
		*******************************************/
		 vector<std::string> loPropertyName;
		 vector<std::string> loAlias;
		 vector<std::string> loType;
		 vector<std::string> loExtra;
		 vector<std::string> loReference;
		 QString loReturnMessageObject;


		 if (!meIceClientServer->getAllPropertiesFromTable(  	loSelectedItem,
																loPropertyName,
																loAlias,
																loType,
																loExtra,
																loReference,
																loReturnMessageObject))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clExportToExcelUI::slotButtonSaveAssPressed -> " + loReturnMessageObject);
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clExportToExcelUI::slotButtonSaveAssPressed -> " + loReturnMessageObject);

		
		/********************************
		* Getting all id's of the table *
		*********************************/
		vector<std::string> loReturnIds;
		vector<std::string> loReturnNames;
        QString loReturnMessage;
		
		QString loStop = QString("0");
		QString loStart = QString("0");

			
		if (!meIceClientServer->getFromTableDatabaseGeneral( 	loSelectedItem,
																loStart,
																loStop,
																loReturnIds,
																loReturnNames,
																loReturnMessage))
		{
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clExportToExcelUI::slotButtonSaveAssPressed -> " + loReturnMessage);
		}
		else
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clExportToExcelUI::slotButtonSaveAssPressed -> " + loReturnMessage);
		
		
		for (int i = 0; i < (int) loReturnIds.size();i++)
		{
			vector<std::string> loReturnValues;
			QString loReturnMessageGetById;
			
			QString loReturnId = QString(loReturnIds.at(i).c_str());

			if (!meIceClientServer->getFromTableDatabaseById(	loSelectedItem,
																loReturnId,
																loPropertyName,
																loReturnValues,
																loReturnMessageGetById))
			{
				meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clExportToExcel::slotButtonSaveAssPressed -> " + loReturnMessageGetById);
			}
			else
			{
				QFile data(meSaveAss);
				if (data.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) 
				{
					QTextStream out(&data);
					for(int j = 0; j < (int) loReturnValues.size();j++)
					{
						out << QString(loReturnValues.at(j).c_str());
						out << QString(";");
					}
					out << QChar((int)'\n');
				}
				data.close();
			}
		}		
		
		//Closing the application
		this->done(0);
		
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clExportToExcelUI::slotButtonSaveAssPressed() -> " + QString(e.what()));
    }
}

void clExportToExcelUI::slotButtonCancelPressed()
{
    try
    {	
	
        this->done(1);
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clExportToExcelUI::slotButtonCancelPressed() -> " + QString(e.what()));
    }
}


