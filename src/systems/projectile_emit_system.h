#ifndef INCLUDED_PROJECTILE_EMIT_SYSTEM_H
#define INCLUDED_PROJECTILE_EMIT_SYSTEM_H

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>

#include <glm/ext/vector_float2.hpp>
#include <memory>

#include "../components/box_collider_component.h"
#include "../components/camera_follow_component.h"
#include "../components/projectile_component.h"
#include "../components/projectile_emitter_component.h"
#include "../components/rigid_body_component.h"
#include "../components/sprite_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"
#include "../event_bus/event_bus.h"
#include "../events/key_pressed_event.h"

class ProjectileEmitSystem : public System {
   public:
    ProjectileEmitSystem() {
        require_component<TransformComponent>();
        require_component<ProjectileEmitterComponent>();
    }

    void on_key_pressed(KeyPressedEvent& e) {
        if (e.symbol == SDLK_SPACE) {
            for (auto ent : get_system_entities()) {
                if (ent.has_component<CameraFollowComponent>()) {
                    const auto projectile_emitter =
                        ent.get_component<ProjectileEmitterComponent>();
                    const auto transform =
                        ent.get_component<TransformComponent>();
                    const auto rigid_body =
                        ent.get_component<RigidBodyComponent>();

                    glm::vec2 project_pos = transform.position;
                    if (ent.has_component<SpriteComponent>()) {
                        auto sprite = ent.get_component<SpriteComponent>();
                        project_pos.x += (transform.scale.x * sprite.width) / 2;
                        project_pos.y +=
                            (transform.scale.y * sprite.height) / 2;
                    }

                    glm::vec2 project_vel = projectile_emitter.project_vel;
                    int direct_x = 0;
                    int direct_y = 0;
                    if (rigid_body.velocity.x > 0) direct_x = +1;
                    if (rigid_body.velocity.x < 0) direct_x = -1;
                    if (rigid_body.velocity.y > 0) direct_y = +1;
                    if (rigid_body.velocity.y < 0) direct_y = -1;
                    project_vel.x = projectile_emitter.project_vel.x * direct_x;
                    project_vel.y = projectile_emitter.project_vel.y * direct_y;

                    // create new projectile entity and add it to the world
                    Entity projectile = ent.registry->create_entity();
                    projectile.group("projectiles");
                    projectile.add_component<TransformComponent>(
                        project_pos, glm::vec2(1.0, 1.0), 0.0);
                    projectile.add_component<RigidBodyComponent>(project_vel);
                    projectile.add_component<SpriteComponent>("bullet-texture",
                                                              4, 4, 99);
                    projectile.add_component<BoxColliderComponent>(4, 4);
                    projectile.add_component<ProjectileComponent>(
                        projectile_emitter.is_friendly,
                        projectile_emitter.hit_percentage,
                        projectile_emitter.project_duration);
                }
            }
        }
    }

    void subscribe_to_events(std::unique_ptr<EventBus>& ebus) {
        ebus->subscribe_to_event<KeyPressedEvent>(
            this, &ProjectileEmitSystem::on_key_pressed);
    }

    void update(std::unique_ptr<Registry>& registry) {
        for (auto ent : get_system_entities()) {
            auto& projectile_emitter =
                ent.get_component<ProjectileEmitterComponent>();
            const auto transform = ent.get_component<TransformComponent>();

            // If emission freq is zero, bypass re-emission logic
            if (projectile_emitter.repeat_freq == 0) {
                continue;
            }

            if (SDL_GetTicks() - projectile_emitter.last_emission_time >
                projectile_emitter.repeat_freq) {
                glm::vec2 project_pos = transform.position;
                if (ent.has_component<SpriteComponent>()) {
                    const auto sprite = ent.get_component<SpriteComponent>();
                    project_pos.x += (transform.scale.x * sprite.width / 2);
                    project_pos.y += (transform.scale.y * sprite.height / 2);
                }

                Entity projectile = registry->create_entity();
                projectile.group("projectiles");
                projectile.add_component<TransformComponent>(
                    project_pos, glm::vec2(1.0, 1.0), 0.0);
                projectile.add_component<RigidBodyComponent>(
                    projectile_emitter.project_vel);
                projectile.add_component<SpriteComponent>("bullet-texture", 4,
                                                          4, 99);
                projectile.add_component<BoxColliderComponent>(4, 4);
                projectile.add_component<ProjectileComponent>(
                    projectile_emitter.is_friendly,
                    projectile_emitter.hit_percentage,
                    projectile_emitter.project_duration);

                projectile_emitter.last_emission_time = SDL_GetTicks();
            }
        }
    }
};

#endif
