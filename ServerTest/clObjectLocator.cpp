#define INFO_BUFFER_SIZE 32767
#include "clObjectLocator.h"

clObjectLocator::clObjectLocator(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, clClassLoader *paClassLoader, QString paConfigurationFile, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;

    meObjectLocator.setupUi(this);
	
	meClassLoader = paClassLoader;


	readMappingIcons();
	
    connect(meObjectLocator.btnRefresh, SIGNAL(clicked()),this,SLOT(slotButtonRefreshPressed()));
    connect(meObjectLocator.tvwLocationView,SIGNAL(itemClicked(QTreeWidgetItem*, int)),this,SLOT(slotTreeClassItemPressed(QTreeWidgetItem*,int)));
    	
	//Fill the userinterfaces
	fillObjectLocations();
	
	
	/////// Setting up the timer function ///////////////////////////////////
	meTimer = new QTimer(this);
	meTimer->setInterval(3000);
	meTimer->connect(meTimer, SIGNAL(timeout()), this, SLOT(slotDoIt()));
	meTimer->start();
}

clObjectLocator::~clObjectLocator()
{
}
void clObjectLocator::slotDoIt()
{
	try
	{	
		if (meObjectLocator.horizontalLayout->count() > 1)
		{
			for (int i = 0; i < meUUID.size(); i++)
			{
				vector<std::string> loGetProperties;
				loGetProperties.push_back("COORD_X");
				loGetProperties.push_back("COORD_Y");
				loGetProperties.push_back("COORD_Z");
						
				vector<std::string> loGetValues;
				QString loReturnMessageGetById;
						
				QString loClassName = QString("OBJECT_TO_LOCATE");
				QString loUUID = meUUID.at(i);
				if (!meIceClientServer->getFromTableDatabaseById(   loClassName,
																	loUUID,
																	loGetProperties,
																	loGetValues,
																	loReturnMessageGetById))
				{
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::slotDoIt -> " + loReturnMessageGetById);					
				}
				else
				{
					transform.at(i)->setTranslation(QVector3D(QString(loGetValues.at(0).c_str()).toFloat()/meX_SIZE, QString(loGetValues.at(2).c_str()).toFloat(), QString(loGetValues.at(1).c_str()).toFloat()/meY_SIZE));
				}
							
			}
		}
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::slotButtonRefreshPressed() -> " + QString(e.what()));
    }		
}
void clObjectLocator::slotButtonRefreshPressed()
{
	try
	{	
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::slotButtonRefreshPressed() -> " + QString(e.what()));
    }	
}

bool clObjectLocator::fillObjectLocations()
{
	try
	{
		vector<std::string> loTables;
		loTables.push_back("LIVING_OBJ_LOCATION");
		if (!createClassesInTreeView(loTables))
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::fillObjectLocations(loTables) Could not create tables in treeview");
		
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::fillObjectLocations() -> " + QString(e.what()));
    }
}
void clObjectLocator::readMappingIcons()
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
QString clObjectLocator::getRelatedIcon(QString paClassName)
{
	if(meClass.size() > 0)
	{
		for (int i = 0; i < meClass.size(); i++)
		{
			if (paClassName.toUpper().compare(meClass.at(i)) == 0)
			{
					return meIcon.at(i);
			}
		}
		
	}
	return QString("");
}

