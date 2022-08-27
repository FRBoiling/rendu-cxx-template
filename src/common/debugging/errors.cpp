/*
* Created by boil on 2022/8/20.
*/

#include "errors.h"
#include "string_format.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <thread>

/**
    @file Errors.cpp

    @brief This file contains definitions of functions used for reporting critical application errors

    It is very important that (std::)abort is NEVER called in place of *((volatile int*)nullptr) = 0;
    Calling abort() on Windows does not invoke unhandled exception filters - a mechanism used by WheatyExceptionReport
    to log crashes. exit(1) calls here are for static analysis tools to indicate that calling functions defined in this file
    terminates the application.
 */

#if RENDU_PLATFORM == RENDU_PLATFORM_WINDOWS
#include <Windows.h>
#include <intrin.h>
#define Crash(message) \
    ULONG_PTR execeptionArgs[] = { reinterpret_cast<ULONG_PTR>(strdup(message)), reinterpret_cast<ULONG_PTR>(_ReturnAddress()) }; \
    RaiseException(EXCEPTION_ASSERTION_FAILURE, 0, 2, execeptionArgs);
#else
#include <cstring>
// should be easily accessible in gdb
extern "C" { RD_COMMON_API char const* RenduAssertionFailedMessage = nullptr; }
#define Crash(message) \
    RenduAssertionFailedMessage = strdup(message); \
    *((volatile int*)nullptr) = 0; \
    exit(1);
#endif

namespace
{
    std::string FormatAssertionMessage(char const* format, va_list args)
    {
        std::string formatted;
        va_list len;

        va_copy(len, args);
        int32 length = vsnprintf(nullptr, 0, format, len);
        va_end(len);

        formatted.resize(length);
        vsnprintf(&formatted[0], length + 1, format, args);

        return formatted;
    }
}

namespace rendu
{

void Assert(char const* file, int line, char const* function, std::string debugInfo, char const* message)
{
    std::string formattedMessage = StringFormat("\n%s:%i in %s ASSERTION FAILED:\n  %s\n", file, line, function, message) + debugInfo + '\n';
    fprintf(stderr, "%s", formattedMessage.c_str());
    fflush(stderr);
    Crash(formattedMessage.c_str());
}

void Assert(char const* file, int line, char const* function, std::string debugInfo, char const* message, char const* format, ...)
{
    va_list args;
    va_start(args, format);

    std::string formattedMessage = StringFormat("\n%s:%i in %s ASSERTION FAILED:\n  %s\n", file, line, function, message) + FormatAssertionMessage(format, args) + '\n' + debugInfo + '\n';
    va_end(args);

    fprintf(stderr, "%s", formattedMessage.c_str());
    fflush(stderr);

    Crash(formattedMessage.c_str());
}

void Fatal(char const* file, int line, char const* function, char const* message, ...)
{
    va_list args;
    va_start(args, message);

    std::string formattedMessage = StringFormat("\n%s:%i in %s FATAL ERROR:\n", file, line, function) + FormatAssertionMessage(message, args) + '\n';
    va_end(args);

    fprintf(stderr, "%s", formattedMessage.c_str());
    fflush(stderr);

    std::this_thread::sleep_for(std::chrono::seconds(10));
    Crash(formattedMessage.c_str());
}

void Error(char const* file, int line, char const* function, char const* message)
{
    std::string formattedMessage = StringFormat("\n%s:%i in %s ERROR:\n  %s\n", file, line, function, message);
    fprintf(stderr, "%s", formattedMessage.c_str());
    fflush(stderr);
    Crash(formattedMessage.c_str());
}

void Warning(char const* file, int line, char const* function, char const* message)
{
    fprintf(stderr, "\n%s:%i in %s WARNING:\n  %s\n",
                   file, line, function, message);
}

void Abort(char const* file, int line, char const* function)
{
    std::string formattedMessage = StringFormat("\n%s:%i in %s ABORTED.\n", file, line, function);
    fprintf(stderr, "%s", formattedMessage.c_str());
    fflush(stderr);
    Crash(formattedMessage.c_str());
}

void Abort(char const* file, int line, char const* function, char const* message, ...)
{
    va_list args;
    va_start(args, message);

    std::string formattedMessage = StringFormat("\n%s:%i in %s ABORTED:\n", file, line, function) + FormatAssertionMessage(message, args) + '\n';
    va_end(args);

    fprintf(stderr, "%s", formattedMessage.c_str());
    fflush(stderr);

    Crash(formattedMessage.c_str());
}

void AbortHandler(int sigval)
{
    // nothing useful to log here, no way to pass args
    std::string formattedMessage = StringFormat("Caught signal %i\n", sigval);
    fprintf(stderr, "%s", formattedMessage.c_str());
    fflush(stderr);
    Crash(formattedMessage.c_str());
}

} // namespace rendu

std::string GetDebugInfo()
{
    return "";
}
