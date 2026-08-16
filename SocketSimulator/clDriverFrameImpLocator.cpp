#include "clDriverFrameImpLocator.h"
clDriverFrameImpLocator::clDriverFrameImpLocator()
{
    try
    {		

    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
    }	
    catch(...)
    {
        printf("clDriverFrame::clDriverFrame(vector <QString> paParameters) -> error ...");
    }
}
clDriverFrameImpLocator::~clDriverFrameImpLocator ()
{
}
bool clDriverFrameImpLocator::createPluginClass(const vector <QString> &paParameters)
{
    try
    {	
		meParameters = paParameters;
		return true;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
			return false;
    }	
    catch(...)
    {
        printf("clDriverFrame::createPluginClass(QString paConfigFile) -> error ...");
		return false;
    }
}

std::vector<QString> clDriverFrameImpLocator::GetParameters() const
{
	return meParameters;
}
bool clDriverFrameImpLocator::parseInputXMLcommand(QByteArray paCommandString)
{
    try
    {	
        //QDomDocument
        QDomDocument loDomDocument;
		
		QString loValue = paCommandString.constData();
		//cout << paCommandString.constData() << endl;
		
		
        if ( !loDomDocument.setContent( loValue.trimmed() ) ) {
            QString loTemp("clDriverFrame::parseInputXMLcommand -> Cound not parse file '");
			cout << loTemp.toStdString() << endl;
            return false;
        }

        // create the tree view out of the DOM
       QDomElement loDocElem = loDomDocument.documentElement();

       QDomNode loDomNode = loDocElem.firstChild();
       while( !loDomNode.isNull() ) 
	   {
		    
            QDomElement loDocElemChild = loDomNode.toElement(); // try to convert the node to an element.
			//cout << loDocElemChild.tagName().toStdString() << endl;
			//cout << loDocElemChild.text().toStdString() << endl;
            if( !loDocElemChild.isNull() )// the node was really an element.
            {	
				//Get the id of the command
				if(loDocElemChild.tagName().compare("id") == 0)
				{
					meCommand.clear();
					//cout << loDocElemChild.text().toStdString() << endl;
					meCommand.push_back(loDocElemChild.text());
				}
                else if (loDocElemChild.hasAttribute(QString("do")))
                {
                        //QString loValue = loDocElemChild.attribute(paAttribute);
                        //paValue = &loValue;
                        QString loValue = loDocElemChild.attribute("do");
                        QString loTemp("Attribute do is '" + loValue + "' for node " + loDocElemChild.tagName());
						if (loValue.compare("true") == 0)
						{
							meCommand.push_back(loDocElemChild.tagName());
							meCommand.push_back(loDocElemChild.text());
							return true;
						}
                }
            }
			
       		  
			loDomNode = loDomNode.nextSibling();
       }		
		return true;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
			return false;
    }	
    catch(...)
    {
        printf("clDriverFrameImpLocator::parseInputXMLcommand(QString paConfigFile) -> error ...");
		return false;
    }	
}
bool clDriverFrameImpLocator::getReturnCommandFinished()
{
    try
    {	
		meReplay.clear();
		meReplay.push_back(QString(QString("<returnHardwareDevice>") +
									QString("	<id>" + meCommand[0] + "</id>") +
									QString("	<state>W1R0P0D0C1T084005</state>") +
									QString("	<datas>") +
									QString("		<object name=\"Magazijn_1001\"><x value=\"30.5\"/><y value=\"30.5\"/><z value=\"1.0\"/></object><object name=\"Employee_200\"><x value=\"60.5\"/><y value=\"50.5\"/><z value=\"0.5\"/></object>") +
									QString("	</datas>") +
									QString("	<error></error>") +
									QString("</returnHardwareDevice>")));
		return true;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
			return false;
    }	
    catch(...)
    {
        printf("clDriverFrame::getReturnCommand(QString paConfigFile) -> error ...");
		return false;
    }		
}
std::vector<QString> clDriverFrameImpLocator::GetCommand() const
{
	return meCommand;
}
std::vector<QString> clDriverFrameImpLocator::GetReplay() const
{
	return meReplay;
}
///////////////////////////////
//Functions to be overridden //
///////////////////////////////
QString clDriverFrameImpLocator::doConnect(QString paValue, QString paParameters) const
{
	QString loReturnValue;
    try
    {	cout << "doConnect" << endl;	
		return loReturnValue;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());
			return loReturnValue;
    }	
    catch(...)
    {
        printf("clDriverFrame::doConnect(QString paConfigFile) -> error ...");
		return loReturnValue;
    }	
}
QString clDriverFrameImpLocator::doDisconnect(QString paValue, QString paParameters) const
{
	QString loReturnValue;
    try
    {		
		cout << "doDisconnect" << endl;		
		return loReturnValue;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());
			return loReturnValue;
    }	
    catch(...)
    {
        printf("clDriverFrame::doConnect(QString paConfigFile) -> error ...");
		return loReturnValue;
    }	
}
QString clDriverFrameImpLocator::doState(QString paValue, QString paParameters) const
{
	QString loReturnValue;
    try
    {		
		cout << "doState" << endl;	
		return loReturnValue;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());
			return loReturnValue;
    }	
    catch(...)
    {
        printf("clDriverFrame::doConnect(QString paConfigFile) -> error ...");
		return loReturnValue;
    }	
}
QString clDriverFrameImpLocator::doRun(QString paValue, QString paParameters) const
{
	QString loReturnValue;
    try
    {		
		cout << "doRun" << endl;	
		return loReturnValue;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());
			return loReturnValue;
    }	
    catch(...)
    {
        printf("clDriverFrame::doConnect(QString paConfigFile) -> error ...");
		return loReturnValue;
    }	
}
QString clDriverFrameImpLocator::doAbort(QString paValue, QString paParameters) const
{
	QString loReturnValue;
    try
    {		
		cout << "doAbort" << endl;	
		return loReturnValue;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());
			return loReturnValue;
    }	
    catch(...)
    {
        printf("clDriverFrame::doConnect(QString paConfigFile) -> error ...");
		return loReturnValue;
    }	
}
QString clDriverFrameImpLocator::doHold(QString paValue, QString paParameters) const
{
	QString loReturnValue;
    try
    {		
		cout << "doHold" << endl;	
		return loReturnValue;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());
			return loReturnValue;
    }	
    catch(...)
    {
        printf("clDriverFrame::doConnect(QString paConfigFile) -> error ...");
		return loReturnValue;
    }	
}
QString clDriverFrameImpLocator::doContinue(QString paValue, QString paParameters) const
{
	QString loReturnValue;
    try
    {		
		cout << "doContinue" << endl;	
		return loReturnValue;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());
			return loReturnValue;
    }	
    catch(...)
    {
        printf("clDriverFrame::doConnect(QString paConfigFile) -> error ...");
		return loReturnValue;
    }	
}
QString clDriverFrameImpLocator::doSendFile(QString paValue, QString paParameters) const
{
	QString loReturnValue;
    try
    {		
		cout << "doSendFile" << endl;	
		return loReturnValue;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());
			return loReturnValue;
    }	
    catch(...)
    {
        printf("clDriverFrame::doConnect(QString paConfigFile) -> error ...");
		return loReturnValue;
    }	
}
QString clDriverFrameImpLocator::doReceiveFile(QString paValue, QString paParameters) const
{
	QString loReturnValue;
    try
    {		
		cout << "doReceiveFile" << endl;	
		return loReturnValue;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());
			return loReturnValue;
    }	
    catch(...)
    {
        printf("clDriverFrame::doConnect(QString paConfigFile) -> error ...");
		return loReturnValue;
    }	
}
QString clDriverFrameImpLocator::doScriptCommand(QString paValue, QString paParameters) const
{
	QString loReturnValue;
    try
    {		
		cout << "doScriptCommand" << endl;	
		return loReturnValue;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());
			return loReturnValue;
    }	
    catch(...)
    {
        printf("clDriverFrame::doConnect(QString paConfigFile) -> error ...");
		return loReturnValue;
    }	
}
QString clDriverFrameImpLocator::doOptionalCommand(QString paValue, QString paParameters) const
{
	QString loReturnValue;
    try
    {		
		cout << "doOptionalCommand" << endl;	
		return loReturnValue;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());
			return loReturnValue;
    }	
    catch(...)
    {
        printf("clDriverFrame::doConnect(QString paConfigFile) -> error ...");
		return loReturnValue;
    }	
}