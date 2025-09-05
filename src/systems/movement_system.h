#ifndef INCLUDED_MOVEMENT_SYSTEM_H
#define INCLUDED_MOVEMENT_SYSTEM_H

#include <SDL2/SDL_render.h>

#include <string>

#include "../components/rigid_body_component.h"
#include "../components/sprite_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"
#include "../event_bus/event_bus.h"
#include "../events/collision_event.h"

class MovementSystem : public System {
   public:
    MovementSystem() {
        require_component<TransformComponent>();
        require_component<RigidBodyComponent>();
    }

    void subscribe_to_events(std::unique_ptr<EventBus>& ebus) {
        ebus->subscribe_to_event<CollisionEvent>(this,
                                                 &MovementSystem::on_collision);
    }

    void on_collision(CollisionEvent& e) {
        Entity ea = e.a;
        Entity eb = e.b;
        Logger::log("Collision between entities " +
                    std::to_string(ea.get_id()) + " and " +
                    std::to_string(eb.get_id()));
        if (ea.in_group("enemies") && eb.in_group("obstacles")) {
            on_enemy_hits_obstacle(ea, eb);
        }
        if (eb.in_group("enemies") && ea.in_group("obstacles")) {
            on_enemy_hits_obstacle(eb, ea);
        }
    }

    void on_enemy_hits_obstacle(Entity enemy, [[maybe_unused]] Entity obs) {
        if (enemy.has_component<RigidBodyComponent>() &&
            enemy.has_component<SpriteComponent>()) {
            auto& rigid_body = enemy.get_component<RigidBodyComponent>();
            auto& sprite = enemy.get_component<SpriteComponent>();

            if (rigid_body.velocity.x != 0) {
                rigid_body.velocity.x *= -1;
                sprite.flip =
                    (sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL
                                                  : SDL_FLIP_NONE);
            }
            if (rigid_body.velocity.y != 0) {
                rigid_body.velocity.y *= -1;
                sprite.flip = (sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_VERTICAL
                                                            : SDL_FLIP_NONE);
            }
        }
    }

    void update(double delta_time) {
        for (auto entity : get_system_entities()) {
            auto& transform = entity.get_component<TransformComponent>();
            const auto rigid_body = entity.get_component<RigidBodyComponent>();

            transform.position.x += rigid_body.velocity.x * delta_time;
            transform.position.y += rigid_body.velocity.y * delta_time;

            if (entity.has_tag("player")) {
                int padding_left = 10;
                int padding_top = 10;
                int padding_right = 50;
                int padding_bottom = 50;

                transform.position.x = transform.position.x < padding_left
                                           ? padding_left
                                           : transform.position.x;
                transform.position.x =
                    transform.position.x > Game::map_width - padding_right
                        ? Game::map_width - padding_right
                        : transform.position.x;
                transform.position.y = transform.position.y < padding_top
                                           ? padding_top
                                           : transform.position.y;
                transform.position.y =
                    transform.position.y > Game::map_height - padding_bottom
                        ? Game::map_height - padding_bottom
                        : transform.position.y;
            }

            bool is_entity_outside_map =
                (transform.position.x < 0 ||
                 transform.position.x > Game::map_width ||
                 transform.position.y < 0 ||
                 transform.position.y > Game::map_height

                );
            if (is_entity_outside_map && !entity.has_tag("player")) {
                entity.kill();
            }
        }
    }
};

#endif
