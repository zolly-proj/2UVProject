
#include "clGraphicsScene.h"
void clGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit sendSceneMouseEventSignal(event);
    QGraphicsScene::mousePressEvent(event);
}

void clGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit sendSceneMouseEventSignal(event);
    QGraphicsScene::mouseReleaseEvent(event);
}

void clGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit sendSceneMouseEventSignal(event);
    QGraphicsScene::mouseMoveEvent(event);
}
