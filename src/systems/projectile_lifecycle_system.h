#ifndef INCLUDED_PROJECTILE_LIFECYCLE_SYSTEM_H
#define INCLUDED_PROJECTILE_LIFECYCLE_SYSTEM_H

#include "../components/projectile_component.h"
#include "../ecs/ecs.h"

class ProjectileLifecycleSystem : public System {
   public:
    ProjectileLifecycleSystem() { require_component<ProjectileComponent>(); }

    void update() {
        for (auto ent : get_system_entities()) {
            auto projectile = ent.get_component<ProjectileComponent>();

            // kill projectiles after they reach duration limit
            if (SDL_GetTicks() - projectile.start_time > projectile.duration) {
                ent.kill();
            }
        }
    }
};

#endif
