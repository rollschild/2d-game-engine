#ifndef INCLUDED_DAMAGE_SYSTEM_H
#define INCLUDED_DAMAGE_SYSTEM_H

#include <memory>
#include <string>

#include "../components/box_collider_component.h"
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

    void on_collision(CollisionEvent& e) {
        Logger::log(
            "Damage system received an event collision between entities " +
            std::to_string(e.a.get_id()) + " and " +
            std::to_string(e.b.get_id()));
        e.a.kill();
        e.b.kill();
    }

    void update() {}
};

#endif
