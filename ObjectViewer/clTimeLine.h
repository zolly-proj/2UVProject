#ifndef clTIMELINE_H
#define clTIMELINE_H

#include <algorithm>
#include <exception>
#include <string>
#include <iostream>

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtGui/QTransform>
#include <QtCore/QDebug>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtCore/QDebug>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtWidgets/QGraphicsItem>
#include <QtCore/QPoint>
#include <QtCore/QDateTime>
#include "Indicator.h"
#include "Track.h"
#include "clGraphicsItem.h"

using namespace std;

class clTimeLine : public QWidget
{
    Q_OBJECT

public:
    explicit clTimeLine(QWidget *parent = nullptr);
    clTimeLine(QGraphicsView *_view, QWidget *_parent = nullptr);
    ~clTimeLine();
    QGraphicsItem * ItemAt(QPointF position){return scene->itemAt(position,QTransform());}
    clGraphicsItem * AddItem(QPointF pos, QRect rect, QPen pen, QBrush brush);
    void SetFrame(int _frame){if(_frame < maxFrame)frame=_frame;}
    void setAxes();
    QGraphicsScene *scene;
    QGraphicsView *view;
//    QPoint indicatorPos;
    QGraphicsItem *indicatorItem;
    QGraphicsItem *indicatorHead;
    Indicator *indicator;

	vector <clGraphicsItem *> meGraphicsItems;

    int frame;
    int minFrame,maxFrame;
    
    long meMinAxes, meMaxAxes;
    int meSpacerInTimeline = 100;
    int meAmountOfEmployees = 0;
};

#endif // TIMELINE_H