bool clObjectLocator::showLocatorMapForLocation(QString paUUID)
{
	try
	{

		auto children = meObjectLocator.horizontalLayout->children();
		for (auto i = 0; i < meObjectLocator.horizontalLayout->count(); i++) {
			QWidget * tmp = meObjectLocator.horizontalLayout->itemAt(i)->widget();
			if (i == 1) 
			{
				meObjectLocator.horizontalLayout->removeWidget(tmp);
				delete tmp;
				material.clear();
				transform.clear();
				sphereMesh.clear();
				sphereEntity.clear();
				meName.clear();
				meUUID.clear();
			}
		}
		
		meDisplayedView = paUUID;
		/*
		if (meContainer != NULL)
		{
			meObjectLocator.horizontalLayout->removeWidget(meContainer);
			delete meContainer;
			delete meViewPort;
		}
		*/
		
		// Create a Qt3D window
		meViewPort = new Qt3DExtras::Qt3DWindow();
		//meObjectLocator.wdgObjectLocation = QWidget::createWindowContainer(meViewPort);
		meContainer = QWidget::createWindowContainer(meViewPort);
		meContainer->setMinimumSize(QSize(800, 600));
		//container->setMinimumSize(QSize(800, 600));
		meObjectLocator.horizontalLayout->addWidget(meContainer);
		// Root entity
		auto rootEntity = new Qt3DCore::QEntity();

		// Add the line entity
		createLineEntity(rootEntity);
		
		/////////////////////////////////////////
		//Getting all the object on this plane
		/////////////////////////////////////////
		vector <std::string> loProperties;
		vector <std::string> loValue;
		vector <std::string> loTypeValue;
		vector <std::string> loLogExp;
		vector <std::string> loReturnIds;
		QString loReturnMessage;
			
			//Must have a current workstep
		loProperties.push_back("OBJ_LOCATION_ID");
		loValue.push_back(paUUID.toStdString());
		loTypeValue.push_back("uuid");
		loLogExp.push_back("=");

		QString loClass = QString("OBJECT_TO_LOCATE");
		QString loStop = QString("0");
		QString loStart = QString("0");

		if(!meIceClientServer->getFromTableDatbaseByProperty(	loClass,
																	loStart,
																	loStop,
																	loProperties,
																	loValue,
																	loTypeValue,
																	loLogExp,
																	loReturnIds,
																	loReturnMessage))
		{
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::showLocatorMapForLocation -> " + loReturnMessage);	
		}
		else
		{
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::showLocatorMapForLocation -> " + loReturnMessage);	
			
			if (loReturnIds.size() > 0)
			{
				
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::showLocatorMapForLocation -> test");	
					for (int i = 0; i < loReturnIds.size(); i++)
					{
						vector<std::string> loGetProperties;
						loGetProperties.push_back("NAME");
						loGetProperties.push_back("OBJECT_TO_LOCATE_KIND");
						loGetProperties.push_back("QR_BAR_CODE");
						loGetProperties.push_back("SSID");
						loGetProperties.push_back("COORD_X");
						loGetProperties.push_back("COORD_Y");
						loGetProperties.push_back("COORD_Z");
						
						vector<std::string> loGetValues;
						QString loReturnMessageGetById;
						
						QString loClass = QString("OBEJCT_TO_LOCATE");
						QString loUUID = QString(loReturnIds.at(i).c_str());
						
						if (!meIceClientServer->getFromTableDatabaseById(   loClass,
																			loUUID,
																			loGetProperties,
																			loGetValues,
																			loReturnMessageGetById))
						{
							meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::showLocatorMapForLocation -> " + loReturnMessageGetById);					
						}
						else
						{
							meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::showLocatorMapForLocation -> test1");	
							QString loXcoord;
							QString loYcoord;
							QString loZcoord;
							QString loName;
							QString loSSID;
							QString loKind;
							QString loQRcode;
							QString loUUID = QString(loReturnIds.at(i).c_str());
							
							for (int j = 0; j < loGetProperties.size(); j++)
							{
								if (QString(loGetProperties.at(j).c_str()).toUpper().compare(QString("NAME")) == 0)
								{ loName = QString(loGetValues.at(j).c_str());}
								else if (QString(loGetProperties.at(j).c_str()).toUpper().compare(QString("OBJECT_TO_LOCATE_KIND")) == 0)
								{ loKind = QString(loGetValues.at(j).c_str());}
								else if (QString(loGetProperties.at(j).c_str()).toUpper().compare(QString("QR_BAR_CODE")) == 0)
								{ loQRcode = QString(loGetValues.at(j).c_str());}
								else if (QString(loGetProperties.at(j).c_str()).toUpper().compare(QString("SSID")) == 0)
								{ loSSID = QString(loGetValues.at(j).c_str());}
								else if (QString(loGetProperties.at(j).c_str()).toUpper().compare(QString("COORD_X")) == 0)
								{ loXcoord = QString(loGetValues.at(j).c_str());}
								else if (QString(loGetProperties.at(j).c_str()).toUpper().compare(QString("COORD_Y")) == 0)
								{ loYcoord = QString(loGetValues.at(j).c_str());}
								else if (QString(loGetProperties.at(j).c_str()).toUpper().compare(QString("COORD_Z")) == 0)
								{ loZcoord = QString(loGetValues.at(j).c_str());}
							}

							createPointEntity(rootEntity, loName, loSSID, loKind, loQRcode, loUUID, loXcoord, loYcoord, loZcoord, i);
						}
					}
				
			}
		}
		
		
		////////////////////////// Getting the plane for display /////////////////////////////////
		vector<std::string> loPlaneProperties;
		loPlaneProperties.push_back("FLOOR_PLAN_IMAGE");
		loPlaneProperties.push_back("SIZE_X");
		loPlaneProperties.push_back("SIZE_Y");
		vector<std::string> loPlaneValues;
		QString loReturnMessagePlaneById;
		
		QString loClassEx = QString("LIVING_OBJ_LOCATION");
		
		
		if (!meIceClientServer->getFromTableDatabaseById(   loClassEx,
															paUUID,
															loPlaneProperties,
															loPlaneValues,
															loReturnMessagePlaneById))
		{
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::showLocatorMapForLocation -> " + loReturnMessagePlaneById);					
		}
		else
		{
			if (loPlaneValues.size() > 0)
			{
				createPlane(rootEntity, QString(loPlaneValues.at(0).c_str()));
				meX_SIZE = QString(loPlaneValues.at(1).c_str()).toFloat();
				meY_SIZE = QString(loPlaneValues.at(2).c_str()).toFloat();
			}
		}
		

		// Camera setup
		auto camera = meViewPort->camera();
		camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
		camera->setPosition(QVector3D(0, 0, -2));
		camera->setViewCenter(QVector3D(0, 0, 0));

		// Camera controller
		auto camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
		camController->setLinearSpeed(50.0f);
		camController->setLookSpeed(180.0f);
		camController->setCamera(camera);

		// Set root entity
		meViewPort->setRootEntity(rootEntity);

		// Show the window
		meContainer->show();		
		//meObjectLocator.wdgObjectLocation->show();
		return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObjectLocator::showLocatorMapForLocation -> " + QString(e.what()));
		return false;
    }	
}

