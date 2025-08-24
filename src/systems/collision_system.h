#ifndef INCLUDED_COLLISION_SYSTEM_H
#define INCLUDED_COLLISION_SYSTEM_H

#include <string>

#include "../components/box_collider_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"

class CollisionSystem : public System {
   public:
    CollisionSystem() {
        require_component<TransformComponent>();
        require_component<BoxColliderComponent>();
    }

    void update() {
        // check all entities that have a box collider
        // see if they are colliding with each other
        auto entities = get_system_entities();
        for (auto i = entities.begin(); i != entities.end(); i++) {
            Entity a = *i;
            auto a_transform = a.get_component<TransformComponent>();
            auto a_collider = a.get_component<BoxColliderComponent>();
            for (auto j = i; j != entities.end(); j++) {
                Entity b = *j;
                if (a == b) continue;
                auto b_transform = b.get_component<TransformComponent>();
                auto b_collider = b.get_component<BoxColliderComponent>();

                bool is_colliding = check_aa_bb_collision(
                    a_transform.position.x + a_collider.offset.x,
                    a_transform.position.y + a_collider.offset.y,
                    a_collider.width, a_collider.height,
                    b_transform.position.x + b_collider.offset.x,
                    b_transform.position.y + b_collider.offset.y,
                    b_collider.width, b_collider.height);
                if (is_colliding) {
                    Logger::log("Entity " + std::to_string(a.get_id()) +
                                " is colliding with entity " +
                                std::to_string(b.get_id()) + "!");
                    a.kill();
                    b.kill();
                }
            }
        }
    }

    bool check_aa_bb_collision(double a_x, double a_y, double a_w, double a_h,
                               double b_x, double b_y, double b_w, double b_h) {
        return (a_x < b_x + b_w && a_x + a_w > b_x && a_y < b_y + b_h &&
                a_y + a_h > b_y);
    }
};

#endif
