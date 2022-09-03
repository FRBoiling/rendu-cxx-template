/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_APPENDER_H_
#define RENDU_APPENDER_H_

#include "define.h"
#include "log_common.h"
#include <stdexcept>
#include <string>
#include <vector>

struct LogMessage;

class RD_COMMON_API Appender {
public:
  Appender(uint8 _id, std::string const &name, LogLevel level = LOG_LEVEL_DISABLED,
           AppenderFlags flags = APPENDER_FLAGS_NONE);

  virtual ~Appender();

  uint8 getId() const;

  std::string const &getName() const;

  virtual AppenderType getType() const = 0;

  LogLevel getLogLevel() const;

  AppenderFlags getFlags() const;

  void setLogLevel(LogLevel);

  void write(LogMessage *message);

  static char const *getLogLevelString(LogLevel level);

  virtual void setRealmId(uint32 /*realmId*/) {}

private:
  virtual void _write(LogMessage const * /*message*/) = 0;

  uint8 id;
  std::string name;
  LogLevel level;
  AppenderFlags flags;
};

class RD_COMMON_API InvalidAppenderArgsException : public std::length_error {
public:
  explicit InvalidAppenderArgsException(std::string const &message) : std::length_error(message) {}
};

#endif //RENDU_APPENDER_H_
