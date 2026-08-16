#define INFO_BUFFER_SIZE 32767
#include "clExtendGraphicsView.h"

clExtendGraphicsView::clExtendGraphicsView(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;
	meGraphicsScene = new QGraphicsScene(this);
	this->setScene(meGraphicsScene);	
	

	
	setAcceptDrops(true);
	
}
clExtendGraphicsView::~clExtendGraphicsView()
{
}

void clExtendGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (QGraphicsItem *item = itemAt(event->pos())) {
		
		if (item->toolTip().toUpper().compare("NODE") == 0)
		{
			clNode *loNode = (clNode*) item;
			meSelectedItem = loNode->meName;
			meSelectedItemUUID = loNode->meUUID;
			QMenu menu(this);
			QAction *setParent=menu.addAction("Set as Parent");
			QAction *setChild=menu.addAction("Set as Child");
			QAction *deleteElem=menu.addAction("Delete");
			QObject::connect(setParent, SIGNAL(triggered()),this,SLOT(slotSetAsParent()));
			QObject::connect(setChild, SIGNAL(triggered()),this,SLOT(slotSetAsChild()));
			QObject::connect(deleteElem, SIGNAL(triggered()),this,SLOT(slotDelete()));
			menu.exec(QCursor::pos());		
		}
		else if (item->toolTip().toUpper().compare("EDGE") == 0)
		{
			clEdge *loEdge = (clEdge*) item;
			meSelectedItemUUID = loEdge->meUUID;
			QMenu menu(this);
			QAction *deleteElem=menu.addAction("Delete");
			QObject::connect(deleteElem, SIGNAL(triggered()),this,SLOT(slotDelete()));
			menu.exec(QCursor::pos());					
		}
    }
}

void clExtendGraphicsView::slotSetAsParent()
{
	try
	{  
		if (meSelectedItem.toUpper().compare("STOP") == 0)
		{
			QMessageBox msgBox;
			msgBox.setText("STOP can not be set as a parent ...");
			msgBox.setStandardButtons(QMessageBox::Cancel);
			int ret = msgBox.exec();			
			meSelectedItemUUID = QString("");
			meSelectedItem = QString("");
			return;
		}
		meParentName = meSelectedItem;
		meParentUUID = meSelectedItemUUID;
		
		meChildName = QString("");
		meChildUUID = QString("");
		
		meSelectedItemUUID = QString("");
		
		//clParentChild * loParentChild = new clParentChild(meParentName, meParentUUID, QString("CYCLE_ROUTINE"));
		//meGraphicsScene->addItem(loParentChild);
	}
	catch(exception &e)
	{
	  cout << e.what() << endl;
	}
		
}

