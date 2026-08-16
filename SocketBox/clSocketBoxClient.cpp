#include "clSocketBoxClient.h"

clSocketBoxClient::clSocketBoxClient(QString paIp,QString paPort, QObject * parent)
{
    try
    {		
        meIp = paIp;
		mePort = paPort;

    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
    }	
    catch(...)
    {
        printf("clSocketBoxClient::clSocketBoxClient(QString paConfigFile) -> error ...");
    }
}

clSocketBoxClient::~clSocketBoxClient ()
{
}

void clSocketBoxClient::sendToServer()
{
	try
	{
		pSocket = new QTcpSocket( this ); // <-- needs to be a member variable: QTcpSocket * _pSocket;
		connect( pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );
		
		//Print ip adress and port
		QByteArray loIp = meIp.toLatin1();
		char *loStringContent = loIp.data();
		cout << loStringContent << endl;		
		cout << mePort.toInt() << endl;
		
		pSocket->connectToHost(meIp, mePort.toInt());
		pSocket->waitForConnected();

		if (pSocket->state() != QAbstractSocket::ConnectedState ) {
			qDebug() << Q_FUNC_INFO << " can't connect to host";
			delete pSocket;
			return;
		}

		QByteArray block;
		QDataStream out(&block, QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_5_5);
		out << QString("Hello");
		out.device()->seek(0);
		cout << pSocket->write(block) << endl;
		
		pSocket->waitForBytesWritten(500);
		
		pSocket->waitForReadyRead(500);
	}
	catch (...)
	{
		cout << "Fail" << endl;
	}
}

void clSocketBoxClient::readTcpData()
{
    meData = pSocket->readAll();
	qDebug() << " Data in: " << meData;
}