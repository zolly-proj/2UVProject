#ifndef _CLDRIVERFRAME_H
#define _CLDRIVERFRAME_H



#include <QtCore/QString>
#include <QtCore/QByteArray>


#include <vector>
#include <iostream>
#include <string>

//#include "clDriverFrameImplementation.h"

using namespace std;

class clDriverFrame
{
public:
    clDriverFrame ();
    virtual ~clDriverFrame ();
	
private:

public:
	virtual bool createPluginClass(const std::vector <QString> &paParameters) = 0;
	virtual std::vector<QString> GetParameters() const = 0;

	virtual bool parseInputXMLcommand(QByteArray paCommandString) = 0;
	virtual std::vector<QString> GetCommand() const = 0;
	virtual bool getReturnCommandFinished() = 0;
	virtual std::vector<QString> GetReplay() const = 0;
	
	//Functions to be overridden
	virtual QString doConnect(QString paValue,  QString paParameters) const = 0;
	virtual QString doDisconnect(QString paValue,  QString paParameters) const = 0;
	virtual QString doState(QString paValue,  QString paParameters) const = 0;
	virtual QString doRun(QString paValue,  QString paParameters) const = 0;
	virtual QString doAbort(QString paValue,  QString paParameters) const = 0;
	virtual QString doHold(QString paValue,  QString paParameters) const = 0;
	virtual QString doContinue(QString paValue,  QString paParameters) const = 0;
	virtual QString doSendFile(QString paValue,  QString paParameters) const = 0;
	virtual QString doReceiveFile(QString paValue,  QString paParameters) const = 0;
	virtual QString doScriptCommand(QString paValue,  QString paParameters) const = 0;
	virtual QString doOptionalCommand(QString paValue,  QString paParameters) const = 0;
	
	
	

	
/*
<hardwareDevice>
	<id></id>
	<connect do="false></connect>
	<disconnect do="false></disconnect>
	<state do="true"></state>
	<run do="false"></run>
	<abort do="false"></abort>
	<hold do="false"></hold>
	<continue do="false"></continue>
	<sendFile do="false"></sendFile>
	<receiveFile do="false"></receiveFile>
	<scriptCommand do="false">NAMEOFSCRIPTCOMMAND</scriptCommand>
	<optionalCommand do="false" name="SendDataCommand2">
		<data type="int"></data>
		<data type="byte"></data>
		<data type="hex"></data>
		<data type="string"></data>
		<data type="float"></data>
		<data type="double"></data>
		...
	</optionalCommand>
	<optionalCommand do="false" name="SendDataCommand1">
		<data type="int"></data>
		<data type="byte"></data>
		<data type="hex"></data>
		<data type="string"></data>
		<data type="float"></data>
		<data type="double"></data>
		...
	</optionalCommand>
</hardwareDevice>
*/
};
#endif