//Diplay an object on the map
Qt3DCore::QEntity* clObjectLocator::createPointEntity(Qt3DCore::QEntity* rootEntity, QString paName, QString paSSID, QString paKind, QString paQRcode, QString paUUID, QString paXcoord, QString paYcoord, QString paZcoord, int paId)
{
	try
	{
		//
		meName.push_back(paName);
		meUUID.push_back(paUUID);
	
		// Sphere
		sphereEntity.push_back(new Qt3DCore::QEntity(rootEntity));
		sphereMesh.push_back(new Qt3DExtras::QSphereMesh);
		sphereMesh.at(sphereMesh.size()-1)->setRadius(0.02f);
		
		transform.push_back(new Qt3DCore::QTransform{sphereMesh.at(sphereMesh.size()-1)});
		//transform->setRotationX(xAngle);
		//transform->setRotationY(yAngle);
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::createPointEntity -> voor parsen");					
		transform.at(transform.size()-1)->setTranslation(QVector3D(paXcoord.toFloat()/100, paZcoord.toFloat()/100, paYcoord.toFloat()/100));
		meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::createPointEntity -> na parsen");					
		// Material
		material.push_back(new Qt3DExtras::QPhongMaterial(rootEntity));
		
		switch (paKind.toInt())
		{
			case 0:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::red));
				break;
			case 10:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::green));
				break;
			case 20:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::blue));
				break;
			case 30:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::black));
				break;
			case 40:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::gray));
				break;
			case 50:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::yellow));		
				break;
			case 60:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::gray));
				break;
			case 70:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::red));
				break;
			case 80:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::red));
				break;
			case 90:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::red));
				break;
			case 100:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::red));
				break;
			default:
				material.at(material.size()-1)->setDiffuse(QColor(Qt::white));
		}
		
		sphereEntity.at(sphereEntity.size()-1)->addComponent(material.at(material.size()-1));
		sphereEntity.at(sphereEntity.size()-1)->addComponent(sphereMesh.at(sphereMesh.size()-1));
		sphereEntity.at(sphereEntity.size()-1)->addComponent(transform.at(transform.size()-1));

		return sphereEntity.at(sphereEntity.size()-1);
	}
    catch(exception &e)
    {
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObjectLocator::createPointEntity -> " + QString(e.what()));
		return NULL;
    }	
}

