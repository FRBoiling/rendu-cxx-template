/*
* Created by boil on 25-2-26.
*/

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <unordered_map>
#include <vector>
#include "Entity.hpp"

namespace core
{
    // Component基类

    class IComponent
    {
    public:
        virtual ~IComponent() = default;
    };

    template <typename T>
    class ComponentStorage : public IComponent
    {
        std::vector<std::unique_ptr<T>> dense_data;
        std::unordered_map<Entity, size_t> sparse_map;
        mutable std::shared_mutex mutex;

    public:
        void add(Entity entity, std::unique_ptr<T>&& component)
        {
            std::unique_lock lock(mutex);
            auto [it, inserted] = sparse_map.emplace(entity, dense_data.size());
            if (inserted)
            {
                dense_data.push_back(std::move(component));
            }
        }

        T* get(Entity entity) const
        {
            std::shared_lock lock(mutex);
            auto it = sparse_map.find(entity);
            return (it != sparse_map.end()) ? dense_data[it->second].get() : nullptr;
        }

        void remove(Entity entity)
        {
            std::unique_lock lock(mutex);
            if (auto it = sparse_map.find(entity); it != sparse_map.end())
            {
                dense_data[it->second].reset();
                sparse_map.erase(it);
            }
        }

        template <typename Func>
        void foreach(Func&& func)
        {
            std::shared_lock lock(mutex);
            for (auto& [entity, index] : sparse_map)
            {
                if (dense_data[index])
                {
                    func(entity, *dense_data[index]);
                }
            }
        }
    };
} // rendu

#endif //COMPONENT_HPP
