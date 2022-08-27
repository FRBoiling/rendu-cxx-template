/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_COMPILERDEFS_H
#define RENDU_COMPILERDEFS_H

#define RENDU_PLATFORM_WINDOWS 0
#define RENDU_PLATFORM_UNIX    1
#define RENDU_PLATFORM_APPLE   2
#define RENDU_PLATFORM_INTEL   3

// must be first (win 64 also define _WIN32)
#if defined( _WIN64 )
#  define RENDU_PLATFORM RENDU_PLATFORM_WINDOWS
#elif defined( __WIN32__ ) || defined( WIN32 ) || defined( _WIN32 )
#  define RENDU_PLATFORM RENDU_PLATFORM_WINDOWS
#elif defined( __APPLE_CC__ )
#  define RENDU_PLATFORM RENDU_PLATFORM_APPLE
#elif defined( __INTEL_COMPILER )
#  define RENDU_PLATFORM RENDU_PLATFORM_INTEL
#else
#  define RENDU_PLATFORM RENDU_PLATFORM_UNIX
#endif

#define RENDU_COMPILER_MICROSOFT 0
#define RENDU_COMPILER_GNU       1
#define RENDU_COMPILER_BORLAND   2
#define RENDU_COMPILER_INTEL     3

#ifdef _MSC_VER
#  define RENDU_COMPILER RENDU_COMPILER_MICROSOFT
#elif defined( __BORLANDC__ )
#  define RENDU_COMPILER RENDU_COMPILER_BORLAND
#elif defined( __INTEL_COMPILER )
#  define RENDU_COMPILER RENDU_COMPILER_INTEL
#elif defined( __GNUC__ )
#  define RENDU_COMPILER RENDU_COMPILER_GNU
#  define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#  error "FATAL ERROR: Unknown compiler."
#endif

#endif
