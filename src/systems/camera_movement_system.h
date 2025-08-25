#ifndef INCLUDED_CAMERA_MOVEMENT_SYSTEM_H
#define INCLUDED_CAMERA_MOVEMENT_SYSTEM_H

#include <SDL2/SDL_rect.h>

#include <string>

#include "../components/camera_follow_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"
#include "../game/game.h"

class CameraMovementSystem : public System {
   public:
    CameraMovementSystem() {
        require_component<CameraFollowComponent>();
        require_component<TransformComponent>();
    }

    void update(SDL_Rect& camera) {
        for (auto ent : get_system_entities()) {
            auto transform = ent.get_component<TransformComponent>();

            if (transform.position.x + (camera.w / 2) < Game::map_width) {
                camera.x = transform.position.x - (Game::window_width / 2);
            }
            if (transform.position.y + (camera.h / 2) < Game::map_height) {
                camera.y = transform.position.y - (Game::window_height / 2);
            }
            camera.x = camera.x < 0 ? 0 : camera.x;
            camera.y = camera.y < 0 ? 0 : camera.y;
            camera.x = camera.x > camera.w ? camera.w : camera.x;
            camera.y = camera.y > camera.h ? camera.h : camera.y;
        }
    }
};

#endif
