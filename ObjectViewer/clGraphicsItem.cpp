#include "clGraphicsItem.h"
#include <QtCore/QDebug>

clGraphicsItem::clGraphicsItem(QGraphicsItem * paItem)
{
	meItem = paItem;
}
clGraphicsItem::~clGraphicsItem()
{

}



/*
void clGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	try
	{
		emit sendMouseDoubleClickEvent(meIndex);
	}
	catch(exception &e)
	{
		qDebug()<<"Exception trown";
	}
}
*/
//	qDebug()<<"Mouse Press event";
	/*
    if(!itemAt(event->pos())){
        std::cout << "Press"<<std::endl;
    }else{
		*/
		
		
		
		
        //auto item = this->scene()->itemAt(event->pos(),QTransform());
//         item = static_cast<NodePin*>(item);
//         if(item){
//             emit sendMousePressEventSignal(event);
//             qDebug()<<"dwsto re malaka ";
//         }
/*
    }
    */
//     node_editor->mousePressEvent(event);
    //QGraphicsView::mousePressEvent(event);
//}

//void clGraphicsItem::mouseMoveEvent(QMouseEvent *event)
//{
//	qDebug()<<"Mouse move event";
    /// WORKS!!!!!!!!!!
   // auto item = this->scene()->itemAt(event->pos(),QTransform());
//     item = static_cast<NodePin*>(item);
//     if(item){
//     //            qDebug()<<item;
//         emit sendMousePressEventSignal(event);
//     }
//     node_editor->mouseMoveEvent(event);

//    QGraphicsView::mouseMoveEvent(event);
//}

//void clGraphicsItem::mouseReleaseEvent(QMouseEvent *event){
//    QGraphicsView::mouseReleaseEvent(event);
//	qDebug()<<"Mouse Release event";
//}
