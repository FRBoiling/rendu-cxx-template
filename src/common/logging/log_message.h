/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef LogMessage_h__
#define LogMessage_h__

#include "define.h"
#include "log_common.h"
#include <ctime>
#include <string>

struct RD_COMMON_API LogMessage
{
    LogMessage(LogLevel _level, std::string const& _type, std::string&& _text);
    LogMessage(LogLevel _level, std::string const& _type, std::string&& _text, std::string&& _param1);

    LogMessage(LogMessage const& /*other*/) = delete;
    LogMessage& operator=(LogMessage const& /*other*/) = delete;

    static std::string getTimeStr(time_t time);
    std::string getTimeStr() const;

    LogLevel const level;
    std::string const type;
    std::string const text;
    std::string prefix;
    std::string param1;
    time_t mtime;

    ///@ Returns size of the log message content in bytes
    uint32 Size() const
    {
        return static_cast<uint32>(prefix.size() + text.size());
    }
};

#endif // LogMessage_h__
