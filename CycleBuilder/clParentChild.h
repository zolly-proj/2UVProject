#ifndef CLPARENTCHILD_H
#define CLPARENTCHILD_H

#include <exception>
#include <string>
#include <iostream>


#include <QtWidgets/QGraphicsItem>
#include <QtCore/QList>

#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QAction>

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QStyleOption>

/*
class Edge;
class GraphWidget;
*/

using namespace std;


QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

//! [0]
class clParentChild : public QObject, public QGraphicsItem
{
	
 Q_OBJECT;
	
	
public:
    //clNode(GraphWidget *graphWidget);
	clParentChild(QString paNameItem, QString paUUID, QString paClassName, QWidget* paParent = 0, const char* paName = 0);

    //void addEdge(Edge *edge);
    //QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    void calculateForces();
    bool advancePosition();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	QString meName;
	QString meUUID;
	QString meClassName;
	
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	
private:
    //QList<Edge *> edgeList;
    QPointF newPos;
    //GraphWidget *graph;

};

#endif
