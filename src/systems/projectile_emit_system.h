#ifndef INCLUDED_PROJECTILE_EMIT_SYSTEM_H
#define INCLUDED_PROJECTILE_EMIT_SYSTEM_H

#include <SDL2/SDL_timer.h>

#include <glm/ext/vector_float2.hpp>
#include <memory>

#include "../components/box_collider_component.h"
#include "../components/projectile_emitter_component.h"
#include "../components/rigid_body_component.h"
#include "../components/sprite_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"

class ProjectileEmitSystem : public System {
   public:
    ProjectileEmitSystem() {
        require_component<TransformComponent>();
        require_component<ProjectileEmitterComponent>();
    }

    void update(std::unique_ptr<Registry>& registry) {
        for (auto ent : get_system_entities()) {
            auto& projectile_emitter =
                ent.get_component<ProjectileEmitterComponent>();
            const auto transform = ent.get_component<TransformComponent>();

            if (SDL_GetTicks() - projectile_emitter.last_emission_time >
                projectile_emitter.repeat_freq) {
                glm::vec2 project_pos = transform.position;
                if (ent.has_component<SpriteComponent>()) {
                    const auto sprite = ent.get_component<SpriteComponent>();
                    project_pos.x += (transform.scale.x * sprite.width / 2);
                    project_pos.y += (transform.scale.y * sprite.height / 2);
                }

                Entity projectile = registry->create_entity();
                projectile.add_component<TransformComponent>(
                    project_pos, glm::vec2(1.0, 1.0), 0.0);
                projectile.add_component<RigidBodyComponent>(
                    projectile_emitter.project_vel);
                projectile.add_component<SpriteComponent>("bullet-image", 4, 4,
                                                          99);
                projectile.add_component<BoxColliderComponent>(4, 4);

                projectile_emitter.last_emission_time = SDL_GetTicks();
            }
        }
    }
};

#endif
