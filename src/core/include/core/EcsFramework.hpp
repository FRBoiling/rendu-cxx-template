/*
* Created by boil on 25-2-28.
*/

#ifndef ECSFRAMEWORK_HPP
#define ECSFRAMEWORK_HPP
#include <typeindex>

#include "ecs/Component.hpp"
#include "ecs/EntityManager.hpp"
#include "systems/SystemManager.hpp"
#include "systems/LogSystem.hpp"


namespace core
{
    class ECSFramework
    {
    public:
        EntityManager entities;
        SystemManager systems;
        std::unordered_map<std::type_index, std::unique_ptr<IComponent>> component_stores;

        void initialize_logging();

        template <typename... Args>
        void log(LogLevel level, Args&&... args)
        {
            Entity log_entity = entities.create();
            std::stringstream ss;
            (ss << ... << args);
            components<LogMessage>().add(log_entity,
                                         std::make_unique<LogMessage>(level, ss.str()));
        }

        template <typename T>
        ComponentStorage<T>& components()
        {
            auto type = std::type_index(typeid(T));
            if (!component_stores.contains(type))
            {
                component_stores[type] = std::make_unique<ComponentStorage<T>>();
            }
            return static_cast<ComponentStorage<T>&>(*component_stores[type]);
        }

        void run();
    };
}

// 日志宏（适配ECS架构）
#define ECS_LOG(level, ...) ECSFramework.log(level, __VA_ARGS__)
#define ECS_DEBUG(...) ECS_LOG(LogLevel::DEBUG, __VA_ARGS__)
#define ECS_INFO(...)  ECS_LOG(LogLevel::INFO, __VA_ARGS__)
#define ECS_WARN(...)  ECS_LOG(LogLevel::WARNING, __VA_ARGS__)
#define ECS_ERROR(...) ECS_LOG(LogLevel::ERROR, __VA_ARGS__)


#endif //ECSFRAMEWORK_HPP
