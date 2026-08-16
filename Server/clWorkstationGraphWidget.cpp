#include "clWorkstationGraphWidget.h"

clWorkstationGraphWidget::clWorkstationGraphWidget(clIceClientLogging * paIceClientLogging, vector <clWorkstationStatus> * paWorkstationStatus) : QChartView()
{
    try
    {
        //this->resize(682,297);
		//horizontalScrollBar()->setRange(0, 0);
		//verticalScrollBar()->setRange(0, 0);
		
		
		
		
		//this->setParent(parent);
		meIceClientLogging = paIceClientLogging;
		meWorkstationStatus = paWorkstationStatus;
		/*
        doSetup(meThread);
        moveToThread(&meThread);
        meThread.start();
		*/
		//meGraphicsItem = new QGraphicsItem(this);
		
		meChart = new QChart();
		//QWidget * object = qobject_cast< QWidget * >(this);
		//meChart = new QChart(object);
		//meChart->setParent(object);
	
		
		
		
		
		
		
		setUpWidget();
		setTimingsInGraph();
		/*
		meTimer = new QTimer(this);
        meTimer->setInterval(1000);
        meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotDoIt()));
        meTimer->start();
		*/
		

		
		
    }
    catch(exception &e)
    {
		meIceClientLogging->insertItem("1","2UVLogServer.exe","clWorkstationGraphWidget::clWorkstationGraphWidget ->" + QString(e.what()));
    }
}

clWorkstationGraphWidget::~clWorkstationGraphWidget ()
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
/*****************************
* UI functions
*****************************/
void clWorkstationGraphWidget::setUpWidget()
{
    try
    {
		/*
		this.setupUi(meChartView);
        meChartView->show();
		*/
    }
    catch(...)
    {
    }
}
/************************************************
* Do thread startup
************************************************/
/*
void clWorkstationGraphWidget::slotDoIt()
{
    try
    {
		printf("START clConfigProductWidget::slotDoIt");
		setTimingsInGraph();
		printf("END clConfigProductWidget::slotDoIt");
    }
    catch (...)
    {
    }
}
*/
void clWorkstationGraphWidget::setTimingsInGraph()
{
	try
	{
		QLineSeries * meSeries = new QLineSeries;
		
		//meChartView->hide();
	    // Customize chart title
		QFont font;
		font.setPixelSize(18);
		meChart->setTitleFont(font);
		meChart->setTitleBrush(QBrush(Qt::blue));
			
		
		
		//We moeten de vector copieren naar een vaste variabele
		vector <clWorkstationStatus> &loWorkstationStatus  = *meWorkstationStatus;



		meChart->setTitle((QString)((clWorkstationStatus)loWorkstationStatus[0]).getWorkstationName());

		//loWorkstationStatus = &meWorkstationStatus;
		//meChart->addSeries(meSeries);
		//meChart->legend()->hide();
		//meChart->setTitle(((clWorkstationStatus)meWorkstationStatus->at(0)).getWorkstationName());
		//meChart->setTitle(((clWorkstationStatus)loWorkstationStatus[0]).getWorkstationName());
		
		for (int i=0; i < loWorkstationStatus.size(); i++)
		{
			meIceClientLogging->insertItem("1","2UVLogServer.exe","clWorkstationGraphWidget::setTimingsInGraph ->" + QString("loWorkstationStatus[i]).getTimeStamp() [") + QString::number(((QDateTime)((clWorkstationStatus)loWorkstationStatus[i]).getTimeStamp()).toMSecsSinceEpoch()) + QString("] with value [" + QString::number(((qreal)((clWorkstationStatus)loWorkstationStatus[i]).getPercentageCpuUsage().toLong())) + QString("]")));
			meSeries->append(((QDateTime)((clWorkstationStatus)loWorkstationStatus[i]).getTimeStamp()).toMSecsSinceEpoch(),((qreal)((clWorkstationStatus)loWorkstationStatus[i]).getPercentageCpuUsage().toDouble()));	
		}
		meChart->removeAllSeries();
		meChart->addSeries(meSeries);
		
		QDateTimeAxis *axisX = new QDateTimeAxis;
		axisX->setFormat("dd-MM-yyyy h:mm:ss");
		
		QValueAxis *axisY = new QValueAxis;
		axisY->setMin(0);
		
		meChart->setAxisX(axisX, meSeries);
		meChart->setAxisY(axisY, meSeries);
	
		this->setChart(meChart);
		this->setRenderHint(QPainter::Antialiasing);
		//this->show();
	}
    catch(exception &e)
    {
		meIceClientLogging->insertItem("1","2UVLogServer.exe","clWorkstationGraphWidget::setTimingsInGraph ->" + QString(e.what()));
    }
}