void clExtendGraphicsView::slotSetAsChild()
{
	try
	{
		//Check up
		if (meSelectedItem.toUpper().compare("START") == 0)
		{
			QMessageBox msgBox;
			msgBox.setText("START can not be set as a child ...");
			msgBox.setStandardButtons(QMessageBox::Cancel);
			int ret = msgBox.exec();			
			meSelectedItemUUID = QString("");
			meSelectedItem = QString("");
			return;
		}		
		
		//Set child variables
		meChildName = meSelectedItem;
		meChildUUID = meSelectedItemUUID;
		
		if(meChildUUID.compare(meParentUUID) == 0)
		{
			meChildUUID = QString("");
			meChildName = QString("");
			return;
		}else if(meChildUUID.compare(QString("")) == 0)
		{
			meChildUUID = QString("");
			meChildName = QString("");
			return;
		}
		
		QList<QGraphicsItem *> loGraphicsItems = meGraphicsScene->items();
		int loChild = -1;
		int loParent = -1;
		for (int i = 0; i < loGraphicsItems.size(); i++)
		{	
			if (loGraphicsItems.at(i)->toolTip().compare("NODE") == 0)
			{
				clNode * loNode = (clNode*) loGraphicsItems.at(i);
				if (loNode->meUUID.compare(meChildUUID) == 0)loChild = i;
				else if (loNode->meUUID.compare(meParentUUID) == 0)loParent = i;
			}
		}
		if(loParent == -1)
		{
			//Message box for creating object
			QMessageBox msgBox;
			msgBox.setText("Select first a parent ...");
			msgBox.setStandardButtons(QMessageBox::Cancel);
			int ret = msgBox.exec();
			return;
		}
		meSelectedItemUUID = QString("");
		clEdge * loEdge = new clEdge((clNode*)loGraphicsItems.at(loParent), (clNode*)loGraphicsItems.at(loChild));
		loEdge->meUUID = QString(meParentUUID + meChildUUID);
		loEdge->setToolTip("EDGE");
		meGraphicsScene->addItem(loEdge);
	}
	catch(exception &e)
	{
	  cout << e.what() << endl;
	}
}
void clExtendGraphicsView::slotDelete()
{
	try
	{
		QList<QGraphicsItem *> loGraphicsItems = meGraphicsScene->items();
		for (int i = 0; i < loGraphicsItems.size(); i++)
		{
			if (loGraphicsItems.at(i)->toolTip().toUpper().compare("NODE") == 0)
			{
				clNode * loNode = (clNode *) loGraphicsItems.at(i);
				if (loNode->meUUID.compare(meSelectedItemUUID) == 0)
				{
					meGraphicsScene->removeItem(loGraphicsItems.at(i));
					break;
				}
			}
			else if (loGraphicsItems.at(i)->toolTip().toUpper().compare("EDGE") == 0)
			{
				clEdge * loEdge = (clEdge *) loGraphicsItems.at(i);
				if (loEdge->meUUID.compare(meSelectedItemUUID) == 0)
				{
					meGraphicsScene->removeItem(loGraphicsItems.at(i));
					break;
				}
			}
		}
		meSelectedItemUUID = QString("");
	}
	catch(exception &e)
	{
	  cout << e.what() << endl;
	}
}

void clExtendGraphicsView::dragEnterEvent(QDragEnterEvent* event) {
  event->acceptProposedAction();
}

void clExtendGraphicsView::dragMoveEvent(QDragMoveEvent* event) {
  event->acceptProposedAction();
}

