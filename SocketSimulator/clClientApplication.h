#ifndef CLSOCKETBOXCLIENTUI_H_WVD
#define CLSOCKETBOXCLIENTUI_H_WVD


#include <string>
#include <iostream>

#include "wdClientSocketBoxUi.h"

#include <QtWidgets/QMainWindow>

#include <QtCore/QString>

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

#include <QtWidgets/QWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtCore/QByteArray>
#include <QtCore/QThread>

using namespace std;

class clClientApplication : public QWidget
{

    Q_OBJECT;

public:

    clClientApplication (QWidget* paParent = 0, const char* paName = 0); //, WFlags paFlags = 0);
    ~clClientApplication ();

    Ui::wdClientSocketBoxUi meClientSocketBox;
	void sendToServer();
	QString convertDataFromSocket(QByteArray paByteArray);
public slots:
    //Slots Camera setup
    void slotButtonConnectPressed();
    void slotButtonDisconnectPressed();
    void slotButtonSendPressed();
	void readTcpData();
private:
	void initializeUI();
	QString meIp;
	QString mePort;
	QTcpSocket *pSocket;
	
	QByteArray meData;
};

#endif
