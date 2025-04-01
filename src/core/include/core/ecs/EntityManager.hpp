/*
* Created by boil on 25-2-28.
*/

#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP
#include <vector>

#include "Entity.hpp"


namespace core {


// 实体管理器
class EntityManager {
    std::vector<Entity> free_list;
    std::atomic<Entity> next_id{1};
    mutable std::shared_mutex mutex;

public:
    Entity create() {
        std::unique_lock lock(mutex);
        if (!free_list.empty()) {
            Entity id = free_list.back();
            free_list.pop_back();
            return id;
        }
        return next_id.fetch_add(1, std::memory_order_relaxed);
    }

    void destroy(Entity entity) {
        std::unique_lock lock(mutex);
        free_list.push_back(entity);
    }

    template<typename Func>
    void foreach(Func&& func) {
        std::shared_lock lock(mutex);
        for (Entity id = 1; id < next_id; ++id) {
            if (std::find(free_list.begin(), free_list.end(), id) == free_list.end()) {
                func(id);
            }
        }
    }
};
}



#endif //ENTITYMANAGER_HPP
