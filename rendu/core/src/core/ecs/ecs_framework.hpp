#pragma once
#include <memory>
#include <vector>
#include <array>
#include <algorithm>
#include <bitset>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>

namespace ecs
{
    

// 前向声明
class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

namespace Internal {
    inline ComponentID getUniqueComponentID() noexcept {
        static ComponentID lastID = 0u;
        return lastID++;
    }
}

template <typename T>
inline ComponentID getComponentID() noexcept {
    static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
    static ComponentID typeID = Internal::getUniqueComponentID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitset = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component {
public:
    Entity* entity;

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}
    virtual ~Component() = default;
};

class Entity {
private:
    friend class Manager;
    friend class SystemManager;
    class Manager& manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;
    ComponentArray componentArray;
    ComponentBitset componentBitset;
    GroupBitset groupBitset;
    std::size_t id;
    static std::size_t nextId;

public:
    Entity(Manager& mManager) : manager(mManager), id(nextId++) {}

    std::size_t getId() const { return id; }

    void update() {
        for (auto& c : components) c->update();
    }

    void draw() {
        for (auto& c : components) c->draw();
    }

    bool isActive() const { return active; }
    void destroy() { active = false; }

    bool hasGroup(Group mGroup) {
        return groupBitset[mGroup];
    }

    void addGroup(Group mGroup);
    void delGroup(Group mGroup) {
        groupBitset[mGroup] = false;
    }

    template <typename T>
    bool hasComponent() const {
        return componentBitset[getComponentID<T>()];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs) {
        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr{ c };
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentID<T>()] = c;
        componentBitset[getComponentID<T>()] = true;

        c->init();
        manager.entityUpdated(this);
        return *c;
    }

    template <typename T>
    T& getComponent() const {
        auto ptr(componentArray[getComponentID<T>()]);
        return *static_cast<T*>(ptr);
    }
};

std::size_t Entity::nextId = 0;

class Manager {
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;
    std::mutex entityMutex;
    std::atomic<bool> running{ true };
    std::vector<std::thread> workerThreads;
    std::queue<std::function<void()>> taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::unordered_map<ComponentBitset, std::vector<Entity*>> entitiesBySignature;
    std::mutex signatureMutex;

    void workerThread() {
        while (running) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this] { return !taskQueue.empty() || !running; });
                if (!running && taskQueue.empty())
                    return;
                task = std::move(taskQueue.front());
                taskQueue.pop();
            }
            task();
        }
    }

public:
    Manager() {
        const unsigned int threadCount = std::thread::hardware_concurrency();
        for (unsigned int i = 0; i < threadCount; ++i) {
            workerThreads.emplace_back(&Manager::workerThread, this);
        }
    }

    ~Manager() {
        running = false;
        condition.notify_all();
        for (auto& thread : workerThreads) {
            if (thread.joinable())
                thread.join();
        }
    }

    void addToGroup(Entity* mEntity, Group mGroup) {
        groupedEntities[mGroup].emplace_back(mEntity);
    }

    void removeFromGroup(Entity* mEntity, Group mGroup) {
        auto& vec = groupedEntities[mGroup];
        vec.erase(std::remove(vec.begin(), vec.end(), mEntity), vec.end());
    }

    std::vector<Entity*>& getEntitiesByGroup(Group mGroup) {
        return groupedEntities[mGroup];
    }

    void entityUpdated(Entity* entity) {
        std::lock_guard<std::mutex> lock(signatureMutex);
        for (auto& pair : entitiesBySignature) {
            const auto& signature = pair.first;
            auto& entities = pair.second;
            
            bool matches = (entity->componentBitset & signature) == signature;
            bool contains = std::find(entities.begin(), entities.end(), entity) != entities.end();
            
            if (matches && !contains) {
                entities.push_back(entity);
            } else if (!matches && contains) {
                entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
            }
        }
    }

    void addSignature(ComponentBitset signature) {
        std::lock_guard<std::mutex> lock(signatureMutex);
        if (entitiesBySignature.find(signature) == entitiesBySignature.end()) {
            entitiesBySignature[signature] = std::vector<Entity*>();
            
            // 检查现有实体是否匹配新签名
            for (auto& entity : entities) {
                if ((entity->componentBitset & signature) == signature) {
                    entitiesBySignature[signature].push_back(entity.get());
                }
            }
        }
    }

    std::vector<Entity*>& getEntitiesBySignature(ComponentBitset signature) {
        std::lock_guard<std::mutex> lock(signatureMutex);
        static std::vector<Entity*> empty;
        auto it = entitiesBySignature.find(signature);
        if (it != entitiesBySignature.end()) {
            return it->second;
        }
        return empty;
    }

    void refresh() {
        for (Group i = 0; i < maxGroups; i++) {
            auto& vec = groupedEntities[i];
            vec.erase(
                std::remove_if(std::begin(vec), std::end(vec),
                    [i](Entity* mEntity) {
                        return !mEntity->isActive() || !mEntity->hasGroup(i);
                    }),
                std::end(vec));
        }

        entities.erase(
            std::remove_if(std::begin(entities), std::end(entities),
                [](const std::unique_ptr<Entity>& mEntity) {
                    return !mEntity->isActive();
                }),
            std::end(entities));

        // 刷新按签名分组的实体
        {
            std::lock_guard<std::mutex> lock(signatureMutex);
            for (auto& pair : entitiesBySignature) {
                auto& entities = pair.second;
                entities.erase(
                    std::remove_if(std::begin(entities), std::end(entities),
                        [](Entity* mEntity) {
                            return !mEntity->isActive();
                        }),
                    std::end(entities));
            }
        }
    }

    Entity& addEntity() {
        std::lock_guard<std::mutex> lock(entityMutex);
        Entity* e = new Entity(*this);
        std::unique_ptr<Entity> uPtr{ e };
        entities.emplace_back(std::move(uPtr));
        return *e;
    }

    template <typename Function>
    void enqueueTask(Function&& task) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.emplace(std::forward<Function>(task));
        }
        condition.notify_one();
    }

    void parallelForEachEntity(std::function<void(Entity&)> task) {
        std::mutex completionMutex;
        std::condition_variable completionCondition;
        int remainingTasks = entities.size();

        for (auto& entity : entities) {
            enqueueTask([&, &entity = *entity]() {
                task(entity);
                {
                    std::lock_guard<std::mutex> lock(completionMutex);
                    --remainingTasks;
                }
                completionCondition.notify_one();
            });
        }

        std::unique_lock<std::mutex> lock(completionMutex);
        completionCondition.wait(lock, [&] { return remainingTasks == 0; });
    }
};

