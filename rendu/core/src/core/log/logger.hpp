#pragma once

#include "core/ecs/ecs_framework.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include <mutex>
#include <memory>
#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <queue>

// 日志级别枚举
enum class LogLevel {
    DEBUG, INFO, WARNING, ERROR, CRITICAL
};

// 日志组件 - 用于实体携带日志信息
struct LogComponent : public ecs::Component {
    std::string message;
    LogLevel level;
    std::time_t timestamp;
    
    LogComponent(const std::string& msg, LogLevel lvl) 
        : message(msg), level(lvl), timestamp(std::time(nullptr)) {}
};

// 日志格式化接口
class LogFormatter {
public:
    virtual std::string format(const LogComponent& log) = 0;
    virtual ~LogFormatter() = default;
};

// 简单文本格式化器
class SimpleTextFormatter : public LogFormatter {
public:
    std::string format(const LogComponent& log) override {
        char timeBuffer[64];
        std::tm* tm_info = std::localtime(&log.timestamp);
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", tm_info);
        std::string timeStr(timeBuffer);

        std::string levelStr;
        switch(log.level) {
            case LogLevel::DEBUG: levelStr = "DEBUG"; break;
            case LogLevel::INFO: levelStr = "INFO"; break;
            case LogLevel::WARNING: levelStr = "WARNING"; break;
            case LogLevel::ERROR: levelStr = "ERROR"; break;
            case LogLevel::CRITICAL: levelStr = "CRITICAL"; break;
        }

        return "[" + timeStr + "] [" + levelStr + "] " + log.message;
    }
};

// 日志输出接口
class LogSink {
protected:
    LogLevel minLevel;
    std::shared_ptr<LogFormatter> formatter;
    
public:
    LogSink(LogLevel level = LogLevel::INFO) 
        : minLevel(level), formatter(std::make_shared<SimpleTextFormatter>()) {}
    
    virtual void write(const LogComponent& log) = 0;
    virtual ~LogSink() = default;
    
    void setFormatter(std::shared_ptr<LogFormatter> fmt) { formatter = fmt; }
    void setMinLevel(LogLevel level) { minLevel = level; }
    bool shouldLog(const LogComponent& log) const { return log.level >= minLevel; }
};

// 控制台输出
class ConsoleSink : public LogSink {
public:
    using LogSink::LogSink;
    
    void write(const LogComponent& log) override {
        if (!shouldLog(log)) return;
        
        std::string formatted = formatter->format(log);
        if (log.level == LogLevel::ERROR || log.level == LogLevel::CRITICAL) {
            std::cerr << formatted << std::endl;
        } else {
            std::cout << formatted << std::endl;
        }
    }
};

// 文件输出
class FileSink : public LogSink {
private:
    std::ofstream fileStream;
    std::mutex fileMutex;
    
public:
    FileSink(const std::string& filename, LogLevel level = LogLevel::INFO) 
        : LogSink(level) {
        fileStream.open(filename, std::ios::app);
        if (!fileStream.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
    }
    
    ~FileSink() override {
        if (fileStream.is_open()) {
            fileStream.close();
        }
    }
    
    void write(const LogComponent& log) override {
        if (!shouldLog(log) || !fileStream.is_open()) return;
        
        std::lock_guard<std::mutex> lock(fileMutex);
        fileStream << formatter->format(log) << std::endl;
    }
};

// 日志系统 - 处理日志组件的ECS系统
class LoggerSystem : public ecs::System {
private:
    std::vector<std::shared_ptr<LogSink>> sinks;
    std::mutex sinkMutex;
    
public:
    LoggerSystem(ecs::Manager& manager) : ecs::System(manager) {
        signature.set(ecs::getComponentTypeID<LogComponent>());
    }
    
    void addSink(std::shared_ptr<LogSink> sink) {
        std::lock_guard<std::mutex> lock(sinkMutex);
        sinks.push_back(sink);
    }
    
    void update(double deltaTime) override {
        // 处理所有携带LogComponent的实体
        for (auto entity : entities) {
            auto& log = entity->getComponent<LogComponent>();
            
            // 多线程安全地写入所有日志目标
            std::lock_guard<std::mutex> lock(sinkMutex);
            for (auto& sink : sinks) {
                sink->write(log);
            }
            
            // 处理后销毁日志实体
            entity->destroy();
        }
    }
};

// 全局日志管理器
class Logger {
private:
    ecs::Manager& ecsManager;
    LoggerSystem* loggerSystem;
    
    Logger(ecs::Manager& manager) 
        : ecsManager(manager) {
        loggerSystem = &ecsManager.registerSystem<LoggerSystem>();
    }
    
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
public:
    static Logger& getInstance(ecs::Manager& manager) {
        static Logger instance(manager);
        return instance;
    }
    
    void addSink(std::shared_ptr<LogSink> sink) {
        loggerSystem->addSink(sink);
    }
    
    void log(LogLevel level, const std::string& message) {
        auto& logEntity = ecsManager.addEntity();
        logEntity.addComponent<LogComponent>(message, level);
    }
    
    // 便捷方法
    void debug(const std::string& message) { log(LogLevel::DEBUG, message); }
    void info(const std::string& message) { log(LogLevel::INFO, message); }
    void warning(const std::string& message) { log(LogLevel::WARNING, message); }
    void error(const std::string& message) { log(LogLevel::ERROR, message); }
    void critical(const std::string& message) { log(LogLevel::CRITICAL, message); }
};

// 日志宏定义
#define ECS_LOG_DEBUG(manager, message) Logger::getInstance(manager).debug(message)
#define ECS_LOG_INFO(manager, message) Logger::getInstance(manager).info(message)
#define ECS_LOG_WARNING(manager, message) Logger::getInstance(manager).warning(message)
#define ECS_LOG_ERROR(manager, message) Logger::getInstance(manager).error(message)
#define ECS_LOG_CRITICAL(manager, message) Logger::getInstance(manager).critical(message)

 