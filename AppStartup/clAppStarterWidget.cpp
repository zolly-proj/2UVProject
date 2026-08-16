#include "clAppStarterWidget.h"
//Constructors
clAppStarterWidget::clAppStarterWidget(char * paIniFile, QWidget *paParent, const char *paName)
{
	readXmlFile(paIniFile);
	initializeUI();


	meTimer = new QTimer(this);
	meTimer->setInterval(1000);
	meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotCheckProcesses()));
	meTimer->start();
}
//Destructors
clAppStarterWidget::~clAppStarterWidget()
{

}

void clAppStarterWidget::slotCheckProcesses()
{
	for (int i = 0; i < meName.size(); i++)
	{
		if (meProcess[i] != NULL)
		{
			if(meProcess[i]->processId() > 0)
			{
				mePid[i]->setText(QString::number(meProcess[i]->processId()));
				mePixmap[i] = mePixmapStart;	
				meProgramStatus[i]->setPixmap(mePixmap[i]->scaled(50,50));					
			}
			else
			{
				mePid[i]->setText(QString::number(meProcess[i]->processId()));
				mePixmap[i] = mePixmapStop;
				meProgramStatus[i]->setPixmap(mePixmap[i]->scaled(50,50));									
			}
		}
		else
		{
			mePixmap[i] = mePixmapStop;
			meProgramStatus[i]->setPixmap(mePixmap[i]->scaled(50,50));	
		}
	}
}

bool clAppStarterWidget::readXmlFile(char * paFileName)
{
    try
    {
        //padatbaseClass = clDatabaseClass(paFileName);
        QString loFileName(paFileName);
		
        QFile loFile(loFileName);
        if ( !loFile.open( QIODevice::ReadOnly ) ) {
            QString loTemp("clAppStarterWidget::readXmlFile() -> Cound not open file '" + loFileName + "' ...");
			cout << loTemp.toStdString(); 
            
            return false;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) {
			QString loTemp("clAppStarterWidget::readXmlFile() -> Cound not open file '" + loFileName + "' ...");
            cout << (loTemp.toStdString()); 
            
            loFile.close();
            return false;
        }

        // create the tree view out of the DOM
       QDomElement loDocElem = loDomDocument.documentElement();

       QDomNode loDomNode = loDocElem.firstChild();
       while( !loDomNode.isNull() ) {

          if(loDomNode.nodeName() == "process")
          {
              QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
              if( !loDocElemChild.isNull() )// the node was really an element.
              {
                  if (loDocElemChild.hasAttribute(QString("name")))
                  {
                        //QString loValue = loDocElemChild.attribute(paAttribute);
                        //paValue = &loValue;
                        QString paValue = loDocElemChild.attribute("name");
                        meName.push_back(paValue);
						
						QDomNode loProcessNode = loDocElemChild.firstChild();
						while (! loProcessNode.isNull() )
						{
						    if(loProcessNode.nodeName() == "command")
							{	
							    QDomElement loProcessNodeElem = loProcessNode.toElement(); // try to convert the node to an element.
								meProgram.push_back(loProcessNodeElem.text());
							}
						    if(loProcessNode.nodeName() == "arg")
							{	
								QDomElement loProcessNodeElem = loProcessNode.toElement(); // try to convert the node to an element.
								meArgument.push_back(loProcessNodeElem.text());
							}
						    if(loProcessNode.nodeName() == "working_dir")
							{	
								QDomElement loProcessNodeElem = loProcessNode.toElement(); // try to convert the node to an element.
								meWorkingDir.push_back(loProcessNodeElem.text());
							}
							loProcessNode = loProcessNode.nextSibling();
						}
                  }
              }
          }
          loDomNode = loDomNode.nextSibling();
       }
       return true;
    }
    catch(...)
    {
        QString loTemp("clAppStarterWidget::readXmlFile() -> error ...");
        cout << (loTemp.toStdString()); 
        return false;
    }
}




