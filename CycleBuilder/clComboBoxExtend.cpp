#define INFO_BUFFER_SIZE 32767
#include "clComboBoxExtend.h"
clComboBoxExtend::clComboBoxExtend(clIceClientLogging * paIceClientLogging, clDatabaseColumn paDatabaseColumn, QWidget* paParent): meDatabaseColumn(paDatabaseColumn)
{
    meIceClientLogging = paIceClientLogging;
    initializeElements();

}
clComboBoxExtend::~clComboBoxExtend()
{
}
void clComboBoxExtend::initializeElements()
{
    try
    {
        QString loFileName = QString(".\\CONFIG\\enums.xml");

        QFile loFile(loFileName);
        if ( !loFile.open( QIODevice::ReadOnly ) ) {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clComboBoxExtend::initializeElements -> Cound not open file '.\\CONFIG\\enumns.xml' ...");
            return;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clComboBoxExtend::initializeElements -> Cound not parse file '.\\CONFIG\\enumns.xml' ...");
            loFile.close();
            return;
        }

        // create the tree view out of the DOM
       QDomElement loDocElem = loDomDocument.documentElement();

       QDomNode loDomNode = loDocElem.firstChild();
       while( !loDomNode.isNull() ) 
	   {

              QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
              if( !loDocElemChild.isNull() )// the node was really an element.
              {
                  if (loDocElemChild.hasAttribute(QString("name")))
                  {
                        //QString loValue = loDocElemChild.attribute(paAttribute);
                        //paValue = &loValue;
						QString loValue;
                        loValue = loDocElemChild.attribute("name");
						if (loValue.toUpper().compare(meDatabaseColumn.getName().toUpper()) == 0)
						{
								QDomNode loNodeEnum = loDocElemChild.firstChild();
								while (!loNodeEnum.isNull() )
								{
									QDomElement loElementEnum = loNodeEnum.toElement();
									if (!loElementEnum.isNull())
									{
										if (loElementEnum.hasAttribute(QString("value")) && loElementEnum.hasAttribute(QString("text")))
										{
											this->addItem(loElementEnum.attribute("text"));
										}
									}
									loNodeEnum = loNodeEnum.nextSibling();
								}
								break;
						}
                  }
              }
		loDomNode = loDomNode.nextSibling();
        }
    }
    catch(exception &e)
    {		
        meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clComboBoxExtend::initializeElements" + QString(e.what()));
    }
}
QString clComboBoxExtend::getValue()
{
	try
	{
		QString loSelectedValue = this->currentText();
        QString loFileName = QString(".\\CONFIG\\enums.xml");

        QFile loFile(loFileName);
        if ( !loFile.open( QIODevice::ReadOnly ) ) {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clComboBoxExtend::initializeElements -> Cound not open file '.\\CONFIG\\enumns.xml' ...");
            return QString("0");
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clComboBoxExtend::initializeElements -> Cound not parse file '.\\CONFIG\\enumns.xml' ...");
            loFile.close();
            return QString("0");
        }

        // create the tree view out of the DOM
       QDomElement loDocElem = loDomDocument.documentElement();

       QDomNode loDomNode = loDocElem.firstChild();
       while( !loDomNode.isNull() ) 
	   {

              QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
              if( !loDocElemChild.isNull() )// the node was really an element.
              {
                  if (loDocElemChild.hasAttribute(QString("name")))
                  {
                        //QString loValue = loDocElemChild.attribute(paAttribute);
                        //paValue = &loValue;
						QString loValue;
                        loValue = loDocElemChild.attribute("name");
						if (loValue.toUpper().compare(meDatabaseColumn.getName().toUpper()) == 0)
						{
								QDomNode loNodeEnum = loDocElemChild.firstChild();
								while (!loNodeEnum.isNull() )
								{
									QDomElement loElementEnum = loNodeEnum.toElement();
									if (!loElementEnum.isNull())
									{
										if (loElementEnum.hasAttribute(QString("value")) && loElementEnum.hasAttribute(QString("text")))
										{
											if(QString(loElementEnum.attribute("text")).toUpper().compare(loSelectedValue.toUpper()) == 0)
											{
												return QString(loElementEnum.attribute("value"));
											}
										}
									}
									loNodeEnum = loNodeEnum.nextSibling();
								}
						}
                  }
              }
			  loDomNode = loDomNode.nextSibling();
        }
		return QString("0");
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clComboBoxExtend::getValue" + QString(e.what()));
		return QString("0");
	}
}
bool clComboBoxExtend::setValue(QString paValue)
{
	try
	{
		QString loSelectedValue = this->currentText();
        QString loFileName = QString(".\\CONFIG\\enums.xml");

        QFile loFile(loFileName);
        if ( !loFile.open( QIODevice::ReadOnly ) ) {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clComboBoxExtend::initializeElements -> Cound not open file '.\\CONFIG\\enumns.xml' ...");
            return false;
        }

        //QDomDocument
        QDomDocument loDomDocument;
        if ( !loDomDocument.setContent( &loFile ) ) {
			meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clComboBoxExtend::initializeElements -> Cound not parse file '.\\CONFIG\\enumns.xml' ...");
            loFile.close();
            return false;
        }

        // create the tree view out of the DOM
       QDomElement loDocElem = loDomDocument.documentElement();

       QDomNode loDomNode = loDocElem.firstChild();
       while( !loDomNode.isNull() ) 
	   {

              QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
              if( !loDocElemChild.isNull() )// the node was really an element.
              {
                  if (loDocElemChild.hasAttribute(QString("name")))
                  {
                        //QString loValue = loDocElemChild.attribute(paAttribute);
                        //paValue = &loValue;
						QString loValue;
                        loValue = loDocElemChild.attribute("name");
						if (loValue.toUpper().compare(meDatabaseColumn.getName().toUpper()) == 0)
						{
								QDomNode loNodeEnum = loDocElemChild.firstChild();
								while (!loNodeEnum.isNull() )
								{
									QDomElement loElementEnum = loNodeEnum.toElement();
									if (!loElementEnum.isNull())
									{
										if (loElementEnum.hasAttribute(QString("value")) && loElementEnum.hasAttribute(QString("text")))
										{
											if(QString(loElementEnum.attribute("value")).toUpper().compare(paValue.toUpper()) == 0)
											{
												QString loValueText = loElementEnum.attribute("text");
												int index = this->findText(loValueText);
												
												if ( index != -1 ) // -1 for not found
													this->setCurrentIndex(index);	
											}
										}
									}
									loNodeEnum = loNodeEnum.nextSibling();
								}
						}
                  }
              }
			  loDomNode = loDomNode.nextSibling();
        }
		return true;
	}
	catch(exception &e)
	{
		meIceClientLogging->insertItem("10",QString(QHostInfo::localHostName()),"2UVServerTest.exe","clComboBoxExtend::setValue" + QString(e.what()));
		return false;
	}
}
