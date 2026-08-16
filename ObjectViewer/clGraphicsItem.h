#ifndef CLGRAPHITEM_H
#define CLGRAPHITEM_H
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsRectItem>
#include <QtGui/QMouseEvent>
#include <iostream>
#include <QtCore/QDebug>

#include "clObject.h"

class clGraphicsItem
{

public:
    clGraphicsItem(QGraphicsItem * paItem);
    ~clGraphicsItem();
    /*
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    */
public:   
    /*
    void sendMouseMoveEventSignal(QMouseEvent *event);
    void sendMouseReleaseEventSignal(QMouseEvent *event);
*/
    QGraphicsItem * meItem = nullptr;
	clObject * meObject;
};
#endif //GraphicsView.h
