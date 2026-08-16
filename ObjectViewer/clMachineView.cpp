#include "clMachineView.h"

clMachineView::clMachineView(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clClassLoader *paClassLoader, QString paConfigurationFile, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;

    meMachineView.setupUi(this);
	
	meClassLoader = paClassLoader;

	readConfigFile();
	
	generateWidgetsFromConfigFile();

}
clMachineView::~clMachineView()
{
}

bool clMachineView::generateWidgetsFromConfigFile()
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

		for (int i = 0; i < meStructWidget.size(); i++)
		{
			if (meStructWidget.at(i).name.toUpper().compare("MACHINE_VIEW_STATE") == 0)
			{
				clMachineViewStateWidget * loMachineViewStateWidget = new clMachineViewStateWidget(meIceClientServer, meIceClientLogging, meClassLoader, meStructWidget.at(i));
				layout->addWidget(loMachineViewStateWidget, meStructWidget.at(i).row.toInt(), meStructWidget.at(i).coll.toInt());
			}
			if (meStructWidget.at(i).name.toUpper().compare("MACHINE_VIEW_CONTENT") == 0)
			{
			
			}
			if (meStructWidget.at(i).name.toUpper().compare("MACHINE_VIEW_GRAPH") == 0)
			{
				clMachineViewGraphWidget * loMachineViewGraphWidget = new clMachineViewGraphWidget(meIceClientServer, meIceClientLogging, meClassLoader, meStructWidget.at(i));
				layout->addWidget(loMachineViewGraphWidget, meStructWidget.at(i).row.toInt(), meStructWidget.at(i).coll.toInt());	
			}
			if (meStructWidget.at(i).name.toUpper().compare("MACHINE_VIEW_LOC") == 0)
			{
			
			} 
		}
		
		meMachineView.gridLayout->addWidget(scrollArea,0,0);
		//this->setLayout(meMainLayout);		
		return true;
	}
	catch(exception &e)
    {	
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clMachineView::generateWidgetsFromConfigFile -> " + QString(e.what()));
		return false;
    }
}


/******************************
* XML Reading function
*******************************/
bool clMachineView::readConfigFile()
{
    try
    {
        QString loFileName = meConfigFileName;

        QFile loFile(loFileName);
        if ( !loFile.open( QIODevice::ReadOnly ) ) {
            QString loTemp("clMachineView::readConfigFile could not open ./CONFIG/machineView_01.xml");
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectView.exe",loTemp);
            return false;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) {
            QString loTemp("clMachineView::readConfigFile could not read content ./CONFIG/machineView_01.xml");
            meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectView.exe",loTemp);
            loFile.close();
            return false;
        }

        // create the tree view out of the DOM
       QDomElement loDocElem = loDomDocument.documentElement();

	/*
      <root>
		<widget name="MACHINE_VIEW_STATE" row="0" coll="0" object_id="" width="200" height="150">
			<picture value="./machine.png"/>
		</widget>
	*/


       QDomNode loDomNode = loDocElem.firstChild();
       while( !loDomNode.isNull() ) {
			
		  struct_widget loStructWidget;
          if(loDomNode.nodeName() == "widget")
          {
              QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
              if( !loDocElemChild.isNull() )// the node was really an element.
              {
                  if (loDocElemChild.hasAttribute(QString("name")))
                  {
                        loStructWidget.name = loDocElemChild.attribute("name");   
                  }
                  if (loDocElemChild.hasAttribute(QString("row")))
                  {
                        loStructWidget.row = loDocElemChild.attribute("row");   
                  }
                  if (loDocElemChild.hasAttribute(QString("coll")))
                  {
                        loStructWidget.coll = loDocElemChild.attribute("coll");   
                  }
                  if (loDocElemChild.hasAttribute(QString("object_id")))
                  {
                        loStructWidget.object_id = loDocElemChild.attribute("object_id");   
                  }
                  if (loDocElemChild.hasAttribute(QString("object_class")))
                  {
                        loStructWidget.object_class = loDocElemChild.attribute("object_class");   
                  }                  
                  if (loDocElemChild.hasAttribute(QString("width")))
                  {
                        loStructWidget.width = loDocElemChild.attribute("width");   
                  }
                  if (loDocElemChild.hasAttribute(QString("height")))
                  {
                        loStructWidget.height = loDocElemChild.attribute("height");   
                  }                                               
                  if (loDocElemChild.hasAttribute(QString("refresh")))
                  {
                        loStructWidget.refresh = loDocElemChild.attribute("refresh");   
                  }          				  
				  QDomNode loDomSubNode = loDocElemChild.firstChild();
				  
				  while( !loDomSubNode.isNull() ) 
				  {
						QDomElement loDocElemSubChild = loDomSubNode.toElement();		
						if(loDomSubNode.nodeName() == "picture")
						{				  
							if (loDocElemSubChild.hasAttribute(QString("value")))
							{
									loStructWidget.value = loDocElemSubChild.attribute("value");   
							}         			
						}
						if(loDomSubNode.nodeName() == "methods")
						{				  
							QDomNode loDomMethodsNode = loDocElemSubChild.firstChild();
							while ( !loDomMethodsNode.isNull() )
							{
								struct_method loStructMethod;
								QDomElement loDocElemMelthodChild = loDomMethodsNode.toElement();
								if (loDocElemMelthodChild.hasAttribute(QString("name")))
								{
										loStructMethod.name = loDocElemMelthodChild.attribute("name");   
								}
								if (loDocElemMelthodChild.hasAttribute(QString("source")))
								{
										loStructMethod.source = loDocElemMelthodChild.attribute("source");   
								}
								if (loDocElemMelthodChild.hasAttribute(QString("path")))
								{
										loStructMethod.path = loDocElemMelthodChild.attribute("path");   
								}
								
								loStructWidget.methods.push_back(loStructMethod);
								loDomMethodsNode = loDomMethodsNode.nextSibling();
							}         			
						}						
						loDomSubNode = loDomSubNode.nextSibling();
				  }                                 
              }
              
              meStructWidget.push_back(loStructWidget);
              
              meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectView.exe",QString("viewcounter[%1]").arg(meStructWidget.size()));
          }
          
          
          loDomNode = loDomNode.nextSibling();
       }

        return true;
    }
    catch(...)
    {
        return false;
    }
}
