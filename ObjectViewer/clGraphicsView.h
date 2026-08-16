#ifndef CLGRAPHVIEW_H
#define CLGRAPHVIEW_H
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtGui/QMouseEvent>
#include <iostream>
#include <QtCore/QDebug>

#include "clGraphicsItem.h"

class clGraphicsView:public QGraphicsView
{
	
	Q_OBJECT;
	
public:
    clGraphicsView(QGraphicsScene *scene):QGraphicsView(scene){}
    
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent *event) override;   

signals:
	void sendMouseDoubleClickEventSignal(QGraphicsItem * paItem);
	/*
    void sendMousePressEventSignal(QMouseEvent *event);
    void sendMouseMoveEventSignal(QMouseEvent *event);
    void sendMouseReleaseEventSignal(QMouseEvent *event);
    */
	    
    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

    // QGraphicsView interface
protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
};
#endif //GraphicsView.h
