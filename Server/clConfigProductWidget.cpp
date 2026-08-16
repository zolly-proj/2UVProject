#include "clConfigProductWidget.h"


clConfigProductWidget::clConfigProductWidget( QWidget* paParent, const char* paName)
{
    try
    {
        this->resize(682,297);
        createTreeViewDock();
    }
    catch(...)
    {

    }
}

clConfigProductWidget::~clConfigProductWidget ()
{
    this->close();
}
/*****************************
* UI functions
***************************************/
void clConfigProductWidget::createTreeViewDock()
{
    try
    {
        //Create the tree widget
        meTreeWidget = new QTreeWidget();
        meTreeView = new QDockWidget(tr("Structure"));
        meTreeView->setWidget(meTreeWidget);
        meTreeView->setAllowedAreas(Qt::LeftDockWidgetArea
                                      | Qt::RightDockWidgetArea);
        //Local function form the main widget
        addDockWidget(Qt::RightDockWidgetArea, meTreeView);
        meTreeView->show();
    }
    catch(...)
    {
    }
}