void clExtendGraphicsView::initializeGraphView(vector <clDatabaseRow> paDatabaseRows) 
{
	try
	{
		//Delete all elements on graph view
		QList<QGraphicsItem *> loGraphicsItems = meGraphicsScene->items();
		for (int i = 0; i < loGraphicsItems.size(); i++)
		{
			meGraphicsScene->removeItem(loGraphicsItems.at(i));		
		}
		
		//Draw start and stop
		clNode *nodeStart = new clNode("START","START","START");
		meGraphicsScene->addItem(nodeStart);
		clNode *nodeStop = new clNode("STOP","STOP","STOP");
		meGraphicsScene->addItem(nodeStop);
		nodeStart->setToolTip("NODE");
		nodeStop->setToolTip("NODE");
		nodeStart->setPos(0, -250);
		nodeStop->setPos(0, 250);
		
		
		for (int i = 0; i< paDatabaseRows.size(); i++)
		{
			clDatabaseRow loDatabaseRow = paDatabaseRows.at(i);
			
			vector <std::string> loDatabaseProperties = loDatabaseRow.getColumnName();
			vector <std::string> loDatabaseValues = loDatabaseRow.getValues();
			vector <std::string> loDatabaseType = loDatabaseRow.getType();
			vector <std::string> loDatabaseAlias = loDatabaseRow.getAlias();
			vector <std::string> loDatabaseExtra = loDatabaseRow.getExtra();
			vector <std::string> loDatabaseReference = loDatabaseRow.getReference();
			
			
			bool loIsParent = false;
			QString loDisplayName = QString("");
			QString loUUID = QString("");
			QString loClassName = QString("CYCLE_ROUTINE");
			QString loChildren = QString("");
			QString loChildClass = QString("");
			
			//First draw the main function
			for (int j = 0; j < loDatabaseProperties.size(); j++)
			{
				//cout << loDatabaseProperties.at(j) << "[" <<  loDatabaseValues.at(j) << "]"<<endl;
				if (QString(loDatabaseProperties.at(j).c_str()).toUpper().compare("PARENT_CLASS") == 0)
				{
					if (QString(loDatabaseValues.at(j).c_str()).toUpper().compare("CYCLES") == 0)
						loIsParent = true;
				}
				if (QString(loDatabaseProperties.at(j).c_str()).toUpper().compare("PKEY") == 0)
					loUUID = QString(loDatabaseValues.at(j).c_str());
				if (QString(loDatabaseProperties.at(j).c_str()).toUpper().compare("NAME") == 0)
					loDisplayName = QString(loDatabaseValues.at(j).c_str());
				if (QString(loDatabaseProperties.at(j).c_str()).toUpper().compare("CHILDREN") == 0)
					loChildren = QString(loDatabaseValues.at(j).c_str());			
				if (QString(loDatabaseProperties.at(j).c_str()).toUpper().compare("CHILD_CLASS") == 0)
					loChildClass = QString(loDatabaseValues.at(j).c_str());					
			}
			
			if (loIsParent)
			{
				//Draw the first class
				clNode *node1 = new clNode(loDisplayName,loUUID,loClassName);
				meGraphicsScene->addItem(node1);
				node1->setPos(0, -180);
				node1->setToolTip("NODE");
				node1->meChildren = loChildren;
				
				clEdge *loEdgeStart = new clEdge(nodeStart,node1);
				meGraphicsScene->addItem(loEdgeStart);
				loEdgeStart->setToolTip("EDGE");

				if (loChildClass.toUpper().compare("CYCLES") == 0)
				{
					//No children close to ending node
					clEdge *loEdgeFin = new clEdge(node1,nodeStop);
					meGraphicsScene->addItem(loEdgeFin);
					loEdgeFin->setToolTip("EDGE");
				}
				
				if (loChildren.toUpper().compare(QString("")) == 0 || loChildren.toUpper().compare(QString("{}")) == 0)
				{
					break;
				}
				else
				{
					QStringList loStringList = loChildren.replace("{","").replace("}","").split(",");
					buildChildren(nodeStop, node1,loStringList,paDatabaseRows, 0, -180);
				}				
				break;
			}
		}		
	}
	catch(exception &e)
	{
	  cout << e.what() << endl;
	}
}

