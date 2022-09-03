/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_DEFINE_H_
#define RENDU_DEFINE_H_

#include "compiler_defs.h"

#if RENDU_COMPILER == RENDU_COMPILER_GNU
#  if !defined(__STDC_FORMAT_MACROS)
#    define __STDC_FORMAT_MACROS
#  endif
#  if !defined(__STDC_CONSTANT_MACROS)
#    define __STDC_CONSTANT_MACROS
#  endif
#  if !defined(_GLIBCXX_USE_NANOSLEEP)
#    define _GLIBCXX_USE_NANOSLEEP
#  endif
#  if defined(HELGRIND)
#    include <valgrind/helgrind.h>
#    undef _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE
#    undef _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER
#    define _GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE(A) ANNOTATE_HAPPENS_BEFORE(A)
#    define _GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER(A)  ANNOTATE_HAPPENS_AFTER(A)
#  endif
#  if defined(VALGRIND)
#    include <valgrind/memcheck.h>
#  endif
#endif

#include <cstddef>
#include <cinttypes>
#include <climits>

#define RENDU_LITTLEENDIAN 0
#define RENDU_BIGENDIAN    1

#if !defined(RENDU_ENDIAN)
#  if defined (BOOST_BIG_ENDIAN)
#    define RENDU_ENDIAN RENDU_BIGENDIAN
#  else
#    define RENDU_ENDIAN RENDU_LITTLEENDIAN
#  endif
#endif

#if RENDU_PLATFORM == RENDU_PLATFORM_WINDOWS
#  define RENDU_PATH_MAX 260
#  define _USE_MATH_DEFINES
#else // RENDU_PLATFORM != RENDU_PLATFORM_WINDOWS
#  define RENDU_PATH_MAX PATH_MAX
#endif // RENDU_PLATFORM

#if !defined(COREDEBUG)
#  define RENDU_INLINE inline
#else //COREDEBUG
#  if !defined(RENDU_DEBUG)
#    define RENDU_DEBUG
#  endif //RENDU_DEBUG
#  define RENDU_INLINE
#endif //!COREDEBUG

#if RENDU_COMPILER == RENDU_COMPILER_GNU
#  define ATTR_PRINTF(F, V) __attribute__ ((__format__ (__printf__, F, V)))
#else //RENDU_COMPILER != RENDU_COMPILER_GNU
#  define ATTR_PRINTF(F, V)
#endif //RENDU_COMPILER == RENDU_COMPILER_GNU

#ifdef RENDU_API_USE_DYNAMIC_LINKING
#  if RENDU_COMPILER == RENDU_COMPILER_MICROSOFT
#    define RD_API_EXPORT __declspec(dllexport)
#    define RD_API_IMPORT __declspec(dllimport)
#  elif RENDU_COMPILER == RENDU_COMPILER_GNU
#    define RD_API_EXPORT __attribute__((visibility("default")))
#    define RD_API_IMPORT
#  else
#    error compiler not supported!
#  endif
#else
#  define RD_API_EXPORT
#  define RD_API_IMPORT
#endif

#ifdef RENDU_API_EXPORT_COMMON
#  define RD_COMMON_API RD_API_EXPORT
#else
#  define RD_COMMON_API RD_API_IMPORT
#endif

//**************Shared*************************
#ifdef RENDU_API_EXPORT_SHARED
#  define RD_SHARED_API RD_API_EXPORT
#else
#  define RD_SHARED_API RD_API_IMPORT
#endif

//**************framework***********************
#ifdef RENDU_API_EXPORT_FRAMEWORK
#  define RD_GAME_API RD_API_EXPORT
#else
#  define RD_GAME_API RD_API_IMPORT
#endif

//*******************##proto********************
#ifdef RENDU_API_EXPORT_PROTO
#  define RD_PROTO_API RD_API_EXPORT
#else
#  define RD_PROTO_API RD_API_IMPORT
#endif

//******************database*********************
#ifdef RENDU_API_EXPORT_DATABASE
#  define RD_DATABASE_API RD_API_EXPORT
#else
#  define RD_DATABASE_API RD_API_IMPORT
#endif

//****************** TODO:BOIL 添加其他API

#define UI64FMTD "%" PRIu64
#define UI64LIT(N) UINT64_C(N)

#define SI64FMTD "%" PRId64
#define SI64LIT(N) INT64_C(N)

#define SZFMTD "%" PRIuPTR

#define STRING_VIEW_FMT "%.*s"
#define STRING_VIEW_FMT_ARG(str) static_cast<int>((str).length()), (str).data()

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

enum DBCFormer {
  FT_STRING = 's',                                        // LocalizedString*
  FT_STRING_NOT_LOCALIZED = 'S',                          // char*
  FT_FLOAT = 'f',                                         // float
  FT_INT = 'i',                                           // uint32
  FT_BYTE = 'b',                                          // uint8
  FT_SHORT = 'h',                                         // uint16
  FT_LONG = 'l'                                           // uint64
};

#endif //RENDU_DEFINE_H_
