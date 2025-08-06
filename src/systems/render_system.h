#ifndef INCLUDED_RENDER_SYSTEM_H
#define INCLUDED_RENDER_SYSTEM_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include <string>

#include "../components/sprite_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"

class RenderSystem : public System {
   public:
    RenderSystem() {
        require_component<TransformComponent>();
        require_component<SpriteComponent>();
    }

    void update(SDL_Renderer* renderer) {
        for (auto entity : get_system_entities()) {
            const auto transform = entity.get_component<TransformComponent>();
            const auto sprite = entity.get_component<SpriteComponent>();

            SDL_Rect obj = {
                static_cast<int>(transform.position.x),
                static_cast<int>(transform.position.y),
                sprite.width,
                sprite.height,
            };

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &obj);
        }
    }
};

#endif
