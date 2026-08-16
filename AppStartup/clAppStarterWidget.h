#ifndef CLAPPSTARTERWIDGET_H_WVD
#define CLAPPSTARTERWIDGET_H_WVD

#include <string>
#include <iostream>
#include <fstream>

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QProcess>
#include <QtCore/QTimer>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QAction>
#include <QtCore/QByteArray>
#include <QtCore/QThread>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QScrollArea>


#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>

using namespace std;


class clAppStarterWidget : public QMainWindow
{

    Q_OBJECT;

public:
    clAppStarterWidget (char * paIniFile, QWidget* paParent = 0, const char* paName = 0); //, WFlags paFlags = 0);
    ~clAppStarterWidget ();


    QThread meThread;

public slots:
    //Slots Camera setup
        /*
    void slotButtonPressed();
    void slotWriteToDB();
    void slotGetFromDB();
    */
	void slotClose();
	void startPressed();
	void stopPressed();
	void slotCheckProcesses();
private:
	void initializeUI();
	bool readXmlFile(char * paFileName);
	bool stopProcess(int paId);
	bool startProcess(int paId);
	
	vector <QString> meProgram;
	vector <QString> meName;
	vector <QString> meArgument;
	vector <QString> meWorkingDir;
	
	
	QLabel *meHeader;
	QPushButton *meClose;
    QVBoxLayout *meMainLayout;
	QWidget *viewport;
	QGridLayout *layout;
	QScrollArea *scrollArea;
	QWidget *meMainWidget;	
	
	QProcess *meProcess[100];
	QLabel *mePrograms[100];
	QLabel *meProgramStatus[100];
	QPushButton *meStartProgram[100];
	QPushButton *meStopProgram[100];
	QLabel *mePid[100];
	QPixmap *mePixmap[100];
	QImage imStart;
	QImage imStop;
	QPixmap *mePixmapStart;
	QPixmap *mePixmapStop;
	QTimer* meTimer;	
};

#endif
