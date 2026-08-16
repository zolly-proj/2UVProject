#include "clCreateOperationForMachine.h"
clCreateOperationForMachine::clCreateOperationForMachine()
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
clCreateOperationForMachine::~clCreateOperationForMachine ()
{
}
int clCreateOperationForMachine::GetReturnParameters()
{
    try
    {
		return 0;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
			return -1;
    }	
    catch(...)
    {
		return -1;
    }
}

bool clCreateOperationForMachine::doMethod(const vector <QString> &paParametersType, const vector <QString> &paParameters)
{
    try
    {
		cout << "This is in the method call " << endl;
		return true;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
			return false;
    }	
    catch(...)
    {
		return false;
    }
}
bool clCreateOperationForMachine::createPluginClass( clIceClientServer * paIceClientServer, clIceClientLogging * paIceClientLogging)	
{
    try
    {
		meIceClientServer = paIceClientServer;
		meIceClientLogging = paIceClientLogging;
		return true;
    }
    catch(const std::exception& ex)
    {
            printf(ex.what());       
			return false;
    }	
    catch(...)
    {
		return false;
    }
}

