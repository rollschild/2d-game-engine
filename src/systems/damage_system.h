#ifndef INCLUDED_DAMAGE_SYSTEM_H
#define INCLUDED_DAMAGE_SYSTEM_H

#include <memory>
#include <string>

#include "../components/box_collider_component.h"
#include "../components/health_component.h"
#include "../components/projectile_component.h"
#include "../ecs/ecs.h"
#include "../event_bus/event_bus.h"
#include "../events/collision_event.h"

class DamageSystem : public System {
   public:
    DamageSystem() { require_component<BoxColliderComponent>(); }

    void subscribe_to_events(std::unique_ptr<EventBus>& ebus) {
        ebus->subscribe_to_event<CollisionEvent>(this,
                                                 &DamageSystem::on_collision);
    }

    void on_projectile_hits_player(Entity projectile, Entity player) {
        auto project_component =
            projectile.get_component<ProjectileComponent>();
        if (!project_component.is_friendly) {
            auto& health = player.get_component<HealthComponent>();
            health.health_pct -= project_component.hit_percentage;
            if (health.health_pct <= 0) {
                player.kill();
            }

            projectile.kill();
        }
    }

    void on_projectile_hits_enemy(Entity proj, Entity enemy) {
        const auto proj_component = proj.get_component<ProjectileComponent>();
        if (proj_component.is_friendly) {
            auto& health = enemy.get_component<HealthComponent>();
            health.health_pct -= proj_component.hit_percentage;
            if (health.health_pct <= 0) {
                enemy.kill();
            }
            proj.kill();
        }
    }

    void on_collision(CollisionEvent& e) {
        Entity ea = e.a;
        Entity eb = e.b;
        Logger::log(
            "Damage system received an event collision between entities " +
            std::to_string(ea.get_id()) + " and " +
            std::to_string(eb.get_id()));
        if (ea.in_group("projectiles") && eb.has_tag("player")) {
            on_projectile_hits_player(ea, eb);
        }
        if (eb.in_group("projectiles") && ea.has_tag("player")) {
            on_projectile_hits_player(eb, ea);
        }
        if (ea.in_group("projectiles") && eb.in_group("enemies")) {
            on_projectile_hits_enemy(ea, eb);
        }
        if (eb.in_group("projectiles") && ea.in_group("enemies")) {
            on_projectile_hits_enemy(eb, ea);
        }
    }

    void update() {}
};

#endif
