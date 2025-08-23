#ifndef INCLUDED_RENDER_COLLIDER_SYSTEM_H
#define INCLUDED_RENDER_COLLIDER_SYSTEM_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "../components/box_collider_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"

class RenderColliderSystem : public System {
   public:
    RenderColliderSystem() {
        require_component<TransformComponent>();
        require_component<BoxColliderComponent>();
    }

    void update(SDL_Renderer* rdr) {
        for (auto ent : get_system_entities()) {
            const auto transform = ent.get_component<TransformComponent>();
            const auto collider = ent.get_component<BoxColliderComponent>();

            SDL_Rect collider_rect = {
                static_cast<int>(transform.position.x + collider.offset.x),
                static_cast<int>(transform.position.y + collider.offset.y),
                static_cast<int>(collider.width),
                static_cast<int>(collider.height),
            };
            SDL_SetRenderDrawColor(rdr, 255, 0, 0, 255);
            SDL_RenderDrawRect(rdr, &collider_rect);
        }
    }
};

#endif
