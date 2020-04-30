/**
 * @file   decl.h
 * @author Max Ortner
 * @date   04-12-2020
 * 
 * This file holds the definitions for exporting classes and methods to a dll if needed as
 * well as some critical definitions needed across the framework that may or may not be
 * platform specific.
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include <cstdlib>

// DLL declspect import/export definition
#if defined(WIN32) || defined(_WIN32)

#   if defined(SGAL_SHARED)

#   if defined(SGAL_BUILD)
#       define SGAL_API __declspec(dllexport)
#   else
#       define SGAL_API __declspec(dllimport)
#   endif

#   else
#       define SGAL_API
#   endif

#endif

#define SG_ASSERT(boolean, message)\
    if (!(boolean))\
    {\
        printf("Assertion failed in %s on line %i.\n\nStatement: " #boolean "\n\nMessage:   %s", __FILE__, __LINE__, message); exit(1);\
    }

// This should be used when in a scope that is compiled in an unsupported operating system    
#define INVALID_OPERATING_SYSTEM SG_ASSERT(false, "SGAL is not supported on this OS.");

// Uses the base class constructor
#define BASE_CONSTRUCTOR(class_name) using class_name::class_name;

#define IPTR void*

// Platform specific definitions
#ifdef WIN32
#   include <Windows.h>
#   include <Windowsx.h> // GET_X_LPARAM, GET_Y_LPARAM

#   define HDC_PTR   HDC
#   define GLRC_PTR  HGLRC
#   define HWND_PTR  HWND
#else
#   define HDC_PTR   void*
#   define GLRC_PTR  void*
#   define HWND_PTR  void*
#endif

#define SG_TEXTURE_ARRAY_SIZE 16
