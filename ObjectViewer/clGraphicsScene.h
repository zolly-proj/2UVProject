#ifndef CLGRAPHSCENE_H
#define CLGRAPHSCENE_H
#include <QtWidgets/QGraphicsScene>
#include <QtGui/QMouseEvent>
#include <iostream>
#include <QtCore/QDebug>

class clGraphicsScene: public QGraphicsScene
{
    Q_OBJECT
public:
    clGraphicsScene(QWidget *parent = nullptr):QGraphicsScene(nullptr){}
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void sendSceneMouseEventSignal(QGraphicsSceneMouseEvent *event);
};
#endif // GRAPHSCENE_H