//Display the plane on the map
Qt3DCore::QEntity* clObjectLocator::createPlane(Qt3DCore::QEntity* rootEntity,QString paPlaneImage) {	
    // Plane shape data
    Qt3DExtras::QPlaneMesh *planeMesh = new Qt3DExtras::QPlaneMesh();
    planeMesh->setWidth(1);
    planeMesh->setHeight(1);

    Qt3DCore::QTransform *planeTransform = new Qt3DCore::QTransform();
    planeTransform->setScale(1.0f);
    planeTransform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 0.0f, 0.0f), 0));
    planeTransform->setTranslation(QVector3D(0.5f, 0.0f, -0.5f));

    auto textureImage = new Qt3DRender::QTextureImage;
    textureImage->setSource(QUrl::fromLocalFile(paPlaneImage));
    auto planeImage = new Qt3DRender::QTexture2D();
    planeImage->addTextureImage(textureImage);
	
    auto planeMaterial = new Qt3DExtras::QDiffuseSpecularMaterial();
    planeMaterial->setDiffuse(QVariant::fromValue(planeImage));
    planeMaterial->setNormal(QVariant::fromValue(planeImage));
    planeMaterial->setSpecular(QVariant::fromValue(planeImage));

	auto m_planeEntity = new Qt3DCore::QEntity(rootEntity);
	m_planeEntity->addComponent(planeMesh);
	m_planeEntity->addComponent(planeMaterial);
	m_planeEntity->addComponent(planeTransform);


    return m_planeEntity;
}

//Display the axes system
Qt3DCore::QEntity* clObjectLocator::createLineEntity(Qt3DCore::QEntity* rootEntity) {
    // Geometry for the line
    auto geometry1 = new Qt3DRender::QGeometry(rootEntity);
	auto geometry2 = new Qt3DRender::QGeometry(rootEntity);
	auto geometry3 = new Qt3DRender::QGeometry(rootEntity);

    // Vertex data (start and end points of the line)
    QByteArray vertexData1;
    vertexData1.resize(6 * sizeof(float)); // 2 points * 3 coordinates (x, y, z)
    float* positions1 = reinterpret_cast<float*>(vertexData1.data());
    positions1[0] = 0.0f; positions1[1] = 0.0f; positions1[2] = 0.0f; // Start point
    positions1[3] = 1.0f; positions1[4] = 0.0f; positions1[5] = 0.0f; // End point

    auto vertexBuffer1 = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry1);
    vertexBuffer1->setData(vertexData1);

    // Vertex data (start and end points of the line)
    QByteArray vertexData2;
    vertexData2.resize(6 * sizeof(float)); // 2 points * 3 coordinates (x, y, z)
    float* positions2 = reinterpret_cast<float*>(vertexData2.data());
    positions2[0] = 0.0f; positions2[1] = 0.0f; positions2[2] = 0.0f; // Start point
    positions2[3] = 0.0f; positions2[4] = 1.0f; positions2[5] = 0.0f; // End point

    auto vertexBuffer2 = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry2);
    vertexBuffer2->setData(vertexData2);


    // Vertex data (start and end points of the line)
    QByteArray vertexData3;
    vertexData3.resize(6 * sizeof(float)); // 2 points * 3 coordinates (x, y, z)
    float* positions3 = reinterpret_cast<float*>(vertexData3.data());
    positions3[0] = 0.0f; positions3[1] = 0.0f; positions3[2] = 0.0f; // Start point
    positions3[3] = 0.0f; positions3[4] = 0.0f; positions3[5] = -1.0f; // End point

    auto vertexBuffer3 = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, geometry3);
    vertexBuffer3->setData(vertexData3);







    auto positionAttribute1 = new Qt3DRender::QAttribute();
    positionAttribute1->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute1->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute1->setVertexSize(3);
    positionAttribute1->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute1->setBuffer(vertexBuffer1);
    positionAttribute1->setByteStride(3 * sizeof(float));
    positionAttribute1->setCount(2);

    auto positionAttribute2 = new Qt3DRender::QAttribute();
    positionAttribute2->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute2->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute2->setVertexSize(3);
    positionAttribute2->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute2->setBuffer(vertexBuffer2);
    positionAttribute2->setByteStride(3 * sizeof(float));
    positionAttribute2->setCount(2);

    auto positionAttribute3 = new Qt3DRender::QAttribute();
    positionAttribute3->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute3->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute3->setVertexSize(3);
    positionAttribute3->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute3->setBuffer(vertexBuffer3);
    positionAttribute3->setByteStride(3 * sizeof(float));
    positionAttribute3->setCount(2);


    geometry1->addAttribute(positionAttribute1);
	geometry2->addAttribute(positionAttribute2);
	geometry3->addAttribute(positionAttribute3);

    // Line renderer
    auto lineRenderer1 = new Qt3DRender::QGeometryRenderer();
    lineRenderer1->setGeometry(geometry1);
    lineRenderer1->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    // Line renderer
    auto lineRenderer2 = new Qt3DRender::QGeometryRenderer();
    lineRenderer2->setGeometry(geometry2);
    lineRenderer2->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    // Line renderer
    auto lineRenderer3 = new Qt3DRender::QGeometryRenderer();
    lineRenderer3->setGeometry(geometry3);
    lineRenderer3->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    // Entity
    auto lineEntity1 = new Qt3DCore::QEntity(rootEntity);
    lineEntity1->addComponent(lineRenderer1);
	auto lineEntity2 = new Qt3DCore::QEntity(rootEntity);
	lineEntity2->addComponent(lineRenderer2);
	auto lineEntity3 = new Qt3DCore::QEntity(rootEntity);
	lineEntity3->addComponent(lineRenderer3);

    // Material
    auto material = new Qt3DExtras::QPhongMaterial(rootEntity);
    material->setDiffuse(QColor(Qt::red));
    lineEntity1->addComponent(material);
	lineEntity2->addComponent(material);
	lineEntity3->addComponent(material);

    return lineEntity1;
}



