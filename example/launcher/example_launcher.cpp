/*
* Created by boil on 2023/2/4.
*/

#include <iostream>
#include <core/ecs/registry.h>

struct position {
  float x;
  float y;
};

struct velocity {
  float dx;
  float dy;
};

void update(rendu::registry &registry) {
  auto view = registry.view<position, velocity>();

  for(auto entity: view) {
    // gets only the components that are going to be used ...

    auto &vel = view.get<velocity>(entity);

    vel.dx = 0.;
    vel.dy = 0.;

    // ...
  }
}

void update(std::uint64_t dt, rendu::registry &registry) {
  registry.view<position, velocity>().each([dt](auto &pos, auto &vel) {
    // gets all the components of the view at once ...

    pos.x += vel.dx * dt;
    pos.y += vel.dy * dt;

    // ...
  });
}



#include <map>

int main() {
  rendu::registry registry;
  std::uint64_t dt = 16;

  for(auto i = 0; i < 10; ++i) {
    auto entity = registry.create();
    registry.emplace<position>(entity, i * 1.f, i * 1.f);
    if(i % 2 == 0) { registry.emplace<velocity>(entity, i * .1f, i * .1f); }
  }

  update(dt, registry);
  update(registry);

  // ...

}
