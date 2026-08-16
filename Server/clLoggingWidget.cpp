#include "clConfigProductWidget.h"


clLoggingWidget::clLoggingWidget( QWidget* paParent, const char* paName)
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

clLoggingWidget::~clLoggingWidget ()
{
    this->close();
}
/*****************************
* UI functions
***************************************/
void clConfigProductWidget::createListViewDock()
{
    try
    {

    }
    catch(...)
    {
    }
}
