/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef LOGOPERATION_H
#define LOGOPERATION_H

#include "define.h"
#include <memory>

class Logger;
struct LogMessage;

class RD_COMMON_API LogOperation
{
    public:
        LogOperation(Logger const* _logger, std::unique_ptr<LogMessage>&& _msg);

        ~LogOperation();

        int call();

    protected:
        Logger const* logger;
        std::unique_ptr<LogMessage> msg;
};

#endif
