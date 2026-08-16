#define INFO_BUFFER_SIZE 32767
#include "clShowQrCodeOfObjectUI.h"

clShowQrCodeOfObjectUI::clShowQrCodeOfObjectUI(clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging, QWidget* paParent, const char* paName)
{
    meIceClientLogging = paIceClientLogging;
    meIceClientServer = paIceClientServer;
    
    meMainLayout = new QVBoxLayout();
	
	meLabels[0] = new QLabel;
	meLabels[1] = new QLabel;

	meButtons[1] = new QPushButton(tr("Close"));
	meButtons[0] = new QPushButton(tr("Save ass"));
	connect(meButtons[0], SIGNAL(clicked()), this, SLOT(slotButtonSaveAssPressed()));
    connect(meButtons[1], SIGNAL(clicked()), this, SLOT(slotButtonCancelPressed()));
	
    meMainLayout->addWidget(meLabels[0]);
	meMainLayout->addWidget(meLabels[1]);
	
	meMainLayout->addWidget(meButtons[0]);
	meMainLayout->addWidget(meButtons[1]);
    setLayout(meMainLayout);
	
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	//QApplication::setQuitOnLastWindowClosed(false);
}
clShowQrCodeOfObjectUI::~clShowQrCodeOfObjectUI()
{
}
void clShowQrCodeOfObjectUI::slotButtonSaveAssPressed()
{
    try
    {
        QString fname = QFileDialog::getSaveFileName(nullptr, "Save QR code as...", ".", "Images (*.png)" );
		
		//Check if the file exists
		QFileInfo loCheck_file(fname);    
		if (loCheck_file.exists() && loCheck_file.isFile()) 
		{ 
			//Delete the file
			QFile loFile (fname);
			loFile.remove();
		}		
		
		meImage->save(QString(fname), "PNG", 100);
    }
    catch(exception &e)
    {
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clShowQrCodeOfObjectUI::slotButtonSaveAssPressed() -> " + QString(e.what()));
    }
}

void clShowQrCodeOfObjectUI::slotButtonCancelPressed()
{
    try
    {	
	
        this->done(1);
    }
    catch(exception &e)
    {
		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clShowQrCodeOfObjectUI::slotButtonCancelPressed() -> " + QString(e.what()));
    }
}


