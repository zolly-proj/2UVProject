#include "clGraphicsView.h"
#include <QtCore/QDebug>

void clGraphicsView::mousePressEvent(QMouseEvent *event)
{
	
    //if(!itemAt(event->pos())){
    //    std::cout << "Press"<<std::endl;
    //}else{
    //    auto item = this->scene()->itemAt(event->pos(),QTransform());
//         item = static_cast<NodePin*>(item);
//         if(item){
//             emit sendMousePressEventSignal(event);
//             qDebug()<<"dwsto re malaka ";
//         }
    //}
//     node_editor->mousePressEvent(event);
    //QGraphicsView::mousePressEvent(event);
}

void clGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if(!itemAt(event->pos())){
        std::cout << "Press"<<std::endl;
    }else{
		QGraphicsItem * loItem = this->itemAt(event->pos());
        std::cout << loItem << std::endl;
        std::cout << &loItem << std::endl;
        if (loItem != nullptr)
			emit sendMouseDoubleClickEventSignal(loItem);
    }	
    
    // * loItem = (clGraphicsItem *) item;
    
    //std::cout << "Press on : " << loItem->meIndex << std::endl;
	
}

void clGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    /// WORKS!!!!!!!!!!
    auto item = this->scene()->itemAt(event->pos(),QTransform());
//     item = static_cast<NodePin*>(item);
//     if(item){
//     //            qDebug()<<item;
//         emit sendMousePressEventSignal(event);
//     }
//     node_editor->mouseMoveEvent(event);

    QGraphicsView::mouseMoveEvent(event);
}

void clGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    QGraphicsView::mouseReleaseEvent(event);
}

void clGraphicsView::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"CLCLLCLC";
    QGraphicsView::keyPressEvent(event);
}

void clGraphicsView::drawBackground(QPainter *painter, const QRectF &rect){
//    QBrush brush(QColor(240,200,150));
    QBrush brush(Qt::darkGray);
    painter->fillRect(rect,brush);
}
