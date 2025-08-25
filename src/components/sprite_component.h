#ifndef INCLUDED_SPRITE_COMPONENT_H
#define INCLUDED_SPRITE_COMPONENT_H

#include <SDL2/SDL_rect.h>

#include <glm/glm.hpp>
#include <string>

struct SpriteComponent {
    std::string asset_id;
    int width;
    int height;
    unsigned z_index;
    bool is_fixed;
    SDL_Rect src_rect;

    SpriteComponent(std::string asset_id = "", int width = 0, int height = 0,
                    unsigned z_index = 0, bool is_fixed = false,
                    int src_rect_x = 0, int src_rect_y = 0)
        : asset_id(asset_id),
          width(width),
          height(height),
          z_index(z_index),
          is_fixed(is_fixed) {
        this->src_rect = {src_rect_x, src_rect_y, width, height};
    }
};

#endif
