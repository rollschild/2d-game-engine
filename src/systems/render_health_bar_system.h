#ifndef INCLUDED_RENDER_HEALTH_BAR_SYSTEM_H
#define INCLUDED_RENDER_HEALTH_BAR_SYSTEM_H

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

#include <memory>
#include <string>

#include "../asset_store/asset_store.h"
#include "../components/health_component.h"
#include "../components/sprite_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"

class RenderHealthBarSystem : public System {
   public:
    RenderHealthBarSystem() {
        require_component<TransformComponent>();
        require_component<SpriteComponent>();
        require_component<HealthComponent>();
    }

    void update(SDL_Renderer* renderer,
                const std::unique_ptr<AssetStore>& asset_store,
                const SDL_Rect& camera) {
        for (auto ent : get_system_entities()) {
            const auto transform = ent.get_component<TransformComponent>();
            const auto sprite = ent.get_component<SpriteComponent>();
            const auto health = ent.get_component<HealthComponent>();

            SDL_Color health_bar_color = {255, 255, 255, 0};
            if (health.health_pct >= 0 && health.health_pct < 40) {
                // red
                health_bar_color = {255, 0, 0, 0};
            }
            if (health.health_pct >= 40 && health.health_pct < 80) {
                // yellow
                health_bar_color = {255, 255, 0, 0};
            }
            if (health.health_pct >= 80 && health.health_pct <= 100) {
                // green
                health_bar_color = {0, 255, 0, 0};
            }

            int health_bar_width = 15;
            int health_bar_height = 3;
            double health_bar_pos_x =
                (transform.position.x + (sprite.width * transform.scale.x)) -
                camera.x;
            double health_bar_pos_y = (transform.position.y) - camera.y;

            SDL_Rect health_bar_rect = {
                static_cast<int>(health_bar_pos_x),
                static_cast<int>(health_bar_pos_y),
                static_cast<int>(health_bar_width *
                                 (health.health_pct / 100.0)),
                static_cast<int>(health_bar_height),
            };

            SDL_SetRenderDrawColor(renderer, health_bar_color.r,
                                   health_bar_color.g, health_bar_color.b,
                                   health_bar_color.a);
            SDL_RenderFillRect(renderer, &health_bar_rect);

            // Render health percentage text label
            std::string health_text = std::to_string(health.health_pct);
            SDL_Surface* surface =
                TTF_RenderText_Blended(asset_store->get_font("pico8-font-5"),
                                       health_text.c_str(), health_bar_color);
            SDL_Texture* texture =
                SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            int label_width = 0;
            int label_height = 0;
            SDL_QueryTexture(texture, NULL, NULL, &label_width, &label_height);

            SDL_Rect health_bar_text_rect = {
                static_cast<int>(health_bar_pos_x),
                static_cast<int>(health_bar_pos_y) + 5, label_width,
                label_height};

            SDL_RenderCopy(renderer, texture, NULL, &health_bar_text_rect);

            // prevent memory leak!
            SDL_DestroyTexture(texture);
        }
    }
};

#endif
