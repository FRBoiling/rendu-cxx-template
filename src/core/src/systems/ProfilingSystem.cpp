#include "core/systems/ProfilingSystem.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

#include "core/systems/SystemManager.hpp"

namespace core
{
    ProfilingSystem::~ProfilingSystem()
    {
    }
    inline bool ProfilingSystem::is_parallel_safe() const noexcept
    {
        return false; // 必须串行执行
    }

    inline void ProfilingSystem::configure()
    {
        // 默认启用
        enabled_ = true;
    }

    void ProfilingSystem::begin_sample()
    {
        if (!enabled_) return;

        auto& mgr = SystemManager::getInstance();
        std::shared_lock lock(mutex_);

        for (auto& system : mgr.systems)
        {
            auto& type = typeid(system);
            profiles_[type].last_start =
                std::chrono::high_resolution_clock::now();
        }
    }

    void ProfilingSystem::end_sample()
    {
        if (!enabled_) return;

        auto& mgr = SystemManager::getInstance();
        std::unique_lock lock(mutex_);

        for (auto& system : mgr.systems)
        {
            auto& type = typeid(system);
            auto now = std::chrono::high_resolution_clock::now();
            auto& profile = profiles_[type];

            auto duration = std::chrono::duration_cast<
                std::chrono::microseconds>(now - profile.last_start);

            profile.total_time += duration.count() / 1000.0; // 转毫秒
            profile.call_count++;
        }
    }

    inline void ProfilingSystem::update(double)
    {
        if (!enabled_) return;

        // 每60帧输出一次报告
        static uint32_t frame_count = 0;
        if (++frame_count % sample_window_ == 0)
        {
            output_console_report();
            save_csv_report();
            reset_counters();
            frame_count = 0;
        }
    }

    inline void ProfilingSystem::output_console_report() const
    {
        std::shared_lock lock(mutex_);
        std::stringstream ss;

        ss << "\n=== Performance Report ==="
            << "\n| System Name           | Calls | Avg(ms) | Total(ms) |"
            << "\n|-----------------------|-------|---------|-----------|";

        // 按总耗时排序
        std::vector<std::pair<std::type_index, SystemProfile>> sorted;
        for (auto& [type, prof] : profiles_)
        {
            sorted.emplace_back(type, prof);
        }

        std::ranges::sort(sorted,
                          [](auto& a, auto& b) { return a.second.total_time > b.second.total_time; });

        for (auto& [type, prof] : sorted)
        {
            double avg = prof.total_time / prof.call_count;

            ss << "\n| " << std::left << std::setw(21) << type.name()
                << " | " << std::setw(5) << prof.call_count
                << " | " << std::setw(7) << std::fixed << std::setprecision(3) << avg
                << " | " << std::setw(9) << prof.total_time << " |";
        }

        ss << "\n===================================\n";
        std::cout << ss.str();
    }

    inline void ProfilingSystem::save_csv_report() const
    {
        std::shared_lock lock(mutex_);
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::stringstream filename;
        filename << "profile_"
            << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".csv";

        std::ofstream file(filename.str());
        file << "System,Calls,Average(ms),Total(ms)\n";

        for (auto& [type, prof] : profiles_)
        {
            double avg = prof.total_time / prof.call_count;
            file << type.name() << ","
                << prof.call_count << ","
                << avg << ","
                << prof.total_time << "\n";
        }
    }

    inline void ProfilingSystem::reset_counters()
    {
        std::unique_lock lock(mutex_);
        for (auto& [_, prof] : profiles_)
        {
            prof.total_time = 0.0;
            prof.call_count = 0;
        }
    }
}
