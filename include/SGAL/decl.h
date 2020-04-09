#pragma once

// DLL declspect import/export definition
#if defined(WIN32) || defined(_WIN32)

#   if defined(SGAL_BUILD)
#       define SGAL_API __declspec(dllexport)
#   else
#       define SGAL_API __declspec(dllimport)
#   endif

#endif
