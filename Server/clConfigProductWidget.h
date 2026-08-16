#ifndef CLCONFIGPRODUCTWIDGET_H_WVD_13102011
#define CLCONFIGPRODUCTWIDGET_H_WVD_13102011

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>
#include <QtCore/QDateTime>

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTreeWidget>
#include <QtCore/QByteArray>
#include <QtCore/QThread>
using namespace std;


class clConfigProductWidget : public QMainWindow
{
    Q_OBJECT;

public:
    clConfigProductWidget(QWidget* paParent = 0, const char* paName = 0);
    ~clConfigProductWidget ();
public:


public slots:


private:
    //UI fucntions
    void createTreeViewDock();


    //Private variables
    QDockWidget *meTreeView;
    QTreeWidget *meTreeWidget;
};

#endif
