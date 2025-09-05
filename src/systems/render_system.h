#ifndef INCLUDED_RENDER_SYSTEM_H
#define INCLUDED_RENDER_SYSTEM_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "../asset_store/asset_store.h"
#include "../components/sprite_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"

class RenderSystem : public System {
   public:
    RenderSystem() {
        require_component<TransformComponent>();
        require_component<SpriteComponent>();
    }

    void update(SDL_Renderer* renderer,
                std::unique_ptr<AssetStore>& asset_store, SDL_Rect& camera) {
        // Sort all entities in the system by z-index
        struct RenderableEntity {
            TransformComponent transform_component;
            SpriteComponent sprite_component;
        };
        std::vector<RenderableEntity> renderable_entities;
        for (auto entity : get_system_entities()) {
            RenderableEntity re;
            re.sprite_component = entity.get_component<SpriteComponent>();
            re.transform_component = entity.get_component<TransformComponent>();

            // Do not render entities that are outside camera view
            bool is_entity_outside_camera =
                (re.transform_component.position.x +
                         (re.transform_component.scale.x *
                          re.sprite_component.width) <
                     camera.x ||
                 re.transform_component.position.x > camera.x + camera.w ||
                 re.transform_component.position.y +
                         (re.transform_component.scale.y *
                          re.sprite_component.height) <
                     camera.y ||
                 re.transform_component.position.y > camera.y + camera.h);
            if (is_entity_outside_camera && !re.sprite_component.is_fixed) {
                continue;
            }
            renderable_entities.emplace_back(re);
        }
        std::sort(renderable_entities.begin(), renderable_entities.end(),
                  [](const RenderableEntity& a, const RenderableEntity& b) {
                      return a.sprite_component.z_index <
                             b.sprite_component.z_index;
                  });

        for (auto entity : renderable_entities) {
            const auto transform = entity.transform_component;
            const auto sprite = entity.sprite_component;

            SDL_Rect src_rect = sprite.src_rect;
            SDL_Rect dst_rect = {
                static_cast<int>(transform.position.x -
                                 (sprite.is_fixed ? 0 : camera.x)),
                static_cast<int>(transform.position.y -
                                 (sprite.is_fixed ? 0 : camera.y)),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y),
            };

            SDL_RenderCopyEx(
                renderer, asset_store->get_texture(sprite.asset_id), &src_rect,
                &dst_rect, transform.rotation, NULL, sprite.flip);

            /*
            SDL_Rect obj = {
                static_cast<int>(transform.position.x),
                static_cast<int>(transform.position.y),
                sprite.width,
                sprite.height,
            };

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &obj);
            */
        }
    }
};

#endif
