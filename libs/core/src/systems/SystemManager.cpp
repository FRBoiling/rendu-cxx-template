/*
* Created by boil on 25-2-28.
*/

#include "core/systems/SystemManager.hpp"

#include <future>

#include "core/systems/ProfilingSystem.hpp"

core::ProfilingSystem& core::SystemManager::get_profiler()
{
    static ProfilingSystem* instance = nullptr;
    if (!instance) {
        instance = &add<ProfilingSystem>();
    }
    return *instance;
}

void core::SystemManager::update(double delta_time)
{
    auto& profiler = get_profiler();
    profiler.begin_sample();

    std::vector<std::future<void>> futures;

    for (auto& system : systems)
    {
        if (system->is_parallel_safe())
        {
            futures.emplace_back(
                std::async(std::launch::async, [&]
                {
                    system->update(delta_time);
                })
            );
        }
        else
        {
            system->update(delta_time);
        }
    }

    for (auto& future : futures)
    {
        future.wait();
    }

    profiler.end_sample();
    profiler.update(delta_time); // 最后更新性能分析系统
}
