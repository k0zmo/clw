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

// VS2012.CTP1 or g++
#if _MSC_FULL_VER == 170051025 || CLW_COMPILER == CLW_COMPILER_GCC
#  define CLW_VARIADIC_TEMPLATES_SUPPORTED
#endif