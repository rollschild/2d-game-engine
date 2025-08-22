#ifndef INCLUDED_ANIMATION_SYSTEM_H
#define INCLUDED_ANIMATION_SYSTEM_H

#include <SDL2/SDL_timer.h>

#include "../components/animation_component.h"
#include "../components/sprite_component.h"
#include "../ecs/ecs.h"

class AnimationSystem : public System {
   public:
    AnimationSystem() {
        require_component<SpriteComponent>();
        require_component<AnimationComponent>();
    }

    void update() {
        for (auto ent : get_system_entities()) {
            auto& animation = ent.get_component<AnimationComponent>();
            auto& sprite = ent.get_component<SpriteComponent>();

            animation.current_frame =
                static_cast<int>((SDL_GetTicks() - animation.start_time) *
                                 animation.frame_speed_rate / 1000.0) %
                animation.num_frames;
            sprite.src_rect.x = animation.current_frame * sprite.width;
        }
    }
};

#endif
