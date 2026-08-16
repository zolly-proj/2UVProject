#define INFO_BUFFER_SIZE 32767
#include "clGraphView.h"

clGraphView::clGraphView(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QString paXmlFile) : QWidget()
{
    try
    {
        //this->resize(682,297);
		//horizontalScrollBar()->setRange(0, 0);
		//verticalScrollBar()->setRange(0, 0);
		
		
		
		
		//this->setParent(parent);
		meIceClientLogging = paIceClientLogging;
		meIceClientServer = paIceClientServer;
		
		
		meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::clGraphView() -> XML file to read:" + paXmlFile);
		if (!readXmlFile(paXmlFile)) meIceClientLogging->insertItem("71",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::clGraphView() -> Problem reading XML file");
		else meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::clGraphView() -> Reading XML file OK");
		//meWorkstationStatus = paWorkstationStatus;
		/*
        doSetup(meThread);
        moveToThread(&meThread);
        meThread.start();
		*/
		//meGraphicsItem = new QGraphicsItem(this);
		

		initializeUI();
		
		//QWidget * object = qobject_cast< QWidget * >(this);
		//meChart = new QChart(object);
		//meChart->setParent(object);
	
		
		
		
		
		
		
		//setUpWidget();
		//setTimingsInGraph();
		
		meTimer = new QTimer(this);
        meTimer->setInterval(meGraphRefresh.toInt());
        meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotDoIt()));
        meTimer->start();
		
		

		
		
    }
      catch(exception &e)
    {
		meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::clGraphView() -> " + QString(e.what()));
    }
}
clGraphView::~clGraphView()
{
	try
	{
		//this->close();
		//meChart->deleteLater();
		//meChartView->deleteLater();
	}
	catch(...)
	{
		
	}
}

void clGraphView::initializeUI()
{
	try
	{
		
		
		meChartView = new QChartView;
		meChart = new QChart();
		meChartView->setChart(meChart);

		layout = new QGridLayout(this);
		
		meLabels[0][0] = new QLabel(QString("Graphical representation:"));
		meLabels[0][1] = new QLabel(QString("Realtime/defined time"));
		meLabels[0][2] = new QLabel(QString("From:"));
		meLabels[0][3] = new QLabel(QString("To:"));
		
		meCheckBox[0] = new QCheckBox;
		meDateTimeEdit[0] = new QDateTimeEdit;
		meDateTimeEdit[0]->setDateTime((QDateTime::currentDateTime()).addSecs(-60 * 60));
		meDateTimeEdit[1] = new QDateTimeEdit;
		meDateTimeEdit[1]->setDateTime(QDateTime::currentDateTime());
		
		meButton[0] = new QPushButton(QString("Generate data"));
		connect(meButton[0], SIGNAL(clicked()),this,SLOT(slotButtonGeneratePressed()));
		
		
		
		layout->addWidget(meLabels[0][0],0,0);
		layout->addWidget(meLabels[0][1],1,0);
		layout->addWidget(meLabels[0][2],2,0);
		layout->addWidget(meLabels[0][3],3,0);
		layout->addWidget(meCheckBox[0],1,1);
		layout->addWidget(meDateTimeEdit[0],2,1);
		layout->addWidget(meDateTimeEdit[1],3,1);
		layout->addWidget(meButton[0],4,0);
		
		
		
		
		layout->addWidget(meChartView,0,1);
		
		for (int i = 0 ; i < 200; i++)
		{
			meSeries[i] = nullptr;
		}
		
		
		
	}
	catch(exception &e)
    {
		meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::clGraphView() -> " + QString(e.what()));
    }
}
/*****************************
* Slots
*****************************/
void clGraphView::slotButtonGeneratePressed()
{
	try
	{
		QString loExportPath = QFileDialog::getSaveFileName(nullptr, "Save excel ass ...", ".", "Excel (*.csv)" );
		if (loExportPath.compare(QString("")) == 0)return;
		
		meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::slotButtonGeneratePressed() -> Path where file will be saved: " + loExportPath);
		////////////////////////////////////////// Getting the operations ///////////////////////////////////////////////////////////////
		vector <vector<QString>> genloResultX = meloResultX;
		vector <vector<QString>> genloResultY = meloResultY;
		
		
		meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe",QString("clGraphView::slotButtonGeneratePressed() -> Values stored [%1]...").arg(genloResultX.size()));
		
		for (int i = 0; i < genloResultX.size();i++)
		{
			
			vector <QString> memLoResultX = genloResultX.at(i);
			vector <QString> memLoResultY = genloResultY.at(i);
			
			QString loFileName = QString("%1%2.csv").arg(loExportPath).arg(QString::number(i+1));		
			QFile data(loFileName);
			meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::slotButtonGeneratePressed() -> Name set ..." + loFileName);
			if (!data.open(QFile::WriteOnly | QFile::Truncate))
			{
				meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::slotButtonGeneratePressed() -> Could not open file : " + data.fileName());
				return;
			}
			QTextStream fileOut(&data);
			{
				for (int y = 0; y < memLoResultX.size(); y++)
				{
					fileOut << QString("%1;%2;%3").arg(QString::number(y)).arg(QString(memLoResultX.at(y))).arg(QString(memLoResultY.at(y)));
					fileOut << Qt::endl;
				}
			}
			data.close();
		}	
		
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::slotButtonGeneratePressed() -> " + QString(e.what()));
	}
}

