/*
* Created by boil on 25-2-28.
*/

#include "core/EcsFramework.hpp"

void core::ECSFramework::initialize_logging()
{
    auto& log_store = components<LogMessage>();
    systems.add<LogSystem>(log_store, entities);
}

void core::ECSFramework::run()
{
    using clock = std::chrono::high_resolution_clock;
    auto last_time = clock::now();
    bool running = true;

    while (running)
    {
        auto current_time = clock::now();
        double delta_time = std::chrono::duration<double>(current_time - last_time).count();
        last_time = current_time;
        systems.update(delta_time);
    }
}
