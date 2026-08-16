#ifndef CLLOGGINGWIDGET_H_WVD_10052012
#define CLLOGGINGWIDGET_H_WVD_10052012

#include <string>
#include <iostream>
#include <vector>

#include <QtCore/QString>
#include <QtCore/QDateTime>

#include <QtGui/QMainWindow>
#include <QtGui/QWorkspace>
#include <QtGui/QFileDialog>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QWidget>
#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QAction>
#include <QtGui/QDockWidget>
#include <QtGui/QTreeWidget>
#include <QtCore/QByteArray>
#include <QThread>
using namespace std;


class clLoggingWidget : public QMainWindow
{
    Q_OBJECT;

public:
    clLoggingWidget(QWidget* paParent = 0, const char* paName = 0);
    ~clLoggingWidget ();
public:


public slots:


private:
    //UI fucntions
    void createListViewDock();


    //Private variables
    QDockWidget *meTreeView;
    QTreeWidget *meTreeWidget;
};

#endif
