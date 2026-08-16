#include "clClientApplication.h"

clClientApplication::clClientApplication(QWidget* paParent, const char* paName)
{
	meClientSocketBox.setupUi(this);
	
    connect(meClientSocketBox.btnConnect, SIGNAL(clicked()),this,SLOT(slotButtonConnectPressed()));
    connect(meClientSocketBox.btnDisconnect, SIGNAL(clicked()),this,SLOT(slotButtonDisconnectPressed()));
    connect(meClientSocketBox.btnSend, SIGNAL(clicked()),this,SLOT(slotButtonSendPressed()));
	
	
	initializeUI();
}

clClientApplication::~clClientApplication()
{
}
void clClientApplication::initializeUI()
{
    try
    {
	
        meClientSocketBox.txtIP->setText(QString("127.0.0.1"));
		meClientSocketBox.txtPort->setText(QString("1234"));
		meClientSocketBox.txtSend->setText(QString("<hardwareDevice>") +
												QString("<id>45789</id>") +
												QString("<connect do='false'></connect>") +
												QString("<disconnect do='false'></disconnect>") +
												QString("<state do='true'></state>") +
												QString("<run do='false'></run>") +
												QString("<abort do='false'></abort>") +
												QString("<hold do='false'></hold>") +
												QString("<continue do='false'></continue>") +
												QString("<sendFile do='false'></sendFile>") +
												QString("<receiveFile do='false'></receiveFile>") +
												QString("<scriptCommand do='false'>NAMEOFSCRIPTCOMMAND</scriptCommand>") +
												QString("<optionalCommand do='false' name='SendDataCommand2'>") +
												QString("	<data type='int'></data>") +
												QString("	<data type='byte'></data>") +
												QString("	<data type='hex'></data>") +
												QString("	<data type='string'></data>") +
												QString("	<data type='float'></data>") +
												QString("	<data type='double'></data>") +
												QString("</optionalCommand>") +
											QString("</hardwareDevice>"));
	
    }
    catch(...)
    {
        printf("clClientApplication::initializeUI -> error ...");
    }
}
void clClientApplication::slotButtonSendPressed()
{
    try
    {
		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_5_5);
		out << meClientSocketBox.txtSend->toPlainText();
		out.device()->seek(0);
		pSocket->write(meClientSocketBox.txtSend->toPlainText().toUtf8());
		
		pSocket->waitForBytesWritten(500);
		
		pSocket->waitForReadyRead(1000);		
        //meIceClient.insertItem(meClientLogger.txtLogNumber->text(),meClientLogger.txtComputerName->text(),QString("LogClient.exe"),meClientLogger.txtMessage->text());
    }
    catch(...)
    {
        printf("clClientApplication::slotButtonSendMessagePressed -> error ...");
    }

}

void clClientApplication::slotButtonConnectPressed()
{
        try
        {
			pSocket = new QTcpSocket( this ); // <-- needs to be a member variable: QTcpSocket * _pSocket;
			connect( pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );
			
			//Print ip adress and port
			meIp = meClientSocketBox.txtIP->text();
			mePort = meClientSocketBox.txtPort->text();
			
			pSocket->connectToHost(meIp, mePort.toInt());
			pSocket->waitForConnected();

			if (pSocket->state() != QAbstractSocket::ConnectedState ) {
				qDebug() << Q_FUNC_INFO << " can't connect to host";
				delete pSocket;
				meClientSocketBox.rdbConnected->setChecked(false);
				return;
			}
			else
			{
				meClientSocketBox.rdbConnected->setChecked(true);
			}
        }
        catch(...)
        {
            printf("clClientApplication::slotButtonPingPressed -> error ...");
        }
}
void clClientApplication::slotButtonDisconnectPressed()
{
    try
    {
        pSocket->disconnectFromHost();
		delete pSocket;
		meClientSocketBox.rdbConnected->setChecked(false);
    }
    catch(...)
    {
        printf("clClientApplication::slotButtonShutdownPressed -> error ...");
    }
}
void clClientApplication::readTcpData()
{
    meData = pSocket->readAll();
	meClientSocketBox.txtReceive->setText(meData.constData());
}
QString clClientApplication::convertDataFromSocket(QByteArray paByteArray)
{
	char fixed[10000];
	int index = 0;
	QByteArray::iterator iter = paByteArray.begin();
	while(iter != paByteArray.end())
	{
		QChar c = *iter;
		if (c != '\0' && c != '\n') fixed[index++] = c.toLatin1();
		iter++;
	}
	fixed[index] = '\0';
	return QString(fixed);
}

