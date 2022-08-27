/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef RENDU_LOGGER_H_
#define RENDU_LOGGER_H_

#include "define.h"
#include "log_common.h"
#include <string>
#include <unordered_map>

class Appender;

struct LogMessage;

class RD_COMMON_API Logger {
public:
  Logger(std::string const &name, LogLevel level);

  void addAppender(uint8 type, Appender *appender);

  void delAppender(uint8 type);

  std::string const &getName() const;

  LogLevel getLogLevel() const;

  void setLogLevel(LogLevel level);

  void write(LogMessage *message) const;

private:
  std::string name;
  LogLevel level;
  std::unordered_map<uint8, Appender *> appenders;
};

#endif//RENDU_LOGGER_H_
