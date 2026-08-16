#define INFO_BUFFER_SIZE 32767
#include "clEditObjectUI.h"

clEditObjectUI::clEditObjectUI(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, vector <clDatabaseColumn> paDatabaseColumns, vector <std::string> paValues, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;
    meDatabaseColumns = paDatabaseColumns;
	
    initializeElements();
	
	
    createGridGroupBox(paValues);

    meMainLayout = new QVBoxLayout();
	
	//Create a widget and set its layout as your new layout created above
	viewport = new QWidget;
	viewport->setLayout(layout);

	//Add the viewport to the scroll area
	scrollArea = new QScrollArea;
	scrollArea->setWidget(viewport);
	
    meMainLayout->addWidget(scrollArea);
    setLayout(meMainLayout);
	
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}
clEditObjectUI::~clEditObjectUI()
{
}
void clEditObjectUI::initializeElements()
{
    try
    {
        for(int i = 0; i < NumGridRows; i++)
        {
            meLineEdits[i] = NULL;
		    meLabels[i] = NULL;
			meDateTimeEdit[i] = NULL;
			meListWidgetExtendString[i] = NULL;
			meListWidgetExtendObject[i] = NULL;
			meWidgetExtendObject[i] = NULL;
			meValues[i] = NULL;		
			meComboBox[i] = NULL;
        }
    }
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clEditObjectUI::initializeElements" + QString(e.what()));
    }
}


 void clEditObjectUI::createGridGroupBox(vector <std::string> paValues)
 {
     //meGridGroupBox = new QGroupBox(tr("Values"));
     layout = new QGridLayout;
	/*
    QWidget *scrollWidget = new QWidget;
    QScrollArea *loScrollArea = new QScrollArea;
    loScrollArea->setWidget(scrollWidget);
	scrollWidget->setLayout(layout);
    */


	

     for (int i = 0; i < meDatabaseColumns.size(); ++i) {
         if (meDatabaseColumns[i].getName().toUpper().compare("PKEY") == 0)
         {

         }
         else
         {	
			 if (meDatabaseColumns[i].getType().toUpper().compare("VARCHAR(64)") == 0 ||
					meDatabaseColumns[i].getType().toUpper().compare("VARCHAR(255)") == 0 ||
					meDatabaseColumns[i].getType().toUpper().compare("TEXT") == 0)
			 {
				 meLabels[i] = new QLabel(meDatabaseColumns[i].getName());
				 meLineEdits[i] = new QLineEdit;
				 meLineEdits[i]->setText(QString(paValues.at(i).c_str()));
				 
				 layout->addWidget(meLabels[i], i + 1, 0);
				 layout->addWidget(meLineEdits[i], i + 1, 1);
				 
			 }
			 else if (meDatabaseColumns[i].getType().toUpper().compare("VARCHAR(64)[]") == 0 ||
						meDatabaseColumns[i].getType().toUpper().compare("VARCHAR(255)[]") == 0 ||
						meDatabaseColumns[i].getType().toUpper().compare("TEXT[]") == 0 ||
						meDatabaseColumns[i].getType().toUpper().compare("TIMESTAMP[]") == 0)
			 {
				 meLabels[i] = new QLabel(meDatabaseColumns[i].getName());
				 meListWidgetExtendString[i] = new clListWidgetExtendForString(meIceClientServer,meIceClientLogging,meDatabaseColumns, meDatabaseColumns[i]);
				 
				 QStringList loElements = QString(paValues.at(i).c_str()).remove("}").remove("{").split(",", QString::SkipEmptyParts);
				 for (int k = 0; k < loElements.size(); k++)
				 {
					meListWidgetExtendString[i]->meListWidget->addItem(loElements.at(k));
				 }
				 
				 
				 layout->addWidget(meLabels[i], i + 1, 0);
				 layout->addWidget(meListWidgetExtendString[i], i + 1, 1);
				 
			 }
			 else if (meDatabaseColumns[i].getType().toUpper().compare("TIMESTAMP") == 0)
			 {
				 meLabels[i] = new QLabel(meDatabaseColumns[i].getName());
				 meDateTimeEdit[i] = new QDateTimeEdit(QDateTime::currentDateTime());
				 
				 //2024-10-03 17:06:13
				 QDateTime loDateTime;
				 loDateTime.fromString(paValues.at(i).c_str(), "yyyy-MM-dd hh:mm:ss.zzz");
				 meDateTimeEdit[i]->setDateTime(loDateTime);
				 
				 layout->addWidget(meLabels[i], i + 1,  0);
				 layout->addWidget(meDateTimeEdit[i], i + 1, 1);
			 }		
			 else if (meDatabaseColumns[i].getType().toUpper().compare("UUID") == 0)
			 {
				 meLabels[i] = new QLabel(meDatabaseColumns[i].getName());
				 meWidgetExtendObject[i] = new clWidgetExtendForObject(meIceClientServer,meIceClientLogging,meDatabaseColumns, meDatabaseColumns[i]);
				 
				 meWidgetExtendObject[i]->meLineEdit->setText(QString(paValues.at(i).c_str()));
				 
				 layout->addWidget(meLabels[i], i + 1, 0);
				 layout->addWidget(meWidgetExtendObject[i], i + 1,1);
			 }			 
			 else if (meDatabaseColumns[i].getType().toUpper().compare("UUID[]") == 0)
			 {
				 meLabels[i] = new QLabel(meDatabaseColumns[i].getName());
				 meListWidgetExtendObject[i] = new clListWidgetExtendForObject(meIceClientServer,meIceClientLogging,meDatabaseColumns, meDatabaseColumns[i]);
				 
				 QStringList loElements = QString(paValues.at(i).c_str()).remove("}").remove("{").split(",", QString::SkipEmptyParts);
				 for (int k = 0; k < loElements.size(); k++)
				 {
					meListWidgetExtendObject[i]->meListWidget->addItem(loElements.at(k));
				 }
				 
				 layout->addWidget(meLabels[i], i + 1, 0);
				 layout->addWidget(meListWidgetExtendObject[i], i + 1,1);
			 }
			 else if (meDatabaseColumns[i].getType().toUpper().compare("INT[]") == 0 ||
						meDatabaseColumns[i].getType().toUpper().compare("INT4[]") == 0 ||
						meDatabaseColumns[i].getType().toUpper().compare("INT8[]") == 0 ||
						meDatabaseColumns[i].getType().toUpper().compare("FLOAT4[]") == 0 ||
						meDatabaseColumns[i].getType().toUpper().compare("FLOAT8[]") == 0)
			 {
				 meLabels[i] = new QLabel(meDatabaseColumns[i].getName());
				 meListWidgetExtendString[i] = new clListWidgetExtendForString(meIceClientServer,meIceClientLogging,meDatabaseColumns, meDatabaseColumns[i]);
				 
				QStringList loElements = QString(paValues.at(i).c_str()).remove("}").remove("{").split(",", QString::SkipEmptyParts);
				 
				 
				 for (int k = 0; k < loElements.size(); k++)
				 {
					meListWidgetExtendString[i]->meListWidget->addItem(loElements.at(k));
				 }
				 
				 layout->addWidget(meLabels[i], i + 1, 0);
				 layout->addWidget(meListWidgetExtendString[i], i + 1, 1);
			 }			 
			 else if (meDatabaseColumns[i].getType().toUpper().compare("INT") == 0 ||
						meDatabaseColumns[i].getType().toUpper().compare("INT4") == 0 ||
						meDatabaseColumns[i].getType().toUpper().compare("INT8") == 0 ||
						meDatabaseColumns[i].getType().toUpper().compare("FLOAT4") == 0 ||
						meDatabaseColumns[i].getType().toUpper().compare("FLOAT8") == 0)
			 {
				 //It is an enum
				 if (meDatabaseColumns[i].getExtra().toUpper().compare("ENUM") == 0)
				 {
					meLabels[i] = new QLabel(meDatabaseColumns[i].getName());
					meComboBox[i] = new clComboBoxExtend(meIceClientLogging,meDatabaseColumns[i]);
					meComboBox[i]->setValue(QString(paValues.at(i).c_str()));
					layout->addWidget(meLabels[i], i + 1, 0);
					layout->addWidget(meComboBox[i], i + 1, 1);
				 }
				 else
				 {				 
					 meLabels[i] = new QLabel(meDatabaseColumns[i].getName());
					 meLineEdits[i] = new QLineEdit;
					 meLineEdits[i]->setText(QString(paValues.at(i).c_str()));
					 
					 layout->addWidget(meLabels[i], i + 1, 0);
					 layout->addWidget(meLineEdits[i], i + 1, 1);
				 }
			 }						 
			 else
			 {
				 meLabels[i] = new QLabel(meDatabaseColumns[i].getName());
				 meLineEdits[i] = new QLineEdit;
				 meLineEdits[i]->setText(QString(paValues.at(i).c_str()));
				 layout->addWidget(meLabels[i], i + 1, 0);
				 layout->addWidget(meLineEdits[i], i  + 1, 1);
			 }
         }
     }
     meButtons[0] = new QPushButton(QString("OK"));
     meButtons[1] = new QPushButton(QString("Cancel"));
     connect(meButtons[0], SIGNAL(clicked()), this, SLOT(slotButtonOKPressed()));
     connect(meButtons[1], SIGNAL(clicked()), this, SLOT(slotButtonCancelPressed()));
     layout->addWidget(meButtons[0], meDatabaseColumns.size() + 1, 0);
     layout->addWidget(meButtons[1], meDatabaseColumns.size() + 1, 1);

     //smallEditor = new QTextEdit;
     //smallEditor->setPlainText(tr("This widget takes up about two thirds of the "
     //                             "grid layout."));
     //layout->addWidget(smallEditor, 0, 2, 4, 1);

     //layout->setColumnStretch(1, 10);
     //layout->setColumnStretch(2, 20);
	
 }


