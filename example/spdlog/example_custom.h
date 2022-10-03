/*
* Created by boil on 2022/9/18.
*/

#ifndef RENDU_EXAMPLE_CUSTOM_H_
#define RENDU_EXAMPLE_CUSTOM_H_

#include <spdlog/spdlog.h>
//#include <spdlog/cfg/env.h>  // support for loading levels from the environment variable
//#include <spdlog/fmt/ostr.h> // support for user defined types
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <cstdio>
#include <chrono>
#include "spdlog/pattern_formatter.h"
#include "spdlog/stopwatch.h"

class CustomFormatterFlag : public spdlog::custom_flag_formatter {
public:
  void format(const spdlog::details::log_msg &, const std::tm &, spdlog::memory_buf_t &dest) override {
    std::string some_txt = "custom-flag";
    dest.append(some_txt.data(), some_txt.data() + some_txt.size());
  }

  std::unique_ptr<custom_flag_formatter> clone() const override {
    return spdlog::details::make_unique<CustomFormatterFlag>();
  }
};
class CustomLog {

public:
  static CustomLog &instance() {
    static CustomLog m_instance;
    return m_instance;
  }

  auto get_logger() const {
    return this->logger_;
  }

private:
  CustomLog() {
    this->init();
  }

  ~CustomLog() {
    spdlog::drop_all(); // 释放所有logger
  }

private:
  void init() {
    this->init_file();
    this->init_logger();
  }

  void init_file() {
    this->log_root_path = "logs/";
    this->log_file_path = "custom.log";
    this->rotation_h = 5; // 分割时间
    this->rotation_m = 59;
  }

  void init_logger() {
    auto file_formatter = std::make_unique<spdlog::pattern_formatter>();
    file_formatter->add_flag<CustomFormatterFlag>('*').set_pattern("[%n] [%*] [%^%l%$] %v");
    std::unique_ptr<spdlog::pattern_formatter> console_formatter = std::make_unique<spdlog::pattern_formatter>();
    console_formatter->add_flag<CustomFormatterFlag>('*').set_pattern("[%^%l%$] %v");

    this->console_sink_ = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    this->console_sink_->set_level(spdlog::level::trace);
    this->console_sink_->set_formatter(std::move(console_formatter));

    this->file_sink_ = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
        this->log_root_path + this->log_file_path, this->rotation_h, this->rotation_m);
    this->file_sink_ ->set_level(spdlog::level::trace);

    this->sinks_.push_back(this->console_sink_);
    this->sinks_.push_back(this->file_sink_);

    this->logger_ = std::make_shared<spdlog::logger>("log_demo", begin(this->sinks_), end(this->sinks_));
    this->logger_->set_level(spdlog::level::trace);
//    this->logger_->set_pattern("[%Y-%m-%d %H:%M:%S,%e][%^%l%$][Process:%P] - %v");

//    this->console_sink_ = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
//    this->console_sink_->set_level(spdlog::level::debug);
//    this->console_sink_->set_pattern("[multi_sink_example] [%^%l%$] %v");
//
//    this->file_sink_ = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
//        this->log_root_path + this->log_file_path, this->rotation_h, this->rotation_m);
//    this->file_sink_->set_level(spdlog::level::trace);
//
//    this->sinks_.push_back(this->console_sink_);
//    this->sinks_.push_back(this->file_sink_);
//
////    spdlog::logger logger("multi_sink", {this->console_sink_, this->file_sink_});
////    logger.set_level(spdlog::level::debug);
//    this->logger_ = std::make_shared<spdlog::logger>("log_demo", begin(this->sinks_), end(this->sinks_));
//    this->logger_->set_pattern("[%l] [%Y-%m-%d %H:%M:%S,%e] [Process:%P] - %v");
//    this->logger_->set_level(spdlog::level::trace);
//    this->logger_->flush_on(spdlog::level::trace); // 设置立刻刷新日志到 disk
//    spdlog::flush_every(std::chrono::seconds(10)); // 每隔10秒刷新一次日志
//    spdlog::register_logger(this->logger_); // 注册logger
////    spdlog::set_default_logger(this->logger_);

  }

private:
  std::shared_ptr<spdlog::logger> logger_;
  std::shared_ptr<spdlog::sinks::daily_file_sink_mt> file_sink_; // file
  std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> console_sink_; // console
  std::vector<spdlog::sink_ptr> sinks_;
  std::string log_root_path;
  std::string log_file_path;
  short int rotation_h{};
  short int rotation_m{};

}; // CustomLog

void custom_example() {
  spdlog::stopwatch sw;
//  std::this_thread::sleep_for(std::chrono::milliseconds(123));
  for (int i = 0; i < 1000; ++i) {
    CustomLog::instance().get_logger()->trace("this should appear in both console and file");
    CustomLog::instance().get_logger()->debug("this should appear in both console and file");
    CustomLog::instance().get_logger()->info("this should appear in both console and file");
    CustomLog::instance().get_logger()->warn("this should appear in both console and file");
    CustomLog::instance().get_logger()->error("this should appear in both console and file");
    CustomLog::instance().get_logger()->critical("this should appear in both console and file");
  }
  spdlog::info("---------------------{}----------------------", sw);

//  sLog->warn("this should appear in both console and file");
//  sLog->info("this message should not appear in the console, only in the file");
}



#endif //RENDU_EXAMPLE_CUSTOM_H_
