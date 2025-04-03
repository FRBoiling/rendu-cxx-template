/*
* Created by boil on 25-2-28.
*/

#ifndef PROFILINGSYSTEM_HPP
#define PROFILINGSYSTEM_HPP

#include <chrono>
#include <shared_mutex>
#include <typeindex>

#include "System.hpp"
#include "SystemManager.hpp"

namespace core
{
    class ProfilingSystem : public ISystem
    {
    public:
        // 添加可能缺少的虚析构函数（如果基类需要）
        ~ProfilingSystem() override ;

        struct SystemProfile
        {
            double total_time = 0.0;
            uint64_t call_count = 0;
            std::chrono::high_resolution_clock::time_point last_start;
        };

        std::unordered_map<std::type_index, SystemProfile> profiles_;
        mutable std::shared_mutex mutex_;
        bool enabled_ = true;
        uint32_t sample_window_ = 60; // 采样窗口帧数

    public:
        bool is_parallel_safe() const noexcept override;

        void configure() override;

        void begin_sample();

        void end_sample();

        void update(double) override;

    private:
        void output_console_report() const;

        void save_csv_report() const;

        void reset_counters();
    };
}


#endif //PROFILINGSYSTEM_HPP
