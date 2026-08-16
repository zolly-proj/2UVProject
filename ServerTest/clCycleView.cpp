#define INFO_BUFFER_SIZE 32767
#include "clCycleView.h"

clCycleView::clCycleView(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clWorkstationCycles * paWorkstationCycles, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;
	meWorkstationCycles = paWorkstationCycles;	
	
	initializeUI();

	meTimer = new QTimer(this);
    meTimer->setInterval(100);
    meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotUpdateValues()));
    meTimer->start();		
}

clCycleView::~clCycleView()
{
}
void clCycleView::slotUpdateValues()
{
	try
	{
		for (int i = 0; i < 20; i++)
		{
			if (meWorkstationCycles->meWorkstationCycle[i] != NULL)
			{
				meLabels[i][1]->setText(meWorkstationCycles->meWorkstationCycle[i]->meObjectId);
				meLabels[i][6]->setText(meWorkstationCycles->meWorkstationCycle[i]->meCurrentCycleRoutineUUID);
				meLabels[i][8]->setText(meWorkstationCycles->meWorkstationCycle[i]->meCurrentRoutineClass);
				meLabels[i][10]->setText(meWorkstationCycles->meWorkstationCycle[i]->meCurrentRoutineUUID);
				meLabels[i][12]->setText(meWorkstationCycles->meWorkstationCycle[i]->meCurrentRoutineName);
				meLabels[i][14]->setText(meWorkstationCycles->meWorkstationCycle[i]->meCurrentRoutineSourceType);
				meLabels[i][16]->setText(meWorkstationCycles->meWorkstationCycle[i]->meCurrentRoutineSourceFile);
				meLabels[i][18]->setText(meWorkstationCycles->meWorkstationCycle[i]->meCurrentRoutineReturnType);
				/*
				meLabels[i][19] = new QLabel(QString("Arguements: "));
				meLabels[i][20] = new QLabel(QString(""));
				*/
				meLabels[i][22]->setText(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meState));
				meLabels[i][24]->setText(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meDebug));				
				meLabels[i][26]->setText(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meResultFromFunction));
				meLabels[i][28]->setText(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meRunning));				
				meLabels[i][30]->setText(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meTimeOutFromCycle));				
				meLabels[i][32]->setText(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meException));				
				meLabels[i][34]->setText(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meQuit));	
			}
		}
	}
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clCycleView::initializeUI() -> " + QString(e.what()));
    }
}
void clCycleView::initializeUI()
{
	try
	{

		layout = new QGridLayout;


		meMainLayout = new QHBoxLayout;
		//Create a widget and set its layout as your new layout created above
		viewport = new QWidget;
		viewport->setLayout(layout);

		//Add the viewport to the scroll area
		scrollArea = new QScrollArea;
		scrollArea->setWidgetResizable (true);
		scrollArea->setWidget(viewport);

		meMainLayout->addWidget(scrollArea);
		this->setLayout(meMainLayout);

		//Initialize elements
        for(int i = 0; i < 20; i++)
        {
			for (int j = 0; j < NumGridRows; j++)
			{
				meLabels[i][j] = NULL;
				meDateTimeEdit[i][j] = NULL;
				meLineEdits[i][j] = NULL;
						
			}
			meButtons[i] = NULL;
			meSpinBox[i] = NULL;
			meLabelsGeneral[i] = NULL;
        }

		//Check the amount of running cycles for this computer
		for (int i = 0; i < 20; i++)
		{
			if (meWorkstationCycles->meWorkstationCycle[i] != NULL)
			{
				meLabels[i][0] = new QLabel(QString("CYCLES[" +QString::number(i) + "]: "));
				meLabels[i][1] = new QLabel(meWorkstationCycles->meWorkstationCycle[i]->meObjectId);
				meLabels[i][2] = new QLabel(meWorkstationCycles->meWorkstationCycle[i]->meCycleName);
				
				meLabels[i][3] = new QLabel(QString("Workstation name: "));
				meLabels[i][4] = new QLabel(meWorkstationCycles->meWorkstationCycle[i]->meCycleWorkstation);
				
				meLabels[i][5] = new QLabel(QString("CYCLE_ROUTINE: "));
				meLabels[i][6] = new QLabel(meWorkstationCycles->meWorkstationCycle[i]->meCurrentCycleRoutineUUID);

				meLabels[i][7] = new QLabel(QString("Routine Class: "));
				meLabels[i][8] = new QLabel(meWorkstationCycles->meWorkstationCycle[i]->meCurrentRoutineClass);
				meLabels[i][9] = new QLabel(QString("Routine UUID: "));
				meLabels[i][10] = new QLabel(meWorkstationCycles->meWorkstationCycle[i]->meCurrentRoutineUUID);
				meLabels[i][11] = new QLabel(QString("Routine name: "));
				meLabels[i][12] = new QLabel(meWorkstationCycles->meWorkstationCycle[i]->meCurrentRoutineName);
				meLabels[i][13] = new QLabel(QString("Source Type: "));
				meLabels[i][14] = new QLabel(meWorkstationCycles->meWorkstationCycle[i]->meCurrentRoutineSourceType);
				meLabels[i][15] = new QLabel(QString("Source File: "));
				meLabels[i][16] = new QLabel(meWorkstationCycles->meWorkstationCycle[i]->meCurrentRoutineSourceFile);
				meLabels[i][17] = new QLabel(QString("Return Type: "));
				meLabels[i][18] = new QLabel(meWorkstationCycles->meWorkstationCycle[i]->meCurrentRoutineReturnType);
				meLabels[i][19] = new QLabel(QString("Arguements: "));
				meLabels[i][20] = new QLabel(QString(""));
				
				meLabels[i][21] = new QLabel(QString("State: "));
				meLabels[i][22] = new QLabel(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meState));
				meLabels[i][23] = new QLabel(QString("Debug: "));
				meLabels[i][24] = new QLabel(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meDebug));				
				meLabels[i][25] = new QLabel(QString("ResultFromFunction: "));
				meLabels[i][26] = new QLabel(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meResultFromFunction));
				meLabels[i][27] = new QLabel(QString("Running: "));
				meLabels[i][28] = new QLabel(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meRunning));				
				meLabels[i][29] = new QLabel(QString("TimeOutFromCycle: "));
				meLabels[i][30] = new QLabel(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meTimeOutFromCycle));				
				meLabels[i][31] = new QLabel(QString("Exeption: "));
				meLabels[i][32] = new QLabel(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meException));				
				meLabels[i][33] = new QLabel(QString("Quit: "));
				meLabels[i][34] = new QLabel(QString::number(meWorkstationCycles->meWorkstationCycle[i]->meQuit));
										
				layout->addWidget(meLabels[i][0], 1, 0 + (i * 3));
				layout->addWidget(meLabels[i][1], 1, 1 + (i * 3));
				
				layout->addWidget(meLabels[i][2], 2, 1 + (i * 3));
				layout->addWidget(meLabels[i][3], 3, 0 + (i * 3));
				layout->addWidget(meLabels[i][4], 3, 1 + (i * 3));
				layout->addWidget(meLabels[i][5], 4, 0 + (i * 3));
				layout->addWidget(meLabels[i][6], 4, 1 + (i * 3));
				layout->addWidget(meLabels[i][7], 5, 0 + (i * 3));
				layout->addWidget(meLabels[i][8], 5, 1 + (i * 3));
				layout->addWidget(meLabels[i][9], 6, 0 + (i * 3));
				layout->addWidget(meLabels[i][10], 6, 1 + (i * 3));
				layout->addWidget(meLabels[i][11], 7, 0 + (i * 3));
				layout->addWidget(meLabels[i][12], 7, 1 + (i * 3));
				layout->addWidget(meLabels[i][13], 8, 0 + (i * 3));
				layout->addWidget(meLabels[i][14], 8, 1 + (i * 3));
				layout->addWidget(meLabels[i][15], 9, 0 + (i * 3));
				layout->addWidget(meLabels[i][16], 9, 1 + (i * 3));
				layout->addWidget(meLabels[i][17], 10, 0 + (i * 3));
				layout->addWidget(meLabels[i][18], 10, 1 + (i * 3));
				layout->addWidget(meLabels[i][19], 11, 0 + (i * 3));
				layout->addWidget(meLabels[i][20], 11, 1 + (i * 3));
				
				layout->addWidget(meLabels[i][21], 12, 0 + (i * 3));
				layout->addWidget(meLabels[i][22], 12, 1 + (i * 3));
				layout->addWidget(meLabels[i][23], 13, 0 + (i * 3));
				layout->addWidget(meLabels[i][24], 13, 1 + (i * 3));
				layout->addWidget(meLabels[i][25], 14, 0 + (i * 3));
				layout->addWidget(meLabels[i][26], 14, 1 + (i * 3));
				layout->addWidget(meLabels[i][27], 15, 0 + (i * 3));
				layout->addWidget(meLabels[i][28], 15, 1 + (i * 3));
				layout->addWidget(meLabels[i][29], 16, 0 + (i * 3));
				layout->addWidget(meLabels[i][30], 16, 1 + (i * 3));
				layout->addWidget(meLabels[i][31], 17, 0 + (i * 3));
				layout->addWidget(meLabels[i][32], 17, 1 + (i * 3));
				layout->addWidget(meLabels[i][33], 18, 0 + (i * 3));
				layout->addWidget(meLabels[i][34], 18, 1 + (i * 3));
				
		
				
				
			}
		}
		
		meButtons[0] = new QPushButton(QString("Start"));
		meButtons[1] = new QPushButton(QString("Stop"));
		meButtons[2] = new QPushButton(QString("Debug ON/OFF"));
		meButtons[3] = new QPushButton(QString("Reset exception"));
		meButtons[4] = new QPushButton(QString("Set delay"));
		
		meSpinBox[0] = new QDoubleSpinBox();
		meSpinBox[0]->setRange(0, 19);
		meSpinBox[0]->setSingleStep(1);
		meSpinBox[0]->setValue(0);
		
		
		meLabelsGeneral[0] = new QLabel(QString("Timing for cycle: "));
		
		meSpinBox[1] = new QDoubleSpinBox();
		meSpinBox[1]->setRange(0, 200000);
		meSpinBox[1]->setSingleStep(1);
		meSpinBox[1]->setValue(50);		
		
		
		connect(meButtons[0], SIGNAL(clicked()), this, SLOT(slotButtonStartPressed()));
		connect(meButtons[1], SIGNAL(clicked()), this, SLOT(slotButtonStopPressed()));
		connect(meButtons[2], SIGNAL(clicked()), this, SLOT(slotButtonDebugPressed()));
		connect(meButtons[3], SIGNAL(clicked()), this, SLOT(slotButtonResetPressed()));
		connect(meButtons[4], SIGNAL(clicked()), this, SLOT(slotButtonSetDelayPressed()));
		
		layout->addWidget(meSpinBox[0], 19, 0);
		layout->addWidget(meLabelsGeneral[0], 20, 0);
		layout->addWidget(meSpinBox[1], 20, 1);
		layout->addWidget(meButtons[4], 20, 2);
		
		layout->addWidget(meButtons[0], 21, 1);
		layout->addWidget(meButtons[1], 21, 2);
		layout->addWidget(meButtons[2], 21, 3);
		layout->addWidget(meButtons[3], 21, 4);






	}
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clCycleView::initializeUI() -> " + QString(e.what()));
    }
}




