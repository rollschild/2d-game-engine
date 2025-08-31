#ifndef INCLUDED_RENDER_TEXT_SYSTEM_H
#define INCLUDED_RENDER_TEXT_SYSTEM_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

#include <memory>

#include "../asset_store/asset_store.h"
#include "../components/text_label_component.h"
#include "../ecs/ecs.h"

class RenderTextSystem : public System {
   public:
    RenderTextSystem() { require_component<TextLabelComponent>(); }

    void update(SDL_Renderer* renderer,
                std::unique_ptr<AssetStore>& asset_store,
                const SDL_Rect& camera) {
        for (auto ent : get_system_entities()) {
            const auto text_label = ent.get_component<TextLabelComponent>();

            SDL_Surface* surface = TTF_RenderText_Blended(
                asset_store->get_font(text_label.asset_id),
                text_label.text.c_str(), text_label.color);

            SDL_Texture* texture =
                SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            int label_width = 0;
            int label_height = 0;
            SDL_QueryTexture(texture, NULL, NULL, &label_width, &label_height);

            SDL_Rect dst_rect = {
                static_cast<int>(text_label.pos.x -
                                 (text_label.is_fixed ? 0 : camera.x)),
                static_cast<int>(text_label.pos.y -
                                 (text_label.is_fixed ? 0 : camera.y)),
                label_width, label_height};

            SDL_RenderCopy(renderer, texture, NULL, &dst_rect);
        }
    }
};

#endif
