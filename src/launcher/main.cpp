#include "core/EcsFramework.hpp"

using namespace core;
// // 示例组件
// struct Transform : IComponent {
//     float x = 0, y = 0, z = 0;
// };
//
// // 示例系统
// class PhysicsSystem : public ISystem {
//     ComponentStorage<Transform>& transforms;
//     EntityManager& entities;
//
// public:
//     PhysicsSystem(ComponentStorage<Transform>& t, EntityManager& e)
//         : transforms(t), entities(e) {}
//
//     void configure() override {}
//
//     bool is_parallel_safe() const override { return true; }
//
//     void update(double delta_time) override {
//         transforms.foreach([delta_time](Entity e, Transform& t) {
//             t.y -= 9.8f * delta_time;
//         });
//     }
// };

int main() {
    ECSFramework framework;
    framework.run();
    return 0;
}