/*
* Created by boil on 2022/9/23.
*/

#ifndef RENDU_LOG_TRACE_H_
#define RENDU_LOG_TRACE_H_

#include <spdlog/spdlog.h>
//#include <spdlog/cfg/env.h>  // support for loading levels from the environment variable
//#include <spdlog/fmt/ostr.h> // support for user defined types
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <cstdio>
#include <chrono>

class LogTrace {
public:
  static LogTrace &instance() {
    static LogTrace m_instance;
    return m_instance;
  }

  auto get_logger() const {
    return this->logger_;
  }

private:
  LogTrace() {
    this->init();
  }

  ~LogTrace() = default;

private:
  void init() {
    init_console();
    init_file();

    this->sinks_.push_back(this->console_sink_);
    this->sinks_.push_back(this->file_sink_);
    this->logger_ = std::make_shared<spdlog::logger>("TLOG", begin(this->sinks_), end(this->sinks_));
    this->logger_->set_level(spdlog::level::trace);
  }

  void init_console() {
    this->console_sink_ = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    this->console_sink_->set_pattern("[%m-%d %H:%M:%S.%e][%n][%L] %v");
  }

  void init_file() {
    std::string log_root_path = "logs/";
    std::string log_file_path = "custom.log";
    int rotation_h = 5; // 分割时间
    int rotation_m = 59;
    this->file_sink_ = std::make_shared<spdlog::sinks::daily_file_sink_mt>(log_root_path + log_file_path, rotation_h,
                                                                           rotation_m);
    this->file_sink_->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%n][%L] %v");
  }

private:
  std::shared_ptr<spdlog::logger> logger_;
  std::vector<spdlog::sink_ptr> sinks_;
  std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink_; // console
  std::shared_ptr<spdlog::sinks::daily_file_sink_mt> file_sink_; // file
};

#define RD_TLOG(logger, ...) SPDLOG_LOGGER_CALL(logger, spdlog::level::trace, __VA_ARGS__)
#define TLOG(...) RD_TLOG(Log::instance().get_logger(), __VA_ARGS__)

#endif //RENDU_LOG_TRACE_H_
