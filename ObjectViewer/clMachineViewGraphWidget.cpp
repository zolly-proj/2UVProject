#include "clMachineViewGraphWidget.h"

clMachineViewGraphWidget::clMachineViewGraphWidget(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clClassLoader *paClassLoader, struct_widget paStructWidget,QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;	
	meClassLoader = paClassLoader;
	meStructWidget = paStructWidget;
    initializeElements();
    
    /////// Setting up the timer function ///////////////////////////////////
	meTimer = new QTimer(this);
	meTimer->setInterval(meStructWidget.refresh.toInt());
	meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotDoIt()));
	meTimer->start();
    
    
}
clMachineViewGraphWidget::~clMachineViewGraphWidget()
{
}

void clMachineViewGraphWidget::initializeElements()
{
    try
    {
		meMainLayout = new QGridLayout();
		mePieSeries = new QPieSeries;
		
		mePieSeries->append("Running", 50);
		mePieSeries->append("Not running", 50);
		
		// 4. Create a QChart and add the series
		meChart = new QChart();
		meChart->addSeries(mePieSeries);
		meChart->setTitle("Machine usage");
		meChart->legend()->hide(); // Optional: hide the legend 
		
		meChartView = new QChartView;
		meChartView->setChart(meChart);
				
		meMainLayout->addWidget(meChartView,0,1,3,1);
		
		
		meRadioDaily = new QRadioButton("Daily");
	    meRadioWeekly = new QRadioButton("Weekly");
		meRadioMonthly = new QRadioButton("Monthly");
		
		meMainLayout->addWidget(meRadioDaily,0,0);
		meMainLayout->addWidget(meRadioWeekly,1,0);
		meMainLayout->addWidget(meRadioMonthly,2,0);
				
		setLayout(meMainLayout);
		
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMahineViewGraphWidget::initializeElements" + QString(e.what()));
    }
}
void clMachineViewGraphWidget::slotDoIt()
{
	try
	{
		clObject loObject = meClassLoader->getObject(meStructWidget.object_class);
		loObject.ClassName = meStructWidget.object_class;
		loObject.ObjectId = meStructWidget.object_id;
		
		
		/*
    virtual bool getFromTableDatbaseByPropertyOrdered(     QString& paTableName,
                                                    QString& paStartValue,
                                                    QString& paMaxValue,
                                                    vector<std::string>& paProperties,
                                                    vector<std::string>& paValue,
                                                    vector<std::string>& paTypeValue,
                                                    vector<std::string>& paLogExp,
													QString& paOrderBy,
                                                    vector<std::string>& paReturnId,
                                                    QString& paReturnMessage);   		
		*/
		QString loTableName = QString("OPERATION");
		QString loStartStop = QString("0");
		
		vector<std::string> loProperties;
		vector<std::string> loValues;
		vector<std::string> loTypeValues;
		vector<std::string> loLogExps;
		vector<std::string> loReturnIds;
		QString loOrderBy = QString ("DATUM_START");
		QString loReturnMessage;
		
		
		QDateTime loDateTimeStart = QDateTime::currentDateTime();
		QDateTime loDateTimeStartTemp;
		
		if (meRadioDaily->isChecked())
			loDateTimeStartTemp = loDateTimeStart.addDays(-1);
		else if (meRadioWeekly->isChecked())
			loDateTimeStartTemp = loDateTimeStart.addDays(-7);
		else if (meRadioMonthly->isChecked())
			loDateTimeStartTemp = loDateTimeStart.addDays(-31);
		else
			loDateTimeStartTemp = loDateTimeStart.addDays(-1);
			
		loProperties.push_back("DATUM_START");
		loValues.push_back(QString(loDateTimeStartTemp.toString("yyyy-MM-dd HH:mm:ss.zzz")).toStdString());
		loTypeValues.push_back("timestamp(3)");
		loLogExps.push_back(">");			

		loProperties.push_back("OBJECT");
		loValues.push_back(loObject.ObjectId.toStdString());
		loTypeValues.push_back("uuid");
		loLogExps.push_back("=");			

		loProperties.push_back("OPERATION_STATE");
		loValues.push_back("20");
		loTypeValues.push_back("int");
		loLogExps.push_back("=");				

		loProperties.push_back("OPERATION_CLOSED_CAUSE");
		loValues.push_back("10");
		loTypeValues.push_back("int");
		loLogExps.push_back("=");
				
		if (meIceClientServer->getFromTableDatbaseByPropertyOrdered(loTableName, loStartStop, loStartStop, loProperties, loValues, loTypeValues, loLogExps, loOrderBy,loReturnIds, loReturnMessage))
		{
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe", "clMahineViewGraphWidget::slotDoIt" + QString(" [%1] operations found").arg(loReturnIds.size()));
		}
		else
		{
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMahineViewGraphWidget::slotDoIt No operations found");
			return;
		}
		
		
		/////////////////////// Max time ///////////////////////
		long maxTimeDifference = loDateTimeStart.toSecsSinceEpoch() - loDateTimeStartTemp.toSecsSinceEpoch();
		long calculatedTimeDifference = 0;
		
		
		if (loReturnIds.size() > 0)
		{
			for (int i = 0; i < loReturnIds.size(); i++)
			{
				//Getting the stop time
				//and not in error
				 vector<std::string> loPropertyName;
				 vector<std::string> loAlias;
				 vector<std::string> loType;
				 vector<std::string> loExtra;
				 vector<std::string> loReference;
				 QString loReturnMessageObject;
				 meIceClientServer->getAllPropertiesFromTable(  loTableName,
																loPropertyName,
																loAlias,
																loType,
																loExtra,
																loReference,
																loReturnMessageObject);
																
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clMahineViewGraphWidget::slotDoIt -> " + loReturnMessageObject);
				
				/*
				    virtual bool getFromTableDatabaseById(  QString& paTableName,
                                            QString& paId,
                                            vector<std::string>& paProperties,
                                            vector<std::string>& paReturnValue,
                                            QString& paReturnMessage);
                */                            
				
				vector<std::string> loReturnValues;
				QString loReturnId = QString(loReturnIds.at(i).c_str());
				
				if (meIceClientServer->getFromTableDatabaseById(loTableName, loReturnId,loPropertyName,loReturnValues,loReturnMessageObject))
				{
					if (loReturnValues.size() > 0)
					{
						QDateTime loMinDateTime;
						QDateTime loMaxDateTime;
						//Loop the properties get start and stop date
						for(int k = 0; k < loReturnValues.size(); k++)
						{
							if (QString(loPropertyName.at(k).c_str()).toUpper().compare(QString("DATUM_STOP")) == 0)
							{
								if (QString(loReturnValues.at(k).c_str()).compare(QString("")) == 0)
									loMaxDateTime = QDateTime::fromString(QString("1980-01-01 00:00:00.000"), "yyyy-MM-dd HH:mm:ss.zzz");
								else
									loMaxDateTime = QDateTime::fromString(QString(loReturnValues.at(k).c_str()), "yyyy-MM-dd HH:mm:ss.zzz");
							}
							else if (QString(loPropertyName.at(k).c_str()).toUpper().compare(QString("DATUM_START")) == 0)
							{
								loMinDateTime = QDateTime::fromString(QString(loReturnValues.at(k).c_str()), "yyyy-MM-dd HH:mm:ss.zzz"); 
							}
						}
						if (loMaxDateTime != QDateTime::fromString(QString("1980-01-01 00:00:00.000"), "yyyy-MM-dd HH:mm:ss.zzz"))
						{
							//Claculate the running time
							long timeDifference = loMaxDateTime.toSecsSinceEpoch() - loMinDateTime.toSecsSinceEpoch();
							calculatedTimeDifference = calculatedTimeDifference + timeDifference;
						}
					}
					
				}
			}
		}
		
		///////////////////////////////////// Set the graph //////////////////////////////////////////////
		double loOnePercent = maxTimeDifference / 100;
		double loPercentage = calculatedTimeDifference / loOnePercent;
		
		//Running		
		mePieSlice_Running = mePieSeries->slices()[0];
		mePieSlice_Running->setValue(loPercentage);
		if (loPercentage < 0) mePieSlice_Running->setExploded();
		//Not Running
		mePieSlice_Offline = mePieSeries->slices()[1];
		mePieSlice_Offline->setValue(100 - loPercentage);
		
		
		//mePieSeries->append("Running", 50);
		//mePieSeries->append("Offline", 50);		
	}
    catch(exception &e)
    {	
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"ObjectViewer.exe","clMahineViewGraphWidget::slotDoIt" + QString(e.what()));
    }		
}
