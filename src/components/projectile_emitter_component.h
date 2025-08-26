#ifndef INCLUDED_PROJECTILE_EMITTER_COMPONENT_H
#define INCLUDED_PROJECTILE_EMITTER_COMPONENT_H

#include <SDL2/SDL_timer.h>

#include <glm/ext/vector_float2.hpp>

struct ProjectileEmitterComponent {
    glm::vec2 project_vel;
    unsigned int repeat_freq;
    unsigned int project_duration;
    unsigned int hit_percentage;
    bool is_friendly;
    unsigned int last_emission_time;  // milliseconds

    ProjectileEmitterComponent(glm::vec2 project_vel = glm::vec2(0),
                               unsigned int repeat_freq = 0,
                               unsigned int project_duration = 10000,
                               unsigned int hit_percentage = 10,
                               bool is_friendly = false)
        : project_vel(project_vel),
          repeat_freq(repeat_freq),
          project_duration(project_duration),
          hit_percentage(hit_percentage),
          is_friendly(is_friendly) {
        this->last_emission_time = SDL_GetTicks();
    }
};

#endif
