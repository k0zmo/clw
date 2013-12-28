/*
    Copyright (c) 2012, 2013 Kajetan Swierk <k0zmo@outlook.com>
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#pragma once
// OS
#define CLW_SYSTEM_WINDOWS 1
#define CLW_SYSTEM_LINUX 2

#if defined(_WIN32) || defined(__WIN32__)
#  define CLW_SYSTEM CLW_SYSTEM_WINDOWS
#elif defined(linux) || defined(__linux)
#  define CLW_SYSTEM CLW_SYSTEM_LINUX
#else
#  error "This OS is not supported"
#endif

// Compiler
#define CLW_COMPILER_MSVC 1
#define CLW_COMPILER_GCC 2
/// TODO: Support clang++

#if defined(_MSC_VER)
#  define CLW_COMPILER CLW_COMPILER_MSVC
#elif defined(__GNUC__)
#  define CLW_COMPILER CLW_COMPILER_GCC
#else
#  error "This compiler is not supported"
#endif

// Import/Export macros
#if CLW_SYSTEM == CLW_SYSTEM_WINDOWS
#  if defined(CLW_STATIC_LIB)
#    define CLW_EXPORT
#  else
#    if defined(CLW_BUILD_SHARED)
#      define CLW_EXPORT __declspec(dllexport)
#    else
#      define CLW_EXPORT __declspec(dllimport)
#    endif
#  endif
#elif CLW_SYSTEM == CLW_SYSTEM_LINUX
#  if defined(CLW_STATIC_LIB)
#    define CLW_EXPORT
#  else
#    define CLW_EXPORT __attribute__ ((visibility ("default")))
#  endif
#endif

#if CLW_COMPILER == CLW_COMPILER_MSVC
// nonstandard extension used: enum used in qualified name
#  pragma warning(disable : 4482)
// class <class1> needs to have dll-interface to be used by clients of class <class2>
#  pragma warning(disable : 4251)
#endif

// VS2012.CTP1 or VS2013 or g++
#if (CLW_COMPILER == CLW_COMPILER_MSVC && (_MSC_FULL_VER == 170051025 || _MSC_VER >= 1800)) || \
     CLW_COMPILER == CLW_COMPILER_GCC
#  define CLW_VARIADIC_TEMPLATES_SUPPORTED
#endif