/*
* Created by boil on 2022/8/20.
*/

#include "log_operation.h"
#include "log_message.h"
#include "logger.h"

LogOperation::LogOperation(Logger const* _logger, std::unique_ptr<LogMessage>&& _msg) : logger(_logger), msg(std::forward<std::unique_ptr<LogMessage>>(_msg))
{
}

LogOperation::~LogOperation()
{
}

int LogOperation::call()
{
    logger->write(msg.get());
    return 0;
}
