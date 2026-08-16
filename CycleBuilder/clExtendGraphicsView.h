#ifndef CLEXTENDGRAPHICSVIEW_H
#define CLEXTENDGRAPHICSVIEW_H

#include <exception>
#include <string>
#include <iostream>

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
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QDateTimeEdit>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QSignalMapper>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QMessageBox>

#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>


#include "clIceClientLogging.h"
#include "clIceClientServer.h"
#include "clClassObjectTreeItem.h"
#include "clDatabaseRow.h"
#include "clNode.h"
#include "clEdge.h"
#include "clParentChild.h"



using namespace std;

class clExtendGraphicsView : public QGraphicsView
{

    Q_OBJECT;

public:

    clExtendGraphicsView(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QWidget* paParent = 0, const char* paName = 0);
    ~clExtendGraphicsView ();
	
	void initializeGraphView(vector <clDatabaseRow> paDatabaseRows);
	vector<clDatabaseRow> safeCycle(vector<clDatabaseRow> paDatabaseRows);
	
	vector <clNode*> getParents(clNode * paNode);
	vector <clNode*> getChilds(clNode * paNode);

	
protected:

  void dragEnterEvent(QDragEnterEvent* event) override;
  void dropEvent(QDropEvent* event) override;
  void dragMoveEvent(QDragMoveEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif  
  //void drawBackground(QPainter *painter, const QRectF &rect) override;  
  
  void scaleView(qreal scaleFactor);
public slots:
	void slotSetAsParent();
	void slotSetAsChild();
	void slotDelete();
	
private:
	void buildChildren(clNode * paNodeStop, clNode * loParentNode, QStringList loChildren, vector<clDatabaseRow> paDatabaseRows, int paXpos, int paYpos);
	clNode* nodeAlreadyExist(QString paUUID);
	clEdge* edgeAlreadyExist(clNode *paSourceNode, clNode *paDesitnationNode);
	
    clIceClientLogging * meIceClientLogging;
    clIceClientServer * meIceClientServer;

	QGraphicsScene * meGraphicsScene;
	
	QString meSelectedItem;
	QString meSelectedItemUUID;
	
	QString meSelectedEdgeUUID;

	QString meParentName;
	QString meParentUUID;
	QString meChildName;
	QString meChildUUID;
	

};

#endif