/*****************************
* UI functions
*****************************/
void clGraphView::slotDoIt()
{
	
    try
    {
		////////////////////////////////////////// Getting the operations ///////////////////////////////////////////////////////////////
		vector <std::string> loProperties;
		vector <std::string> loValues;
		vector <std::string> loTypeValues;
		vector <std::string> loLogExp;
		vector <std::string> loReturnIds;
		QString loReturnMessage;
		
		loProperties.push_back(meGraphProperty.toStdString());
		loValues.push_back(meGraphValue.toStdString());
		loTypeValues.push_back(meGraphType.toStdString());
		loLogExp.push_back(meGraphExpression.toStdString());
		
		if (meCheckBox[0]->isChecked())
		{
			loProperties.push_back("DATUM_STOP");
			loValues.push_back(QString((meDateTimeEdit[1]->dateTime()).toString("yyyy-MM-dd HH:mm:ss.zzz")).toStdString());
			loTypeValues.push_back("timestamp(3)");
			loLogExp.push_back("<");

			//3 hours before
			loProperties.push_back("DATUM_STOP");
			loValues.push_back(QString((meDateTimeEdit[0]->dateTime()).toString("yyyy-MM-dd HH:mm:ss.zzz")).toStdString());
			loTypeValues.push_back("timestamp(3)");
			loLogExp.push_back(">");		
		}
		else
		{		
			loProperties.push_back("DATUM_STOP");
			loValues.push_back(QString((QDateTime::currentDateTime()).toString("yyyy-MM-dd HH:mm:ss.zzz")).toStdString());
			loTypeValues.push_back("timestamp(3)");
			loLogExp.push_back("<");

			//3 hours before
			loProperties.push_back("DATUM_STOP");
			loValues.push_back(QString((QDateTime::currentDateTime()).addSecs(-60 * 60).toString("yyyy-MM-dd HH:mm:ss.zzz")).toStdString());
			loTypeValues.push_back("timestamp(3)");
			loLogExp.push_back(">");
		}


		loProperties.push_back(meGraphProperty.toStdString());
		loValues.push_back(meGraphValue.toStdString());
		loTypeValues.push_back(meGraphType.toStdString());
		loLogExp.push_back(meGraphExpression.toStdString());
		
		
		
		QString loStart = QString("0");
		QString loStop = QString("0");
		if (!meIceClientServer->getFromTableDatbaseByProperty(meGraphClass,loStart,loStop,loProperties,loValues,loTypeValues,loLogExp,loReturnIds,loReturnMessage))
		{
			meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::slotDoIt() -> " + loReturnMessage);
			return;
		}
		if (loReturnIds.size() < 1)
		{
			meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::slotDoIt() -> no result returned");
			return;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		//Create the line series for the graph
		for (int r = 0; r < 200; r++)
		{
			meSeries[r] = nullptr;
		}
		for (int r = 0; r < meGraphLines.size(); r++)
		{
			meSeries[r] = new QLineSeries;
		}
		
		
		
		//Clear the graph lines//
		meChart->removeAllSeries();
		
		//Remove the axes
		QList<QAbstractAxis *>	axes = meChart->axes(Qt::Horizontal|Qt::Vertical);
		for (int r = 0; r < axes.size(); r++)
		{
			meChart->removeAxis(axes.at(r));
		}
		
		
	

		QString loID("");
		QString loClass("");
		QString loProperty("");
		QString loTypeX("");
		QString loTypeY("");
		QString loValue("");

		vector <QString> loResultX;
		vector <QString> loResultY;
		vector <int> loIndexX;
		vector <int> loIndexY;

		vector <vector<QString>> memLoResultX;
		vector <vector<QString>> memLoResultY;
		memLoResultX.clear();
		memLoResultY.clear();


		//For each graph line
		for (int t = 0; t < meGraphLines.size(); t++)
		{
			clGraphLine loGraphLine = meGraphLines.at(t);
			vector <clQuery> meQuery_X = loGraphLine.getQuery_X();
			vector <clQuery> meQuery_Y = loGraphLine.getQuery_Y();
			
			loResultX.clear();
			loResultY.clear();
			loIndexX.clear();
			loIndexY.clear();

		






			for (int i = 0; i < loReturnIds.size(); i++)
			{			


					loID = QString(loReturnIds.at(i).c_str());

					for (int k = 0; k < meQuery_X.size(); k++)
					{
						loClass = meQuery_X.at(k).getClass();
						loProperty = meQuery_X.at(k).getProperty();
						loTypeX = meQuery_X.at(k).getType();
						
						/*
							virtual bool getFromTableDatabaseById(  QString& paTableName,
												QString& paId,
												vector<std::string>& paProperties,
												vector<std::string>& paReturnValue,
												QString& paReturnMessage);
						*/
						
						vector <std::string> loPropertiesSub;
						loPropertiesSub.clear();
						loPropertiesSub.push_back(loProperty.toStdString());
						vector <std::string> loValuesSub;
						loValuesSub.clear();
						QString loReturnMessageSub;
						
						
						if (!meIceClientServer->getFromTableDatabaseById(loClass, loID,loPropertiesSub,loValuesSub,loReturnMessageSub))
						{
							meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::slotDoIt() -> " + loReturnMessageSub);
							return;
						}
						
						if(loValuesSub.size() < 1)
						{
							meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::slotDoIt() -> no result");
							return;
						}
						
						//Setting the new id or get the value
						if (loTypeX.toUpper().compare(QString("UUID")) == 0)
							loID = QString(loValuesSub.at(0).c_str());
						else
						{
							loResultX.push_back(QString(loValuesSub.at(0).c_str()));
							loIndexX.push_back(meQuery_X.at(k).getIndex());
							
						}
					}
					
					loID = QString(loReturnIds.at(i).c_str());
					
					for (int l = 0; l < meQuery_Y.size(); l++)
					{
						loClass = meQuery_Y.at(l).getClass();
						loProperty = meQuery_Y.at(l).getProperty();
						loTypeY = meQuery_Y.at(l).getType();
						/*
							virtual bool getFromTableDatabaseById(  QString& paTableName,
												QString& paId,
												vector<std::string>& paProperties,
												vector<std::string>& paReturnValue,
												QString& paReturnMessage);
						*/
						
						vector <std::string> loPropertiesSub;
						loPropertiesSub.clear();
						loPropertiesSub.push_back(loProperty.toStdString());
						vector <std::string> loValuesSub;
						loValuesSub.clear();
						QString loReturnMessageSub;					
						
						if (!meIceClientServer->getFromTableDatabaseById(loClass, loID,loPropertiesSub,loValuesSub,loReturnMessageSub))
						{
							meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::slotDoIt() getFromTableDatabaseById-> " + loReturnMessageSub);
							return;
						}
						
						if(loValuesSub.size() < 1)
						{
							meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::slotDoIt() -> no result");
							return;
						}
						
						//Setting the new id or get the value
						if (loTypeY.toUpper().compare(QString("UUID")) == 0)
							loID = QString(loValuesSub.at(0).c_str());
						else
						{
							loResultY.push_back(QString(loValuesSub.at(0).c_str()));
							loIndexY.push_back(meQuery_Y.at(l).getIndex());
							
						}
					}
				}
				
				if (!addGraphLine(t,loResultX, loTypeX, loIndexX, loResultY, loTypeY, loIndexY, "test","blue"))
					meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::slotDoIt() -> could not add values to graph ...");
				else
				{
					memLoResultX.push_back(loResultX);
					memLoResultY.push_back(loResultY);
					meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::slotDoIt() -> addGraphLine called with success ...");	
				}
		}
		//For memorty purpose
		meloResultX.clear();
		meloResultY.clear();
		meloResultX = memLoResultX;
		meloResultY = memLoResultY;
		
		meChart->removeAllSeries();	
		//Add series to the chart	
		for (int u = 0; u < 200; u++)
		{
			if (meSeries[u] != nullptr)
			{
				meChart->addSeries(meSeries[u]);
			}
		}
		
		
		//Set the axes values
		QDateTimeAxis *axisY;		
		QDateTimeAxis *axisX;
		QValueAxis *axisXval;
		QValueAxis *axisYval;
				
		if (loTypeX.compare(QString("timestamp(3)")) == 0 && loTypeY.compare(QString("timestamp(3)")) == 0)
		{
			
			axisX = new QDateTimeAxis;
			axisX->setFormat("dd-MM-yyyy h:mm:ss.zzz");
			
			axisY = new QDateTimeAxis;
			axisY->setFormat("dd-MM-yyyy h:mm:ss.zzz");
			
			meChart->addAxis(axisX, Qt::AlignBottom);
			meChart->addAxis(axisY, Qt::AlignLeft);

			for (int w = 0; w < 200; w++)
			{
				if (meSeries[w] != NULL)
				{
					meSeries[w]->attachAxis(axisX);
					meSeries[w]->attachAxis(axisY);					
				}
			}

		}
		else if (loTypeX.compare(QString("timestamp(3)")) != 0 && loTypeY.compare(QString("timestamp(3)")) == 0)
		{					
			axisY = new QDateTimeAxis;
			axisY->setFormat("dd-MM-yyyy h:mm:ss.zzz");
			
			axisXval = new QValueAxis;
			axisXval->setMin(0);				
			
			meChart->addAxis(axisXval, Qt::AlignBottom);
			meChart->addAxis(axisY, Qt::AlignLeft);							
			for (int w = 0; w < 200; w++)
			{
				if (meSeries[w] != NULL)
				{
					meSeries[w]->attachAxis(axisXval);
					meSeries[w]->attachAxis(axisY);					
				}
			}
		}
		else if (loTypeX.compare(QString("timestamp(3)")) == 0 && loTypeY.compare(QString("timestamp(3)")) != 0)
		{
			
			
			axisX = new QDateTimeAxis;
			axisX->setFormat("dd-MM-yyyy h:mm:ss.zzz");
			axisX->setTitleText("Date");
			
			axisYval = new QValueAxis;
			//axisYval->setMin(-1);				
			axisYval->setTitleText("Value");
			
			meChart->addAxis(axisX, Qt::AlignBottom);
			meChart->addAxis(axisYval, Qt::AlignLeft);							
			
			for (int w = 0; w < 200; w++)
			{
				if (meSeries[w] != nullptr)
				{
					meSeries[w]->attachAxis(axisX);
					meSeries[w]->attachAxis(axisYval);					
				}
			}
		}
		else if (loTypeX.compare(QString("timestamp(3")) != 0 && loTypeY.compare(QString("timestamp(3)")) != 0)
		{
			

			axisXval = new QValueAxis;
			axisXval->setMin(0);				
			
			axisYval = new QValueAxis;
			axisYval->setMin(0);				
			
			meChart->addAxis(axisXval, Qt::AlignBottom);
			meChart->addAxis(axisYval, Qt::AlignLeft);							
			for (int w = 0; w < 200; w++)
			{
				if (meSeries[w] != NULL)
				{
					meSeries[w]->attachAxis(axisXval);
					meSeries[w]->attachAxis(axisYval);					
				}
			}
		}
    }
    catch(exception &e)
    {
		meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clGraphView::slotDoIt() -> " + QString(e.what()));
    }
}
bool clGraphView::readXmlFile(QString paXmlFile)
{
	try
	{
        QString loFileName = paXmlFile;

        QFile loFile(loFileName);
        if ( !loFile.open( QIODevice::ReadOnly ) ) {
			cout << "Could not open file: " << loFileName.toStdString() << endl;
            return false;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) {
            loFile.close();
			cout << "Could not parse file: " << loFileName.toStdString() << endl;
            return false;
        }
		
		
        // create the tree view out of the DOM
		QDomElement loDocElem = loDomDocument.documentElement();

		QDomNode loDomNode = loDocElem.firstChild();
		while( !loDomNode.isNull() ) 
		{
			cout << "loDomNode" << endl;
          if(loDomNode.nodeName() == "operationQuery")
          {
              QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
              if( !loDocElemChild.isNull() )// the node was really an element.
              {
                  if (loDocElemChild.hasAttribute(QString("class")))
                  {
                        meGraphClass = loDocElemChild.attribute("class");
                  }
                  if (loDocElemChild.hasAttribute(QString("property")))
                  {
                        meGraphProperty = loDocElemChild.attribute("property");
                  }
                  if (loDocElemChild.hasAttribute(QString("type")))
                  {
                        meGraphType = loDocElemChild.attribute("type");
                  }
                  if (loDocElemChild.hasAttribute(QString("value")))
                  {
                        meGraphValue = loDocElemChild.attribute("value");
                  }
                  if (loDocElemChild.hasAttribute(QString("refreshMS")))
                  {
                        meGraphRefresh = loDocElemChild.attribute("refreshMS");
                  }				  
                  if (loDocElemChild.hasAttribute(QString("expression")))
                  {
                        meGraphExpression = loDocElemChild.attribute("expression");
                  }						  
              }
          }
          if(loDomNode.nodeName() == "GraphLines")
          {
              QDomElement loDocElemChild = loDomNode.toElement();
              if(!loDocElemChild.isNull())
              {
				//Get the Graphline nodes
				QDomNode loNodeGraphLine = loDocElemChild.firstChild();
				
				QString loName;
				QString loColor;
				
				vector<clGraphLine> loGraphLine;
				
				while(!loNodeGraphLine.isNull())
				{
					
					QDomElement loNodeGraphLineElem = loNodeGraphLine.toElement();
						
					//Get information of the GraphLineNode
					loName = loNodeGraphLineElem.attribute("name");
					loColor = loNodeGraphLineElem.attribute("color");

					QDomNode loNodeGraphLineXY = loNodeGraphLineElem.firstChild();
										
					vector<clQuery> loGraphLineQuerys_X;
					vector<clQuery> loGraphLineQuerys_Y;
					
					while(!loNodeGraphLineXY.isNull())
					{						
						QDomElement loNodeGraphLineXYElem = loNodeGraphLineXY.toElement();
													
						QDomNode loNodeQuery = loNodeGraphLineXYElem.firstChild();
						while(!loNodeQuery.isNull())
						{
							QDomElement loNodeQueryElem = loNodeQuery.toElement();
							clQuery loQuery(loNodeQueryElem.attribute("class"),loNodeQueryElem.attribute("property"),loNodeQueryElem.attribute("type"), loNodeQueryElem.attribute("index").toInt());
							if(loNodeGraphLineXY.nodeName() == "valueQuery_Y")loGraphLineQuerys_Y.push_back(loQuery);
							if(loNodeGraphLineXY.nodeName() == "valueQuery_X")loGraphLineQuerys_X.push_back(loQuery);
							loNodeQuery = loNodeQuery.nextSibling();
						}
						loNodeGraphLineXY = loNodeGraphLineXY.nextSibling();
					}
					clGraphLine loGraphLine(loColor,loName,loGraphLineQuerys_X,loGraphLineQuerys_Y);
					meGraphLines.push_back(loGraphLine);
					
					loGraphLineQuerys_X.clear();
					loGraphLineQuerys_Y.clear();
					
					loNodeGraphLine = loNodeGraphLine.nextSibling();
				}
				
              }
          }		  
          loDomNode = loDomNode.nextSibling();
		}
		return true;
	}
    catch(exception &e)
    {
		meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::readXmlFile() -> " + QString(e.what()));
		return false;
    }
	
}
/************************************************
* Do thread startup
************************************************/
bool clGraphView::addGraphLine(int paLineSeries, vector<QString> paValuesX, QString paTypeX, vector <int> paIndexX, vector<QString> paValuesY, QString paTypeY, vector <int> paIndexY, QString paName, QString paColor)
{
	try
	{		
		
		meSeries[paLineSeries]->clear();
		
		
		if (paTypeX.compare(QString("timestamp(3)")) == 0 && paTypeY.compare(QString("timestamp(3)")) == 0)
		{
			cout << "Chart 1" << endl;
			for (int i=0; i < (int) paValuesX.size(); i++)
			{
				meSeries[paLineSeries]->append((QDateTime::fromString(paValuesX.at(i),"yyyy-MM-dd HH:mm:ss.zzz")).toMSecsSinceEpoch(),(QDateTime::fromString(paValuesY.at(i),"yyyy-MM-dd HH:mm:ss.zzz")).toMSecsSinceEpoch());
			}
		}
		else if (paTypeX.compare(QString("timestamp(3)")) != 0 && paTypeY.compare(QString("timestamp(3)")) == 0)
		{
			cout << "Chart 2" << endl;
			
			
			
			for (int i=0; i < (int) paValuesX.size(); i++)
			{
				bool loIsArrayX = false;
			    bool loIsArrayY = false;
				QString loXvalue;
				QString loYvalue;
				if (paTypeX.toUpper().compare("VARCHAR(64)[]") == 0 ||
					paTypeX.toUpper().compare("VARCHAR(255)[]") == 0 ||
					paTypeX.toUpper().compare("TEXT[]") == 0 ||
					paTypeX.toUpper().compare("TIMESTAMP[]") == 0 ||
					paTypeX.toUpper().compare("TIMESTAMP(3)[]") == 0)
				{
					 QStringList loElements = QString(paValuesX.at(i)).remove("}").remove("{").split(",", QString::SkipEmptyParts);
					 if (loElements.size() > paIndexX.at(i))
						loXvalue = loElements.at(paIndexX.at(i));
					else
						loXvalue = loElements.at(0);
					 loIsArrayX = true;
				}
				else if (paTypeX.toUpper().compare("INT[]") == 0 ||
							paTypeX.toUpper().compare("INT4[]") == 0 ||
							paTypeX.toUpper().compare("INT8[]") == 0 ||
							paTypeX.toUpper().compare("FLOAT4[]") == 0 ||
							paTypeX.toUpper().compare("FLOAT8[]") == 0)
				{
					QStringList loElements = QString(paValuesX.at(i)).remove("}").remove("{").split(",", QString::SkipEmptyParts);					 
					 if (loElements.size() > paIndexX.at(i))
						loXvalue = loElements.at(paIndexX.at(i));
					else
						loXvalue = loElements.at(0);
					loIsArrayX = true;					
				}
				if (paTypeY.toUpper().compare("VARCHAR(64)[]") == 0 ||
					paTypeY.toUpper().compare("VARCHAR(255)[]") == 0 ||
					paTypeY.toUpper().compare("TEXT[]") == 0 ||
					paTypeY.toUpper().compare("TIMESTAMP[]") == 0 ||
					paTypeY.toUpper().compare("TIMESTAMP(3)[]") == 0)
				{
					 QStringList loElements = QString(paValuesY.at(i)).remove("}").remove("{").split(",", QString::SkipEmptyParts);
					 if (loElements.size() > paIndexY.at(i))
						loYvalue = loElements.at(paIndexY.at(i));
					 else
						loYvalue = loElements.at(0);
					 loIsArrayY = true;
				}
				else if (paTypeY.toUpper().compare("INT[]") == 0 ||
							paTypeY.toUpper().compare("INT4[]") == 0 ||
							paTypeY.toUpper().compare("INT8[]") == 0 ||
							paTypeY.toUpper().compare("FLOAT4[]") == 0 ||
							paTypeY.toUpper().compare("FLOAT8[]") == 0)
				{
					QStringList loElements = QString(paValuesY.at(i)).remove("}").remove("{").split(",", QString::SkipEmptyParts);					 
					 if (loElements.size() > paIndexY.at(i))
						loYvalue = loElements.at(paIndexY.at(i));
					 else
						loYvalue = loElements.at(0);
					loIsArrayY = true;
				}
				
				if (!loIsArrayX) loXvalue = paValuesX.at(i);
				if (!loIsArrayY) loYvalue = paValuesY.at(i); 
				meSeries[paLineSeries]->append((qreal)(loXvalue.toDouble()),(QDateTime::fromString(loYvalue,"yyyy-MM-dd HH:mm:ss.zzz")).toMSecsSinceEpoch());	
			}
		}
		else if (paTypeX.compare(QString("timestamp(3)")) == 0 && paTypeY.compare(QString("timestamp(3)")) != 0)
		{
			for (int i=0; i < (int) paValuesX.size(); i++)
			{
			    bool loIsArrayX = false;
			    bool loIsArrayY = false;
				QString loXvalue;
				QString loYvalue;
				if (paTypeX.toUpper().compare("VARCHAR(64)[]") == 0 ||
					paTypeX.toUpper().compare("VARCHAR(255)[]") == 0 ||
					paTypeX.toUpper().compare("TEXT[]") == 0 ||
					paTypeX.toUpper().compare("TIMESTAMP[]") == 0 ||
					paTypeX.toUpper().compare("TIMESTAMP(3)[]") == 0)
				{
					 QStringList loElements = QString(paValuesX.at(i)).remove("}").remove("{").split(",", QString::SkipEmptyParts);
					 if (loElements.size() > paIndexX.at(i))
						loXvalue = loElements.at(paIndexX.at(i));
					else
						loXvalue = loElements.at(0);
					 loIsArrayX = true;
				}
				else if (paTypeX.toUpper().compare("INT[]") == 0 ||
							paTypeX.toUpper().compare("INT4[]") == 0 ||
							paTypeX.toUpper().compare("INT8[]") == 0 ||
							paTypeX.toUpper().compare("FLOAT4[]") == 0 ||
							paTypeX.toUpper().compare("FLOAT8[]") == 0)
				{
					QStringList loElements = QString(paValuesX.at(i)).remove("}").remove("{").split(",", QString::SkipEmptyParts);					 
					if (loElements.size() > paIndexX.at(i))
						loXvalue = loElements.at(paIndexX.at(i));
					else
						loXvalue = loElements.at(0);
					loIsArrayX = true;					
				}
				if (paTypeY.toUpper().compare("VARCHAR(64)[]") == 0 ||
					paTypeY.toUpper().compare("VARCHAR(255)[]") == 0 ||
					paTypeY.toUpper().compare("TEXT[]") == 0 ||
					paTypeY.toUpper().compare("TIMESTAMP[]") == 0 ||
					paTypeY.toUpper().compare("TIMESTAMP(3)[]") == 0)
				{
					 QStringList loElements = QString(paValuesY.at(i)).remove("}").remove("{").split(",", QString::SkipEmptyParts);
					 if (loElements.size() > paIndexY.at(i))
						loYvalue = loElements.at(paIndexY.at(i));
					 else
						loYvalue = loElements.at(0);
					 loIsArrayY = true;
				}
				else if (paTypeY.toUpper().compare("INT[]") == 0 ||
							paTypeY.toUpper().compare("INT4[]") == 0 ||
							paTypeY.toUpper().compare("INT8[]") == 0 ||
							paTypeY.toUpper().compare("FLOAT4[]") == 0 ||
							paTypeY.toUpper().compare("FLOAT8[]") == 0)
				{
					QStringList loElements = QString(paValuesY.at(i)).remove("}").remove("{").split(",", QString::SkipEmptyParts);					 			    					
					 if (loElements.size() > paIndexY.at(i))
						loYvalue = loElements.at(paIndexY.at(i));
					 else
						loYvalue = loElements.at(0);					
					loIsArrayY = true;
				}
				
				if (!loIsArrayX) loXvalue = paValuesX.at(i);
				if (!loIsArrayY) loYvalue = paValuesY.at(i);
				//meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe",QString("clGraphView::addGraphLine() appended values-> %1 %2").arg(loXvalue).arg(loYvalue));
				meSeries[paLineSeries]->append((QDateTime::fromString(loXvalue,"yyyy-MM-dd HH:mm:ss.zzz")).toMSecsSinceEpoch(),(qreal)(loYvalue.toDouble()));	
			}
		}
		else if (paTypeX.compare(QString("timestamp(3)")) != 0 && paTypeY.compare(QString("timestamp(3)")) != 0)
		{
			cout << "Chart 4" << endl;
			for (int i=0; i < paValuesX.size(); i++)
			{
				bool loIsArrayX = false;
			    bool loIsArrayY = false;
				QString loXvalue;
				QString loYvalue;
				if (paTypeX.toUpper().compare("VARCHAR(64)[]") == 0 ||
					paTypeX.toUpper().compare("VARCHAR(255)[]") == 0 ||
					paTypeX.toUpper().compare("TEXT[]") == 0 ||
					paTypeX.toUpper().compare("TIMESTAMP[]") == 0 ||
					paTypeX.toUpper().compare("TIMESTAMP(3)[]") == 0)
				{
					 QStringList loElements = QString(paValuesX.at(i)).remove("}").remove("{").split(",", QString::SkipEmptyParts);
					 if (loElements.size() > paIndexX.at(i))
						loXvalue = loElements.at(paIndexX.at(i));
					 else
						loXvalue = loElements.at(0);
					 loIsArrayX = true;
				}
				else if (paTypeX.toUpper().compare("INT[]") == 0 ||
							paTypeX.toUpper().compare("INT4[]") == 0 ||
							paTypeX.toUpper().compare("INT8[]") == 0 ||
							paTypeX.toUpper().compare("FLOAT4[]") == 0 ||
							paTypeX.toUpper().compare("FLOAT8[]") == 0)
				{
					QStringList loElements = QString(paValuesX.at(i)).remove("}").remove("{").split(",", QString::SkipEmptyParts);					 
					if (loElements.size() > paIndexX.at(i))
						loXvalue = loElements.at(paIndexX.at(i));
					else
						loXvalue = loElements.at(0);
					loIsArrayX = true;					
				}
				if (paTypeY.toUpper().compare("VARCHAR(64)[]") == 0 ||
					paTypeY.toUpper().compare("VARCHAR(255)[]") == 0 ||
					paTypeY.toUpper().compare("TEXT[]") == 0 ||
					paTypeY.toUpper().compare("TIMESTAMP[]") == 0 ||
					paTypeY.toUpper().compare("TIMESTAMP(3)[]") == 0)
				{
					 QStringList loElements = QString(paValuesY.at(i)).remove("}").remove("{").split(",", QString::SkipEmptyParts);
					 if (loElements.size() > paIndexY.at(i))
						loYvalue = loElements.at(paIndexY.at(i));
					 else
						loYvalue = loElements.at(0);
					 loIsArrayY = true;
				}
				else if (paTypeY.toUpper().compare("INT[]") == 0 ||
							paTypeY.toUpper().compare("INT4[]") == 0 ||
							paTypeY.toUpper().compare("INT8[]") == 0 ||
							paTypeY.toUpper().compare("FLOAT4[]") == 0 ||
							paTypeY.toUpper().compare("FLOAT8[]") == 0)
				{
					QStringList loElements = QString(paValuesY.at(i)).remove("}").remove("{").split(",", QString::SkipEmptyParts);					 
					 if (loElements.size() > paIndexY.at(i))
						loYvalue = loElements.at(paIndexY.at(i));
					 else
						loYvalue = loElements.at(0);
					loIsArrayY = true;
				}
				
				if (!loIsArrayX) loXvalue = paValuesX.at(i);
				if (!loIsArrayY) loYvalue = paValuesY.at(i);				
				meSeries[paLineSeries]->append((qreal)(loXvalue.toDouble()),(qreal)(loYvalue.toDouble()));	
			}			
		}
		return true;
	}
    catch(exception &e)
    {
		meIceClientLogging->insertItem("70",QString(QHostInfo::localHostName()),"2UVGraphClient.exe","clGraphView::addGraphLine() -> " + QString(e.what()));
		return false;
    }
}