void clExtendGraphicsView::buildChildren(clNode * paNodeStop, clNode * paParentNode, QStringList paChildren, vector<clDatabaseRow> paDatabaseRows, int paXpos, int paYpos)
{	
	for (int i = 0; i< paChildren.size();i++)
	{
		
		for (int j = 0; j < paDatabaseRows.size(); j++)
		{
			clDatabaseRow loDatabaseRow = paDatabaseRows.at(j);
			
			vector <std::string> loDatabaseProperties = loDatabaseRow.getColumnName();
			vector <std::string> loDatabaseValues = loDatabaseRow.getValues();
			vector <std::string> loDatabaseType = loDatabaseRow.getType();
			vector <std::string> loDatabaseAlias = loDatabaseRow.getAlias();
			vector <std::string> loDatabaseExtra = loDatabaseRow.getExtra();
			vector <std::string> loDatabaseReference = loDatabaseRow.getReference();
			
			bool loFound = false;
			QString loDisplayName = QString("");
			QString loUUID = QString("");
			QString loClassName = QString("CYCLE_ROUTINE");
			QString loChildren = QString("");
			QString loChildClass = QString("");
			
			for(int z = 0; z < loDatabaseProperties.size();z++)
			{
				if (QString(loDatabaseProperties.at(z).c_str()).toUpper().compare("PKEY") == 0)
				{
					if (QString(loDatabaseValues.at(z).c_str()).compare(paChildren.at(i)) == 0)
					{
						loUUID = QString(loDatabaseValues.at(z).c_str());
						loFound = true;
					}
				}
				else if (QString(loDatabaseProperties.at(z).c_str()).toUpper().compare("NAME") == 0)
					loDisplayName = QString(loDatabaseValues.at(z).c_str());
				else if (QString(loDatabaseProperties.at(z).c_str()).toUpper().compare("CHILDREN") == 0)
					loChildren = QString(loDatabaseValues.at(z).c_str());
				else if (QString(loDatabaseProperties.at(z).c_str()).toUpper().compare("CHILD_CLASS") == 0){
					loChildClass = QString(loDatabaseValues.at(z).c_str());					
				}

			}
			if (loFound)
			{
				clNode *node1 = nodeAlreadyExist(loUUID);
				if (node1 == NULL)
				{
					//Draw the first class
					node1 = new clNode(loDisplayName,loUUID,loClassName);
					meGraphicsScene->addItem(node1);
					node1->setPos(paXpos + (200 * i), paYpos + 100);
					node1->setToolTip("NODE");
					node1->meChildren = loChildren;
					
					clEdge *loEdge = new clEdge(paParentNode,node1);
					meGraphicsScene->addItem(loEdge);
					loEdge->setToolTip("EDGE");

					if (loChildClass.toUpper().compare("CYCLES") == 0)
					{
						//No children close to ending node
						clEdge *loEdgeFin = new clEdge(node1,paNodeStop);
						meGraphicsScene->addItem(loEdgeFin);
						loEdgeFin->setToolTip("EDGE");
					}
					
					if (loChildren.toUpper().compare(QString("")) == 0 || loChildren.toUpper().compare(QString("{}")) == 0)
					{
						break;
					}					
					else
					{
						QStringList loStringList = loChildren.replace("{","").replace("}","").split(",");
						buildChildren(paNodeStop, node1,loStringList,paDatabaseRows, 0, paYpos + 100);
					}
					break;					
				}
				else
				{
					clEdge *loEdge = edgeAlreadyExist(paParentNode,node1);
					if (loEdge == NULL)
					{
						loEdge = new clEdge(paParentNode,node1);
						meGraphicsScene->addItem(loEdge);
						loEdge->setToolTip("EDGE");						
					}
					break;
				}
				

			}			
		}
	}
}
clNode* clExtendGraphicsView::nodeAlreadyExist(QString paUUID)
{
	try
	{
		QList<QGraphicsItem *> loGraphicsItems = meGraphicsScene->items();
		for (int i = 0; i < loGraphicsItems.size(); i++)
		{
			if (loGraphicsItems.at(i)->toolTip().toUpper().compare("NODE") == 0)
			{
				clNode * loNode = (clNode *) loGraphicsItems.at(i);
				if (loNode->meUUID.compare(paUUID) == 0)
					return loNode;
			}
		}
		return NULL;
	}
	catch(exception &e)
	{
	  cout << e.what() << endl;
	  return NULL;
	}
}
clEdge* clExtendGraphicsView::edgeAlreadyExist(clNode *paSourceNode, clNode *paDesitnationNode)
{
	try
	{
		QList<QGraphicsItem *> loGraphicsItems = meGraphicsScene->items();
		for (int i = 0; i < loGraphicsItems.size(); i++)
		{
			if (loGraphicsItems.at(i)->toolTip().toUpper().compare("EDGE") == 0)
			{
				clEdge * loEdge = (clEdge *) loGraphicsItems.at(i);
				clNode * loSourceNode = loEdge->sourceNode();
				clNode * loDestinationNode = loEdge->destNode();
				if (loSourceNode->meUUID.compare(paSourceNode->meUUID) == 0 && loDestinationNode->meUUID.compare(paDesitnationNode->meUUID) == 0)
					return loEdge;
			}
		}
		return NULL;
	}
	catch(exception &e)
	{
	  cout << e.what() << endl;
	  return NULL;
	}
}

