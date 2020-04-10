#pragma once

#include <cstdlib>

// DLL declspect import/export definition
#if defined(WIN32) || defined(_WIN32)

#   if defined(SGAL_BUILD)
#       define SGAL_API __declspec(dllexport)
#   else
#       define SGAL_API __declspec(dllimport)
#   endif

#endif

#define SG_ASSERT(boolean, message)\
    if (!(boolean))\
    {\
        printf("Assertion failed in %s on line %i.\n\nStatement: " #boolean "\n\nMessage:   %s", __FILE__, __LINE__, message); exit(1);\
    }
#define INVALID_OPERATING_SYSTEM SG_ASSERT(false, "SGAL is not supported on this OS.");

#define IPTR void*
