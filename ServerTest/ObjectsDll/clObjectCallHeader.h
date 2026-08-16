#pragma once
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
#include <QtCore/QString>
#include "clObjectCall.h"

typedef clObjectCall *(*CreateModuleObjectFn)();
