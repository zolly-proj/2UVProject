#define INFO_BUFFER_SIZE 32767
#include "clObjectCall.h"
clObjectCall::clObjectCall()
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
        printf("clObjectCall::clObjectCall -> error ...");
    }
}
clObjectCall::~clObjectCall ()
{
}
