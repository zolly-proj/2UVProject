#ifndef CLNODE_H
#define CLNODE_H

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

#include <QtCore/qmath.h>
#include <QtGui/QPainter>
#include "clEdge.h"

class clEdge;
/*
class GraphWidget;
*/

using namespace std;


QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

//! [0]
class clNode : public QObject, public QGraphicsItem
{
	
 Q_OBJECT;
	
	
public:
    //clNode(GraphWidget *graphWidget);
	clNode(QString paNameItem, QString paUUID, QString paClassName, QWidget* paParent = 0, const char* paName = 0);

    void addEdge(clEdge *edge);
    QList<clEdge *> edges() const;

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
	QString meChildren;
public slots:
	void slotSetAsParent();
	void slotSetAsChild();
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QList<clEdge *> edgeList;
    QPointF newPos;
    //GraphWidget *graph;

};

#endif
