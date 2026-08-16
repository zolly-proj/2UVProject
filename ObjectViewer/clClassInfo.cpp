#define INFO_BUFFER_SIZE 32767
#include "clClassInfo.h"

clClassInfo::clClassInfo(QString paTableName, vector <clDatabaseColumn> paDatabaseColumns, QWidget* paParent, const char* paName)
{
    meDatabaseColumns = paDatabaseColumns;
	meTableName = paTableName;
	
    initializeElements();
	readMappingIcons();
	
    createGridGroupBox();

    meMainLayout = new QVBoxLayout();
	
	//Create a widget and set its layout as your new layout created above
	viewport = new QWidget;
	viewport->setLayout(layout);

	//Add the viewport to the scroll area
	scrollArea = new QScrollArea;
	scrollArea->setWidget(viewport);
	
    meMainLayout->addWidget(scrollArea);
    setLayout(meMainLayout);
}
clClassInfo::~clClassInfo()
{
}
void clClassInfo::readMappingIcons()
{
	try
	{
		QFile inputFile(".\\ICONS\\mapping.txt");
		if (inputFile.open(QIODevice::ReadOnly))
		{
		   QTextStream in(&inputFile);
		   while (!in.atEnd())
		   {
			  QString line = in.readLine();
			  QStringList loElements = line.split(";");
			  if (loElements.size() != 0)
			  {
				  if (loElements.size() == 2)
				  {
					  meIcon.push_back(loElements.at(0));
					  meClass.push_back(loElements.at(1));
				  }
			  }
		   }
		   inputFile.close();
		}
	}
	catch(...)
	{
			cout << "Mapping problem ..." << endl;
	}
}
void clClassInfo::initializeElements()
{
    try
    {

    }
    catch(...)
    {
    }
}


 void clClassInfo::createGridGroupBox()
 {
	try
	{

		layout = new QGridLayout;

		meLabels[0] = new QLabel("Class name:");
		meLabels[1] = new QLabel(meTableName);
		layout->addWidget(meLabels[0], 1, 0);
		layout->addWidget(meLabels[1], 1, 1);
		
		meLabels[3] = new QLabel("Columns:");
		layout->addWidget(meLabels[3], 2, 0);
		
		meTreeWidget = new QTreeWidget();
		meTreeWidget->setColumnCount(5);
		QStringList loColumnNames;
		loColumnNames << "Name" << "Alias" << "Type" << "Extra" << "Reference";
		meTreeWidget->setHeaderLabels(loColumnNames);		
		
		for (int i = 0; i < meDatabaseColumns.size(); ++i) 
		{	
			clDatabaseColumn loDatabaseColumn = meDatabaseColumns.at(i);
			
			clClassObjectTreeItem *newItem;
			newItem = new clClassObjectTreeItem(meTreeWidget, meTreeWidget->currentItem());
			const QIcon Icon(".\\ICONS\\" + getRelatedIcon(meTableName));
			newItem->setIcon(0,Icon);
			newItem->setText(0,loDatabaseColumn.getName());	
			newItem->setText(1,loDatabaseColumn.getAlias());	
			newItem->setText(2,loDatabaseColumn.getType());	
			newItem->setText(3,loDatabaseColumn.getExtra());	
			newItem->setText(4,loDatabaseColumn.getReference());
			
		}	
		layout->addWidget(meTreeWidget, 2, 1);
		 
		meButtons[0] = new QPushButton(QString("Close"));
		connect(meButtons[0], SIGNAL(clicked()), this, SLOT(slotButtonOKPressed()));
		layout->addWidget(meButtons[0], 3, 0);

	}
	catch(exception &e)
	{
		
		
	}
 }


void clClassInfo::slotButtonOKPressed()
{
    try
    {		
        this->done(0);
    }
    catch(exception &e)
    {
    }
}
QString clClassInfo::getRelatedIcon(QString paClassName)
{
	if(meClass.size() > 0)
	{
		for (int i = 0; i < meClass.size(); i++)
		{
			if (paClassName.toUpper().compare(meClass.at(i)) == 0)
			{
					return meIcon.at(i);
			}
		}
		
	}
	return QString("");
}