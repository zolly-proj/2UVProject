#ifndef _CLDRIVERFRAMEIMPLEMENTATION_H
#define _CLDRIVERFRAMEIMPLEMENTATION_H


#include <QtCore/QString>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>

#include <vector>
#include <iostream>

#include <string>

#include "clDriverFrame.h"

using namespace std;


/*
#ifdef CLDRIVERFRAMEIMPLEMENTATION_EXPORTS
#define CLDRIVERFRAMEIMPLEMENTATION_API __declspec(dllexport)
#else
#define CLDRIVERFRAMEIMPLEMENTATION_API __declspec(dllimport)
#endif

typedef struct clDriverFrameImplementationHandle_* clDriverFrameImplementationHandle;

CLDRIVERFRAMEIMPLEMENTATION_API clDriverFrameImplementationHandle CreateFlubber();
CLDRIVERFRAMEIMPLEMENTATION_API void DestroyFlubber(clDriverFrameImplementationHandle flubber);
CLDRIVERFRAMEIMPLEMENTATION_API QString clDriverDoConnect(QString paValue, QString paParameters);
*/
class clDriverFrameImpLocator : public clDriverFrame
{
public:
    clDriverFrameImpLocator ();
    ~clDriverFrameImpLocator ();

	//BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
		//if(fdwReason == DLL_PROCESS_ATTACH){
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProc, 0, 0, 0);
public:
	bool createPluginClass(const vector <QString> &paParameters) override;
	std::vector<QString> GetParameters() const override;
	
	bool parseInputXMLcommand(QByteArray paCommandString) override;
	std::vector<QString> GetCommand() const override;
	bool getReturnCommandFinished() override;
	std::vector<QString> GetReplay() const override;
	
	
	//Functions to be overridden
	QString doConnect(QString paValue,  QString paParameters) const override;
	QString doDisconnect(QString paValue,  QString paParameters) const override;
	QString doState(QString paValue,  QString paParameters) const override;
	QString doRun(QString paValue,  QString paParameters) const override;
	QString doAbort(QString paValue,  QString paParameters) const override;
	QString doHold(QString paValue,  QString paParameters) const override;
	QString doContinue(QString paValue,  QString paParameters) const override;
	QString doSendFile(QString paValue,  QString paParameters) const override;
	QString doReceiveFile(QString paValue,  QString paParameters) const override;
	QString doScriptCommand(QString paValue,  QString paParameters) const override;
	QString doOptionalCommand(QString paValue,  QString paParameters) const override;	
	
private:
	vector <QString> meParameters;
	vector <QString> meReplay;
	vector <QString> meCommand;
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
extern "C" clDriverFrame* CreateModule()
{
    // call the constructor of the actual implementation
    clDriverFrame * module = new clDriverFrameImpLocator();
    // return the created function
    return module;
}
