#include "core/ecs/ecs_framework.hpp"
#include "core/log/logger.hpp"
#include <iostream>
#include <thread>
#include <vector>

// 定义组件
struct Position : public ecs::Component {
    float x = 0.0f;
    float y = 0.0f;
};

struct Velocity : public ecs::Component {
    float dx = 0.0f;
    float dy = 0.0f;
};

// 移动系统
class MovementSystem : public ecs::System {
private:
    ecs::Manager& manager;
    
public:
    MovementSystem(ecs::Manager& mgr) : ecs::System(mgr), manager(mgr) {
        signature.set(ecs::getComponentTypeID<Position>());
        signature.set(ecs::getComponentTypeID<Velocity>());
    }
    
    void update(double dt) override {
        for (auto entity : entities) {
            auto& pos = entity->getComponent<Position>();
            auto& vel = entity->getComponent<Velocity>();
            
            pos.x += vel.dx * dt;
            pos.y += vel.dy * dt;
            
            ECS_LOG_DEBUG(manager, "Entity " + std::to_string(entity->getId()) + 
                         " moved to position (" + std::to_string(pos.x) + ", " + 
                         std::to_string(pos.y) + ")");
        }
    }
};

int main() {
    ecs::Manager manager;
    
    // 初始化日志系统
    auto& logger = Logger::getInstance(manager);
    logger.addSink(std::make_shared<ConsoleSink>(LogLevel::INFO));
    logger.addSink(std::make_shared<FileSink>("game.log", LogLevel::DEBUG));
    
    ECS_LOG_INFO(manager, "ECS Game Started");
    
    // 注册系统
    auto& movementSystem = manager.registerSystem<MovementSystem>();
    
    // 创建实体
    for (int i = 0; i < 5; ++i) {
        auto& entity = manager.addEntity();
        entity.addComponent<Position>().x = static_cast<float>(i * 10);
        entity.addComponent<Velocity>().dx = 1.0f;
        
        ECS_LOG_INFO(manager, "Created entity " + std::to_string(entity.getId()));
    }
    
    // 游戏循环
    for (int frame = 0; frame < 10; ++frame) {
        ECS_LOG_DEBUG(manager, "Processing frame " + std::to_string(frame));
        
        movementSystem.update(1.0);
        manager.refresh();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    ECS_LOG_INFO(manager, "ECS Game Shutdown");
    
    return 0;
}    