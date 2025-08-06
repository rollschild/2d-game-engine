#ifndef INCLUDED_MOVEMENT_SYSTEM_H
#define INCLUDED_MOVEMENT_SYSTEM_H

#include <string>

#include "../components/rigid_body_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"

class MovementSystem : public System {
   public:
    MovementSystem() {
        require_component<TransformComponent>();
        require_component<RigidBodyComponent>();
    }

    void update(double delta_time) {
        for (auto entity : get_system_entities()) {
            auto& transform = entity.get_component<TransformComponent>();
            const auto rigid_body = entity.get_component<RigidBodyComponent>();

            transform.position.x += rigid_body.velocity.x * delta_time;
            transform.position.y += rigid_body.velocity.y * delta_time;
        }
    }
};

#endif