void clCycleView::readMappingIcons()
{
	try
	{
		QFile inputFile(".\\ICONS\\mapping.txt");
		if (inputFile.open(QIODevice::ReadOnly))
		{
		   QTextStream in(&inputFile);
		   while (!in.atEnd())
		   {
			  QString line = in.readLine();
			  QStringList loElements = line.split(";");
			  if (loElements.size() != 0)
			  {
				  if (loElements.size() == 2)
				  {
					  meIcon.push_back(loElements.at(0));
					  meClass.push_back(loElements.at(1));
				  }
			  }
		   }
		   inputFile.close();
		}
	}
	catch(...)
	{
			cout << "Mapping problem ..." << endl;
	}
}
void clCycleView::slotButtonSetDelayPressed()
{
	try
	{
		if (meWorkstationCycles->meWorkstationCycle[(int)meSpinBox[0]->value()] != NULL)
			meWorkstationCycles->meWorkstationCycle[(int)meSpinBox[0]->value()]->meTimeOutFromCycle = (int) meSpinBox[1]->value();
	}
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clCycleView::slotButtonSetDelayPressed() -> " + QString(e.what()));
    }	
}

void clCycleView::slotButtonStartPressed()
{
	try
	{
		if (meWorkstationCycles->meWorkstationCycle[(int)meSpinBox[0]->value()] != NULL)
			meWorkstationCycles->meWorkstationCycle[(int)meSpinBox[0]->value()]->start();
	}
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clCycleView::slotButtonStartPressed() -> " + QString(e.what()));
    }
}
void clCycleView::slotButtonStopPressed()
{
	try
	{
			if (meWorkstationCycles->meWorkstationCycle[(int)meSpinBox[0]->value()] != NULL)
				meWorkstationCycles->meWorkstationCycle[(int)meSpinBox[0]->value()]->meQuit = true;
	}
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clCycleView::slotButtonStopPressed() -> " + QString(e.what()));
    }
}
void clCycleView::slotButtonDebugPressed()
{
	try
	{
			if (meWorkstationCycles->meWorkstationCycle[(int)meSpinBox[0]->value()] != NULL)
				if (meWorkstationCycles->meWorkstationCycle[(int)meSpinBox[0]->value()]->meDebug)
					meWorkstationCycles->meWorkstationCycle[(int)meSpinBox[0]->value()]->meDebug = false;
				else
					meWorkstationCycles->meWorkstationCycle[(int)meSpinBox[0]->value()]->meDebug = true;
	}
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clCycleView::slotButtonDebugPressed() -> " + QString(e.what()));
    }
}
void clCycleView::slotButtonResetPressed()
{
	try
	{
			if (meWorkstationCycles->meWorkstationCycle[(int)meSpinBox[0]->value()] != NULL)
				meWorkstationCycles->meWorkstationCycle[(int)meSpinBox[0]->value()]->meException = false;
	}
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clCycleView::slotButtonResetPressed() -> " + QString(e.what()));
    }
}
