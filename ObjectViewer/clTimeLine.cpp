#include "clTimeLine.h"

clTimeLine::clTimeLine(QWidget *parent) :
    QWidget(parent)
{
}
QColor Track::selectedColor = QColor(200,30,180);
QColor Track::selectedColorOutline = Track::selectedColor.lighter(30);
int Track::penWidth = 2;
bool Track::hasShadow = false;
float Track::thresholdShadow=0.0f;
int Track::rounded = 10;

clTimeLine::clTimeLine(QGraphicsView *_view, QWidget *_parent)
{
    if (_view) view = _view;
    else view = new QGraphicsView(this);
    scene = view->scene();
	setMouseTracking(true);
    


	/*
    indicator = new Indicator(height());
    scene->addItem(indicator);
    indicator->setZValue(101);


    Track *track = new Track(200,QColor(Qt::cyan),scene);
    scene->addItem(track);
    track->setPos(QPointF(300,0));
    */
}

clTimeLine::~clTimeLine()
{
}

void clTimeLine::setAxes()
{
    for(int i = 0; i < ((meMaxAxes-meMinAxes)/900); i++){
		QDateTime loDateTime;
		loDateTime.setSecsSinceEpoch( (900 * i) + meMinAxes);
		
        QGraphicsItem *item = scene->addText(loDateTime.toString("dd.MM.yy hh:mm"));
        item->setPos(i*meSpacerInTimeline,-40);
        item->setRotation(-90);
        
        QPen pen = QPen(Qt::black,1);
        QGraphicsItem *item_line = scene->addLine(i*meSpacerInTimeline,-40,i*meSpacerInTimeline,100 * meAmountOfEmployees + 1,pen);
        
        
    }
}

clGraphicsItem * clTimeLine::AddItem(QPointF pos, QRect rect, QPen pen, QBrush brush)
{
    QGraphicsItem *item = scene->addRect(rect,pen,brush);
    item->setPos(pos);        
    clGraphicsItem * loGraphicsItem = new clGraphicsItem(item);
    meGraphicsItems.push_back(loGraphicsItem);
    return loGraphicsItem;
}
