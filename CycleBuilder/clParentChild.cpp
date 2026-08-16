#define INFO_BUFFER_SIZE 32767
//#include "edge.h"
#include "clParentChild.h"
//#include "graphwidget.h"



//! [0]
clParentChild::clParentChild(QString paNameItem, QString paUUID, QString paClassName, QWidget* paParent, const char* paName)
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
/*
void clNode::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> clNode::edges() const
{
    return edgeList;
}
*/
//! [1]

//! [2]
/*
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
        Node *node = qgraphicsitem_cast<Node *>(item);
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
    foreach (Edge *edge, edgeList) {
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
*/
bool clParentChild::advancePosition()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}
//! [7]

//! [8]
QRectF clParentChild::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 300 + adjust, 100 + adjust);
}
//! [8]

//! [9]
QPainterPath clParentChild::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 300, 50);
    return path;
}
//! [9]

//! [10]
void clParentChild::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{


	painter->setBrush(Qt::gray);
	painter->setPen(QPen(Qt::black, 1));
    painter->drawRect(-10,-10,300,70);
    
	painter->setPen(QPen(Qt::black, 0));
	
	QRect boundingRect;
	const QRect rectangle1 = QRect(0, 0, 300, 20);
	painter->drawText(rectangle1, 0, QString("Select child or exit (double click):"), &boundingRect);	
	const QRect rectangle2 = QRect(0, 20, 300, 20);
	painter->drawText(rectangle2, 0, QString(meClassName + " [" + meUUID + "]"), &boundingRect);
	const QRect rectangle3 = QRect(0, 40, 300, 20);
	painter->drawText(rectangle3, 0, QString("NAME [" + meName + "]"), &boundingRect);	
}
//! [10]

//! [11]
QVariant clParentChild::itemChange(GraphicsItemChange change, const QVariant &value)
{
	/*
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };
	*/
    return QGraphicsItem::itemChange(change, value);
}
//! [11]

//! [12]
void clParentChild::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void clParentChild::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
