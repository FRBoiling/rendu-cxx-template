/*
* Created by boil on 2022/9/9.
*/

#ifndef RENDU_LOG_H_
#define RENDU_LOG_H_
#include <spdlog/spdlog.h>
#include "spdlog/pattern_formatter.h"

#define RD_LOG_DEBUG SPDLOG_DEBUG
#define RD_LOG_TRACE SPDLOG_TRACE
#define RD_LOG_INFO SPDLOG_INFO
#define RD_LOG_WARN SPDLOG_WARN
#define RD_LOG_ERROR SPDLOG_ERROR
#define RD_LOG_CRITICAL SPDLOG_CRITICAL

#endif //RENDU_LOG_H_