void clExtendGraphicsView::dropEvent(QDropEvent* event) {
  try
  {
	  QTreeWidget *loTreeWidget = (QTreeWidget*) event->source();
	  clClassObjectTreeItem *loItemRoutineCycle = (clClassObjectTreeItem*)loTreeWidget->selectedItems()[0];
	  //cout << string(QString(loItemRoutineCycle->meUUID).toUtf8()) << endl;
	  //cout << string(QString(loItemRoutineCycle->getClassName()).toUtf8()) << endl;
	  //meNode[0] = new Node(this);
	  clNode *node1 = nodeAlreadyExist(loItemRoutineCycle->meUUID);
	  if (node1 == NULL)
	  {
		  node1 = new clNode(loItemRoutineCycle->meNameDisplay,loItemRoutineCycle->meUUID,loItemRoutineCycle->getClassName());
		  meGraphicsScene->addItem(node1);
		  node1->setPos(0, 0);
		  node1->setToolTip("NODE");		  
	  }
	  else
	  {
		  	QMessageBox msgBox;
			msgBox.setText("Node already exists in workflow ...");
			msgBox.setStandardButtons(QMessageBox::Cancel);
			int ret = msgBox.exec();	
	  }
  }
  catch(exception &e)
  {
	  cout << e.what() << endl;
  }
}
vector<clDatabaseRow> clExtendGraphicsView::safeCycle(vector<clDatabaseRow> paDatabaseRows)
{
	vector<clDatabaseRow> loDatabaseRows;
	try
	{
		QList<QGraphicsItem *> loGraphicsItems = meGraphicsScene->items();
		for (int i = 0; i < loGraphicsItems.size(); i++)
		{
			if (loGraphicsItems.at(i)->toolTip().toUpper().compare("NODE") == 0)
			{
				clNode * loNode = (clNode *) loGraphicsItems.at(i);
				if (loNode->meUUID.compare("START") == 0 || loNode->meUUID.compare("STOP") == 0)
				{
					//Node not used
				}
				else
				{
					//Node used
					vector <clNode*> loParentNode = getParents(loNode);
					vector <clNode*> loChildNodes = getChilds(loNode);
					
					QString loParents = QString("");
					QString loParentClass = QString("");
					for (int j = 0; j < loParentNode.size(); j++)
					{
						if (loParentNode.at(j)->meUUID.compare("START") == 0)
							loParentClass = QString("CYCLES");
						else
						{
							if (loParents.compare("") == 0) loParents = loParents + loParentNode.at(j)->meUUID;
							else loParents = loParents + QString("$;$") + loParentNode.at(j)->meUUID;
						}
					}
					
					QString loChilds = QString("");
					QString loChildClass = QString("");
					for (int k = 0; k < loChildNodes.size(); k++)
					{
						if (loChildNodes.at(k)->meUUID.compare("STOP") == 0)
							loChildClass = QString("CYCLES");
						else
						{
							if (loChilds.compare("") == 0) loChilds = loChilds + loChildNodes.at(k)->meUUID;
							else loChilds = loChilds + QString("$;$") + loChildNodes.at(k)->meUUID;
						}						
					}
					
					for(int ll=0; ll < paDatabaseRows.size(); ll++)
					{
						clDatabaseRow loDatabaseRow = paDatabaseRows.at(ll);
						
						
						vector <std::string> loDatabaseProperties = loDatabaseRow.getColumnName();
						vector <std::string> loDatabaseValues = loDatabaseRow.getValues();
						vector <std::string> loDatabaseType = loDatabaseRow.getType();
						vector <std::string> loDatabaseAlias = loDatabaseRow.getAlias();
						vector <std::string> loDatabaseExtra = loDatabaseRow.getExtra();
						vector <std::string> loDatabaseReference = loDatabaseRow.getReference();
						bool loFound = false;
						for (int kk = 0; kk < loDatabaseProperties.size(); kk++)
						{
							if (QString(loDatabaseProperties.at(kk).c_str()).toUpper().compare("PKEY") == 0)
								if (QString(loDatabaseValues.at(kk).c_str()).compare(loNode->meUUID) == 0)
								{
									loFound = true;
									break;
								}	
						}
						if (loFound)
						{
							for (int kk = 0; kk < loDatabaseProperties.size(); kk++)
							{
								if (QString(loDatabaseProperties.at(kk).c_str()).toUpper().compare("PARENT") == 0)
								{
									loDatabaseValues.at(kk) = string(loParents.toStdString());
								}
								if (QString(loDatabaseProperties.at(kk).c_str()).toUpper().compare("CHILDREN") == 0)
								{
									loDatabaseValues.at(kk) = string(loChilds.toStdString());
								}
								if (QString(loDatabaseProperties.at(kk).c_str()).toUpper().compare("PARENT_CLASS") == 0)
									loDatabaseValues.at(kk) = string(loParentClass.toStdString());
								if (QString(loDatabaseProperties.at(kk).c_str()).toUpper().compare("CHILD_CLASS") == 0)
									loDatabaseValues.at(kk) = string(loChildClass.toStdString());

										
							}							
							clDatabaseRow loDatabaseRowNew = clDatabaseRow(QString("CYCLE_ROUTINE"),loDatabaseValues,loDatabaseProperties,loDatabaseAlias,loDatabaseType,loDatabaseExtra,loDatabaseReference);
							loDatabaseRows.push_back(loDatabaseRowNew);
						}
					}
				}
			}
		}
		return loDatabaseRows;
	}
	catch(exception &e)
	{
		cout << e.what() << endl;
		return loDatabaseRows;
	}	
}
vector <clNode*> clExtendGraphicsView::getParents(clNode * paNode)
{
	vector <clNode*> loNodes;
	try
	{
		QList<QGraphicsItem *> loGraphicsItems = meGraphicsScene->items();
		for (int i = 0; i < loGraphicsItems.size(); i++)
		{
			if (loGraphicsItems.at(i)->toolTip().toUpper().compare("EDGE") == 0)
			{
				clEdge * loEdge = (clEdge*) loGraphicsItems.at(i);
				clNode * loParentNode = loEdge->sourceNode();
				clNode * loChildNode = loEdge->destNode();
				if (loChildNode->meUUID.compare(paNode->meUUID) == 0)
				{
					loNodes.push_back(loParentNode);
				}
			}
		}
		return loNodes;
	}
	catch(exception &e)
	{
		cout << e.what() << endl;
		return loNodes;
	}		
}
vector <clNode*> clExtendGraphicsView::getChilds(clNode * paNode)
{
	vector <clNode*> loNodes;
	try
	{
		QList<QGraphicsItem *> loGraphicsItems = meGraphicsScene->items();
		for (int i = 0; i < loGraphicsItems.size(); i++)
		{
			if (loGraphicsItems.at(i)->toolTip().toUpper().compare("EDGE") == 0)
			{
				clEdge * loEdge = (clEdge*) loGraphicsItems.at(i);
				clNode * loParentNode = loEdge->sourceNode();
				clNode * loChildNode = loEdge->destNode();
				if (loParentNode->meUUID.compare(paNode->meUUID) == 0)
				{
					loNodes.push_back(loChildNode);
				}
			}
		}
		return loNodes;
	}
	catch(exception &e)
	{
		cout << e.what() << endl;
		return loNodes;
	}		
}
void clExtendGraphicsView::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}
#if QT_CONFIG(wheelevent)
//! [5]
void clExtendGraphicsView::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
//! [5]
#endif
/*
void clExtendGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr("Create your cycle ... "
                       "wheel or the '+' and '-' keys"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
}
*/