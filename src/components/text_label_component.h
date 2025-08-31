#ifndef INCLUDED_TEXT_LABEL_COMPONENT
#define INCLUDED_TEXT_LABEL_COMPONENT

#include <SDL2/SDL_pixels.h>

#include <glm/ext/vector_float2.hpp>
#include <string>

struct TextLabelComponent {
    glm::vec2 pos;
    std::string text;
    std::string asset_id;
    SDL_Color color;
    bool is_fixed;

    TextLabelComponent(glm::vec2 pos = glm::vec2(0), std::string text = "",
                       std::string asset_id = "",
                       const SDL_Color& color = {0, 0, 0}, bool is_fixed = true)
        : pos(pos),
          text(text),
          asset_id(asset_id),
          color(color),
          is_fixed(is_fixed) {}
};

#endif
