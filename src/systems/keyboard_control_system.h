#ifndef INCLUDED_KEYBOARD_CONTROL_SYSTEM_H
#define INCLUDED_KEYBOARD_CONTROL_SYSTEM_H

#include <SDL2/SDL_keycode.h>

#include <memory>
#include <string>

#include "../components/keyboard_controlled_component.h"
#include "../components/rigid_body_component.h"
#include "../components/sprite_component.h"
#include "../ecs/ecs.h"
#include "../event_bus/event_bus.h"
#include "../events/collision_event.h"
#include "../events/key_pressed_event.h"

class KeyboardControlSystem : public System {
   public:
    KeyboardControlSystem() {
        require_component<KeyboardControlledComponent>();
        require_component<SpriteComponent>();
        require_component<RigidBodyComponent>();
    }

    void subscribe_to_events(std::unique_ptr<EventBus>& ebus) {
        ebus->subscribe_to_event<KeyPressedEvent>(
            this, &KeyboardControlSystem::on_key_pressed);
    }

    void on_key_pressed(KeyPressedEvent& e) {
        std::string key_code = std::to_string(e.symbol);
        std::string key_symbol(1, e.symbol);

        Logger::log("Key pressed event emitted: [" + key_code + "] " +
                    key_symbol);

        for (auto ent : get_system_entities()) {
            const auto keyboard_control =
                ent.get_component<KeyboardControlledComponent>();
            auto& sprite = ent.get_component<SpriteComponent>();
            auto& rigid_body = ent.get_component<RigidBodyComponent>();

            switch (e.symbol) {
                case SDLK_UP:
                    rigid_body.velocity = keyboard_control.up_vel;
                    sprite.src_rect.y = sprite.height * 0;
                    break;
                case SDLK_RIGHT:
                    rigid_body.velocity = keyboard_control.right_vel;
                    sprite.src_rect.y = sprite.height * 1;
                    break;
                case SDLK_DOWN:
                    rigid_body.velocity = keyboard_control.down_vel;
                    sprite.src_rect.y = sprite.height * 2;
                    break;
                case SDLK_LEFT:
                    rigid_body.velocity = keyboard_control.left_vel;
                    sprite.src_rect.y = sprite.height * 3;
                    break;
            }
        }
    }

    void update() {}
};

#endif
