/*
* Created by boil on 2023/2/11.
*/

#ifndef RENDU_CORE_DEFINE_DEFINE_H_
#define RENDU_CORE_DEFINE_DEFINE_H_

#include "version.h"

#if defined(__cpp_exceptions) && !defined(RD_NOEXCEPTION)
#    define RD_CONSTEXPR
#    define RD_THROW throw
#    define RD_TRY try
#    define RD_CATCH catch(...)
#else
#    define RD_CONSTEXPR constexpr // use only with throwing functions (waiting for C++20)
#    define RD_THROW
#    define RD_TRY if(true)
#    define RD_CATCH if(false)
#endif

#ifdef RD_USE_ATOMIC
#    include <atomic>
#    define RD_MAYBE_ATOMIC(Type) std::atomic<Type>
#else
#    define RD_MAYBE_ATOMIC(Type) Type
#endif

#ifndef RD_ID_TYPE
#    include <cstdint>
#    define RD_ID_TYPE std::uint32_t
#endif

#ifndef RD_SPARSE_PAGE
#    define RD_SPARSE_PAGE 4096
#endif

#ifndef RD_PACKED_PAGE
#    define RD_PACKED_PAGE 1024
#endif

#ifdef RD_DISABLE_ASSERT
#    undef RD_ASSERT
#    define RD_ASSERT(condition, msg) (void(0))
#elif !defined RD_ASSERT
#    include <cassert>
#    define RD_ASSERT(condition, msg) assert(condition)
#endif

#ifdef RD_DISABLE_ASSERT
#    undef RD_ASSERT_CONSTEXPR
#    define RD_ASSERT_CONSTEXPR(condition, msg) (void(0))
#elif !defined RD_ASSERT_CONSTEXPR
#    define RD_ASSERT_CONSTEXPR(condition, msg) RD_ASSERT(condition, msg)
#endif

#define RD_FAIL(msg) RD_ASSERT(false, msg);

#ifdef RD_NO_ETO
#    define RD_ETO_TYPE(Type) void
#else
#    define RD_ETO_TYPE(Type) Type
#endif

#ifdef RD_STANDARD_CPP
#    define RD_NONSTD false
#else
#    define RD_NONSTD true
#    if defined __clang__ || defined __GNUC__
#        define RD_PRETTY_FUNCTION __PRETTY_FUNCTION__
#        define RD_PRETTY_FUNCTION_PREFIX '='
#        define RD_PRETTY_FUNCTION_SUFFIX ']'
#    elif defined _MSC_VER
#        define RD_PRETTY_FUNCTION __FUNCSIG__
#        define RD_PRETTY_FUNCTION_PREFIX '<'
#        define RD_PRETTY_FUNCTION_SUFFIX '>'
#    endif
#endif

#if defined _MSC_VER
#    pragma detect_mismatch("entt.version", RD_VERSION)
#    pragma detect_mismatch("entt.noexcept", RD_XSTR(RD_TRY))
#    pragma detect_mismatch("entt.id", RD_XSTR(RD_ID_TYPE))
#    pragma detect_mismatch("entt.nonstd", RD_XSTR(RD_NONSTD))
#endif

#endif //RENDU_CORE_DEFINE_DEFINE_H_
