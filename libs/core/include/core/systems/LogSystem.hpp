/*
* Created by boil on 25-2-28.
*/

#ifndef LOGSYSTEM_HPP
#define LOGSYSTEM_HPP

#include <unordered_map>

#include <shared_mutex>
#include <chrono>
#include <fstream>
#include <iostream>

namespace core {

// 日志级别枚举
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    // 日志输出接口（策略模式）
    class ILogAppender
    {
    public:
        virtual ~ILogAppender() = default;
        virtual void write(const std::string& message) = 0;
    };

    // 控制台输出器
    class ConsoleAppender : public ILogAppender
    {
        std::mutex mutex_;

    public:
        void write(const std::string& msg) override
        {
            std::unique_lock lock(mutex_);
            std::cout << msg << std::endl;
        }
    };

    // 文件输出器
    class FileAppender : public ILogAppender
    {
        std::ofstream file_;
        std::mutex mutex_;

    public:
        explicit FileAppender(const std::string& filename)
            : file_(filename, std::ios::app)
        {
        }

        void write(const std::string& msg) override
        {
            std::unique_lock lock(mutex_);
            if (file_.is_open())
            {
                file_ << msg << std::endl;
            }
        }
    };


    // 新增日志组件类型
    struct LogMessage : public IComponent
    {
        std::chrono::system_clock::time_point timestamp;
        std::thread::id thread_id;
        LogLevel level;
        std::string message;

        LogMessage(LogLevel lvl, std::string msg)
            : timestamp(std::chrono::system_clock::now()),
              thread_id(std::this_thread::get_id()),
              level(lvl),
              message(std::move(msg))
        {
        }
    };

    // 日志系统（继承自ISystem）
    class LogSystem : public ISystem
    {
        ComponentStorage<LogMessage>& log_store_;
        EntityManager& entities_;
        std::vector<std::unique_ptr<ILogAppender>> appenders_;
        std::atomic<LogLevel> log_level_{LogLevel::INFO};
        mutable std::shared_mutex config_mutex_;

    public:
        LogSystem(ComponentStorage<LogMessage>& store, EntityManager& entities)
            : log_store_(store), entities_(entities)
        {
        }

        void configure() override
        {
            // 默认添加控制台输出
            add_appender<ConsoleAppender>();
        }

        bool is_parallel_safe() const override {
            return false; // 日志系统通常需要串行访问保证顺序
        }


        template <typename T, typename... Args>
        void add_appender(Args&&... args)
        {
            std::unique_lock lock(config_mutex_);
            appenders_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        void set_level(LogLevel level)
        {
            log_level_.store(level, std::memory_order_release);
        }

        void update(double) override
        {
            // 批量处理日志消息
            log_store_.foreach([this](Entity e, LogMessage& log)
            {
                if (log.level >= log_level_.load(std::memory_order_acquire))
                {
                    format_and_write(log);
                }
                entities_.destroy(e); // 处理完成后销毁实体
            });
        }

    private:
        void format_and_write(const LogMessage& log)
        {
            std::stringstream ss;
            auto t = std::chrono::system_clock::to_time_t(log.timestamp);
            ss << std::put_time(std::localtime(&t), "[%Y-%m-%d %H:%M:%S]")
                << " [Thread:" << log.thread_id << "] "
                << to_string(log.level) << " " << log.message;

            std::shared_lock lock(config_mutex_);
            for (auto& appender : appenders_)
            {
                appender->write(ss.str());
            }
        }

        static const char* to_string(LogLevel level)
        {
            switch (level)
            {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO ";
            case LogLevel::WARNING: return "WARN ";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
            }
        }
    };

}

#endif //LOGSYSTEM_HPP
