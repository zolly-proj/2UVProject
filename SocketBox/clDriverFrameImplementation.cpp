#include "clDriverFrameImplementation.h"
clDriverFrameImplementation::clDriverFrameImplementation()
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
clDriverFrameImplementation::~clDriverFrameImplementation ()
{
}
bool clDriverFrameImplementation::createPluginClass(const vector <QString> &paParameters)
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

std::vector<QString> clDriverFrameImplementation::GetParameters() const
{
	return meParameters;
}
bool clDriverFrameImplementation::parseInputXMLcommand(QByteArray paCommandString)
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
        printf("clDriverFrame::parseInputXMLcommand(QString paConfigFile) -> error ...");
		return false;
    }	
}
bool clDriverFrameImplementation::getReturnCommandFinished()
{
    try
    {	
		meReplay.clear();
		meReplay.push_back(QString(QString("<returnHardwareDevice>") +
									QString("	<id>" + meCommand[0] + "</id>") +
									QString("	<state>OK*TESTINGTHIS</state>") +
									QString("	<datas>") +
									QString("		<data>1</data>") +
									QString("		<data>2</data>") +
									QString("		<data>3</data>") +
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
std::vector<QString> clDriverFrameImplementation::GetCommand() const
{
	return meCommand;
}
std::vector<QString> clDriverFrameImplementation::GetReplay() const
{
	return meReplay;
}
///////////////////////////////
//Functions to be overridden //
///////////////////////////////
QString clDriverFrameImplementation::doConnect(QString paValue, QString paParameters) const
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
QString clDriverFrameImplementation::doDisconnect(QString paValue, QString paParameters) const
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
QString clDriverFrameImplementation::doState(QString paValue, QString paParameters) const
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
QString clDriverFrameImplementation::doRun(QString paValue, QString paParameters) const
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
QString clDriverFrameImplementation::doAbort(QString paValue, QString paParameters) const
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
QString clDriverFrameImplementation::doHold(QString paValue, QString paParameters) const
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
QString clDriverFrameImplementation::doContinue(QString paValue, QString paParameters) const
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
QString clDriverFrameImplementation::doSendFile(QString paValue, QString paParameters) const
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
QString clDriverFrameImplementation::doReceiveFile(QString paValue, QString paParameters) const
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
QString clDriverFrameImplementation::doScriptCommand(QString paValue, QString paParameters) const
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
QString clDriverFrameImplementation::doOptionalCommand(QString paValue, QString paParameters) const
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