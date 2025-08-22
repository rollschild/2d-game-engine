#ifndef INCLUDED_ANIMATION_COMPONENT_H
#define INCLUDED_ANIMATION_COMPONENT_H

#include <SDL2/SDL_timer.h>
struct AnimationComponent {
    int num_frames;
    int current_frame;
    int frame_speed_rate;
    bool is_loop;
    int start_time;

    AnimationComponent(int num_frames = 1, int frame_speed_rate = 1,
                       bool is_loop = true)
        : num_frames(num_frames),
          current_frame(1),
          frame_speed_rate(frame_speed_rate),
          is_loop(is_loop) {
        this->start_time = SDL_GetTicks();
    }
};

#endif
