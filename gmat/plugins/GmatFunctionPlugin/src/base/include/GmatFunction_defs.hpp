//$Id: GmatFunction_defs.hpp 10897 2012-09-20 15:39:44Z djcinsb $
//------------------------------------------------------------------------------
//                 Gmat C Function Import/Export Definitions
//------------------------------------------------------------------------------
// GMAT: General Mission Analysis Tool
//
// Copyright (c) 2002-2011 United States Government as represented by the
// Administrator of The National Aeronautics and Space Administration.
// All Other Rights Reserved.
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under the FDSS
// contract, task order 28.
//
// Author: Darrel Conway (Thinking Systems)
// Created: 2012/09/18
//
/**
 * Definition for library code interfaces.
 */
//------------------------------------------------------------------------------

#ifndef GmatFunction_defs_hpp
#define GmatFunction_defs_hpp


#ifdef _WIN32  // Windows
   #ifdef _MSC_VER  // Microsoft Visual C++

      #define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
      // Windows Header File entry point:
      #include <windows.h>

      #define  _USE_MATH_DEFINES  // makes Msoft define pi

   #endif  // End Microsoft C++ specific block

   #ifdef _DYNAMICLINK  // Only used for Visual C++ Windows DLLs
      #ifdef GMATFUNCTION_EXPORTS
         #define GMATFUNCTION_API __declspec(dllexport)
      #else
         #define GMATFUNCTION_API __declspec(dllimport)
      #endif

      // Provide the storage class specifier (extern for an .exe file, null
      // for DLL) and the __declspec specifier (dllimport for .an .exe file,
      // dllexport for DLL).
      // You must define EXP_STL when compiling the DLL.
      // You can now use this header file in both the .exe file and DLL - a
      // much safer means of using common declarations than two different
      // header files.
      #ifdef EXP_STL
      #    define DECLSPECIFIER __declspec(dllexport)
      #    define EXPIMP_TEMPLATE
      #else
      #    define DECLSPECIFIER __declspec(dllimport)
      #    define EXPIMP_TEMPLATE extern
      #endif

      #define EXPORT_TEMPLATES
   #endif
#else
   #define GMATFUNCTION_API
#endif //  End of OS nits

#ifdef IMPEXP_STDSTRING
    EXPIMP_TEMPLATE template class DECLSPECIFIER std::allocator<char>;
    EXPIMP_TEMPLATE template class DECLSPECIFIER std::basic_string<char, std::char_traits<char>, std::allocator<char>>;
#endif

#ifndef GMATFUNCTION_API
   #define GMATFUNCTION_API
#endif

#endif
