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
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::initializeElements");

    }
    catch(exception &e)
    {

		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::initializeElements" + QString(e.what()));
    }
}


 void clWidgetExtendForObject::createGridGroupBox()
 {
     try
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

        meLineEdit = new QLineEdit(this);
        meValues = QString("");
        layout->addWidget(meLineEdit, 1,0);


         meGridGroupBox->setLayout(layout);

     }
     catch(exception &e)
     {
         meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::createGridGroupBox" + QString(e.what()));
     }
 }


void clWidgetExtendForObject::slotButtonDeletePressed()
{
    try
    {
		meLineEdit->setText(QString(""));
		meValues = QString("");
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::slotButtonDeletePressed() -> " + QString(e.what()));
    }
}
bool clWidgetExtendForObject::getCurrentValue()
{
    try
    {
		if (meValues.compare(QString("")) == 0)
        {
            meLineEdit->setText(QString(""));

		}
		else
        {
			QString loReferences = meDatabaseColumn.getReference();
			
			QStringList loClasses = loReferences.split("$;$");
			if (loClasses.size() > 1)
			{
				bool loFound = false;
				for (int j = 0; j < loClasses.size(); j++)
				{
					//Getting the objectsID's from the database
					vector<std::string> loPropertyName;
					loPropertyName.push_back("NAME");
					vector<std::string> loReturnValues;
					QString loReturnMessageGetById;


                    QString loClassNameT = QString(loClasses.at(j));

                    if (!meIceClientServer->getFromTableDatabaseById(   loClassNameT,
																		meValues,
																		loPropertyName,
																		loReturnValues,
																		loReturnMessageGetById))
					{
                        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::getCurrentValue() -> " + loReturnMessageGetById);
					}
					else
					{
                        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::getCurrentValue() -> " + loReturnMessageGetById);
						for (int i = 0; i < loReturnValues.size(); i++)
						{
							meLineEdit->setText(QString(loReturnValues[i].c_str()));
							loFound = true;
						}									
					}
					//Break out of te for loop if the class is found
					if (loFound)break;
				}
			}
			else
			{
				//Getting the objectsID's from the database
				vector<std::string> loPropertyName;
				loPropertyName.push_back("NAME");
				vector<std::string> loReturnValues;
				QString loReturnMessageGetById;

				if (!meIceClientServer->getFromTableDatabaseById(   loReferences,
																	meValues,
																	loPropertyName,
																	loReturnValues,
																	loReturnMessageGetById))
				{
                    meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::getCurrentValue() -> " + loReturnMessageGetById);
				}
				else
				{
                    meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::getCurrentValue() -> " + loReturnMessageGetById);
					for (int i = 0; i < loReturnValues.size(); i++)
					{
                        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::getCurrentValue() Name -> " + QString(loReturnValues.at(i).c_str()));
                        meLineEdit->setText(QString(loReturnValues.at(i).c_str()));
                        break;
					}
				}
			}
		}
        return true;
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clWidgetExtendForObject::getCurrentValue() -> " + QString(e.what()));
        return false;
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
			
			
			//meLineEdit->setText(QString(loListWidgetSelectObject->meValues[0]->toStdString().c_str()));
			meLineEdit->setText(QString(loListWidgetSelectObject->meNames[0]->toStdString().c_str()));
			meValues = QString(loListWidgetSelectObject->meValues[0]->toStdString().c_str());
			
            meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe",QString("clWidgetExtendForObject::slotButtonAddPressed() -> Added " + meValues));
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
