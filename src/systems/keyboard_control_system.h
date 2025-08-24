#ifndef INCLUDED_KEYBOARD_CONTROL_SYSTEM_H
#define INCLUDED_KEYBOARD_CONTROL_SYSTEM_H

#include <memory>
#include <string>

#include "../components/box_collider_component.h"
#include "../ecs/ecs.h"
#include "../event_bus/event_bus.h"
#include "../events/collision_event.h"
#include "../events/key_pressed_event.h"

class KeyboardControlSystem : public System {
   public:
    KeyboardControlSystem() { require_component<BoxColliderComponent>(); }

    void subscribe_to_events(std::unique_ptr<EventBus>& ebus) {
        ebus->subscribe_to_event<KeyPressedEvent>(
            this, &KeyboardControlSystem::on_key_pressed);
    }

    void on_key_pressed(KeyPressedEvent& e) {
        std::string key_code = std::to_string(e.symbol);
        std::string key_symbol(1, e.symbol);

        Logger::log("Key pressed event emitted: [" + key_code + "] " +
                    key_symbol);
    }

    void update() {}
};

#endif