void clEditObjectUI::slotButtonOKPressed()
{
    try
    {
        for(int i = 0; i < NumGridRows; i++)
        {
            if(meLineEdits[i] != NULL)
            {
                meValues[i] = new QString(meLineEdits[i]->text());
            }
			else if (meDateTimeEdit[i] != NULL)
			{
				QDateTime dateTime = meDateTimeEdit[i]->dateTime();
				meValues[i] = new QString(dateTime.toString("yyyy-MM-dd HH:mm:ss.zzz"));
			}
			else if (meWidgetExtendObject[i] != NULL)
			{
				meValues[i] = new QString(meWidgetExtendObject[i]->meLineEdit->text());
			}
			else if (meListWidgetExtendObject[i] != NULL)
			{
				QString loValues;
				for (int j = 0; j < meListWidgetExtendObject[i]->meListWidget->count(); j++)
				{
					QListWidgetItem *loListWidgetItem = meListWidgetExtendObject[i]->meListWidget->item(j);
					if (j == 0) loValues = loListWidgetItem->text();
					else loValues = loValues +  QString("$;$") + loListWidgetItem->text();
				}
				meValues[i] = new QString(loValues);
			}		
			else if (meListWidgetExtendString[i] != NULL)
			{
				QString loValues;
				for (int j = 0; j < meListWidgetExtendString[i]->meListWidget->count(); j++)
				{
					QListWidgetItem *loListWidgetItem = meListWidgetExtendString[i]->meListWidget->item(j);
					if (j == 0) loValues = loListWidgetItem->text();
					else loValues = loValues +  QString("$;$") + loListWidgetItem->text();
				}
				meValues[i] = new QString(loValues);
			}else if(meComboBox[i] != NULL)
			{
				meValues[i] = new QString(meComboBox[i]->getValue());
			}					
        }
	
		
        this->done(0);
    }
    catch(exception &e)
    {
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clEditObjectUI::slotButtonOKPressed() -> " + QString(e.what()));
    }
}

void clEditObjectUI::slotButtonCancelPressed()
{
    try
    {	
	
        this->done(1);
    }
    catch(exception &e)
    {	
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clEditObjectUI::slotButtonCancelPressed() -> " + QString(e.what()));
    }
}


