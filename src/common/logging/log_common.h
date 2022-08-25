/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef LogCommon_h__
#define LogCommon_h__

#include "define.h"

// EnumUtils: DESCRIBE THIS
enum LogLevel : uint8
{
    LOG_LEVEL_DISABLED                           = 0,
    LOG_LEVEL_TRACE                              = 1,
    LOG_LEVEL_DEBUG                              = 2,
    LOG_LEVEL_INFO                               = 3,
    LOG_LEVEL_WARN                               = 4,
    LOG_LEVEL_ERROR                              = 5,
    LOG_LEVEL_FATAL                              = 6,

    NUM_ENABLED_LOG_LEVELS = LOG_LEVEL_FATAL, // SKIP
    LOG_LEVEL_INVALID = 0xFF // SKIP
};

// EnumUtils: DESCRIBE THIS
enum AppenderType : uint8
{
    APPENDER_NONE,
    APPENDER_CONSOLE,
    APPENDER_FILE,
    APPENDER_DB,

    APPENDER_INVALID = 0xFF // SKIP
};

enum AppenderFlags : uint8
{
    APPENDER_FLAGS_NONE                          = 0x00,
    APPENDER_FLAGS_PREFIX_TIMESTAMP              = 0x01,
    APPENDER_FLAGS_PREFIX_LOGLEVEL               = 0x02,
    APPENDER_FLAGS_PREFIX_LOGFILTERTYPE          = 0x04,
    APPENDER_FLAGS_USE_TIMESTAMP                 = 0x08, // only used by FileAppender
    APPENDER_FLAGS_MAKE_FILE_BACKUP              = 0x10  // only used by FileAppender
};

#endif // LogCommon_h__