void clObjectLocator::slotTreeClassItemPressed(QTreeWidgetItem *item, int index)
{
    try
    {
		clClassObjectTreeItem *loClassObjectTreeItem = (clClassObjectTreeItem *)item;
		
        //Class is pressed so get the objects
        if( item->parent() == NULL)
        {
			
			showLocatorMapForLocation(QString(loClassObjectTreeItem->meUUID));
			
			vector <std::string> loProperties;
			vector <std::string> loValue;
			vector <std::string> loTypeValue;
			vector <std::string> loLogExp;
			vector <std::string> loReturnIds;
			QString loReturnMessage;
			
			//Must have a current workstep
			loProperties.push_back("OBJ_LOCATION_ID");
			loValue.push_back(QString(loClassObjectTreeItem->meUUID).toStdString());
			loTypeValue.push_back("uuid");
			loLogExp.push_back("=");


			QString loClass = QString("OBJECT_TO_LOCATE");
			QString loStart = QString("0");
			QString loStop = QString("0");




			if(!meIceClientServer->getFromTableDatbaseByProperty(	loClass,
																	loStart,
																	loStop,
																	loProperties,
																	loValue,
																	loTypeValue,
																	loLogExp,
																	loReturnIds,
																	loReturnMessage))
			{
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::slotTreeClassItemPressed -> " + loReturnMessage);	
			}
			else
			{
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::slotTreeClassItemPressed -> " + loReturnMessage);
				
				if (loReturnIds.size() > 0)
				{
					//Getting the names of the id's//
					vector<std::string> loReturnNames;
					for (int i = 0; i < loReturnIds.size(); i++)
					{
						vector<std::string> loPropertyRequestedName;
						loPropertyRequestedName.push_back("NAME");
						
						vector<std::string> loReturnValues;
						QString loReturnMessageGetById;
						
						QString loClass = QString("OBJECT_TO_LOCATE");
						QString loID = QString(loReturnIds.at(i).c_str());
						
						
						meIceClientServer->getFromTableDatabaseById(    loClass,
																		loID,
																		loPropertyRequestedName,
																		loReturnValues,
																		loReturnMessageGetById);
						loReturnNames.push_back(loReturnValues.at(0));																		
					}
					addElementsToQTreeWidget(item, loReturnIds, loReturnNames);
				}
			}
        }
		else
		{
			for (int i = 0; i < meUUID.size(); i++)
			{
				if (QString(loClassObjectTreeItem->meUUID).compare(meUUID.at(i)) == 0)
				{
					material.at(i)->setDiffuse(QColor(Qt::yellow));
				}
				else
				{
					material.at(i)->setDiffuse(QColor(Qt::green));
				}
			}
			
		}
    }
    catch(exception &e)
    {

		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clObjectLocator::slotTreeClassItemPressed -> " + QString(e.what()));
    }
}

