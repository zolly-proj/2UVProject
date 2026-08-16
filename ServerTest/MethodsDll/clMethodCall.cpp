#include "clMethodCall.h"
clMethodCall::clMethodCall()
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
clMethodCall::~clMethodCall ()
{
}