void Entity::addGroup(Group mGroup) {
    groupBitset[mGroup] = true;
    manager.addToGroup(this, mGroup);
}

// 系统基类
class System {
protected:
    Manager& manager;
    ComponentBitset signature;
    std::vector<Entity*> entities;
    std::mutex entityMutex;
    
    friend class SystemManager;
public:
    System(Manager& mManager) : manager(mManager) {
        manager.addSignature(signature);
    }

    virtual ~System() = default;

    virtual void init() {}
    virtual void update(double dt) = 0;
    virtual void draw() {}

    void addEntity(Entity* entity) {
        std::lock_guard<std::mutex> lock(entityMutex);
        entities.push_back(entity);
    }

    void removeEntity(Entity* entity) {
        std::lock_guard<std::mutex> lock(entityMutex);
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
    }

    ComponentBitset getComponentBitset() const {
        return signature;
    }

    void refresh() {
        std::lock_guard<std::mutex> lock(entityMutex);
        entities = manager.getEntitiesBySignature(signature);
    }
};

// 系统管理器
class SystemManager {
private:
    Manager& manager;
    std::unordered_map<ComponentBitset, std::shared_ptr<System>> systems;
    std::mutex systemMutex;

public:
    SystemManager(Manager& mManager) : manager(mManager) {}

    template <typename T, typename... Args>
    std::shared_ptr<T> registerSystem(Args&&... args) {
        static_assert(std::is_base_of<System, T>::value, "T must inherit from System");
        auto system = std::make_shared<T>(manager, std::forward<Args>(args)...);
        systems[system->getComponentBitset()] = system;
        system->init();
        return system;
    }

    void entityUpdated(Entity* entity) {
        std::lock_guard<std::mutex> lock(systemMutex);
        for (auto& pair : systems) {
            const auto& bitset = pair.first;
            auto& system = pair.second;

            bool fits = (entity->componentBitset & bitset) == bitset;
            bool contains = std::find(system->entities.begin(), system->entities.end(), entity) != system->entities.end();

            if (fits && !contains) {
                system->addEntity(entity);
            } else if (!fits && contains) {
                system->removeEntity(entity);
            }
        }
    }

    void update(double dt) {
        std::lock_guard<std::mutex> lock(systemMutex);
        for (auto& pair : systems) {
            auto& system = pair.second;
            system->refresh();
            manager.enqueueTask([system, dt]() {
                system->update(dt);
            });
        }
    }

    void draw() {
        std::lock_guard<std::mutex> lock(systemMutex);
        for (auto& pair : systems) {
            auto& system = pair.second;
            system->draw();
        }
    }
};    

} // namespace ecs
