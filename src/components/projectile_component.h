#ifndef INCLUDED_PROJECTILE_COMPONENT_H
#define INCLUDED_PROJECTILE_COMPONENT_H

#include <SDL2/SDL_timer.h>

#include <glm/ext/vector_float2.hpp>
struct ProjectileComponent {
    bool is_friendly;
    unsigned int hit_percentage;
    unsigned int duration;
    unsigned int start_time;

    ProjectileComponent(bool is_friendly = false,
                        unsigned int hit_percentage = 0,
                        unsigned int duration = 0)
        : is_friendly(is_friendly),
          hit_percentage(hit_percentage),
          duration(duration) {
        this->start_time = SDL_GetTicks();
    }
};

#endif