/********************************************
* User interface funcitons
************************************************/
bool clObjectLocator::createClassesInTreeView(vector<std::string> paTables)
{
    try
    {
        removeElementsFromObjectLocations();
        meObjectLocator.tvwLocationView->setColumnCount(2);

        QStringList headers;
        headers << tr("Classes") << tr("Objects");
        meObjectLocator.tvwLocationView->setHeaderLabels(headers);

        for (int i = 0; i < paTables.size();i++)
        {
            QString loTableName = QString(paTables[i].c_str());

			
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
															
			meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::createClassesInTreeView -> " + loReturnMessageObject);
			
			//Store the table information in vector
			vector<clDatabaseColumn> loDatabaseColumnArr;
			for (int l=0; l< loPropertyName.size(); l++)
			{
				clDatabaseColumn loDatabaseColumn(  QString(loPropertyName[l].c_str()),
													QString(loAlias[l].c_str()),
													QString(loType[l].c_str()),
													QString(loExtra[l].c_str()),
													QString(loReference[l].c_str()));
				loDatabaseColumnArr.push_back(loDatabaseColumn);
			}

			vector <std::string> loReturnId;
			vector <std::string> loReturnName;
			QString loReturnMessage;
			
			QString loStop = QString("0");
			QString loStart = QString("0");
						
			if (!meIceClientServer->getFromTableDatabaseGeneral( loTableName, loStart, loStop, loReturnId, loReturnName, loReturnMessage))
			{
				meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::createClassesInTreeView -> " + loReturnMessage);	
			}
			else
			{
				for (int k = 0; k <  (int)loReturnId.size(); k++)
				{
					clClassObjectTreeItem *newItem;
					newItem = new clClassObjectTreeItem(meObjectLocator.tvwLocationView, meObjectLocator.tvwLocationView->currentItem());
					//newItem->setDatabaseActions(loDatabaseActionArr);
					newItem->setDatabaseColumns(loDatabaseColumnArr);
					newItem->setClassName(loTableName);
					newItem->meUUID = QString(loReturnId.at(k).c_str());
					newItem->meNameDisplay = QString(loReturnName.at(k).c_str());
					newItem->meIsInfo = false;
					const QIcon Icon(".\\ICONS\\" + getRelatedIcon(loTableName));
					newItem->setIcon(0,Icon);
					newItem->setText(0,QString(loReturnName.at(k).c_str()));
					
					meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clProductPlanner::createClassesInTreeView -> Added ID [" + newItem->meUUID + "]");					
				}
			}




        }
		

        return true;
    }
    catch(exception &e)
    {	
		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectViewer::createClassesInTreeView -> " + QString(e.what()));
        return false;
    }
}


bool clObjectLocator::removeElementsFromObjectLocations()
{
    try
    {
        QList<QTreeWidgetItem *> paColumnItems;

        for (int i = 0; i < meObjectLocator.tvwLocationView->topLevelItemCount(); ++i)
        {
            paColumnItems << meObjectLocator.tvwLocationView->topLevelItem(i);
        }
        qDeleteAll(paColumnItems.begin(),paColumnItems.end());
        return true;
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::removeElementsFromObjectLocations -> " + QString(e.what()));
        return false;
    }
}
bool clObjectLocator::addElementsToQTreeWidget(QTreeWidgetItem * paItem, vector<std::string> paReturnIds, vector<std::string> paReturnNames)
{
    try
    {
        paItem->takeChildren();
		clClassObjectTreeItem *loItem = (clClassObjectTreeItem*)paItem;

        for (int i = 0; i< (int) paReturnIds.size();i++)
        {
            clClassObjectTreeItem *newItem;
            newItem = new clClassObjectTreeItem();
			newItem->setClassName(paItem->text(0));
			newItem->meUUID = QString(paReturnIds[i].c_str());
			newItem->meNameDisplay = QString(paReturnNames[i].c_str());
			newItem->setDatabaseActions(loItem->getDatabaseActions());
			newItem->setDatabaseColumns(loItem->getDatabaseColumns());
			
			const QIcon Icon(".\\ICONS\\" + getRelatedIcon(paItem->text(0)));
			newItem->setIcon(0,Icon);
			newItem->setText(0,QString(paReturnNames[i].c_str()));
            paItem->addChild(newItem);
        }
        return true;
    }
    catch(exception &e)
    {
        meIceClientLogging->insertItem("50",QString(QHostInfo::localHostName()),"2UVObjectViewer.exe","clObjectLocator::addElementsToQTreeWidget -> " + QString(e.what()));
        return false;
    }
}
