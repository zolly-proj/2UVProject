#ifndef _CLMETHODCALL_H
#define _CLMETHODCALL_H


#ifdef CLMETHODCALL_EXPORTS
#define CLMETHODCALL_API __declspec(dllexport)
#else
#define CLMETHODCALL_API __declspec(dllimport)
#endif

#include <QtCore/QString>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QByteArray>

#include <vector>
#include <iostream>
#include <string>

#ifdef WIN32
#include <windows.h>
#elif defined(_WIN32)
#include <windows.h>
#elif defined(__WIN32)
#include <windows.h>
#elif defined(__WIN32__)
#include <windows.h>
#else

#endif

#include "clDatabaseColumn.h"
#include "clIceClientLogging.h"
#include "clIceClientServer.h"

using namespace std;

class clMethodCall
{
public:
    clMethodCall ();
    virtual ~clMethodCall ();

	//BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved){
		//if(fdwReason == DLL_PROCESS_ATTACH){
		//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadProc, 0, 0, 0);
public:
	virtual bool createPluginClass(clIceClientServer * paIceClientServer,clIceClientLogging * paIceClientLogging) = 0;
	virtual bool doMethod(const vector <QString> &paParametersType, const vector <QString> &paParameters) = 0;
	virtual int GetReturnParameters() = 0;

};
#endif
