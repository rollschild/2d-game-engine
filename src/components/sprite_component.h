#ifndef INCLUDED_SPRITE_COMPONENT_H
#define INCLUDED_SPRITE_COMPONENT_H

#include <glm/glm.hpp>

struct SpriteComponent {
    int width;
    int height;

    SpriteComponent(int width = 0, int height = 0)
        : width(width), height(height) {}
};

#endif