/****************************
* UI ACTIONS
****************************/
void clAppStarterWidget::initializeUI()
{
	try
	{
		for (int i = 0; i < 100; i++)
		{
			mePrograms[i] = NULL;
			meProgramStatus[i] = NULL;
			meStartProgram[i] = NULL;
			meStopProgram[i] = NULL;
			mePid[i] = NULL;
			mePixmap[i] = NULL;
			meProcess[i] = NULL;
		}
		
	
		meMainWidget = new QWidget();	
		meMainLayout = new QVBoxLayout();
		
		//Label
		QImage imHeader("./img/2UV_01.png");
		QPixmap * loPixmap = new QPixmap(QPixmap::fromImage(imHeader));
		meHeader = new QLabel;
		meHeader->setPixmap(loPixmap->scaled(200,150));			
		meMainLayout->addWidget(meHeader);

		//Create a widget and set its layout as your new layout created above
		layout = new QGridLayout;
		
		QImage loStart("./img/start.png");
		QImage loStop("./img/stop.png");
		imStart = loStart;
		imStop = loStop;
		mePixmapStart = new QPixmap(QPixmap::fromImage(imStart));	
		mePixmapStop = new QPixmap(QPixmap::fromImage(imStop));	
		
		for (int i = 0; i < meName.size(); i++)
		{
			mePrograms[i] = new QLabel(meName.at(i));
			meProgramStatus[i] = new QLabel;
		
			mePixmap[i] = new QPixmap(QPixmap::fromImage(imStop));	
			meProgramStatus[i]->setPixmap(mePixmap[i]->scaled(50,50));
			meStartProgram[i] = new QPushButton("Start [" + QString::number(i) + "]");
			meStopProgram[i] = new QPushButton("Stop [" + QString::number(i) + "]");
			connect(meStartProgram[i], SIGNAL(clicked()), this, SLOT(startPressed()));
			connect(meStopProgram[i], SIGNAL(clicked()), this, SLOT(stopPressed()));
			mePid[i] = new QLabel("0000");
			
			layout->addWidget(meProgramStatus[i], i + 1,0);
			layout->addWidget(mePrograms[i], i + 1,1);
			layout->addWidget(meStartProgram[i], i + 1,2);
			layout->addWidget(meStopProgram[i], i + 1,3);
			layout->addWidget(mePid[i], i + 1,4);
		}		
	
		viewport = new QWidget;
		viewport->setLayout(layout);
		//Add the viewport to the scroll area
		scrollArea = new QScrollArea;
		scrollArea->setWidget(viewport);
		meMainLayout->addWidget(scrollArea);
		
		meClose = new QPushButton("Close");
		connect(meClose, SIGNAL(clicked()),this,SLOT(slotClose()));
		meMainLayout->addWidget(meClose);
		
		meMainWidget->setLayout(meMainLayout);
		
		this->setCentralWidget(meMainWidget);
		
	}
	catch(...)
	{
		
	}
}

 void clAppStarterWidget::stopPressed()
 {
     QPushButton* pButton = qobject_cast<QPushButton*>(sender());
     if (pButton) // this is the type we expect
     {
         QString buttonText = pButton->text();
         // recognize buttonText here
		 QStringList loList = buttonText.split("[");
		 QString number = loList.at(1);
		 int loId = number.replace("]","").toInt();
		 cout << number.toStdString();
		 stopProcess(loId);
     }
 }
 void clAppStarterWidget::startPressed()
 {
     QPushButton* pButton = qobject_cast<QPushButton*>(sender());
     if (pButton) // this is the type we expect
     {
         QString buttonText = pButton->text();
         // recognize buttonText here
		 QStringList loList = buttonText.split("[");
		 QString number = loList.at(1);
		 int loId = number.replace("]","").toInt();
		 cout << number.toStdString();
		 startProcess(loId);
     }
 }
bool clAppStarterWidget::stopProcess(int paId)
{
	try
	{
		if (meProcess[paId] != NULL)
		{
			meProcess[paId]->close();
		}
		return true;
	}
	catch(...)
	{
		cout << "clAppStarterWidget::stopProcess()-> error ...";
		return false;		
	}
}
bool clAppStarterWidget::startProcess(int paId)
{
	try
	{
		if (meProcess[paId] == NULL)
		{
			meProcess[paId] = new QProcess(this);
			meProcess[paId]->setWorkingDirectory(meWorkingDir[paId]);
			QString program = meProgram[paId];
			QString arg = meArgument[paId];
			meProcess[paId]->start(program, QStringList() << arg);					
		}
		else
		{
			meProcess[paId]->close();
			//meProcess[paId] = new QProcess(this);
			meProcess[paId]->setWorkingDirectory(meWorkingDir[paId]);
			QString program = meProgram[paId];
			QString arg = meArgument[paId];
			meProcess[paId]->start(program, QStringList() << arg);			
		}
		return true;
	}
	catch(...)
	{
		cout << "clAppStarterWidget::startProcess()-> error ...";
		return false;
	}
}

void clAppStarterWidget::slotClose()
{
	this->close();
}