/*
* Created by boil on 25-2-28.
*/

#ifndef SYSTEMMANAGER_HPP
#define SYSTEMMANAGER_HPP

#include <vector>

#include "System.hpp"
#include "core/async/ThreadPool.hpp"

namespace core
{
    class ProfilingSystem;

    // 系统管理器
    class SystemManager
    {
    public:
        std::vector<std::unique_ptr<ISystem>> systems;
        async::ThreadPool pool;

        static SystemManager& getInstance()
        {
            static SystemManager instance;
            return instance;
        }

        ProfilingSystem& get_profiler();

        template <typename T, typename... Args>
        T& add(Args&&... args)
        {
            auto system = std::make_unique<T>(std::forward<Args>(args)...);
            system->configure();
            auto& ref = *system;
            systems.push_back(std::move(system));
            return ref;
        }

        void update(double delta_time) ;
    };
}


#endif //SYSTEMMANAGER_HPP
