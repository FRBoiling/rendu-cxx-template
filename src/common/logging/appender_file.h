/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_APPENDER_FILE_H_
#define RENDU_APPENDER_FILE_H_

#include "appender.h"
#include <atomic>

class RD_COMMON_API AppenderFile : public Appender {
public:
  static constexpr AppenderType type = APPENDER_FILE;
  AppenderFile(uint8 id, std::string const &name, LogLevel level, AppenderFlags flags,
               std::vector<std::string_view> const &args);
  ~AppenderFile() override;

  FILE *OpenFile(std::string const &name, std::string const &mode, bool backup);

  AppenderType getType() const override { return type; }

private:
  void CloseFile();

  void _write(LogMessage const *message) override;

  FILE *logfile;
  std::string _fileName;
  std::string _logDir;
  bool _dynamicName;
  bool _backup;
  uint64 _maxFileSize;
  std::atomic<uint64> _fileSize;
};

#endif //RENDU_APPENDER_FILE_H_
