#define INFO_BUFFER_SIZE 32767
//#include "edge.h"
#include "clNode.h"
//#include "graphwidget.h"



//! [0]
clNode::clNode(QString paNameItem, QString paUUID, QString paClassName, QWidget* paParent, const char* paName)
{
	meName = paNameItem;
	meUUID = paUUID;
	meClassName = paClassName;
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}
//! [0]

//! [1]

void clNode::addEdge(clEdge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<clEdge *> clNode::edges() const
{
    return edgeList;
}

//! [1]

//! [2]

void clNode::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }
//! [2]

//! [3]
    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items()) {
        clNode *node = qgraphicsitem_cast<clNode *>(item);
        if (!node)
            continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }
//! [3]

//! [4]
    // Now subtract all forces pulling items together
    double weight = (edgeList.size() + 1) * 10;
    foreach (clEdge *edge, edgeList) {
        QPointF vec;
        if (edge->sourceNode() == this)
            vec = mapToItem(edge->destNode(), 0, 0);
        else
            vec = mapToItem(edge->sourceNode(), 0, 0);
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }
//! [4]

//! [5]
    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;
//! [5]

//! [6]
    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}
//! [6]

//! [7]

bool clNode::advancePosition()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}
//! [7]

//! [8]
QRectF clNode::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 300 + adjust, 23 + adjust);
}
//! [8]

//! [9]
QPainterPath clNode::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}
//! [9]

//! [10]
void clNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{	
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) 
	{
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
		if (meUUID.toUpper().compare("START") == 0 || meUUID.toUpper().compare("STOP") == 0)
		{
			gradient.setColorAt(1, QColor(Qt::blue).light(120));
			gradient.setColorAt(0, QColor(Qt::darkBlue).light(120));
		}
		else
		{
			gradient.setColorAt(1, QColor(Qt::yellow).light(120));
			gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));			
		}
    } 
	else 
	{
		if (meUUID.toUpper().compare("START") == 0 || meUUID.toUpper().compare("STOP") == 0)
		{
			gradient.setColorAt(0, Qt::blue);
			gradient.setColorAt(1, Qt::darkBlue);			
		}
		else
		{
			gradient.setColorAt(0, Qt::yellow);
			gradient.setColorAt(1, Qt::darkYellow);			
		}		
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
	
	painter->setBrush(Qt::darkGray);
	painter->setPen(QPen(Qt::black, 0));
	const QRect rectangle = QRect(20, 0, 300, 100);
	QRect boundingRect;
	painter->drawText(rectangle, 0, QString("[" + meName + "]"), &boundingRect);		
}
//! [10]

//! [11]
QVariant clNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
	
    switch (change) {
    case ItemPositionHasChanged:
        foreach (clEdge *edge, edgeList)
            edge->adjust();
        //graph->itemMoved();
        break;
    default:
        break;
    };
	
    return QGraphicsItem::itemChange(change, value);
}
//! [11]

//! [12]
void clNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void clNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
//! [12]
void clNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	// Get and display the name of the computer.
	//TCHAR infoBuf[INFO_BUFFER_SIZE];
	//DWORD  bufCharCount = INFO_BUFFER_SIZE;
	//GetComputerName(infoBuf,&bufCharCount);	
	try
	{
		/*
			QMenu menu(this);
            QAction *setParent=menu.addAction("SetParent");
			QAction *setChild=menu.addAction("SetChild");
			QObject::connect(setParent, SIGNAL(triggered()),this,SLOT(slotSetAsParent()));
			QObject::connect(setChild, SIGNAL(triggered()),this,SLOT(slotSetAsChild()));
            menu.exec(QCursor::pos());
			*/
	}
    catch(exception &e)
    {		
        //meIceClientLogging->insertItem("30",QString(infoBuf),"2UVCycleBuilder.exe","clNode::mouseDoubleClickEvent() -> " + QString(e.what()));
    }
}
void clNode::slotSetAsParent()
{
	
}
void clNode::slotSetAsChild()
{
	
}
