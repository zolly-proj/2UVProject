#ifndef CLEDGE_H
#define CLEDGE_H

#include <QtWidgets/QGraphicsItem>


#include <QtCore/qmath.h>
#include <QtGui/QPainter>

#include "clNode.h"


class clNode;

//! [0]
class clEdge : public QGraphicsItem
{
public:
    clEdge(clNode *sourceNode, clNode *destNode);

    clNode *sourceNode() const;
    clNode *destNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }
	
	QString meUUID;
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    clNode *source, *dest;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};
//! [0]

#endif // EDGE_H
